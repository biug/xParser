#ifndef _GRAPH_TRANSITION_DEPPARSER_BASE_H
#define _GRAPH_TRANSITION_DEPPARSER_BASE_H

#include <tuple>
#include <vector>
#include <unordered_set>

#include "graph_transition_macros_base.h"
#include "common/parser/agenda.h"
#include "include/learning/tree/lca.h"
#include "common/parser/depparser_base.h"
#include "include/learning/perceptron/score.h"

#include "common/token/word.h"
#include "common/token/pos.h"

template<class STATE_TYPE>
class GraphDepParserBase : public DepParserBase {
protected:

	static WordPOSTag empty_taggedword;
	static WordPOSTag start_taggedword;
	static WordPOSTag end_taggedword;
	static WordPOSTag middle_taggedword;
	static TagSet empty_tagset;

	bool m_bChar;
	bool m_bPath;
	bool m_bSuperTag;

	LCA m_lcaAnalyzer;

	int m_nSentenceLength;
	Sentence m_sSentence;
	DependencyTree m_dtSyntaxTree;
	WordPOSTag m_lSentence[MAX_SENTENCE_SIZE];

	ScoredAction m_saScoredaction;

	AgendaBeam<STATE_TYPE, AGENDA_SIZE> m_abItems[2];
	AgendaBeam<STATE_TYPE, AGENDA_SIZE> m_abFinished;
	AgendaBeam<STATE_TYPE, AGENDA_SIZE> * m_pGenerator;
	AgendaBeam<STATE_TYPE, AGENDA_SIZE> * m_pGenerated;
	AgendaBeam<ScoredAction, AGENDA_SIZE> m_abScores;

	STATE_TYPE m_iStatesItem;
	STATE_TYPE m_iStateItem;
	STATE_TYPE m_iCandidate;
	STATE_TYPE m_iCorrect;

	void update();
	void generate(DependencyGraph * retval, const DependencyGraph & correct);
	virtual void getOrUpdateFeatureScore(const STATE_TYPE & item, const AddScoreType & amount) = 0;
	void updateScoreForState(const STATE_TYPE & from, const STATE_TYPE & output, int action_index, const int & amount);

	void work(DependencyGraph * retval, const DependencyGraph & correct);

public:
	GraphDepParserBase(const std::string & sFeatureInput, int nState, const bool & bChar, const bool & bPath, const bool & bSTag) :
		DepParserBase(sFeatureInput, nState), m_bChar(bChar), m_bPath(bPath), m_bSuperTag(bSTag), m_nSentenceLength(0) {}
	virtual ~GraphDepParserBase() {};

	void decodeArcs() override;
	void goldCheck(const DependencyGraph & correct);
	void train(const DependencyGraph & correct, const int & round);
	void parse(const DependencyGraph & sentence, DependencyGraph * retval);
};


template<class STATE_TYPE> WordPOSTag GraphDepParserBase<STATE_TYPE>::empty_taggedword = WordPOSTag();
template<class STATE_TYPE> WordPOSTag GraphDepParserBase<STATE_TYPE>::start_taggedword = WordPOSTag();
template<class STATE_TYPE> WordPOSTag GraphDepParserBase<STATE_TYPE>::end_taggedword = WordPOSTag();
template<class STATE_TYPE> WordPOSTag GraphDepParserBase<STATE_TYPE>::middle_taggedword = WordPOSTag();
template<class STATE_TYPE> TagSet GraphDepParserBase<STATE_TYPE>::empty_tagset = TagSet();

template<class STATE_TYPE>
void GraphDepParserBase<STATE_TYPE>::update() {
	m_iStatesItem.clear();
	const STATE_TYPE & output = m_pGenerator->bestUnsortItem();

	int index = 0;
	if (output != m_iCorrect) {
		while (m_iStatesItem != output && index <= output.actionBack() && index <= m_iCorrect.actionBack()) {
			const int & action = output.action(index);
			const int & correct_action = m_iCorrect.action(index);
			if (action == correct_action) {
				m_iStatesItem.move(action);
			}
			else {
				break;
			}
			++index;
		}

		updateScoreForState(m_iStatesItem, m_iCorrect, index, 1);
		updateScoreForState(m_iStatesItem, output, index, -1);

		++m_nTotalErrors;
	}
}

template<class STATE_TYPE>
void GraphDepParserBase<STATE_TYPE>::generate(DependencyGraph * retval, const DependencyGraph & correct) {
	m_abFinished.bestUnsortItem().generateGraph(correct, *retval);
}

