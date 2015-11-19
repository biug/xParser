#ifndef _TWOWAY_GRAPH_TRANSITION_DEPPARSER_BASE_H
#define _TWOWAY_GRAPH_TRANSITION_DEPPARSER_BASE_H

#include <tuple>
#include <vector>
#include <fstream>
#include <unordered_set>

#include "graph_transition_macros.h"
#include "common/parser/agenda.h"
#include "include/learning/tree/lca.h"
#include "common/parser/depparser_base.h"
#include "include/learning/perceptron/score.h"

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
class TwoWayGraphDepParserBase : public DepParserBase {
public:
	bool m_bChar;
	bool m_bPath;
	bool m_bSuperTag;

	int m_nInputIndex;

	LCA m_lcaAnalyzer;
	RET_TYPE m_lPackedScore;

	int m_nSentenceLength;
	Sentence m_sSentence;
	DependencyTree m_dtSyntaxTree;
	WordPOSTag m_lSentence[MAX_SENTENCE_SIZE];

protected:
	ScoredAction m_saScoredaction;

	AgendaBeam<STATE_TYPE, AGENDA_SIZE> m_abItems[2];
	AgendaBeam<STATE_TYPE, AGENDA_SIZE> m_abFinished;
	AgendaBeam<ScoredAction, AGENDA_SIZE> m_abScores;

	STATE_TYPE m_iStatesItem;
	STATE_TYPE m_iStateItem;
	STATE_TYPE m_iCandidate;
	STATE_TYPE m_iCorrect;

	int m_lTotalErrors[2];
	int m_lTrainingRounds[2];
	WeightBase * m_lWeights[2];

	DWord m_tWords[2];
	DPOSTag m_tPOSTags[2];
	DLabel m_tLabels[2];
	DSuperTag m_tSuperTags[2];

	ACTION_TYPE m_cActions;
	ACTION_TYPE m_lActions[2];
	SuperTagCandidates m_mapSuperTagCandidatesOfWords[2];
	SuperTagCandidates m_mapSuperTagCandidatesOfPOSTags[2];

	AgendaBeam<STATE_TYPE, AGENDA_SIZE> * m_pGenerator;
	AgendaBeam<STATE_TYPE, AGENDA_SIZE> * m_pGenerated;

	void loadTokens(const int & inputIndex = -1, const std::string & sInput = "");
	void initConstant(const std::string & sInput = "");

	void update();
	void generate(DependencyGraph * retval, const DependencyGraph & correct);
	virtual void decode() = 0;
	virtual void getOrUpdateFeatureScores(const STATE_TYPE & item, const ActionScoreIncrement & amount) = 0;
	void updateScoreForState(const STATE_TYPE & from, const STATE_TYPE & output, int action_index, const int & amount);

	void work(DependencyGraph * retval, const DependencyGraph & correct);

public:
	TwoWayGraphDepParserBase(int nState, const bool & bChar, const bool & bPath, const bool & bSTag) :
		DepParserBase(nState), m_bChar(bChar), m_bPath(bPath), m_bSuperTag(bSTag), m_nInputIndex(0), m_nSentenceLength(0),
		m_pGenerator(nullptr), m_pGenerated(nullptr) {
		m_lTotalErrors[0] = 0;			m_lTotalErrors[1] = 0;
		m_lTrainingRounds[0] = 0;		m_lTrainingRounds[1] = 0;
		m_tWords[0] = DWord(1);			m_tWords[1] = DWord(1);
		m_tPOSTags[0] = DPOSTag(1);		m_tPOSTags[1] = DPOSTag(1);
		m_tLabels[0] = DLabel(1);		m_tLabels[1] = DLabel(1);
		m_tSuperTags[0] = DSuperTag(1);	m_tSuperTags[1] = DSuperTag(1);
	}
	virtual ~TwoWayGraphDepParserBase() {};

