#ifndef _MINITWOSTACK_WEIGHT_H
#define _MINITWOSTACK_WEIGHT_H

#include "minitwostack_macros.h"
#include "common/parser/weight_base.h"

#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace minitwostack {
	class Weight : public WeightBase {
	public:

		// uni-gram
		// st, n0, st2, sst
		// st
		WordMap m_mapSTw;
		POSTagMap m_mapSTpt;
		// n0
		WordMap m_mapN0w;
		POSTagMap m_mapN0pt;
		// st2
		WordMap m_mapST2w;
		POSTagMap m_mapST2pt;
		// sst
		WordMap m_mapSSTw;
		POSTagMap m_mapSSTpt;

		// unigram context
		// st, n0
		// st context
		POSTagIntMap m_mapSTipt;
		POSTagSet2IntMap m_mapSTiptSTjpt;
		// n0 context
		POSTagIntMap m_mapN0ipt;
		POSTagSet2IntMap m_mapN0iptN0jpt;

		// bi-gram
		// st + n0
		TwoWordsMap m_mapSTwN0w;
		WordPOSTagMap m_mapSTwN0pt;
		WordPOSTagMap m_mapSTptN0w;
		POSTagSet2Map m_mapSTptN0pt;
		// st + n0 + dis
		POSTagSet2IntMap m_mapSTptN0ptD;
		// st + n0 + st left/right head/pred
		POSTagSet2IntMap m_mapSTptN0ptSTLPi;
		POSTagSet2IntMap m_mapSTptN0ptSTRPi;
		POSTagSet2IntMap m_mapSTptN0ptN0LPi;

		// st2 + n0
		TwoWordsMap m_mapST2wN0w;
		WordPOSTagMap m_mapST2wN0pt;
		WordPOSTagMap m_mapST2ptN0w;
		POSTagSet2Map m_mapST2ptN0pt;
		// st2 + n0 + dis
		POSTagSet2IntMap m_mapST2ptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		POSTagSet2IntMap m_mapST2ptN0ptST2LPi;
		POSTagSet2IntMap m_mapST2ptN0ptST2RPi;
		POSTagSet2IntMap m_mapST2ptN0ptN0LPi;

		// sst + n0
		TwoWordsMap m_mapSSTwN0w;
		WordPOSTagMap m_mapSSTwN0pt;
		WordPOSTagMap m_mapSSTptN0w;
		POSTagSet2Map m_mapSSTptN0pt;
		// sst + n0 + dis
		POSTagSet2IntMap m_mapSSTptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		POSTagSet2IntMap m_mapSSTptN0ptSSTLPi;
		POSTagSet2IntMap m_mapSSTptN0ptSSTRPi;
		POSTagSet2IntMap m_mapSSTptN0ptN0LPi;

		// st + st2
		TwoWordsMap m_mapSTwST2w;
		WordPOSTagMap m_mapSTwST2pt;
		WordPOSTagMap m_mapSTptST2w;
		POSTagSet2Map m_mapSTptST2pt;

		// st + sst
		TwoWordsMap m_mapSTwSSTw;
		WordPOSTagMap m_mapSTwSSTpt;
		WordPOSTagMap m_mapSTptSSTw;
		POSTagSet2Map m_mapSTptSSTpt;

		// tri-gram
		// st + n0 + st2
		POSTagSet3Map m_mapSTptN0ptST2pt;
		// st + n0 + sst
		POSTagSet3Map m_mapSTptN0ptSSTpt;

		// st + n0 + st left/right head/pred
		POSTagSet3IntMap m_mapSTptN0ptSTLHptSTLHl;
		POSTagSet3IntMap m_mapSTptN0ptSTLPptSTLPl;
		POSTagSet3IntMap m_mapSTptN0ptSTRHptSTRHl;
		POSTagSet3IntMap m_mapSTptN0ptSTRPptSTRPl;

		// st + n0 + n0 left head/pred
		POSTagSet3IntMap m_mapSTptN0ptN0LHptN0LHl;
		POSTagSet3IntMap m_mapSTptN0ptN0LPptN0LPl;

		// quar-gram
		// st + n0 + st right head + st right pred
		POSTagSet4TwoIntsMap m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl;
		// st + n0 + st left pred + st left pred 2
		POSTagSet4TwoIntsMap m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + n0 + st right pred + st right pred 2
		POSTagSet4TwoIntsMap m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + n0 + n0 left head + n0 left pred
		POSTagSet4TwoIntsMap m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st + n0 + n0 left pred + n0 left pred 2
		POSTagSet4TwoIntsMap m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st2 + n0 + st2 left/right head/pred
		POSTagSet3IntMap m_mapST2ptN0ptST2LHptST2LHl;
		POSTagSet3IntMap m_mapST2ptN0ptST2LPptST2LPl;
		POSTagSet3IntMap m_mapST2ptN0ptST2RHptST2RHl;
		POSTagSet3IntMap m_mapST2ptN0ptST2RPptST2RPl;

		// st + n0 + n0 left head/pred
		POSTagSet3IntMap m_mapST2ptN0ptN0LHptN0LHl;
		POSTagSet3IntMap m_mapST2ptN0ptN0LPptN0LPl;

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		POSTagSet4TwoIntsMap m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl;
		// st2 + n0 + st2 left pred + st2 left pred 2
		POSTagSet4TwoIntsMap m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l;
		// st2 + n0 + st2 right pred + st2 right pred 2
		POSTagSet4TwoIntsMap m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l;
		// st2 + n0 + n0 left head + n0 left pred
		POSTagSet4TwoIntsMap m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st2 + n0 + n0 left pred + n0 left pred 2
		POSTagSet4TwoIntsMap m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// sst + n0 + sst left/right head/pred
		POSTagSet3IntMap m_mapSSTptN0ptSSTLHptSSTLHl;
		POSTagSet3IntMap m_mapSSTptN0ptSSTLPptSSTLPl;
		POSTagSet3IntMap m_mapSSTptN0ptSSTRHptSSTRHl;
		POSTagSet3IntMap m_mapSSTptN0ptSSTRPptSSTRPl;

		// st + n0 + n0 left head/pred
		POSTagSet3IntMap m_mapSSTptN0ptN0LHptN0LHl;
		POSTagSet3IntMap m_mapSSTptN0ptN0LPptN0LPl;

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		POSTagSet4TwoIntsMap m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl;
		// sst + n0 + sst left pred + sst left pred 2
		POSTagSet4TwoIntsMap m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l;
		// sst + n0 + sst right pred + sst right pred 2
		POSTagSet4TwoIntsMap m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l;
		// sst + n0 + n0 left head + n0 left pred
		POSTagSet4TwoIntsMap m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// sst + n0 + n0 left pred + n0 left pred 2
		POSTagSet4TwoIntsMap m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st + n0 + label set
		POSTagSet2TagsetMap m_mapSTptN0ptSTll;
		POSTagSet2TagsetMap m_mapSTptN0ptSTrl;
		POSTagSet2TagsetMap m_mapSTptN0ptN0ll;
		// st2 + n0 + label set
		POSTagSet2TagsetMap m_mapST2ptN0ptST2ll;
		POSTagSet2TagsetMap m_mapST2ptN0ptST2rl;
		POSTagSet2TagsetMap m_mapST2ptN0ptN0ll;
		// sst + n0 + label set
		POSTagSet2TagsetMap m_mapSSTptN0ptSSTll;
		POSTagSet2TagsetMap m_mapSSTptN0ptSSTrl;
		POSTagSet2TagsetMap m_mapSSTptN0ptN0ll;

		// char feature (for chinese)
		// st context char
		StringMap m_map1CharBeforeST;
		StringMap m_map2CharBeforeST;
		StringMap m_map3CharBeforeST;
		StringMap m_map1CharAfterST;
		StringMap m_map2CharAfterST;
		StringMap m_map3CharAfterST;
		// n0 context char
		StringMap m_map1CharBeforeN0;
		StringMap m_map2CharBeforeN0;
		StringMap m_map3CharBeforeN0;
		StringMap m_map1CharAfterN0;
		StringMap m_map2CharAfterN0;
		StringMap m_map3CharAfterN0;

		// path feature
		// st - n0
		StringMap m_mapSTPOSPath;
		StringMap m_mapSTFPOSPath;
		WordWordPOSTagMap m_mapSTwN0wSTsynhpt;
		WordWordPOSTagMap m_mapSTwN0wN0synhpt;
		// st2 - n0
		StringMap m_mapST2POSPath;
		StringMap m_mapST2FPOSPath;
		WordWordPOSTagMap m_mapST2wN0wST2synhpt;
		WordWordPOSTagMap m_mapST2wN0wN0synhpt;
		// sst - n0
		StringMap m_mapSSTPOSPath;
		StringMap m_mapSSTFPOSPath;
		WordWordPOSTagMap m_mapSSTwN0wSSTsynhpt;
		WordWordPOSTagMap m_mapSSTwN0wN0synhpt;

	public:
		Weight(const std::string & sRead, const std::string & sRecord);
		~Weight();

		void loadScores() override;
		void saveScores() const override;
		void computeAverageFeatureWeights(const int & round) override;
	};
}

#endif
