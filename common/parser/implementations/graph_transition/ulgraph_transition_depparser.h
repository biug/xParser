#ifndef _ULGRAPH_TRANSITION_DEPPARSER_BASE_H
#define _ULGRAPH_TRANSITION_DEPPARSER_BASE_H

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
class ULGraphDepParserBase : public DepParserBase {
public:
	bool m_bChar;
	bool m_bPath;

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

	DWord m_tWords;
	DPOSTag m_tPOSTags;

	ACTION_TYPE m_cActions;

	AgendaBeam<STATE_TYPE, AGENDA_SIZE> * m_pGenerator;
	AgendaBeam<STATE_TYPE, AGENDA_SIZE> * m_pGenerated;

	void update();
	void generate(DependencyGraph * retval, const DependencyGraph & correct);
	virtual void decode() = 0;
	virtual void getOrUpdateFeatureScores(const STATE_TYPE & item, const ActionScoreIncrement & amount) = 0;
	void updateScoreForState(const STATE_TYPE & from, const STATE_TYPE & output, int action_index, const int & amount);

	void work(DependencyGraph * retval, const DependencyGraph & correct);

public:
	ULGraphDepParserBase(int nState, const bool & bChar, const bool & bPath) :
		DepParserBase(nState), m_bChar(bChar), m_bPath(bPath), m_nSentenceLength(0),
		m_tWords(1), m_tPOSTags(1), m_pGenerator(nullptr), m_pGenerated(nullptr) { }
	virtual ~ULGraphDepParserBase() {};

	void goldCheck(DependencyGraph & correct);
	void train(DependencyGraph & correct, const int & round);
	void parse(DependencyGraph & sentence, DependencyGraph * retval);
};

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void ULGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::update() {
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

		++m_nTotalErrors;
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void ULGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::generate(DependencyGraph * retval, const DependencyGraph & correct) {
	m_abFinished.bestUnsortItem().generateGraph(correct, *retval);
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void ULGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::updateScoreForState(const STATE_TYPE & from, const STATE_TYPE & output, int action_index, const int & amount) {
	m_iStateItem = from;
	std::pair<int, int> m_pairAmount(ACTION_START, amount);
	while (m_iStateItem != output && action_index <= output.actionBack()) {
		m_pairAmount.first = output.action(action_index++);
		getOrUpdateFeatureScores(m_iStateItem, m_pairAmount);
		m_cActions.doAction(&m_iStateItem, m_pairAmount.first);
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void ULGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::work(DependencyGraph * retval, const DependencyGraph & correct) {

	m_abItems[0].clear();
	m_abItems[1].clear();
	m_abFinished.clear();
	m_iCandidate.clear();

	m_iCorrect.clear();

	if (m_nState == TRAIN && !m_cActions.extractOracle(&m_iCorrect, correct)) {
		return;
	}

//	m_iCorrect.print();

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

				std::cout << std::endl << step << std::endl;
				std::cout << "gold" << std::endl;
				clearItem.print();
				std::cout << "beam" << std::endl;
				for (const auto & item : *m_pGenerator) {
					item->print();
				}

				return;
			}
		}

		if (m_pGenerated->size() == 0) {
			break;
		}

		std::swap(m_pGenerated, m_pGenerator);

		m_cActions.doAction(&clearItem, m_iCorrect.action(step++));
	}

	std::cout << std::endl << step << std::endl;

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
void ULGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::goldCheck(DependencyGraph & correct) {
	m_iCorrect.clear();
	if (!m_cActions.extractOracle(&m_iCorrect, correct)) {
		++m_nTotalErrors;
		if (m_nTotalErrors > 1) {
			nBackSpace("error No. " + std::to_string(m_nTotalErrors - 1));
		}
		std::cout << "error No." << m_nTotalErrors << std::flush;
//		std::cout << "error" << std::endl;
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void ULGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::train(DependencyGraph & correct, const int & round) {
	// initialize
	int idx = 0;
	m_sSentence.clear();
	m_dtSyntaxTree.clear();
	m_nSentenceLength = correct.size();
	for (const auto & node : correct) {
		m_sSentence.push_back(POSTaggedWord(node.m_sWord, node.m_sPOSTag));
		m_dtSyntaxTree.push_back(DependencyTreeNode(m_sSentence.back(), node.m_nTreeHead, "_"));
		m_lSentence[idx++].refer(m_tWords.lookup(node.m_sWord), m_tPOSTags.lookup(node.m_sPOSTag));
	}
	if (m_bPath) {
		m_lcaAnalyzer.loadPath(m_dtSyntaxTree);
	}
	// train
	int lastTotalErrors = m_nTotalErrors;
	int lastTrainingRound = m_nTrainingRound;
	work(nullptr, correct);
	if (lastTrainingRound > 0) {
		nBackSpace("error rate 0.0000 ( " + std::to_string(lastTotalErrors) + " / " + std::to_string(lastTrainingRound) + " ) ");
	}
	if (m_nTrainingRound > 0) {
		std::cout << "error rate " << ((double)m_nTotalErrors / (double)m_nTrainingRound);
		std::cout << " ( " << m_nTotalErrors << " / " << m_nTrainingRound << " ) " << std::flush;
	}
}

template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
void ULGraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE>::parse(DependencyGraph & sentence, DependencyGraph * retval) {
	int idx = 0;
	m_sSentence.clear();
	m_dtSyntaxTree.clear();
	m_nSentenceLength = sentence.size();
	for (const auto & token : sentence) {
		m_sSentence.push_back(POSTaggedWord(token.m_sWord, token.m_sPOSTag));
		m_dtSyntaxTree.push_back(DependencyTreeNode(m_sSentence.back(), token.m_nTreeHead, "_"));
		m_lSentence[idx++].refer(m_tWords.lookup(token.m_sWord), m_tPOSTags.lookup(token.m_sPOSTag));
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