	void goldCheck(DependencyGraph & correct);
	void train(DependencyGraph & correct, const int & round);
	void parse(DependencyGraph & sentence, DependencyGraph * retval);
	void finishtraining() override;
};

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::loadTokens(const int & inputIndex, const std::string & sInputFile) {
	if (inputIndex == -1 || sInputFile.empty()) {
		return;
	}
	DependencyGraph graph;
	std::ifstream input(sInputFile);

	if (input) {
		// initialize super tag candidates
		std::unordered_map<int, std::unordered_set<int>> wordsCandidates;
		std::unordered_map<int, std::unordered_set<int>> postagsCandidates;
		// load candidates
		for (const auto & word_vec : m_mapSuperTagCandidatesOfWords[inputIndex]) {
			auto & word_set = wordsCandidates[word_vec.first];
			for (const auto & t : word_vec.second) {
				word_set.insert(t);
			}
		}
		for (const auto & postag_vec : m_mapSuperTagCandidatesOfPOSTags[inputIndex]) {
			auto & postag_set = wordsCandidates[postag_vec.first];
			for (const auto & t : postag_vec.second) {
				postag_set.insert(t);
			}
		}
		while (input >> graph) {
			for (const auto & node : graph) {
				// add word
				int w = m_tWords[inputIndex].lookup(node.m_sWord);
				// add POS
				int p = m_tPOSTags[inputIndex].lookup(node.m_sPOSTag);
				// add super tag
				int t = m_tSuperTags[inputIndex].lookup(node.m_sSuperTag);
				// add label
				for (const auto & arc : node.m_vecRightArcs) {
					m_tLabels[inputIndex].lookup(arc.second);
				}
				if (t != 0) {
					wordsCandidates[w].insert(t);
					postagsCandidates[p].insert(t);
				}
			}
			if (!input) {
				break;
			}
		}
		m_mapSuperTagCandidatesOfWords[inputIndex].clear();
		for (const auto & word_set : wordsCandidates) {
			for (const auto & t : word_set.second) {
				m_mapSuperTagCandidatesOfWords[inputIndex][word_set.first].push_back(t);
			}
		}
		m_mapSuperTagCandidatesOfPOSTags[inputIndex].clear();
		for (const auto & postag_set : postagsCandidates) {
			for (const auto & t : postag_set.second) {
				m_mapSuperTagCandidatesOfPOSTags[inputIndex][postag_set.first].push_back(t);
			}
		}
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::initConstant(const std::string & sInputFile) {
	// split file
	DependencyGraph graph;
	std::ifstream input(sInputFile);
	std::ofstream output1("output0");
	std::ofstream output2("output1");
	while (input >> graph) {
		auto graphPair = graph.splitPlanar();
		output1 << graphPair.first;
		output2 << -graphPair.second;
	}
	output2.close();
	output1.close();
	input.close();

	loadTokens(0, "output0");
	loadTokens(1, "output1");
	m_lActions[0].loadConstant(m_tLabels[0]);
	m_lActions[1].loadConstant(m_tLabels[1]);

	std::cout << "constant load complete." << std::endl;

	std::cout << (m_bChar ? "use char" : "without char") << std::endl;
	std::cout << (m_bPath ? "use path" : "without path") << std::endl;
	std::cout << (m_bSuperTag ? "use supertag" : "without supertag") << std::endl;
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::update() {
	m_iStatesItem.clear();
	const STATE_TYPE & output = m_pGenerator->bestUnsortItem();

	int index = 0;
	if (output != m_iCorrect) {
		while (m_iStatesItem != output && index <= output.actionBack() && index <= m_iCorrect.actionBack()) {
			const int & action = output.action(index);
			const int & correct_action = m_iCorrect.action(index);
			if (action == correct_action) {
				m_cActions.doAction(&m_iStatesItem, action);
			}
			else {
				break;
			}
			++index;
		}

		updateScoreForState(m_iStatesItem, m_iCorrect, index, 1);
		updateScoreForState(m_iStatesItem, output, index, -1);

		++m_lTotalErrors[m_nInputIndex];
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::generate(DependencyGraph * retval, const DependencyGraph & correct) {
	m_abFinished.bestUnsortItem().generateGraph(correct, *retval, m_tLabels[m_nInputIndex]);
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::updateScoreForState(const STATE_TYPE & from, const STATE_TYPE & output, int action_index, const int & amount) {
	m_iStateItem = from;
	std::pair<int, int> m_pairAmount(ACTION_START, amount);
	while (m_iStateItem != output && action_index <= output.actionBack()) {
		m_pairAmount.first = output.action(action_index++);
		getOrUpdateFeatureScores(m_iStateItem, m_pairAmount);
		m_cActions.doAction(&m_iStateItem, m_pairAmount.first);
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::work(DependencyGraph * retval, const DependencyGraph & correct) {

	m_abItems[0].clear();
	m_abItems[1].clear();
	m_abFinished.clear();
	m_iCandidate.clear();

	m_iCorrect.clear();

	if (m_nState == TRAIN && !m_cActions.extractOracle(&m_iCorrect, correct)) {
		return;
	}

	// training only if it has an oracle
	++m_lTrainingRounds[m_nInputIndex];

	m_pGenerator = &m_abItems[0];
	m_pGenerated = &m_abItems[1];

	m_pGenerator->insertItem(m_iCandidate);

	int step = 1;
	STATE_TYPE clearItem;

	while (step <= m_iCorrect.actionBack()) {
		decode();

		if (m_nState == TRAIN) {
			// early update check
			bool bCorrect = false;
			for (const auto & item : *m_pGenerator) {
				if (*item == clearItem) {
					bCorrect = true;
					break;
				}
			}
			if (!bCorrect) {
				m_iCorrect = clearItem;
				update();
				return;
			}
		}

		if (m_pGenerated->size() == 0) {
			break;
		}

		std::swap(m_pGenerated, m_pGenerator);

		m_cActions.doAction(&clearItem, m_iCorrect.action(step++));
	}

	if (m_nState == PARSE && m_pGenerator->size() > 0) {
		while (true) {
			decode();
			if (m_pGenerated->size() == 0) {
				break;
			}

			std::swap(m_pGenerated, m_pGenerator);
		}
	}

	switch (m_nState) {
	case ParserState::TRAIN:
		update();
		break;
	case ParserState::PARSE:
		generate(retval, correct);
		break;
	default:
		break;
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::goldCheck(DependencyGraph & correct) {
	m_iCorrect.clear();
	STATE_TYPE item0, item1;
	DependencyGraph g, g0, g1;
	auto correctPair = correct.splitPlanar();
	correctPair.second = -correctPair.second;
	correctPair.first.setTagsAndLabels(m_tLabels[0], m_tSuperTags[0], m_lActions[0].m_vecLabelMap);
	correctPair.second.setTagsAndLabels(m_tLabels[1], m_tSuperTags[1], m_lActions[1].m_vecLabelMap);
	m_lActions[0].extractOracle(&item0, correctPair.first);
	m_lActions[1].extractOracle(&item1, correctPair.second);
	item0.generateGraph(correctPair.first, g0, m_tLabels[0]);
	item1.generateGraph(correctPair.second, g1, m_tLabels[1]);
	g = g0 + (-g1);
	if (g != correct) {
		++m_nTotalErrors;
		if (m_nTotalErrors > 1) {
			nBackSpace("error No. " + std::to_string(m_nTotalErrors - 1));
		}
		std::cout << "error No." << m_nTotalErrors << std::flush;
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::train(DependencyGraph & correct, const int & round) {
	// initialize
	int lastTotalErrors0 = m_lTotalErrors[0];
	int lastTotalErrors1 = m_lTotalErrors[1];
	int lastTrainingRound0 = m_lTrainingRounds[0];
	int lastTrainingRound1 = m_lTrainingRounds[1];

	auto correctPair = correct.splitPlanar();
	correctPair.second = -correctPair.second;
	correctPair.first.setTagsAndLabels(m_tLabels[0], m_tSuperTags[0], m_lActions[0].m_vecLabelMap);
	correctPair.second.setTagsAndLabels(m_tLabels[1], m_tSuperTags[1], m_lActions[1].m_vecLabelMap);
	for (m_nInputIndex = 0; m_nInputIndex < 2; ++m_nInputIndex) {
		int idx = 0;
		m_cActions = m_lActions[m_nInputIndex];
		correct = m_nInputIndex == 0 ? correctPair.first : correctPair.second;
		m_sSentence.clear();
		m_dtSyntaxTree.clear();
		m_nSentenceLength = correct.size();
		for (const auto & node : correct) {
			m_sSentence.push_back(POSTaggedWord(node.m_sWord, node.m_sPOSTag));
			m_dtSyntaxTree.push_back(DependencyTreeNode(m_sSentence.back(), node.m_nTreeHead, "_"));
			m_lSentence[idx++].refer(m_tWords[m_nInputIndex].lookup(node.m_sWord), m_tPOSTags[m_nInputIndex].lookup(node.m_sPOSTag));
		}
		if (m_bPath) {
			m_lcaAnalyzer.loadPath(m_dtSyntaxTree);
		}
		// train
		work(nullptr, correct);
	}
	if (lastTrainingRound0 > 0 || lastTrainingRound1 > 0) {
		nBackSpace("error rate 0.0000 ( " + std::to_string(lastTotalErrors0) + " / " + std::to_string(lastTrainingRound0) + " ) and 0.0000 ( " + std::to_string(lastTotalErrors1) + " / " + std::to_string(lastTrainingRound1) + " ) ");
	}
	if (m_lTrainingRounds[0] > 0 || m_lTrainingRounds[1] > 0) {
		std::cout << "error rate " << ((double)m_lTotalErrors[0] / (double)m_lTrainingRounds[0]);
		std::cout << " ( " << std::to_string(m_lTotalErrors[0]) << " / " << m_lTrainingRounds[0] << " ) and ";
		std::cout << ((double)m_lTotalErrors[1] / (double)m_lTrainingRounds[1]);
		std::cout << " ( " << std::to_string(m_lTotalErrors[1]) << " / " << m_lTrainingRounds[1] << " ) " << std::flush;
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::parse(DependencyGraph & sentence, DependencyGraph * retval) {
	DependencyGraph graph[2];
	auto sentencePair = sentence.splitPlanar();
	sentencePair.second = -sentencePair.second;
	sentencePair.first.setTagsAndLabels(m_tLabels[0], m_tSuperTags[0], m_lActions[0].m_vecLabelMap);
	sentencePair.second.setTagsAndLabels(m_tLabels[1], m_tSuperTags[1], m_lActions[1].m_vecLabelMap);
	for (m_nInputIndex = 0; m_nInputIndex < 2; ++m_nInputIndex) {
		int idx = 0;
		retval->clear();
		m_cActions = m_lActions[m_nInputIndex];
		sentence = m_nInputIndex == 0 ? sentencePair.first : sentencePair.second;
		m_sSentence.clear();
		m_dtSyntaxTree.clear();
		m_nSentenceLength = sentence.size();
		for (const auto & token : sentence) {
			m_sSentence.push_back(POSTaggedWord(token.m_sWord, token.m_sPOSTag));
			m_dtSyntaxTree.push_back(DependencyTreeNode(m_sSentence.back(), token.m_nTreeHead, "_"));
			m_lSentence[idx++].refer(m_tWords[m_nInputIndex].lookup(token.m_sWord), m_tPOSTags[m_nInputIndex].lookup(token.m_sPOSTag));
		}
		if (m_bPath) {
			m_lcaAnalyzer.loadPath(m_dtSyntaxTree);
		}
		work(retval, sentence);
		graph[m_nInputIndex] = *retval;
	}
	*retval = graph[0] + (-graph[1]);
	if (m_nTrainingRound > 0) {
		nBackSpace("parsing sentence " + std::to_string(m_nTrainingRound));
	}
	std::cout << "parsing sentence " << ++m_nTrainingRound << std::flush;
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::finishtraining() {
	m_lWeights[0]->computeAverageFeatureWeights(m_lTrainingRounds[0]);
	m_lWeights[0]->saveScores();
	std::cout << "Total number of training1 errors are: " << m_lTotalErrors[0] << std::endl;
	m_lWeights[1]->computeAverageFeatureWeights(m_lTrainingRounds[1]);
	m_lWeights[1]->saveScores();
	std::cout << "Total number of training1 errors are: " << m_lTotalErrors[1] << std::endl;
}

#endif
