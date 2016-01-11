#ifndef _ARCEAGERPATH_DEPPARSER_H
#define _ARCEAGERPATH_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "arceagerpath_state.h"
#include "arceagerpath_weight.h"
#include "common/parser/depparser_base.h"
#include "include/learning/tree/lca.h"

namespace arceagerpath {

	class DepParser : public DepParserBase {
	private:

		static WordPOSTag empty_taggedword;
		static SetOfDepLabels empty_setoflabels;

		StateAgenda m_abItems[2];
		StateAgenda * m_pGenerator;
		StateAgenda * m_pGenerated;
		ScoreAgenda m_abScores;

		StateItem m_iItemForState;
		StateItem m_iItemForStates;

		StateItem m_iStatesItem;
		StateItem m_iStateItem;
		StateItem m_iCandidate;
		StateItem m_iCorrect;

		WordPOSTag m_lSentence[MAX_SENTENCE_SIZE];
		int m_nSentenceLength;

		WordWordPOSTagPOSTag st_word_tag_n0_word_tag;
		TwoWords st_word_n0_word;

		Word word;
		POSTag tag;
		Int label;
		WordPOSTag word_tag;
		WordInt word_int;
		POSTagInt tag_int;
		WordPOSTagPOSTag word_tag_tag;
		WordWordPOSTag word_word_tag;
		TwoWordsInt word_word_int;
		POSTagSet2Int tag_tag_int;
		WordSetOfDepLabels word_tagset;
		POSTagSetOfDepLabels tag_tagset;
		Int set_of_2_tags;
		Int set_of_3_tags;

		std::vector<LCA> m_lcaAnalyzer;
		std::vector<DependencyTree> m_dtSyntaxTree;

		PackedScoreType m_mapPackedScore;

		ScoredAction m_saScoredaction;

		void getActionScores(const StateItem & item);

		void update();
		void generate(DependencyTree * retval, const DependencyPathTree & correct);

		void getOrUpdateStackScore(const StateItem & item, const ActionScoreIncrement & amount);
		void updateScoreForState(const StateItem & from, const StateItem & output, const int & amount);

		void reduce(const tscore & score);
		void arcLeft(const tscore & score);
		void arcRight(const tscore & score);
		void shift(const tscore & score);
		void popRoot(const tscore & score);

	public:
		DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, int nState);
		~DepParser();

		void decode();

		void decode(const DependencyTree & correct);

		void train(const DependencyPathTree & correct, const int & round);
		void parse(const DependencyPathTree & sentence, DependencyTree * retval);
		void work(DependencyTree * retval, const DependencyPathTree & correct);
		void goldCheck(const DependencyPathTree & correct);
	};

	extern int AL_FIRST;
	extern int AR_FIRST;

	extern int AL_END;
	extern int AR_END;

	inline void DepParser::reduce(const tscore & score) {
		m_abScores.insertItem(ScoredAction(REDUCE, score + m_mapPackedScore[REDUCE]));
	}

	inline void DepParser::arcLeft(const tscore & score) {
		for (int action = AL_FIRST; action < AL_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::arcRight(const tscore & score) {
		for (int action = AR_FIRST; action < AR_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::shift(const tscore & score) {
		m_abScores.insertItem(ScoredAction(SHIFT, score + m_mapPackedScore[SHIFT]));
	}

	inline void DepParser::popRoot(const tscore & score) {
		m_abScores.insertItem(ScoredAction(POP_ROOT, score + m_mapPackedScore[POP_ROOT]));
	}
}

#endif