template<class STATE_TYPE>
void GraphDepParserBase<STATE_TYPE>::updateScoreForState(const STATE_TYPE & from, const STATE_TYPE & output, int action_index, const int & amount) {
	m_iStateItem = from;
	std::pair<int, int> m_pairAmount(ACTION_START, amount);
	while (m_iStateItem != output && action_index <= output.actionBack()) {
		m_pairAmount.first = output.action(action_index++);
		getOrUpdateFeatureScore(m_iStateItem, m_pairAmount);
		m_iStateItem.move(m_pairAmount.first);
	}
}

template<class STATE_TYPE>
void GraphDepParserBase<STATE_TYPE>::work(DependencyGraph * retval, const DependencyGraph & correct) {

	m_abItems[0].clear();
	m_abItems[1].clear();
	m_abFinished.clear();
	m_iCandidate.clear();

	m_iCorrect.clear();
	if (m_nState == TRAIN && !m_iCorrect.extractOracle(correct)) {
		return;
	}

	// training only if it has an oracle
	++m_nTrainingRound;

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

		clearItem.move(m_iCorrect.action(step++));
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

template<class STATE_TYPE>
void GraphDepParserBase<STATE_TYPE>::decodeArcs() {}

template<class STATE_TYPE>
void GraphDepParserBase<STATE_TYPE>::goldCheck(const DependencyGraph & correct) {
	m_iCorrect.clear();
	if (!m_iCorrect.extractOracle(correct)) {
		++m_nTotalErrors;
		if (m_nTotalErrors > 1) {
			nBackSpace("Error No. " + std::to_string(m_nTotalErrors - 1));
		}
		std::cout << "Error No." << m_nTotalErrors << std::flush;
	}
	m_iCorrect.check();
}

template<class STATE_TYPE>
void GraphDepParserBase<STATE_TYPE>::train(const DependencyGraph & correct, const int & round) {
	// initialize
	int idx = 0;
	m_sSentence.clear();
	m_dtSyntaxTree.clear();
	m_nSentenceLength = correct.size();
	for (const auto & node : correct) {
		m_sSentence.push_back(GRAPHNODE_POSTAGGEDWORD(node));
		m_dtSyntaxTree.push_back(DependencyTreeNode(GRAPHNODE_POSTAGGEDWORD(node), GRAPHNODE_TREEHEAD(node), GRAPHNODE_TREELABEL(node)));
		m_lSentence[idx++].refer(TWord::code(GRAPHNODE_WORD(node)), TPOSTag::code(GRAPHNODE_POSTAG(node)));
	}
	if (m_bPath) {
		m_lcaAnalyzer.loadPath(m_dtSyntaxTree);
	}
	// train
	int lastTotalErrors = m_nTotalErrors;
	int lastTrainingRound = m_nTrainingRound;
	work(nullptr, correct);
	if (lastTrainingRound > 0) {
		nBackSpace("Error rate 0.0000 ( " + std::to_string(lastTotalErrors) + " / " + std::to_string(lastTrainingRound) + " ) ");
	}
	if (m_nTrainingRound > 0) {
		std::cout << "Error rate " << ((double)m_nTotalErrors / (double)m_nTrainingRound);
		std::cout << " ( " << m_nTotalErrors << " / " << m_nTrainingRound << " ) " << std::flush;
	}
}

template<class STATE_TYPE>
void GraphDepParserBase<STATE_TYPE>::parse(const DependencyGraph & sentence, DependencyGraph * retval) {
	int idx = 0;
	m_sSentence.clear();
	m_dtSyntaxTree.clear();
	m_nSentenceLength = sentence.size();
	for (const auto & token : sentence) {
		m_sSentence.push_back(GRAPHNODE_POSTAGGEDWORD(token));
		m_dtSyntaxTree.push_back(DependencyTreeNode(GRAPHNODE_POSTAGGEDWORD(token), GRAPHNODE_TREEHEAD(token), GRAPHNODE_TREELABEL(token)));
		m_lSentence[idx++].refer(TWord::code(GRAPHNODE_WORD(token)), TPOSTag::code(GRAPHNODE_POSTAG(token)));
	}
	if (m_bPath) {
		m_lcaAnalyzer.loadPath(m_dtSyntaxTree);
	}
	work(retval, sentence);
	if (m_nTrainingRound > 1) {
		nBackSpace("parsing sentence " + std::to_string(m_nTrainingRound - 1));
	}
	std::cout << "parsing sentence " << m_nTrainingRound << std::flush;
}

#endif
