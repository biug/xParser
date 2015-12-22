#ifndef _MINITWOSTACK_DEPPARSER_H
#define _MINITWOSTACK_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "minitwostack_state.h"
#include "minitwostack_weight.h"
#include "include/learning/tree/lca.h"
#include "common/parser/depparser_base.h"

namespace minitwostack {

	class DepParser : public DepParserBase {
	private:

		bool m_bChar;
		bool m_bPath;

		static WordPOSTag empty_taggedword;
		static WordPOSTag start_taggedword;
		static WordPOSTag end_taggedword;
		static WordPOSTag middle_taggedword;
		static TagSet empty_tagset;

		// feature variable
		Unsigned poses_feature;
		Int uni_feature;
		TwoInts bi_features;
		ThreeInts tri_features;
		FourInts quar_features;
		IntTagSet uni_tagset;
		TwoIntsTagSet bi_tagset;

		LCA m_lcaAnalyzer;

		StateAgenda m_abItems[2];
		StateAgenda * m_pGenerator;
		StateAgenda * m_pGenerated;
		ScoreAgenda m_abScores;
		StateAgenda m_abFinished;

		StateItem m_iItemForState;
		StateItem m_iItemForStates;

		StateItem m_iStatesItem;
		StateItem m_iStateItem;
		StateItem m_iCandidate;
		StateItem m_iCorrect;

		DependencyTree m_dtSyntaxTree;
		WordPOSTag m_lSentence[MAX_SENTENCE_SIZE];
		int m_lKeepSentence[MAX_SENTENCE_SIZE];
		int m_nSentenceLength;
		int m_nTrueSentenceLength;

		PackedScoreType m_mapPackedScore;

		ScoredAction m_saScoredaction;

		void getActionScores(const StateItem & item);

		void update();
		void generate(DependencyGraph * retval, const DependencyGraph & correct);

		void getOrUpdateStackScore(const StateItem & item, const ActionScoreIncrement & amount);
		void updateScoreForState(const StateItem & from, const StateItem & output, int action_index, const int & amount);

		void mem(const tscore & score);
		void recall(const tscore & score);
		void shift(const tscore & score);
		void reduce(const tscore & score);
		void arcMem(const tscore & score);
		void arcRecall(const tscore & score);
		void arcShift(const tscore & score);
		void arcReduce(const tscore & score);

	public:
		DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, int nState, bool bChar = false, bool bPath = false);
		~DepParser();

		void decode();

		void train(const DependencyGraph & correct, const int & round);
		void parse(const DependencyGraph & sentence, DependencyGraph * retval);
		void work(DependencyGraph * retval, const DependencyGraph & correct);
		void goldCheck(const DependencyGraph & correct);
	};

	extern int A_MM_FIRST;
	extern int A_RC_FIRST;
	extern int A_SH_FIRST;
	extern int A_RE_FIRST;

	extern int A_MM_END;
	extern int A_RC_END;
	extern int A_SH_END;
	extern int A_RE_END;

	inline void DepParser::mem(const tscore & score) {
		m_abScores.insertItem(ScoredAction(MEM, score + m_mapPackedScore[MEM]));
	}

	inline void DepParser::recall(const tscore & score) {
		m_abScores.insertItem(ScoredAction(RECALL, score + m_mapPackedScore[RECALL]));
	}

	inline void DepParser::shift(const tscore & score) {
		m_abScores.insertItem(ScoredAction(SHIFT, score + m_mapPackedScore[SHIFT]));
	}

	inline void DepParser::reduce(const tscore & score) {
		m_abScores.insertItem(ScoredAction(REDUCE, score + m_mapPackedScore[REDUCE]));
	}

	inline void DepParser::arcMem(const tscore & score) {
		for (int action = A_MM_FIRST; action < A_MM_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::arcRecall(const tscore & score) {
		for (int action = A_RC_FIRST; action < A_RC_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::arcShift(const tscore & score) {
		for (int action = A_SH_FIRST; action < A_SH_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::arcReduce(const tscore & score) {
		for (int action = A_RE_FIRST; action < A_RE_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}
}

#endif
