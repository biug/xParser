#ifndef _TITOV_WEIGHT_H
#define _TITOV_WEIGHT_H

#include "titov_macros.h"
#include "common/parser/weight_base.h"
#include "include/learning/perceptron/packed_score.h"

#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace titov {
	class Weight : public WeightBase {
	public:

		// uni-gram
		// st, n0, st2
		WordMap m_mapSTw;
		POSTagMap m_mapSTpt;
		WordMap m_mapST2w;
		POSTagMap m_mapST2pt;
		WordMap m_mapN0w;
		POSTagMap m_mapN0pt;

		// unigram context
		// st + sti, n0 + n0i
		TwoWordsIntMap m_mapSTwSTiw;
		ThreeWordsIntMap m_mapSTwSTiwSTjw;
		TwoWordsIntMap m_mapN0wN0iw;
		ThreeWordsIntMap m_mapN0wN0iwN0jw;
		POSTagSet2IntMap m_mapSTptSTipt;
		POSTagSet3IntMap m_mapSTptSTiptSTjpt;
		POSTagSet2IntMap m_mapN0ptN0ipt;
		POSTagSet3IntMap m_mapN0ptN0iptN0jpt;

		// unigram with arity
		// st, n0, st2
		WordIntMap m_mapSTwSTLHi;
		WordIntMap m_mapSTwSTLPi;
		WordIntMap m_mapSTwSTRHi;
		WordIntMap m_mapSTwSTRPi;
		WordIntMap m_mapN0wN0LHi;
		WordIntMap m_mapN0wN0LPi;

		// bi-gram
		// st + n0
		TwoWordsMap m_mapSTwN0w;
		WordPOSTagMap m_mapSTwN0pt;
		WordPOSTagMap m_mapSTptN0w;
		POSTagSet2Map m_mapSTptN0pt;
		WordWordPOSTagMap m_mapSTwptN0w;
		WordWordPOSTagMap m_mapSTwN0wpt;
		WordPOSTagPOSTagMap m_mapSTwptN0pt;
		WordPOSTagPOSTagMap m_mapSTptN0wpt;
		WordWordPOSTagPOSTagMap m_mapSTwptN0wpt;
		// st + n0 + dis
		TwoWordsIntMap m_mapSTwN0wD;
		WordPOSTagIntMap m_mapSTwN0ptD;
		WordPOSTagIntMap m_mapSTptN0wD;
		POSTagSet2IntMap m_mapSTptN0ptD;
		// st + n0 + st left/right head/pred
		TwoWordsIntMap m_mapSTwN0wSTLHi;
		TwoWordsIntMap m_mapSTwN0wSTLPi;
		TwoWordsIntMap m_mapSTwN0wSTRHi;
		TwoWordsIntMap m_mapSTwN0wSTRPi;
		TwoWordsIntMap m_mapSTwN0wN0LHi;
		TwoWordsIntMap m_mapSTwN0wN0LPi;

		// st2 + n0
		TwoWordsMap m_mapST2wN0w;
		WordPOSTagMap m_mapST2wN0pt;
		WordPOSTagMap m_mapST2ptN0w;
		POSTagSet2Map m_mapST2ptN0pt;
		WordWordPOSTagMap m_mapST2wptN0w;
		WordWordPOSTagMap m_mapST2wN0wpt;
		WordPOSTagPOSTagMap m_mapST2wptN0pt;
		WordPOSTagPOSTagMap m_mapST2ptN0wpt;
		WordWordPOSTagPOSTagMap m_mapST2wptN0wpt;
		// st2 + n0 + dis
		TwoWordsIntMap m_mapST2wN0wD;
		WordPOSTagIntMap m_mapST2wN0ptD;
		WordPOSTagIntMap m_mapST2ptN0wD;
		POSTagSet2IntMap m_mapST2ptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		TwoWordsIntMap m_mapST2wN0wST2LHi;
		TwoWordsIntMap m_mapST2wN0wST2LPi;
		TwoWordsIntMap m_mapST2wN0wST2RHi;
		TwoWordsIntMap m_mapST2wN0wST2RPi;
		TwoWordsIntMap m_mapST2wN0wN0LHi;
		TwoWordsIntMap m_mapST2wN0wN0LPi;

		// st + st2
		TwoWordsMap m_mapSTwST2w;
		WordPOSTagMap m_mapSTwST2pt;
		WordPOSTagMap m_mapSTptST2w;
		POSTagSet2Map m_mapSTptST2pt;
		WordWordPOSTagMap m_mapSTwptST2w;
		WordWordPOSTagMap m_mapSTwST2wpt;
		WordPOSTagPOSTagMap m_mapSTwptST2pt;
		WordPOSTagPOSTagMap m_mapSTptST2wpt;
		WordWordPOSTagPOSTagMap m_mapSTwptST2wpt;

		// tri-gram
		// st + n0 + st2
		WordPOSTagPOSTagMap m_mapSTwN0ptST2pt;
		WordPOSTagPOSTagMap m_mapSTptN0wST2pt;
		WordPOSTagPOSTagMap m_mapSTptN0ptST2w;
		POSTagSet3Map m_mapSTptN0ptST2pt;

		// st + n0 + st left/right head/pred
		WordPOSTagPOSTagMap m_mapSTwN0ptSTLHpt;
		WordPOSTagPOSTagMap m_mapSTptN0wSTLHpt;
		WordPOSTagPOSTagMap m_mapSTptN0ptSTLHw;
		POSTagSet3Map m_mapSTptN0ptSTLHpt;
		WordPOSTagPOSTagMap m_mapSTwN0ptSTLPpt;
		WordPOSTagPOSTagMap m_mapSTptN0wSTLPpt;
		WordPOSTagPOSTagMap m_mapSTptN0ptSTLPw;
		POSTagSet3Map m_mapSTptN0ptSTLPpt;
		WordPOSTagPOSTagMap m_mapSTwN0ptSTRHpt;
		WordPOSTagPOSTagMap m_mapSTptN0wSTRHpt;
		WordPOSTagPOSTagMap m_mapSTptN0ptSTRHw;
		POSTagSet3Map m_mapSTptN0ptSTRHpt;
		WordPOSTagPOSTagMap m_mapSTwN0ptSTRPpt;
		WordPOSTagPOSTagMap m_mapSTptN0wSTRPpt;
		WordPOSTagPOSTagMap m_mapSTptN0ptSTRPw;
		POSTagSet3Map m_mapSTptN0ptSTRPpt;

		// st + n0 + n0 left head/pred
		WordPOSTagPOSTagMap m_mapSTwN0ptN0LHpt;
		WordPOSTagPOSTagMap m_mapSTptN0wN0LHpt;
		WordPOSTagPOSTagMap m_mapSTptN0ptN0LHw;
		POSTagSet3Map m_mapSTptN0ptN0LHpt;
		WordPOSTagPOSTagMap m_mapSTwN0ptN0LPpt;
		WordPOSTagPOSTagMap m_mapSTptN0wN0LPpt;
		WordPOSTagPOSTagMap m_mapSTptN0ptN0LPw;
		POSTagSet3Map m_mapSTptN0ptN0LPpt;

		// st + n0 + st left/right head/pred
		WordPOSTagPOSTagMap m_mapST2wN0ptST2LHpt;
		WordPOSTagPOSTagMap m_mapST2ptN0wST2LHpt;
		WordPOSTagPOSTagMap m_mapST2ptN0ptST2LHw;
		POSTagSet3Map m_mapST2ptN0ptST2LHpt;
		WordPOSTagPOSTagMap m_mapST2wN0ptST2LPpt;
		WordPOSTagPOSTagMap m_mapST2ptN0wST2LPpt;
		WordPOSTagPOSTagMap m_mapST2ptN0ptST2LPw;
		POSTagSet3Map m_mapST2ptN0ptST2LPpt;
		WordPOSTagPOSTagMap m_mapST2wN0ptST2RHpt;
		WordPOSTagPOSTagMap m_mapST2ptN0wST2RHpt;
		WordPOSTagPOSTagMap m_mapST2ptN0ptST2RHw;
		POSTagSet3Map m_mapST2ptN0ptST2RHpt;
		WordPOSTagPOSTagMap m_mapST2wN0ptST2RPpt;
		WordPOSTagPOSTagMap m_mapST2ptN0wST2RPpt;
		WordPOSTagPOSTagMap m_mapST2ptN0ptST2RPw;
		POSTagSet3Map m_mapST2ptN0ptST2RPpt;

		// st + n0 + n0 left head/pred
		WordPOSTagPOSTagMap m_mapST2wN0ptN0LHpt;
		WordPOSTagPOSTagMap m_mapST2ptN0wN0LHpt;
		WordPOSTagPOSTagMap m_mapST2ptN0ptN0LHw;
		POSTagSet3Map m_mapST2ptN0ptN0LHpt;
		WordPOSTagPOSTagMap m_mapST2wN0ptN0LPpt;
		WordPOSTagPOSTagMap m_mapST2ptN0wN0LPpt;
		WordPOSTagPOSTagMap m_mapST2ptN0ptN0LPw;
		POSTagSet3Map m_mapST2ptN0ptN0LPpt;

		// quar-gram
		// st + n0 + st left head + st left pred
		POSTagSet4Map m_mapSTptN0ptSTLHptSTLPpt;
		// st + n0 + st right head + st right pred
		POSTagSet4Map m_mapSTptN0ptSTRHptSTRPpt;
		// st + n0 + st left pred + st left pred 2
		POSTagSet4Map m_mapSTptN0ptSTLPptSTLP2pt;
		// st + n0 + st right pred + st right pred 2
		POSTagSet4Map m_mapSTptN0ptSTRPptSTRP2pt;
		// st + n0 + n0 left pred + n0 left pred 2
		POSTagSet4Map m_mapSTptN0ptN0LPptN0LP2pt;

		// char feature
		// for chinese
		StringMap m_map1CharBefore;
		StringMap m_map2CharBefore;
		StringMap m_map3CharBefore;
		StringMap m_map1CharAfter;
		StringMap m_map2CharAfter;
		StringMap m_map3CharAfter;

		// string feature
		StringMap m_mapSTPOSPath;
		StringMap m_mapST2POSPath;
		StringMap m_mapSTFPOSPath;
		StringMap m_mapST2FPOSPath;

	public:
		Weight(const std::string & sRead, const std::string & sRecord);
		~Weight();

		void loadScores() override;
		void saveScores() const override;
		void computeAverageFeatureWeights(const int & round) override;
	};
}

#endif
