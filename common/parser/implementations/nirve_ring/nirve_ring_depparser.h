#ifndef _NIRVE_RING_DEPPARSER_H
#define _NIRVE_RING_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "nirve_ring_state.h"
#include "nirve_ring_weight.h"
#include "common/parser/depparser_base.h"
#include "include/learning/tree/lca.h"

namespace nirve_ring {

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
		static WordPOSTag start_taggedword;
		static WordPOSTag middle_taggedword;
		static WordPOSTag end_taggedword;
		static Tagset empty_tagset;

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

		WordInt word_int;
		POSTagInt postag_int;
		WordPOSTag word_postag;
		WordPOSTagInt word_postag_int;
		WordWordPOSTag word_word_postag;
		WordPOSTagPOSTag word_postag_postag;
		WordPOSTagPOSTagInt word_postag_postag_int;
		WordWordPOSTagPOSTag word_word_postag_postag;
		TwoWordsInt word_word_int;
		TwoWords two_words;
		TwoWordsInt two_words_int;
		ThreeWords three_words;
		ThreeWordsInt three_words_int;
		POSTagSet2Int postag_set_2_int;
		POSTagSet3Int postag_set_3_int;
		Int set_of_2_postags;
		Int set_of_3_postags;
		Int set_of_4_postags;
		WordTagset word_tagset;
		POSTagTagset postag_tagset;
		TwoWordsTagset two_words_tagset;
		POSTagSet2Tagset postag_set_2_tagset;

		PackedScoreType m_mapPackedScore;

		ScoredAction m_saScoredaction;

		void getActionScores(const StateItem & item);

		void update();
		void generate(DependencyCONLLGraph * retval, const DependencyCONLLGraph & correct);

		void getOrUpdateStackScore(const StateItem & item, const AddScoreType & amount);
		void updateScoreForState(const StateItem & from, const StateItem & output, const int & start_action_index, const int & amount);

		void reduce(const tscore & score);
		void shift(const tscore & score);
		void swap(const tscore & score);

		void arcSwap(const tscore & score);
		void arcShift(const tscore & score);
		void arcReduce(const tscore & score);

	public:
		DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, const bool & bChar, const bool & bPath, const bool & bLabel, int nState);
		~DepParser();

		void decode() override;
		void decodeArcs() override;

		void decode(const DependencyCONLLGraph & correct);

		void train(const DependencyCONLLGraph & correct, const int & round);
		void parse(const DependencyCONLLGraph & sentence, DependencyCONLLGraph * retval);
		void work(DependencyCONLLGraph * retval, const DependencyCONLLGraph & correct);
		void goldCheck(const DependencyCONLLGraph & correct);

		void printTime() {
//			std::cout << "total time tick is " << GetTickCount() - m_tStartTime << std::endl;
			std::cout << "total get score time is " << m_tGetScoreTime << std::endl;
			std::cout << "total update score time is " << m_tUpdateScoreTime << std::endl;
			std::cout << "total decode time is " << m_tDecodeTime << std::endl;
		}
	};

	extern int A_SW_FIRST;
	extern int A_SH_FIRST;
	extern int A_RE_FIRST;

	extern int A_SW_END;
	extern int A_SH_END;
	extern int A_RE_END;

	inline void DepParser::reduce(const tscore & score) {
		m_abScores.insertItem(ScoredAction(REDUCE, score + m_mapPackedScore[REDUCE]));
	}

	inline void DepParser::shift(const tscore & score) {
		m_abScores.insertItem(ScoredAction(SHIFT, score + m_mapPackedScore[SHIFT]));
	}

	inline void DepParser::swap(const tscore & score) {
		m_abScores.insertItem(ScoredAction(SWAP, score + m_mapPackedScore[SWAP]));
	}

	inline void DepParser::arcSwap(const tscore & score) {
		for (int action = A_SW_FIRST; action < A_SW_END; ++action) {
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
