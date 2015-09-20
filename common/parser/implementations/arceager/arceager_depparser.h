#ifndef _ARCEAGER_DEPPARSER_H
#define _ARCEAGER_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "arceager_state.h"
#include "arceager_weight.h"
#include "common/parser/depparser_base.h"

namespace arceager {

	class DepParser : public DepParserBase {
	private:
		double m_tDecodeTime;
		double m_tGetScoreTime;
		double m_tUpdateScoreTime;
		double m_tStartTime;

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

		PackedScoreType m_mapPackedScore;

		ScoredAction m_saScoredaction;

		void getActionScores(const StateItem & item);

		void update();
		void generate(DependencyTree * retval, const DependencyTree & correct);

		void getOrUpdateStackScore(const StateItem & item, const AddScoreType & amount);
		void updateScoreForState(const StateItem & from, const StateItem & output, const int & amount);

		void reduce(const tscore & score);
		void arcLeft(const tscore & score);
		void arcRight(const tscore & score);
		void shift(const tscore & score);
		void popRoot(const tscore & score);

	public:
		DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, int nState);
		~DepParser();

		void decode() override;
		void decodeArcs() override;

		void decode(const DependencyTree & correct);

		void train(const DependencyTree & correct, const int & round);
		void parse(const Sentence & sentence, DependencyTree * retval);
		void work(DependencyTree * retval, const DependencyTree & correct);
		void goldCheck(const DependencyTree & correct);

		void printTime() {
//			std::cout << "total time tick is " << GetTickCount() - m_tStartTime << std::endl;
			std::cout << "total get score time is " << m_tGetScoreTime << std::endl;
			std::cout << "total update score time is " << m_tUpdateScoreTime << std::endl;
			std::cout << "total decode time is " << m_tDecodeTime << std::endl;
		}
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
