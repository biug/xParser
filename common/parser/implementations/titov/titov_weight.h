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
		WordIntMap m_mapSTiw;
		TwoWordsIntMap m_mapSTiwSTjw;
		WordIntMap m_mapN0iw;
		TwoWordsIntMap m_mapN0iwN0jw;
		POSTagIntMap m_mapSTipt;
		POSTagSet2IntMap m_mapSTiptSTjpt;
		POSTagIntMap m_mapN0ipt;
		POSTagSet2IntMap m_mapN0iptN0jpt;

		// unigram with label
		// st, n0
		WordIntMap m_mapSTwSTLHl;
		WordIntMap m_mapSTwSTLPl;
		WordIntMap m_mapSTwSTRHl;
		WordIntMap m_mapSTwSTRPl;
		WordIntMap m_mapN0wN0LHl;
		WordIntMap m_mapN0wN0LPl;

		// unigram with arity
		// st, n0
		WordIntMap m_mapSTwSTLHi;
		WordIntMap m_mapSTwSTLPi;
		WordIntMap m_mapSTwSTRHi;
		WordIntMap m_mapSTwSTRPi;
		WordIntMap m_mapSTwSTHi;
		WordIntMap m_mapSTwSTPi;
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
		TwoWordsIntMap m_mapSTwN0wSTRPi;
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
		WordPOSTagPOSTagIntMap m_mapSTwN0ptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap m_mapSTptN0wSTLHptSTLHl;
		WordPOSTagPOSTagIntMap m_mapSTptN0ptSTLHwSTLHl;
		POSTagSet3IntMap m_mapSTptN0ptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap m_mapSTwN0ptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap m_mapSTptN0wSTLPptSTLPl;
		WordPOSTagPOSTagIntMap m_mapSTptN0ptSTLPwSTLPl;
		POSTagSet3IntMap m_mapSTptN0ptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap m_mapSTwN0ptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap m_mapSTptN0wSTRHptSTRHl;
		WordPOSTagPOSTagIntMap m_mapSTptN0ptSTRHwSTRHl;
		POSTagSet3IntMap m_mapSTptN0ptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap m_mapSTwN0ptSTRPptSTRPl;
		WordPOSTagPOSTagIntMap m_mapSTptN0wSTRPptSTRPl;
		WordPOSTagPOSTagIntMap m_mapSTptN0ptSTRPwSTRPl;
		POSTagSet3IntMap m_mapSTptN0ptSTRPptSTRPl;

		// st + n0 + n0 left head/pred
		WordPOSTagPOSTagIntMap m_mapSTwN0ptN0LHptN0LHl;
		WordPOSTagPOSTagIntMap m_mapSTptN0wN0LHptN0LHl;
		WordPOSTagPOSTagIntMap m_mapSTptN0ptN0LHwN0LHl;
		POSTagSet3IntMap m_mapSTptN0ptN0LHptN0LHl;
		WordPOSTagPOSTagIntMap m_mapSTwN0ptN0LPptN0LPl;
		WordPOSTagPOSTagIntMap m_mapSTptN0wN0LPptN0LPl;
		WordPOSTagPOSTagIntMap m_mapSTptN0ptN0LPwN0LPl;
		POSTagSet3IntMap m_mapSTptN0ptN0LPptN0LPl;

		// quar-gram
		// st + n0 + st right head + st right pred
		POSTagSet4Map m_mapSTptN0ptSTRHptSTRPpt;
		// st + n0 + st left pred + st left pred 2
		POSTagSet4Map m_mapSTptN0ptSTLPptSTLP2pt;
		// st + n0 + st right pred + st right pred 2
		POSTagSet4Map m_mapSTptN0ptSTRPptSTRP2pt;
		// st + n0 + n0 left pred + n0 left pred 2
		POSTagSet4Map m_mapSTptN0ptN0LPptN0LP2pt;

		WordTagsetMap m_mapSTwSTll;
		WordTagsetMap m_mapSTwSTrl;
		WordTagsetMap m_mapN0wN0ll;
		POSTagTagsetMap m_mapSTptSTll;
		POSTagTagsetMap m_mapSTptSTrl;
		POSTagTagsetMap m_mapN0ptN0ll;
		POSTagSet2TagsetMap m_mapSTptN0ptSTll;
		POSTagSet2TagsetMap m_mapSTptN0ptSTrl;
		POSTagSet2TagsetMap m_mapSTptN0ptN0ll;

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
