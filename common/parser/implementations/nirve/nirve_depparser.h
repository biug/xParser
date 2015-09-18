#ifndef _NIRVE_DEPPARSER_H
#define _NIRVE_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "nirve_state.h"
#include "nirve_weight.h"
#include "common/parser/depparser_base.h"
#include "include/learning/tree/lca.h"

namespace nirve {

	class DepParser : public DepParserBase {
	private:
		double m_tDecodeTime;
		double m_tGetScoreTime;
		double m_tUpdateScoreTime;
		double m_tStartTime;

		bool m_bChar;
		bool m_bPath;
		bool m_bLabel;

		static WordPOSTag empty_taggedword;
		static SetOfDepLabels empty_setoflabels;

		LCA m_lcaAnalyzer;

		StateAgenda m_abItems[2];
		StateAgenda m_abFinished;
		StateAgenda * m_pGenerator;
		StateAgenda * m_pGenerated;
		ScoreAgenda m_abScores;

		StateItem m_iItemForState;
		StateItem m_iItemForStates;

		StateItem m_iStatesItem;
		StateItem m_iStateItem;
		StateItem m_iCandidate;
		StateItem m_iCorrect;

		Sentence m_sSentence;
		DependencyTree m_dtSyntaxTree;
		WordPOSTag m_lSentence[MAX_SENTENCE_SIZE];
		int m_nSentenceLength;

		WordPOSTag word_tag;
		WordWordPOSTag word_word_tag;
		WordPOSTagPOSTag word_tag_tag;
		TwoWordsInt word_word_int;
		TwoWords bi_word;
		ThreeWords tri_word;
		Int set_of_2_tags;
		Int set_of_3_tags;

		PackedScoreType m_mapPackedScore;

		ScoredAction m_saScoredaction;

		void getActionScores(const StateItem & item);

		void update();
		void generate(DependencyGraph * retval, const DependencyGraph & correct);

		void getOrUpdateStackScore(const StateItem & item, const AddScoreType & amount);
		void updateScoreForState(const StateItem & from, const StateItem & output, const int & start_action_index, const int & amount);

		void reduce(const tscore & score);
		void shift(const tscore & score);
		void swap(const tscore & score);
		void shiftReduce(const tscore & score);

		void arcLeftSwap(const tscore & score);
		void arcLeftShift(const tscore & score);
		void arcLeftReduce(const tscore & score);
		void arcRightSwap(const tscore & score);
		void arcRightShift(const tscore & score);
		void arcRightReduce(const tscore & score);

	public:
		DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, const bool & bChar, const bool & bPath, const bool & bLabel, int nState);
		~DepParser();

		void decode() override;
		void decodeArcs() override;

		void decode(const DependencyGraph & correct);

		void train(const DependencyGraph & correct, const int & round);
		void parse(const DependencyGraph & sentence, DependencyGraph * retval);
		void work(DependencyGraph * retval, const DependencyGraph & correct);
		void goldCheck(const DependencyGraph & correct);

		void printTime() {
//			std::cout << "total time tick is " << GetTickCount() - m_tStartTime << std::endl;
			std::cout << "total get score time is " << m_tGetScoreTime << std::endl;
			std::cout << "total update score time is " << m_tUpdateScoreTime << std::endl;
			std::cout << "total decode time is " << m_tDecodeTime << std::endl;
		}
	};

	extern int AL_SW_FIRST;
	extern int AR_SW_FIRST;
	extern int AL_SH_FIRST;
	extern int AR_SH_FIRST;
	extern int AL_RE_FIRST;
	extern int AR_RE_FIRST;

	extern int AL_SW_END;
	extern int AR_SW_END;
	extern int AL_SH_END;
	extern int AR_SH_END;
	extern int AL_RE_END;
	extern int AR_RE_END;

	inline void DepParser::reduce(const tscore & score) {
		m_abScores.insertItem(ScoredAction(REDUCE, score + m_mapPackedScore[REDUCE]));
	}

	inline void DepParser::shift(const tscore & score) {
		m_abScores.insertItem(ScoredAction(SHIFT, score + m_mapPackedScore[SHIFT]));
	}

	inline void DepParser::swap(const tscore & score) {
		m_abScores.insertItem(ScoredAction(SWAP, score + m_mapPackedScore[SWAP]));
	}

	inline void DepParser::shiftReduce(const tscore & score) {
		m_abScores.insertItem(ScoredAction(SHIFT_REDUCE, score + m_mapPackedScore[SHIFT_REDUCE]));
	}

	inline void DepParser::arcLeftSwap(const tscore & score) {
		for (int action = AL_SW_FIRST; action < AL_SW_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::arcRightSwap(const tscore & score) {
		for (int action = AR_SW_FIRST; action < AR_SW_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::arcLeftShift(const tscore & score) {
		for (int action = AL_SH_FIRST; action < AL_SH_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::arcRightShift(const tscore & score) {
		for (int action = AR_SH_FIRST; action < AR_SH_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::arcLeftReduce(const tscore & score) {
		for (int action = AL_RE_FIRST; action < AL_RE_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}

	inline void DepParser::arcRightReduce(const tscore & score) {
		for (int action = AR_RE_FIRST; action < AR_RE_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_mapPackedScore[action]));
		}
	}
}

#endif
