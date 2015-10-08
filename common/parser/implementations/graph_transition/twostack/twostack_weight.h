#ifndef _TWOSTACK_WEIGHT_H
#define _TWOSTACK_WEIGHT_H

#include "twostack_macros.h"
#include "common/parser/weight_base.h"
#include "include/learning/perceptron/packed_score.h"

#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

extern SuperTagCandidates g_mapSuperTagCandidatesOfWords;
extern SuperTagCandidates g_mapSuperTagCandidatesOfPOSTags;

namespace twostack {
	template<class RET_TYPE>
	class Weight : public WeightBase {
	public:
		// uni-gram
		// st, n0, st2, sst
		// st
		WordMap<RET_TYPE> m_mapSTw;
		POSTagMap<RET_TYPE> m_mapSTpt;
		// n0
		WordMap<RET_TYPE> m_mapN0w;
		POSTagMap<RET_TYPE> m_mapN0pt;
		// st2
		WordMap<RET_TYPE> m_mapST2w;
		POSTagMap<RET_TYPE> m_mapST2pt;
		// sst
		WordMap<RET_TYPE> m_mapSSTw;
		POSTagMap<RET_TYPE> m_mapSSTpt;

		// unigram context
		// st, n0
		// st context
		WordIntMap<RET_TYPE> m_mapSTiw;
		TwoWordsIntMap<RET_TYPE> m_mapSTiwSTjw;
		POSTagIntMap<RET_TYPE> m_mapSTipt;
		POSTagSet2IntMap<RET_TYPE> m_mapSTiptSTjpt;
		// n0 context
		WordIntMap<RET_TYPE> m_mapN0iw;
		TwoWordsIntMap<RET_TYPE> m_mapN0iwN0jw;
		POSTagIntMap<RET_TYPE> m_mapN0ipt;
		POSTagSet2IntMap<RET_TYPE> m_mapN0iptN0jpt;

		// unigram with label
		// st, n0, st2, sst
		// st
		WordIntMap<RET_TYPE> m_mapSTwSTLHl;
		WordIntMap<RET_TYPE> m_mapSTwSTLPl;
		WordIntMap<RET_TYPE> m_mapSTwSTRHl;
		WordIntMap<RET_TYPE> m_mapSTwSTRPl;
		// n0
		WordIntMap<RET_TYPE> m_mapN0wN0LHl;
		WordIntMap<RET_TYPE> m_mapN0wN0LPl;
		POSTagIntMap<RET_TYPE> m_mapN0ptN0LHl;
		POSTagIntMap<RET_TYPE> m_mapN0ptN0LPl;
		// st2
		WordIntMap<RET_TYPE> m_mapST2wST2LHl;
		WordIntMap<RET_TYPE> m_mapST2wST2LPl;
		WordIntMap<RET_TYPE> m_mapST2wST2RHl;
		WordIntMap<RET_TYPE> m_mapST2wST2RPl;
		// sst
		WordIntMap<RET_TYPE> m_mapSSTwSSTLHl;
		WordIntMap<RET_TYPE> m_mapSSTwSSTLPl;
		WordIntMap<RET_TYPE> m_mapSSTwSSTRHl;
		WordIntMap<RET_TYPE> m_mapSSTwSSTRPl;

		// unigram with arity
		// st, n0, st2, sst
		// st
		WordIntMap<RET_TYPE> m_mapSTwSTLHi;
		WordIntMap<RET_TYPE> m_mapSTwSTLPi;
		WordIntMap<RET_TYPE> m_mapSTwSTRHi;
		WordIntMap<RET_TYPE> m_mapSTwSTRPi;
		WordIntMap<RET_TYPE> m_mapSTwSTHi;
		WordIntMap<RET_TYPE> m_mapSTwSTPi;
		// n0
		WordIntMap<RET_TYPE> m_mapN0wN0LHi;
		WordIntMap<RET_TYPE> m_mapN0wN0LPi;
		POSTagIntMap<RET_TYPE> m_mapN0ptN0LHi;
		POSTagIntMap<RET_TYPE> m_mapN0ptN0LPi;
		// st2
		WordIntMap<RET_TYPE> m_mapST2wST2LHi;
		WordIntMap<RET_TYPE> m_mapST2wST2LPi;
		WordIntMap<RET_TYPE> m_mapST2wST2RHi;
		WordIntMap<RET_TYPE> m_mapST2wST2RPi;
		WordIntMap<RET_TYPE> m_mapST2wST2Hi;
		WordIntMap<RET_TYPE> m_mapST2wST2Pi;
		// sst
		WordIntMap<RET_TYPE> m_mapSSTwSSTLHi;
		WordIntMap<RET_TYPE> m_mapSSTwSSTLPi;
		WordIntMap<RET_TYPE> m_mapSSTwSSTRHi;
		WordIntMap<RET_TYPE> m_mapSSTwSSTRPi;
		WordIntMap<RET_TYPE> m_mapSSTwSSTHi;
		WordIntMap<RET_TYPE> m_mapSSTwSSTPi;

		// bi-gram
		// st + n0
		TwoWordsMap<RET_TYPE> m_mapSTwN0w;
		WordPOSTagMap<RET_TYPE> m_mapSTwN0pt;
		WordPOSTagMap<RET_TYPE> m_mapSTptN0w;
		POSTagSet2Map<RET_TYPE> m_mapSTptN0pt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwptN0w;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwN0wpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptN0pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptN0wpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptN0wpt;
		// st + n0 + dis
		TwoWordsIntMap<RET_TYPE> m_mapSTwN0wD;
		WordPOSTagIntMap<RET_TYPE> m_mapSTwN0ptD;
		WordPOSTagIntMap<RET_TYPE> m_mapSTptN0wD;
		POSTagSet2IntMap<RET_TYPE> m_mapSTptN0ptD;
		// st + n0 + st left/right head/pred
		TwoWordsIntMap<RET_TYPE> m_mapSTwN0wSTLPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSTwN0ptSTLPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSTptN0wSTLPi;
		POSTagSet2IntMap<RET_TYPE> m_mapSTptN0ptSTLPi;
		TwoWordsIntMap<RET_TYPE> m_mapSTwN0wSTRPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSTwN0ptSTRPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSTptN0wSTRPi;
		POSTagSet2IntMap<RET_TYPE> m_mapSTptN0ptSTRPi;
		TwoWordsIntMap<RET_TYPE> m_mapSTwN0wN0LPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSTwN0ptN0LPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSTptN0wN0LPi;
		POSTagSet2IntMap<RET_TYPE> m_mapSTptN0ptN0LPi;

		// st2 + n0
		TwoWordsMap<RET_TYPE> m_mapST2wN0w;
		WordPOSTagMap<RET_TYPE> m_mapST2wN0pt;
		WordPOSTagMap<RET_TYPE> m_mapST2ptN0w;
		POSTagSet2Map<RET_TYPE> m_mapST2ptN0pt;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wptN0w;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wN0wpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapST2wptN0pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapST2ptN0wpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapST2wptN0wpt;
		// st2 + n0 + dis
		TwoWordsIntMap<RET_TYPE> m_mapST2wN0wD;
		WordPOSTagIntMap<RET_TYPE> m_mapST2wN0ptD;
		WordPOSTagIntMap<RET_TYPE> m_mapST2ptN0wD;
		POSTagSet2IntMap<RET_TYPE> m_mapST2ptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		TwoWordsIntMap<RET_TYPE> m_mapST2wN0wST2LPi;
		WordPOSTagIntMap<RET_TYPE> m_mapST2wN0ptST2LPi;
		WordPOSTagIntMap<RET_TYPE> m_mapST2ptN0wST2LPi;
		POSTagSet2IntMap<RET_TYPE> m_mapST2ptN0ptST2LPi;
		TwoWordsIntMap<RET_TYPE> m_mapST2wN0wST2RPi;
		WordPOSTagIntMap<RET_TYPE> m_mapST2wN0ptST2RPi;
		WordPOSTagIntMap<RET_TYPE> m_mapST2ptN0wST2RPi;
		POSTagSet2IntMap<RET_TYPE> m_mapST2ptN0ptST2RPi;
		TwoWordsIntMap<RET_TYPE> m_mapST2wN0wN0LPi;
		WordPOSTagIntMap<RET_TYPE> m_mapST2wN0ptN0LPi;
		WordPOSTagIntMap<RET_TYPE> m_mapST2ptN0wN0LPi;
		POSTagSet2IntMap<RET_TYPE> m_mapST2ptN0ptN0LPi;

		// sst + n0
		TwoWordsMap<RET_TYPE> m_mapSSTwN0w;
		WordPOSTagMap<RET_TYPE> m_mapSSTwN0pt;
		WordPOSTagMap<RET_TYPE> m_mapSSTptN0w;
		POSTagSet2Map<RET_TYPE> m_mapSSTptN0pt;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwptN0w;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwN0wpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSSTwptN0pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSSTptN0wpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapSSTwptN0wpt;
		// sst + n0 + dis
		TwoWordsIntMap<RET_TYPE> m_mapSSTwN0wD;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptD;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTptN0wD;
		POSTagSet2IntMap<RET_TYPE> m_mapSSTptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		TwoWordsIntMap<RET_TYPE> m_mapSSTwN0wSSTLPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptSSTLPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTptN0wSSTLPi;
		POSTagSet2IntMap<RET_TYPE> m_mapSSTptN0ptSSTLPi;
		TwoWordsIntMap<RET_TYPE> m_mapSSTwN0wSSTRPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptSSTRPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTptN0wSSTRPi;
		POSTagSet2IntMap<RET_TYPE> m_mapSSTptN0ptSSTRPi;
		TwoWordsIntMap<RET_TYPE> m_mapSSTwN0wN0LPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptN0LPi;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTptN0wN0LPi;
		POSTagSet2IntMap<RET_TYPE> m_mapSSTptN0ptN0LPi;

		// st + st2
		TwoWordsMap<RET_TYPE> m_mapSTwST2w;
		WordPOSTagMap<RET_TYPE> m_mapSTwST2pt;
		WordPOSTagMap<RET_TYPE> m_mapSTptST2w;
		POSTagSet2Map<RET_TYPE> m_mapSTptST2pt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwptST2w;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwST2wpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptST2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptST2wpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptST2wpt;

		// st + sst
		TwoWordsMap<RET_TYPE> m_mapSTwSSTw;
		WordPOSTagMap<RET_TYPE> m_mapSTwSSTpt;
		WordPOSTagMap<RET_TYPE> m_mapSTptSSTw;
		POSTagSet2Map<RET_TYPE> m_mapSTptSSTpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwptSSTw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwSSTwpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptSSTpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptSSTwpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptSSTwpt;

		// tri-gram
		// st + n0 + st2
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwN0ptST2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptN0wST2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptN0ptST2w;
		POSTagSet3Map<RET_TYPE> m_mapSTptN0ptST2pt;
		// st + n0 + sst
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwN0ptSSTpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptN0wSSTpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptN0ptSSTw;
		POSTagSet3Map<RET_TYPE> m_mapSTptN0ptSSTpt;

		// st + n0 + st left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwN0ptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0wSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0ptSTLHwSTLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptN0ptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwN0ptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0wSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0ptSTLPwSTLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptN0ptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwN0ptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0wSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0ptSTRHwSTRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptN0ptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwN0ptSTRPptSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0wSTRPptSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0ptSTRPwSTRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptN0ptSTRPptSTRPl;

		// st + n0 + n0 left head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwN0ptN0LHptN0LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0wN0LHptN0LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0ptN0LHwN0LHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptN0ptN0LHptN0LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwN0ptN0LPptN0LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0wN0LPptN0LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptN0ptN0LPwN0LPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptN0ptN0LPptN0LPl;

		// quar-gram
		// st + n0 + st right head + st right pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl;
		// st + n0 + st left pred + st left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + n0 + st right pred + st right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + n0 + n0 left head + n0 left pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st + n0 + n0 left pred + n0 left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st2 + n0 + st2 left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wN0ptST2LHptST2LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0wST2LHptST2LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0ptST2LHwST2LHl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptN0ptST2LHptST2LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wN0ptST2LPptST2LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0wST2LPptST2LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0ptST2LPwST2LPl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptN0ptST2LPptST2LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wN0ptST2RHptST2RHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0wST2RHptST2RHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0ptST2RHwST2RHl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptN0ptST2RHptST2RHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wN0ptST2RPptST2RPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0wST2RPptST2RPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0ptST2RPwST2RPl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptN0ptST2RPptST2RPl;

		// st + n0 + n0 left head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wN0ptN0LHptN0LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0wN0LHptN0LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0ptN0LHwN0LHl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptN0ptN0LHptN0LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wN0ptN0LPptN0LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0wN0LPptN0LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptN0ptN0LPwN0LPl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptN0ptN0LPptN0LPl;

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl;
		// st2 + n0 + st2 left pred + st2 left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l;
		// st2 + n0 + st2 right pred + st2 right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l;
		// st2 + n0 + n0 left head + n0 left pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st2 + n0 + n0 left pred + n0 left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// sst + n0 + sst left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptSSTLHptSSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0wSSTLHptSSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0ptSSTLHwSSTLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptN0ptSSTLHptSSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptSSTLPptSSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0wSSTLPptSSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0ptSSTLPwSSTLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptN0ptSSTLPptSSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptSSTRHptSSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0wSSTRHptSSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0ptSSTRHwSSTRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptN0ptSSTRHptSSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptSSTRPptSSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0wSSTRPptSSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0ptSSTRPwSSTRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptN0ptSSTRPptSSTRPl;

		// st + n0 + n0 left head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptN0LHptN0LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0wN0LHptN0LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0ptN0LHwN0LHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptN0ptN0LHptN0LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwN0ptN0LPptN0LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0wN0LPptN0LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptN0ptN0LPwN0LPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptN0ptN0LPptN0LPl;

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl;
		// sst + n0 + sst left pred + sst left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l;
		// sst + n0 + sst right pred + sst right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l;
		// sst + n0 + n0 left head + n0 left pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// sst + n0 + n0 left pred + n0 left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st + n0 + label set
		WordTagsetMap<RET_TYPE> m_mapSTwSTll;
		WordTagsetMap<RET_TYPE> m_mapSTwSTrl;
		WordTagsetMap<RET_TYPE> m_mapN0wN0ll;
		POSTagTagsetMap<RET_TYPE> m_mapSTptSTll;
		POSTagTagsetMap<RET_TYPE> m_mapSTptSTrl;
		POSTagTagsetMap<RET_TYPE> m_mapN0ptN0ll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptN0ptSTll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptN0ptSTrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptN0ptN0ll;
		// st2 + n0 + label set
		WordTagsetMap<RET_TYPE> m_mapST2wST2ll;
		WordTagsetMap<RET_TYPE> m_mapST2wST2rl;
		POSTagTagsetMap<RET_TYPE> m_mapST2ptST2ll;
		POSTagTagsetMap<RET_TYPE> m_mapST2ptST2rl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapST2ptN0ptST2ll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapST2ptN0ptST2rl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapST2ptN0ptN0ll;
		// sst + n0 + label set
		WordTagsetMap<RET_TYPE> m_mapSSTwSSTll;
		WordTagsetMap<RET_TYPE> m_mapSSTwSSTrl;
		POSTagTagsetMap<RET_TYPE> m_mapSSTptSSTll;
		POSTagTagsetMap<RET_TYPE> m_mapSSTptSSTrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSSTptN0ptSSTll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSSTptN0ptSSTrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSSTptN0ptN0ll;

		// char feature (for chinese)
		// st context char
		StringMap<RET_TYPE> m_map1CharBeforeST;
		StringMap<RET_TYPE> m_map2CharBeforeST;
		StringMap<RET_TYPE> m_map3CharBeforeST;
		StringMap<RET_TYPE> m_map1CharAfterST;
		StringMap<RET_TYPE> m_map2CharAfterST;
		StringMap<RET_TYPE> m_map3CharAfterST;
		// n0 context char
		StringMap<RET_TYPE> m_map1CharBeforeN0;
		StringMap<RET_TYPE> m_map2CharBeforeN0;
		StringMap<RET_TYPE> m_map3CharBeforeN0;
		StringMap<RET_TYPE> m_map1CharAfterN0;
		StringMap<RET_TYPE> m_map2CharAfterN0;
		StringMap<RET_TYPE> m_map3CharAfterN0;

		// path feature
		// st - n0
		StringMap<RET_TYPE> m_mapSTPOSPath;
		StringMap<RET_TYPE> m_mapSTFPOSPath;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwN0ptSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwN0wSTsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTptN0wSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwN0ptN0synhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwN0wN0synhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTptN0wN0synhw;
		// st2 - n0
		StringMap<RET_TYPE> m_mapST2POSPath;
		StringMap<RET_TYPE> m_mapST2FPOSPath;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wN0ptST2synhw;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wN0wST2synhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapST2ptN0wST2synhw;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wN0ptN0synhw;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wN0wN0synhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapST2ptN0wN0synhw;
		// sst - n0
		StringMap<RET_TYPE> m_mapSSTPOSPath;
		StringMap<RET_TYPE> m_mapSSTFPOSPath;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwN0ptSSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwN0wSSTsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTptN0wSSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwN0ptN0synhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwN0wN0synhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTptN0wN0synhw;

		// supertag feature
		SuperTagMap<RET_TYPE> m_mapSTst;
		SuperTagMap<RET_TYPE> m_mapST2st;
		SuperTagMap<RET_TYPE> m_mapSSTst;
		SuperTagIntMap<RET_TYPE> m_mapN0ist;
		SuperTagSet2Map<RET_TYPE> m_mapSTstST2st;
		SuperTagSet2Map<RET_TYPE> m_mapSTstSSTst;
		WordSuperTagMap<RET_TYPE> m_mapSTstN0w;
		POSTagSuperTagMap<RET_TYPE> m_mapSTstN0pt;
		WordPOSTagSuperTagMap<RET_TYPE> m_mapSTstN0wpt;

	public:
		Weight(const std::string & sRead, const std::string & sRecord);
		~Weight();

		void loadScores() override;
		void saveScores() const override;
		void computeAverageFeatureWeights(const int & round) override;
	};

	template<class RET_TYPE>
	Weight<RET_TYPE>::Weight(const std::string & sRead, const std::string & sRecord) :
		WeightBase(sRead, sRecord),
		// uni-gram
		// st, n0, st2, sst
		// st
		m_mapSTw("m_mapSTw"),
		m_mapSTpt("m_mapSTpt"),
		// n0
		m_mapN0w("m_mapN0w"),
		m_mapN0pt("m_mapN0pt"),
		// st2
		m_mapST2w("m_mapST2w"),
		m_mapST2pt("m_mapST2pt"),
		// sst
		m_mapSSTw("m_mapSSTw"),
		m_mapSSTpt("m_mapSSTpt"),

		// unigram context
		// st, n0
		// st context
		m_mapSTiw("m_mapSTiw"),
		m_mapSTiwSTjw("m_mapSTiwSTjw"),
		m_mapSTipt("m_mapSTipt"),
		m_mapSTiptSTjpt("m_mapSTiptSTjpt"),
		// n0 context
		m_mapN0iw("m_mapN0iw"),
		m_mapN0iwN0jw("m_mapN0iwN0jw"),
		m_mapN0ipt("m_mapN0ipt"),
		m_mapN0iptN0jpt("m_mapN0iptN0jpt"),

		// unigram with label
		// st, n0, st2, sst
		// st
		m_mapSTwSTLHl("m_mapSTwSTLHl"),
		m_mapSTwSTLPl("m_mapSTwSTLPl"),
		m_mapSTwSTRHl("m_mapSTwSTRHl"),
		m_mapSTwSTRPl("m_mapSTwSTRPl"),
		// n0
		m_mapN0wN0LHl("m_mapN0wN0LHl"),
		m_mapN0wN0LPl("m_mapN0wN0LPl"),
		m_mapN0ptN0LHl("m_mapN0ptN0LHl"),
		m_mapN0ptN0LPl("m_mapN0ptN0LPl"),
		// st2
		m_mapST2wST2LHl("m_mapST2wST2LHl"),
		m_mapST2wST2LPl("m_mapST2wST2LPl"),
		m_mapST2wST2RHl("m_mapST2wST2RHl"),
		m_mapST2wST2RPl("m_mapST2wST2RPl"),
		// sst
		m_mapSSTwSSTLHl("m_mapSSTwSSTLHl"),
		m_mapSSTwSSTLPl("m_mapSSTwSSTLPl"),
		m_mapSSTwSSTRHl("m_mapSSTwSSTRHl"),
		m_mapSSTwSSTRPl("m_mapSSTwSSTRPl"),

		// unigram with arity
		// st, n0, st2, sst
		// st
		m_mapSTwSTLHi("m_mapSTwSTLHi"),
		m_mapSTwSTLPi("m_mapSTwSTLPi"),
		m_mapSTwSTRHi("m_mapSTwSTRHi"),
		m_mapSTwSTRPi("m_mapSTwSTRPi"),
		m_mapSTwSTHi("m_mapSTwSTHi"),
		m_mapSTwSTPi("m_mapSTwSTPi"),
		// n0
		m_mapN0wN0LHi("m_mapN0wN0LHi"),
		m_mapN0wN0LPi("m_mapN0wN0LPi"),
		m_mapN0ptN0LHi("m_mapN0ptN0LHi"),
		m_mapN0ptN0LPi("m_mapN0ptN0LPi"),
		// st2
		m_mapST2wST2LHi("m_mapST2wST2LHi"),
		m_mapST2wST2LPi("m_mapST2wST2LPi"),
		m_mapST2wST2RHi("m_mapST2wST2RHi"),
		m_mapST2wST2RPi("m_mapST2wST2RPi"),
		m_mapST2wST2Hi("m_mapST2wST2Hi"),
		m_mapST2wST2Pi("m_mapST2wST2Pi"),
		// sst
		m_mapSSTwSSTLHi("m_mapSSTwSSTLHi"),
		m_mapSSTwSSTLPi("m_mapSSTwSSTLPi"),
		m_mapSSTwSSTRHi("m_mapSSTwSSTRHi"),
		m_mapSSTwSSTRPi("m_mapSSTwSSTRPi"),
		m_mapSSTwSSTHi("m_mapSSTwSSTHi"),
		m_mapSSTwSSTPi("m_mapSSTwSSTPi"),

		// bi-gram
		// st + n0
		m_mapSTwN0w("m_mapSTwN0w"),
		m_mapSTwN0pt("m_mapSTwN0pt"),
		m_mapSTptN0w("m_mapSTptN0w"),
		m_mapSTptN0pt("m_mapSTptN0pt"),
		m_mapSTwptN0w("m_mapSTwptN0w"),
		m_mapSTwN0wpt("m_mapSTwN0wpt"),
		m_mapSTwptN0pt("m_mapSTwptN0pt"),
		m_mapSTptN0wpt("m_mapSTptN0wpt"),
		m_mapSTwptN0wpt("m_mapSTwptN0wpt"),
		// st + n0 + dis
		m_mapSTwN0wD("m_mapSTwN0wD"),
		m_mapSTwN0ptD("m_mapSTwN0ptD"),
		m_mapSTptN0wD("m_mapSTptN0wD"),
		m_mapSTptN0ptD("m_mapSTptN0ptD"),
		// st + n0 + st left/right head/pred
		m_mapSTwN0wSTLPi("m_mapSTwN0wSTLPi"),
		m_mapSTwN0ptSTLPi("m_mapSTwN0ptSTLPi"),
		m_mapSTptN0wSTLPi("m_mapSTptN0wSTLPi"),
		m_mapSTptN0ptSTLPi("m_mapSTptN0ptSTLPi"),
		m_mapSTwN0wSTRPi("m_mapSTwN0wSTRPi"),
		m_mapSTwN0ptSTRPi("m_mapSTwN0ptSTRPi"),
		m_mapSTptN0wSTRPi("m_mapSTptN0wSTRPi"),
		m_mapSTptN0ptSTRPi("m_mapSTptN0ptSTRPi"),
		m_mapSTwN0wN0LPi("m_mapSTwN0wN0LPi"),
		m_mapSTwN0ptN0LPi("m_mapSTwN0ptN0LPi"),
		m_mapSTptN0wN0LPi("m_mapSTptN0wN0LPi"),
		m_mapSTptN0ptN0LPi("m_mapSTptN0ptN0LPi"),

		// st2 + n0
		m_mapST2wN0w("m_mapST2wN0w"),
		m_mapST2wN0pt("m_mapST2wN0pt"),
		m_mapST2ptN0w("m_mapST2ptN0w"),
		m_mapST2ptN0pt("m_mapST2ptN0pt"),
		m_mapST2wptN0w("m_mapST2wptN0w"),
		m_mapST2wN0wpt("m_mapST2wN0wpt"),
		m_mapST2wptN0pt("m_mapST2wptN0pt"),
		m_mapST2ptN0wpt("m_mapST2ptN0wpt"),
		m_mapST2wptN0wpt("m_mapST2wptN0wpt"),
		// st2 + n0 + dis
		m_mapST2wN0wD("m_mapST2wN0wD"),
		m_mapST2wN0ptD("m_mapST2wN0ptD"),
		m_mapST2ptN0wD("m_mapST2ptN0wD"),
		m_mapST2ptN0ptD("m_mapST2ptN0ptD"),
		// st2 + n0 + st2 left/right head/pred
		m_mapST2wN0wST2LPi("m_mapST2wN0wST2LPi"),
		m_mapST2wN0ptST2LPi("m_mapST2wN0ptST2LPi"),
		m_mapST2ptN0wST2LPi("m_mapST2ptN0wST2LPi"),
		m_mapST2ptN0ptST2LPi("m_mapST2ptN0ptST2LPi"),
		m_mapST2wN0wST2RPi("m_mapST2wN0wST2RPi"),
		m_mapST2wN0ptST2RPi("m_mapST2wN0ptST2RPi"),
		m_mapST2ptN0wST2RPi("m_mapST2ptN0wST2RPi"),
		m_mapST2ptN0ptST2RPi("m_mapST2ptN0ptST2RPi"),
		m_mapST2wN0wN0LPi("m_mapST2wN0wN0LPi"),
		m_mapST2wN0ptN0LPi("m_mapST2wN0ptN0LPi"),
		m_mapST2ptN0wN0LPi("m_mapST2ptN0wN0LPi"),
		m_mapST2ptN0ptN0LPi("m_mapST2ptN0ptN0LPi"),

		// sst + n0
		m_mapSSTwN0w("m_mapSSTwN0w"),
		m_mapSSTwN0pt("m_mapSSTwN0pt"),
		m_mapSSTptN0w("m_mapSSTptN0w"),
		m_mapSSTptN0pt("m_mapSSTptN0pt"),
		m_mapSSTwptN0w("m_mapSSTwptN0w"),
		m_mapSSTwN0wpt("m_mapSSTwN0wpt"),
		m_mapSSTwptN0pt("m_mapSSTwptN0pt"),
		m_mapSSTptN0wpt("m_mapSSTptN0wpt"),
		m_mapSSTwptN0wpt("m_mapSSTwptN0wpt"),
		// sst + n0 + dis
		m_mapSSTwN0wD("m_mapSSTwN0wD"),
		m_mapSSTwN0ptD("m_mapSSTwN0ptD"),
		m_mapSSTptN0wD("m_mapSSTptN0wD"),
		m_mapSSTptN0ptD("m_mapSSTptN0ptD"),
		// st2 + n0 + st2 left/right head/pred
		m_mapSSTwN0wSSTLPi("m_mapSSTwN0wSSTLPi"),
		m_mapSSTwN0ptSSTLPi("m_mapSSTwN0ptSSTLPi"),
		m_mapSSTptN0wSSTLPi("m_mapSSTptN0wSSTLPi"),
		m_mapSSTptN0ptSSTLPi("m_mapSSTptN0ptSSTLPi"),
		m_mapSSTwN0wSSTRPi("m_mapSSTwN0wSSTRPi"),
		m_mapSSTwN0ptSSTRPi("m_mapSSTwN0ptSSTRPi"),
		m_mapSSTptN0wSSTRPi("m_mapSSTptN0wSSTRPi"),
		m_mapSSTptN0ptSSTRPi("m_mapSSTptN0ptSSTRPi"),
		m_mapSSTwN0wN0LPi("m_mapSSTwN0wN0LPi"),
		m_mapSSTwN0ptN0LPi("m_mapSSTwN0ptN0LPi"),
		m_mapSSTptN0wN0LPi("m_mapSSTptN0wN0LPi"),
		m_mapSSTptN0ptN0LPi("m_mapSSTptN0ptN0LPi"),

		// st + st2
		m_mapSTwST2w("m_mapSTwST2w"),
		m_mapSTwST2pt("m_mapSTwST2pt"),
		m_mapSTptST2w("m_mapSTptST2w"),
		m_mapSTptST2pt("m_mapSTptST2pt"),
		m_mapSTwptST2w("m_mapSTwptST2w"),
		m_mapSTwST2wpt("m_mapSTwST2wpt"),
		m_mapSTwptST2pt("m_mapSTwptST2pt"),
		m_mapSTptST2wpt("m_mapSTptST2wpt"),
		m_mapSTwptST2wpt("m_mapSTwptST2wpt"),

		// st + sst
		m_mapSTwSSTw("m_mapSTwSSTw"),
		m_mapSTwSSTpt("m_mapSTwSSTpt"),
		m_mapSTptSSTw("m_mapSTptSSTw"),
		m_mapSTptSSTpt("m_mapSTptSSTpt"),
		m_mapSTwptSSTw("m_mapSTwptSSTw"),
		m_mapSTwSSTwpt("m_mapSTwSSTwpt"),
		m_mapSTwptSSTpt("m_mapSTwptSSTpt"),
		m_mapSTptSSTwpt("m_mapSTptSSTwpt"),
		m_mapSTwptSSTwpt("m_mapSTwptSSTwpt"),

		// tri-gram
		// st + n0 + st2
		m_mapSTwN0ptST2pt("m_mapSTwN0ptST2pt"),
		m_mapSTptN0wST2pt("m_mapSTptN0wST2pt"),
		m_mapSTptN0ptST2w("m_mapSTptN0ptST2w"),
		m_mapSTptN0ptST2pt("m_mapSTptN0ptST2pt"),
		// st + n0 + sst
		m_mapSTwN0ptSSTpt("m_mapSTwN0ptSSTpt"),
		m_mapSTptN0wSSTpt("m_mapSTptN0wSSTpt"),
		m_mapSTptN0ptSSTw("m_mapSTptN0ptSSTw"),
		m_mapSTptN0ptSSTpt("m_mapSTptN0ptSSTpt"),

		// st + n0 + st left/right head/pred
		m_mapSTwN0ptSTLHptSTLHl("m_mapSTwN0ptSTLHptSTLHl"),
		m_mapSTptN0wSTLHptSTLHl("m_mapSTptN0wSTLHptSTLHl"),
		m_mapSTptN0ptSTLHwSTLHl("m_mapSTptN0ptSTLHwSTLHl"),
		m_mapSTptN0ptSTLHptSTLHl("m_mapSTptN0ptSTLHptSTLHl"),
		m_mapSTwN0ptSTLPptSTLPl("m_mapSTwN0ptSTLPptSTLPl"),
		m_mapSTptN0wSTLPptSTLPl("m_mapSTptN0wSTLPptSTLPl"),
		m_mapSTptN0ptSTLPwSTLPl("m_mapSTptN0ptSTLPwSTLPl"),
		m_mapSTptN0ptSTLPptSTLPl("m_mapSTptN0ptSTLPptSTLPl"),
		m_mapSTwN0ptSTRHptSTRHl("m_mapSTwN0ptSTRHptSTRHl"),
		m_mapSTptN0wSTRHptSTRHl("m_mapSTptN0wSTRHptSTRHl"),
		m_mapSTptN0ptSTRHwSTRHl("m_mapSTptN0ptSTRHwSTRHl"),
		m_mapSTptN0ptSTRHptSTRHl("m_mapSTptN0ptSTRHptSTRHl"),
		m_mapSTwN0ptSTRPptSTRPl("m_mapSTwN0ptSTRPptSTRPl"),
		m_mapSTptN0wSTRPptSTRPl("m_mapSTptN0wSTRPptSTRPl"),
		m_mapSTptN0ptSTRPwSTRPl("m_mapSTptN0ptSTRPwSTRPl"),
		m_mapSTptN0ptSTRPptSTRPl("m_mapSTptN0ptSTRPptSTRPl"),

		// st + n0 + n0 left head/pred
		m_mapSTwN0ptN0LHptN0LHl("m_mapSTwN0ptN0LHptN0LHl"),
		m_mapSTptN0wN0LHptN0LHl("m_mapSTptN0wN0LHptN0LHl"),
		m_mapSTptN0ptN0LHwN0LHl("m_mapSTptN0ptN0LHwN0LHl"),
		m_mapSTptN0ptN0LHptN0LHl("m_mapSTptN0ptN0LHptN0LHl"),
		m_mapSTwN0ptN0LPptN0LPl("m_mapSTwN0ptN0LPptN0LPl"),
		m_mapSTptN0wN0LPptN0LPl("m_mapSTptN0wN0LPptN0LPl"),
		m_mapSTptN0ptN0LPwN0LPl("m_mapSTptN0ptN0LPwN0LPl"),
		m_mapSTptN0ptN0LPptN0LPl("m_mapSTptN0ptN0LPptN0LPl"),

		// quar-gram
		// st + n0 + st right head + st right pred
		m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl("m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl"),
		// st + n0 + st left pred + st left pred 2
		m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l("m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l"),
		// st + n0 + st right pred + st right pred 2
		m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l("m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l"),
		// st + n0 + n0 left head + n0 left pred
		m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl("m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl"),
		// st + n0 + n0 left pred + n0 left pred 2
		m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l("m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l"),

		// st2 + n0 + st2 left/right head/pred
		m_mapST2wN0ptST2LHptST2LHl("m_mapST2wN0ptST2LHptST2LHl"),
		m_mapST2ptN0wST2LHptST2LHl("m_mapST2ptN0wST2LHptST2LHl"),
		m_mapST2ptN0ptST2LHwST2LHl("m_mapST2ptN0ptST2LHwST2LHl"),
		m_mapST2ptN0ptST2LHptST2LHl("m_mapST2ptN0ptST2LHptST2LHl"),
		m_mapST2wN0ptST2LPptST2LPl("m_mapST2wN0ptST2LPptST2LPl"),
		m_mapST2ptN0wST2LPptST2LPl("m_mapST2ptN0wST2LPptST2LPl"),
		m_mapST2ptN0ptST2LPwST2LPl("m_mapST2ptN0ptST2LPwST2LPl"),
		m_mapST2ptN0ptST2LPptST2LPl("m_mapST2ptN0ptST2LPptST2LPl"),
		m_mapST2wN0ptST2RHptST2RHl("m_mapST2wN0ptST2RHptST2RHl"),
		m_mapST2ptN0wST2RHptST2RHl("m_mapST2ptN0wST2RHptST2RHl"),
		m_mapST2ptN0ptST2RHwST2RHl("m_mapST2ptN0ptST2RHwST2RHl"),
		m_mapST2ptN0ptST2RHptST2RHl("m_mapST2ptN0ptST2RHptST2RHl"),
		m_mapST2wN0ptST2RPptST2RPl("m_mapST2wN0ptST2RPptST2RPl"),
		m_mapST2ptN0wST2RPptST2RPl("m_mapST2ptN0wST2RPptST2RPl"),
		m_mapST2ptN0ptST2RPwST2RPl("m_mapST2ptN0ptST2RPwST2RPl"),
		m_mapST2ptN0ptST2RPptST2RPl("m_mapST2ptN0ptST2RPptST2RPl"),

		// st + n0 + n0 left head/pred
		m_mapST2wN0ptN0LHptN0LHl("m_mapST2wN0ptN0LHptN0LHl"),
		m_mapST2ptN0wN0LHptN0LHl("m_mapST2ptN0wN0LHptN0LHl"),
		m_mapST2ptN0ptN0LHwN0LHl("m_mapST2ptN0ptN0LHwN0LHl"),
		m_mapST2ptN0ptN0LHptN0LHl("m_mapST2ptN0ptN0LHptN0LHl"),
		m_mapST2wN0ptN0LPptN0LPl("m_mapST2wN0ptN0LPptN0LPl"),
		m_mapST2ptN0wN0LPptN0LPl("m_mapST2ptN0wN0LPptN0LPl"),
		m_mapST2ptN0ptN0LPwN0LPl("m_mapST2ptN0ptN0LPwN0LPl"),
		m_mapST2ptN0ptN0LPptN0LPl("m_mapST2ptN0ptN0LPptN0LPl"),

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl("m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl"),
		// st2 + n0 + st2 left pred + st2 left pred 2
		m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l("m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l"),
		// st2 + n0 + st2 right pred + st2 right pred 2
		m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l("m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l"),
		// st2 + n0 + n0 left head + n0 left pred
		m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl("m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl"),
		// st2 + n0 + n0 left pred + n0 left pred 2
		m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l("m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l"),

		// sst + n0 + sst left/right head/pred
		m_mapSSTwN0ptSSTLHptSSTLHl("m_mapSSTwN0ptSSTLHptSSTLHl"),
		m_mapSSTptN0wSSTLHptSSTLHl("m_mapSSTptN0wSSTLHptSSTLHl"),
		m_mapSSTptN0ptSSTLHwSSTLHl("m_mapSSTptN0ptSSTLHwSSTLHl"),
		m_mapSSTptN0ptSSTLHptSSTLHl("m_mapSSTptN0ptSSTLHptSSTLHl"),
		m_mapSSTwN0ptSSTLPptSSTLPl("m_mapSSTwN0ptSSTLPptSSTLPl"),
		m_mapSSTptN0wSSTLPptSSTLPl("m_mapSSTptN0wSSTLPptSSTLPl"),
		m_mapSSTptN0ptSSTLPwSSTLPl("m_mapSSTptN0ptSSTLPwSSTLPl"),
		m_mapSSTptN0ptSSTLPptSSTLPl("m_mapSSTptN0ptSSTLPptSSTLPl"),
		m_mapSSTwN0ptSSTRHptSSTRHl("m_mapSSTwN0ptSSTRHptSSTRHl"),
		m_mapSSTptN0wSSTRHptSSTRHl("m_mapSSTptN0wSSTRHptSSTRHl"),
		m_mapSSTptN0ptSSTRHwSSTRHl("m_mapSSTptN0ptSSTRHwSSTRHl"),
		m_mapSSTptN0ptSSTRHptSSTRHl("m_mapSSTptN0ptSSTRHptSSTRHl"),
		m_mapSSTwN0ptSSTRPptSSTRPl("m_mapSSTwN0ptSSTRPptSSTRPl"),
		m_mapSSTptN0wSSTRPptSSTRPl("m_mapSSTptN0wSSTRPptSSTRPl"),
		m_mapSSTptN0ptSSTRPwSSTRPl("m_mapSSTptN0ptSSTRPwSSTRPl"),
		m_mapSSTptN0ptSSTRPptSSTRPl("m_mapSSTptN0ptSSTRPptSSTRPl"),

		// st + n0 + n0 left head/pred
		m_mapSSTwN0ptN0LHptN0LHl("m_mapSSTwN0ptN0LHptN0LHl"),
		m_mapSSTptN0wN0LHptN0LHl("m_mapSSTptN0wN0LHptN0LHl"),
		m_mapSSTptN0ptN0LHwN0LHl("m_mapSSTptN0ptN0LHwN0LHl"),
		m_mapSSTptN0ptN0LHptN0LHl("m_mapSSTptN0ptN0LHptN0LHl"),
		m_mapSSTwN0ptN0LPptN0LPl("m_mapSSTwN0ptN0LPptN0LPl"),
		m_mapSSTptN0wN0LPptN0LPl("m_mapSSTptN0wN0LPptN0LPl"),
		m_mapSSTptN0ptN0LPwN0LPl("m_mapSSTptN0ptN0LPwN0LPl"),
		m_mapSSTptN0ptN0LPptN0LPl("m_mapSSTptN0ptN0LPptN0LPl"),

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl("m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl"),
		// sst + n0 + sst left pred + sst left pred 2
		m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l("m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l"),
		// sst + n0 + sst right pred + sst right pred 2
		m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l("m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l"),
		// sst + n0 + n0 left head + n0 left pred
		m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl("m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl"),
		// sst + n0 + n0 left pred + n0 left pred 2
		m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l("m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l"),

		// st + n0 + label set
		m_mapSTwSTll("m_mapSTwSTll"),
		m_mapSTwSTrl("m_mapSTwSTrl"),
		m_mapN0wN0ll("m_mapN0wN0ll"),
		m_mapSTptSTll("m_mapSTptSTll"),
		m_mapSTptSTrl("m_mapSTptSTrl"),
		m_mapN0ptN0ll("m_mapN0ptN0ll"),
		m_mapSTptN0ptSTll("m_mapSTptN0ptSTll"),
		m_mapSTptN0ptSTrl("m_mapSTptN0ptSTrl"),
		m_mapSTptN0ptN0ll("m_mapSTptN0ptN0ll"),
		// st2 + n0 + label set
		m_mapST2wST2ll("m_mapST2wST2ll"),
		m_mapST2wST2rl("m_mapST2wST2rl"),
		m_mapST2ptST2ll("m_mapST2ptST2ll"),
		m_mapST2ptST2rl("m_mapST2ptST2rl"),
		m_mapST2ptN0ptST2ll("m_mapST2ptN0ptST2ll"),
		m_mapST2ptN0ptST2rl("m_mapST2ptN0ptST2rl"),
		m_mapST2ptN0ptN0ll("m_mapST2ptN0ptN0ll"),
		// sst + n0 + label set
		m_mapSSTwSSTll("m_mapSSTwSSTll"),
		m_mapSSTwSSTrl("m_mapSSTwSSTrl"),
		m_mapSSTptSSTll("m_mapSSTptSSTll"),
		m_mapSSTptSSTrl("m_mapSSTptSSTrl"),
		m_mapSSTptN0ptSSTll("m_mapSSTptN0ptSSTll"),
		m_mapSSTptN0ptSSTrl("m_mapSSTptN0ptSSTrl"),
		m_mapSSTptN0ptN0ll("m_mapSSTptN0ptN0ll"),

		// char feature (for chinese)
		// st context char
		m_map1CharBeforeST("m_map1CharBeforeST"),
		m_map2CharBeforeST("m_map2CharBeforeST"),
		m_map3CharBeforeST("m_map3CharBeforeST"),
		m_map1CharAfterST("m_map1CharAfterST"),
		m_map2CharAfterST("m_map2CharAfterST"),
		m_map3CharAfterST("m_map3CharAfterST"),
		// n0 context char
		m_map1CharBeforeN0("m_map1CharBeforeN0"),
		m_map2CharBeforeN0("m_map2CharBeforeN0"),
		m_map3CharBeforeN0("m_map3CharBeforeN0"),
		m_map1CharAfterN0("m_map1CharAfterN0"),
		m_map2CharAfterN0("m_map2CharAfterN0"),
		m_map3CharAfterN0("m_map3CharAfterN0"),

		// path feature
		// st - n0
		m_mapSTPOSPath("m_mapSTPOSPath"),
		m_mapSTFPOSPath("m_mapSTFPOSPath"),
		m_mapSTwN0ptSTsynhw("m_mapSTwN0ptSTsynhw"),
		m_mapSTwN0wSTsynhpt("m_mapSTwN0wSTsynhpt"),
		m_mapSTptN0wSTsynhw("m_mapSTptN0wSTsynhw"),
		m_mapSTwN0ptN0synhw("m_mapSTwN0ptN0synhw"),
		m_mapSTwN0wN0synhpt("m_mapSTwN0wN0synhpt"),
		m_mapSTptN0wN0synhw("m_mapSTptN0wN0synhw"),
		// st2 - n0
		m_mapST2POSPath("m_mapST2POSPath"),
		m_mapST2FPOSPath("m_mapST2FPOSPath"),
		m_mapST2wN0ptST2synhw("m_mapST2wN0ptST2synhw"),
		m_mapST2wN0wST2synhpt("m_mapST2wN0wST2synhpt"),
		m_mapST2ptN0wST2synhw("m_mapST2ptN0wST2synhw"),
		m_mapST2wN0ptN0synhw("m_mapST2wN0ptN0synhw"),
		m_mapST2wN0wN0synhpt("m_mapST2wN0wN0synhpt"),
		m_mapST2ptN0wN0synhw("m_mapST2ptN0wN0synhw"),
		// sst - n0
		m_mapSSTPOSPath("m_mapSSTPOSPath"),
		m_mapSSTFPOSPath("m_mapSSTFPOSPath"),
		m_mapSSTwN0ptSSTsynhw("m_mapSSTwN0ptSSTsynhw"),
		m_mapSSTwN0wSSTsynhpt("m_mapSSTwN0wSSTsynhpt"),
		m_mapSSTptN0wSSTsynhw("m_mapSSTptN0wSSTsynhw"),
		m_mapSSTwN0ptN0synhw("m_mapSSTwN0ptN0synhw"),
		m_mapSSTwN0wN0synhpt("m_mapSSTwN0wN0synhpt"),
		m_mapSSTptN0wN0synhw("m_mapSSTptN0wN0synhw"),

		// supertag feature
		m_mapSTst("m_mapSTst"),
		m_mapST2st("m_mapST2st"),
		m_mapSSTst("m_mapSSTst"),
		m_mapN0ist("m_mapN0ist"),
		m_mapSTstST2st("m_mapSTstST2st"),
		m_mapSTstSSTst("m_mapSTstSSTst"),
		m_mapSTstN0w("m_mapSTstN0w"),
		m_mapSTstN0pt("m_mapSTstN0pt"),
		m_mapSTstN0wpt("m_mapSTstN0wpt")
	{
		loadScores();
		std::cout << "load complete." << std::endl;
	}

	template<class RET_TYPE>
	Weight<RET_TYPE>::~Weight() = default;

	template<class RET_TYPE>
	void Weight<RET_TYPE>::loadScores() {

		if (m_sReadPath.empty()) {
			std::cout << "empty path" << std::endl;
			return;
		}
		std::ifstream input(m_sReadPath);
		if (!input) {
			std::cout << m_sReadPath << std::endl;
			std::cout << "bad path" << std::endl;
			return;
		}

		input >> TWord::getTokenizer();

		input >> TPOSTag::getTokenizer();

		input >> TDepLabel::getTokenizer();

		input >> TSuperTag::getTokenizer();

		input >> g_mapSuperTagCandidatesOfWords;

		input >> g_mapSuperTagCandidatesOfPOSTags;

		// uni-gram
		// st, n0, st2, sst
		// st
		input >> m_mapSTw;
		input >> m_mapSTpt;
		// n0
		input >> m_mapN0w;
		input >> m_mapN0pt;
		// st2
		input >> m_mapST2w;
		input >> m_mapST2pt;
		// sst
		input >> m_mapSSTw;
		input >> m_mapSSTpt;

		// unigram context
		// st, n0
		// st context
		input >> m_mapSTiw;
		input >> m_mapSTiwSTjw;
		input >> m_mapSTipt;
		input >> m_mapSTiptSTjpt;
		// n0 context
		input >> m_mapN0iw;
		input >> m_mapN0iwN0jw;
		input >> m_mapN0ipt;
		input >> m_mapN0iptN0jpt;

		// unigram with label
		// st, n0, st2, sst
		// st
		input >> m_mapSTwSTLHl;
		input >> m_mapSTwSTLPl;
		input >> m_mapSTwSTRHl;
		input >> m_mapSTwSTRPl;
		// n0
		input >> m_mapN0wN0LHl;
		input >> m_mapN0wN0LPl;
		input >> m_mapN0ptN0LHl;
		input >> m_mapN0ptN0LPl;
		// st2
		input >> m_mapST2wST2LHl;
		input >> m_mapST2wST2LPl;
		input >> m_mapST2wST2RHl;
		input >> m_mapST2wST2RPl;
		// sst
		input >> m_mapSSTwSSTLHl;
		input >> m_mapSSTwSSTLPl;
		input >> m_mapSSTwSSTRHl;
		input >> m_mapSSTwSSTRPl;

		// unigram with arity
		// st, n0, st2, sst
		// st
		input >> m_mapSTwSTLHi;
		input >> m_mapSTwSTLPi;
		input >> m_mapSTwSTRHi;
		input >> m_mapSTwSTRPi;
		input >> m_mapSTwSTHi;
		input >> m_mapSTwSTPi;
		// n0
		input >> m_mapN0wN0LHi;
		input >> m_mapN0wN0LPi;
		input >> m_mapN0ptN0LHi;
		input >> m_mapN0ptN0LPi;
		// st2
		input >> m_mapST2wST2LHi;
		input >> m_mapST2wST2LPi;
		input >> m_mapST2wST2RHi;
		input >> m_mapST2wST2RPi;
		input >> m_mapST2wST2Hi;
		input >> m_mapST2wST2Pi;
		// sst
		input >> m_mapSSTwSSTLHi;
		input >> m_mapSSTwSSTLPi;
		input >> m_mapSSTwSSTRHi;
		input >> m_mapSSTwSSTRPi;
		input >> m_mapSSTwSSTHi;
		input >> m_mapSSTwSSTPi;

		// bi-gram
		// st + n0
		input >> m_mapSTwN0w;
		input >> m_mapSTwN0pt;
		input >> m_mapSTptN0w;
		input >> m_mapSTptN0pt;
		input >> m_mapSTwptN0w;
		input >> m_mapSTwN0wpt;
		input >> m_mapSTwptN0pt;
		input >> m_mapSTptN0wpt;
		input >> m_mapSTwptN0wpt;
		// st + n0 + dis
		input >> m_mapSTwN0wD;
		input >> m_mapSTwN0ptD;
		input >> m_mapSTptN0wD;
		input >> m_mapSTptN0ptD;
		// st + n0 + st left/right head/pred
		input >> m_mapSTwN0wSTLPi;
		input >> m_mapSTwN0ptSTLPi;
		input >> m_mapSTptN0wSTLPi;
		input >> m_mapSTptN0ptSTLPi;
		input >> m_mapSTwN0wSTRPi;
		input >> m_mapSTwN0ptSTRPi;
		input >> m_mapSTptN0wSTRPi;
		input >> m_mapSTptN0ptSTRPi;
		input >> m_mapSTwN0wN0LPi;
		input >> m_mapSTwN0ptN0LPi;
		input >> m_mapSTptN0wN0LPi;
		input >> m_mapSTptN0ptN0LPi;

		// st2 + n0
		input >> m_mapST2wN0w;
		input >> m_mapST2wN0pt;
		input >> m_mapST2ptN0w;
		input >> m_mapST2ptN0pt;
		input >> m_mapST2wptN0w;
		input >> m_mapST2wN0wpt;
		input >> m_mapST2wptN0pt;
		input >> m_mapST2ptN0wpt;
		input >> m_mapST2wptN0wpt;
		// st2 + n0 + dis
		input >> m_mapST2wN0wD;
		input >> m_mapST2wN0ptD;
		input >> m_mapST2ptN0wD;
		input >> m_mapST2ptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		input >> m_mapST2wN0wST2LPi;
		input >> m_mapST2wN0ptST2LPi;
		input >> m_mapST2ptN0wST2LPi;
		input >> m_mapST2ptN0ptST2LPi;
		input >> m_mapST2wN0wST2RPi;
		input >> m_mapST2wN0ptST2RPi;
		input >> m_mapST2ptN0wST2RPi;
		input >> m_mapST2ptN0ptST2RPi;
		input >> m_mapST2wN0wN0LPi;
		input >> m_mapST2wN0ptN0LPi;
		input >> m_mapST2ptN0wN0LPi;
		input >> m_mapST2ptN0ptN0LPi;

		// sst + n0
		input >> m_mapSSTwN0w;
		input >> m_mapSSTwN0pt;
		input >> m_mapSSTptN0w;
		input >> m_mapSSTptN0pt;
		input >> m_mapSSTwptN0w;
		input >> m_mapSSTwN0wpt;
		input >> m_mapSSTwptN0pt;
		input >> m_mapSSTptN0wpt;
		input >> m_mapSSTwptN0wpt;
		// sst + n0 + dis
		input >> m_mapSSTwN0wD;
		input >> m_mapSSTwN0ptD;
		input >> m_mapSSTptN0wD;
		input >> m_mapSSTptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		input >> m_mapSSTwN0wSSTLPi;
		input >> m_mapSSTwN0ptSSTLPi;
		input >> m_mapSSTptN0wSSTLPi;
		input >> m_mapSSTptN0ptSSTLPi;
		input >> m_mapSSTwN0wSSTRPi;
		input >> m_mapSSTwN0ptSSTRPi;
		input >> m_mapSSTptN0wSSTRPi;
		input >> m_mapSSTptN0ptSSTRPi;
		input >> m_mapSSTwN0wN0LPi;
		input >> m_mapSSTwN0ptN0LPi;
		input >> m_mapSSTptN0wN0LPi;
		input >> m_mapSSTptN0ptN0LPi;

		// st + st2
		input >> m_mapSTwST2w;
		input >> m_mapSTwST2pt;
		input >> m_mapSTptST2w;
		input >> m_mapSTptST2pt;
		input >> m_mapSTwptST2w;
		input >> m_mapSTwST2wpt;
		input >> m_mapSTwptST2pt;
		input >> m_mapSTptST2wpt;
		input >> m_mapSTwptST2wpt;

		// st + sst
		input >> m_mapSTwSSTw;
		input >> m_mapSTwSSTpt;
		input >> m_mapSTptSSTw;
		input >> m_mapSTptSSTpt;
		input >> m_mapSTwptSSTw;
		input >> m_mapSTwSSTwpt;
		input >> m_mapSTwptSSTpt;
		input >> m_mapSTptSSTwpt;
		input >> m_mapSTwptSSTwpt;

		// tri-gram
		// st + n0 + st2
		input >> m_mapSTwN0ptST2pt;
		input >> m_mapSTptN0wST2pt;
		input >> m_mapSTptN0ptST2w;
		input >> m_mapSTptN0ptST2pt;
		// st + n0 + sst
		input >> m_mapSTwN0ptSSTpt;
		input >> m_mapSTptN0wSSTpt;
		input >> m_mapSTptN0ptSSTw;
		input >> m_mapSTptN0ptSSTpt;

		// st + n0 + st left/right head/pred
		input >> m_mapSTwN0ptSTLHptSTLHl;
		input >> m_mapSTptN0wSTLHptSTLHl;
		input >> m_mapSTptN0ptSTLHwSTLHl;
		input >> m_mapSTptN0ptSTLHptSTLHl;
		input >> m_mapSTwN0ptSTLPptSTLPl;
		input >> m_mapSTptN0wSTLPptSTLPl;
		input >> m_mapSTptN0ptSTLPwSTLPl;
		input >> m_mapSTptN0ptSTLPptSTLPl;
		input >> m_mapSTwN0ptSTRHptSTRHl;
		input >> m_mapSTptN0wSTRHptSTRHl;
		input >> m_mapSTptN0ptSTRHwSTRHl;
		input >> m_mapSTptN0ptSTRHptSTRHl;
		input >> m_mapSTwN0ptSTRPptSTRPl;
		input >> m_mapSTptN0wSTRPptSTRPl;
		input >> m_mapSTptN0ptSTRPwSTRPl;
		input >> m_mapSTptN0ptSTRPptSTRPl;

		// st + n0 + n0 left head/pred
		input >> m_mapSTwN0ptN0LHptN0LHl;
		input >> m_mapSTptN0wN0LHptN0LHl;
		input >> m_mapSTptN0ptN0LHwN0LHl;
		input >> m_mapSTptN0ptN0LHptN0LHl;
		input >> m_mapSTwN0ptN0LPptN0LPl;
		input >> m_mapSTptN0wN0LPptN0LPl;
		input >> m_mapSTptN0ptN0LPwN0LPl;
		input >> m_mapSTptN0ptN0LPptN0LPl;

		// quar-gram
		// st + n0 + st right head + st right pred
		input >> m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl;
		// st + n0 + st left pred + st left pred 2
		input >> m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + n0 + st right pred + st right pred 2
		input >> m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + n0 + n0 left head + n0 left pred
		input >> m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st + n0 + n0 left pred + n0 left pred 2
		input >> m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st2 + n0 + st2 left/right head/pred
		input >> m_mapST2wN0ptST2LHptST2LHl;
		input >> m_mapST2ptN0wST2LHptST2LHl;
		input >> m_mapST2ptN0ptST2LHwST2LHl;
		input >> m_mapST2ptN0ptST2LHptST2LHl;
		input >> m_mapST2wN0ptST2LPptST2LPl;
		input >> m_mapST2ptN0wST2LPptST2LPl;
		input >> m_mapST2ptN0ptST2LPwST2LPl;
		input >> m_mapST2ptN0ptST2LPptST2LPl;
		input >> m_mapST2wN0ptST2RHptST2RHl;
		input >> m_mapST2ptN0wST2RHptST2RHl;
		input >> m_mapST2ptN0ptST2RHwST2RHl;
		input >> m_mapST2ptN0ptST2RHptST2RHl;
		input >> m_mapST2wN0ptST2RPptST2RPl;
		input >> m_mapST2ptN0wST2RPptST2RPl;
		input >> m_mapST2ptN0ptST2RPwST2RPl;
		input >> m_mapST2ptN0ptST2RPptST2RPl;

		// st + n0 + n0 left head/pred
		input >> m_mapST2wN0ptN0LHptN0LHl;
		input >> m_mapST2ptN0wN0LHptN0LHl;
		input >> m_mapST2ptN0ptN0LHwN0LHl;
		input >> m_mapST2ptN0ptN0LHptN0LHl;
		input >> m_mapST2wN0ptN0LPptN0LPl;
		input >> m_mapST2ptN0wN0LPptN0LPl;
		input >> m_mapST2ptN0ptN0LPwN0LPl;
		input >> m_mapST2ptN0ptN0LPptN0LPl;

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		input >> m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl;
		// st2 + n0 + st2 left pred + st2 left pred 2
		input >> m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l;
		// st2 + n0 + st2 right pred + st2 right pred 2
		input >> m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l;
		// st2 + n0 + n0 left head + n0 left pred
		input >> m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st2 + n0 + n0 left pred + n0 left pred 2
		input >> m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// sst + n0 + sst left/right head/pred
		input >> m_mapSSTwN0ptSSTLHptSSTLHl;
		input >> m_mapSSTptN0wSSTLHptSSTLHl;
		input >> m_mapSSTptN0ptSSTLHwSSTLHl;
		input >> m_mapSSTptN0ptSSTLHptSSTLHl;
		input >> m_mapSSTwN0ptSSTLPptSSTLPl;
		input >> m_mapSSTptN0wSSTLPptSSTLPl;
		input >> m_mapSSTptN0ptSSTLPwSSTLPl;
		input >> m_mapSSTptN0ptSSTLPptSSTLPl;
		input >> m_mapSSTwN0ptSSTRHptSSTRHl;
		input >> m_mapSSTptN0wSSTRHptSSTRHl;
		input >> m_mapSSTptN0ptSSTRHwSSTRHl;
		input >> m_mapSSTptN0ptSSTRHptSSTRHl;
		input >> m_mapSSTwN0ptSSTRPptSSTRPl;
		input >> m_mapSSTptN0wSSTRPptSSTRPl;
		input >> m_mapSSTptN0ptSSTRPwSSTRPl;
		input >> m_mapSSTptN0ptSSTRPptSSTRPl;

		// st + n0 + n0 left head/pred
		input >> m_mapSSTwN0ptN0LHptN0LHl;
		input >> m_mapSSTptN0wN0LHptN0LHl;
		input >> m_mapSSTptN0ptN0LHwN0LHl;
		input >> m_mapSSTptN0ptN0LHptN0LHl;
		input >> m_mapSSTwN0ptN0LPptN0LPl;
		input >> m_mapSSTptN0wN0LPptN0LPl;
		input >> m_mapSSTptN0ptN0LPwN0LPl;
		input >> m_mapSSTptN0ptN0LPptN0LPl;

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		input >> m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl;
		// sst + n0 + sst left pred + sst left pred 2
		input >> m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l;
		// sst + n0 + sst right pred + sst right pred 2
		input >> m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l;
		// sst + n0 + n0 left head + n0 left pred
		input >> m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// sst + n0 + n0 left pred + n0 left pred 2
		input >> m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st + n0 + label set
		input >> m_mapSTwSTll;
		input >> m_mapSTwSTrl;
		input >> m_mapN0wN0ll;
		input >> m_mapSTptSTll;
		input >> m_mapSTptSTrl;
		input >> m_mapN0ptN0ll;
		input >> m_mapSTptN0ptSTll;
		input >> m_mapSTptN0ptSTrl;
		input >> m_mapSTptN0ptN0ll;
		// st2 + n0 + label set
		input >> m_mapST2wST2ll;
		input >> m_mapST2wST2rl;
		input >> m_mapST2ptST2ll;
		input >> m_mapST2ptST2rl;
		input >> m_mapST2ptN0ptST2ll;
		input >> m_mapST2ptN0ptST2rl;
		input >> m_mapST2ptN0ptN0ll;
		// sst + n0 + label set
		input >> m_mapSSTwSSTll;
		input >> m_mapSSTwSSTrl;
		input >> m_mapSSTptSSTll;
		input >> m_mapSSTptSSTrl;
		input >> m_mapSSTptN0ptSSTll;
		input >> m_mapSSTptN0ptSSTrl;
		input >> m_mapSSTptN0ptN0ll;

		// char feature (for chinese)
		// st context char
		input >> m_map1CharBeforeST;
		input >> m_map2CharBeforeST;
		input >> m_map3CharBeforeST;
		input >> m_map1CharAfterST;
		input >> m_map2CharAfterST;
		input >> m_map3CharAfterST;
		// n0 context char
		input >> m_map1CharBeforeN0;
		input >> m_map2CharBeforeN0;
		input >> m_map3CharBeforeN0;
		input >> m_map1CharAfterN0;
		input >> m_map2CharAfterN0;
		input >> m_map3CharAfterN0;

		// path feature
		// st - n0
		input >> m_mapSTPOSPath;
		input >> m_mapSTFPOSPath;
		input >> m_mapSTwN0ptSTsynhw;
		input >> m_mapSTwN0wSTsynhpt;
		input >> m_mapSTptN0wSTsynhw;
		input >> m_mapSTwN0ptN0synhw;
		input >> m_mapSTwN0wN0synhpt;
		input >> m_mapSTptN0wN0synhw;
		// st2 - n0
		input >> m_mapST2POSPath;
		input >> m_mapST2FPOSPath;
		input >> m_mapST2wN0ptST2synhw;
		input >> m_mapST2wN0wST2synhpt;
		input >> m_mapST2ptN0wST2synhw;
		input >> m_mapST2wN0ptN0synhw;
		input >> m_mapST2wN0wN0synhpt;
		input >> m_mapST2ptN0wN0synhw;
		// sst - n0
		input >> m_mapSSTPOSPath;
		input >> m_mapSSTFPOSPath;
		input >> m_mapSSTwN0ptSSTsynhw;
		input >> m_mapSSTwN0wSSTsynhpt;
		input >> m_mapSSTptN0wSSTsynhw;
		input >> m_mapSSTwN0ptN0synhw;
		input >> m_mapSSTwN0wN0synhpt;
		input >> m_mapSSTptN0wN0synhw;

		// supertag feature
		input >> m_mapSTst;
		input >> m_mapST2st;
		input >> m_mapSSTst;
		input >> m_mapN0ist;
		input >> m_mapSTstST2st;
		input >> m_mapSTstSSTst;
		input >> m_mapSTstN0w;
		input >> m_mapSTstN0pt;
		input >> m_mapSTstN0wpt;

		input.close();
	}

	template<class RET_TYPE>
	void Weight<RET_TYPE>::saveScores() const {

		if (m_sRecordPath.empty()) {
			return;
		}
		std::ofstream output(m_sRecordPath);
		if (!output) {
			return;
		}

		output << TWord::getTokenizer();

		output << TPOSTag::getTokenizer();

		output << TDepLabel::getTokenizer();

		output << TSuperTag::getTokenizer();

		output << g_mapSuperTagCandidatesOfWords;

		output << g_mapSuperTagCandidatesOfPOSTags;

		// uni-gram
		// st, n0, st2, sst
		// st
		output << m_mapSTw;
		output << m_mapSTpt;
		// n0
		output << m_mapN0w;
		output << m_mapN0pt;
		// st2
		output << m_mapST2w;
		output << m_mapST2pt;
		// sst
		output << m_mapSSTw;
		output << m_mapSSTpt;

		// unigram context
		// st, n0
		// st context
		output << m_mapSTiw;
		output << m_mapSTiwSTjw;
		output << m_mapSTipt;
		output << m_mapSTiptSTjpt;
		// n0 context
		output << m_mapN0iw;
		output << m_mapN0iwN0jw;
		output << m_mapN0ipt;
		output << m_mapN0iptN0jpt;

		// unigram with label
		// st, n0, st2, sst
		// st
		output << m_mapSTwSTLHl;
		output << m_mapSTwSTLPl;
		output << m_mapSTwSTRHl;
		output << m_mapSTwSTRPl;
		// n0
		output << m_mapN0wN0LHl;
		output << m_mapN0wN0LPl;
		output << m_mapN0ptN0LHl;
		output << m_mapN0ptN0LPl;
		// st2
		output << m_mapST2wST2LHl;
		output << m_mapST2wST2LPl;
		output << m_mapST2wST2RHl;
		output << m_mapST2wST2RPl;
		// sst
		output << m_mapSSTwSSTLHl;
		output << m_mapSSTwSSTLPl;
		output << m_mapSSTwSSTRHl;
		output << m_mapSSTwSSTRPl;

		// unigram with arity
		// st, n0, st2, sst
		// st
		output << m_mapSTwSTLHi;
		output << m_mapSTwSTLPi;
		output << m_mapSTwSTRHi;
		output << m_mapSTwSTRPi;
		output << m_mapSTwSTHi;
		output << m_mapSTwSTPi;
		// n0
		output << m_mapN0wN0LHi;
		output << m_mapN0wN0LPi;
		output << m_mapN0ptN0LHi;
		output << m_mapN0ptN0LPi;
		// st2
		output << m_mapST2wST2LHi;
		output << m_mapST2wST2LPi;
		output << m_mapST2wST2RHi;
		output << m_mapST2wST2RPi;
		output << m_mapST2wST2Hi;
		output << m_mapST2wST2Pi;
		// sst
		output << m_mapSSTwSSTLHi;
		output << m_mapSSTwSSTLPi;
		output << m_mapSSTwSSTRHi;
		output << m_mapSSTwSSTRPi;
		output << m_mapSSTwSSTHi;
		output << m_mapSSTwSSTPi;

		// bi-gram
		// st + n0
		output << m_mapSTwN0w;
		output << m_mapSTwN0pt;
		output << m_mapSTptN0w;
		output << m_mapSTptN0pt;
		output << m_mapSTwptN0w;
		output << m_mapSTwN0wpt;
		output << m_mapSTwptN0pt;
		output << m_mapSTptN0wpt;
		output << m_mapSTwptN0wpt;
		// st + n0 + dis
		output << m_mapSTwN0wD;
		output << m_mapSTwN0ptD;
		output << m_mapSTptN0wD;
		output << m_mapSTptN0ptD;
		// st + n0 + st left/right head/pred
		output << m_mapSTwN0wSTLPi;
		output << m_mapSTwN0ptSTLPi;
		output << m_mapSTptN0wSTLPi;
		output << m_mapSTptN0ptSTLPi;
		output << m_mapSTwN0wSTRPi;
		output << m_mapSTwN0ptSTRPi;
		output << m_mapSTptN0wSTRPi;
		output << m_mapSTptN0ptSTRPi;
		output << m_mapSTwN0wN0LPi;
		output << m_mapSTwN0ptN0LPi;
		output << m_mapSTptN0wN0LPi;
		output << m_mapSTptN0ptN0LPi;

		// st2 + n0
		output << m_mapST2wN0w;
		output << m_mapST2wN0pt;
		output << m_mapST2ptN0w;
		output << m_mapST2ptN0pt;
		output << m_mapST2wptN0w;
		output << m_mapST2wN0wpt;
		output << m_mapST2wptN0pt;
		output << m_mapST2ptN0wpt;
		output << m_mapST2wptN0wpt;
		// st2 + n0 + dis
		output << m_mapST2wN0wD;
		output << m_mapST2wN0ptD;
		output << m_mapST2ptN0wD;
		output << m_mapST2ptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		output << m_mapST2wN0wST2LPi;
		output << m_mapST2wN0ptST2LPi;
		output << m_mapST2ptN0wST2LPi;
		output << m_mapST2ptN0ptST2LPi;
		output << m_mapST2wN0wST2RPi;
		output << m_mapST2wN0ptST2RPi;
		output << m_mapST2ptN0wST2RPi;
		output << m_mapST2ptN0ptST2RPi;
		output << m_mapST2wN0wN0LPi;
		output << m_mapST2wN0ptN0LPi;
		output << m_mapST2ptN0wN0LPi;
		output << m_mapST2ptN0ptN0LPi;

		// sst + n0
		output << m_mapSSTwN0w;
		output << m_mapSSTwN0pt;
		output << m_mapSSTptN0w;
		output << m_mapSSTptN0pt;
		output << m_mapSSTwptN0w;
		output << m_mapSSTwN0wpt;
		output << m_mapSSTwptN0pt;
		output << m_mapSSTptN0wpt;
		output << m_mapSSTwptN0wpt;
		// sst + n0 + dis
		output << m_mapSSTwN0wD;
		output << m_mapSSTwN0ptD;
		output << m_mapSSTptN0wD;
		output << m_mapSSTptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		output << m_mapSSTwN0wSSTLPi;
		output << m_mapSSTwN0ptSSTLPi;
		output << m_mapSSTptN0wSSTLPi;
		output << m_mapSSTptN0ptSSTLPi;
		output << m_mapSSTwN0wSSTRPi;
		output << m_mapSSTwN0ptSSTRPi;
		output << m_mapSSTptN0wSSTRPi;
		output << m_mapSSTptN0ptSSTRPi;
		output << m_mapSSTwN0wN0LPi;
		output << m_mapSSTwN0ptN0LPi;
		output << m_mapSSTptN0wN0LPi;
		output << m_mapSSTptN0ptN0LPi;

		// st + st2
		output << m_mapSTwST2w;
		output << m_mapSTwST2pt;
		output << m_mapSTptST2w;
		output << m_mapSTptST2pt;
		output << m_mapSTwptST2w;
		output << m_mapSTwST2wpt;
		output << m_mapSTwptST2pt;
		output << m_mapSTptST2wpt;
		output << m_mapSTwptST2wpt;

		// st + sst
		output << m_mapSTwSSTw;
		output << m_mapSTwSSTpt;
		output << m_mapSTptSSTw;
		output << m_mapSTptSSTpt;
		output << m_mapSTwptSSTw;
		output << m_mapSTwSSTwpt;
		output << m_mapSTwptSSTpt;
		output << m_mapSTptSSTwpt;
		output << m_mapSTwptSSTwpt;

		// tri-gram
		// st + n0 + st2
		output << m_mapSTwN0ptST2pt;
		output << m_mapSTptN0wST2pt;
		output << m_mapSTptN0ptST2w;
		output << m_mapSTptN0ptST2pt;
		// st + n0 + sst
		output << m_mapSTwN0ptSSTpt;
		output << m_mapSTptN0wSSTpt;
		output << m_mapSTptN0ptSSTw;
		output << m_mapSTptN0ptSSTpt;

		// st + n0 + st left/right head/pred
		output << m_mapSTwN0ptSTLHptSTLHl;
		output << m_mapSTptN0wSTLHptSTLHl;
		output << m_mapSTptN0ptSTLHwSTLHl;
		output << m_mapSTptN0ptSTLHptSTLHl;
		output << m_mapSTwN0ptSTLPptSTLPl;
		output << m_mapSTptN0wSTLPptSTLPl;
		output << m_mapSTptN0ptSTLPwSTLPl;
		output << m_mapSTptN0ptSTLPptSTLPl;
		output << m_mapSTwN0ptSTRHptSTRHl;
		output << m_mapSTptN0wSTRHptSTRHl;
		output << m_mapSTptN0ptSTRHwSTRHl;
		output << m_mapSTptN0ptSTRHptSTRHl;
		output << m_mapSTwN0ptSTRPptSTRPl;
		output << m_mapSTptN0wSTRPptSTRPl;
		output << m_mapSTptN0ptSTRPwSTRPl;
		output << m_mapSTptN0ptSTRPptSTRPl;

		// st + n0 + n0 left head/pred
		output << m_mapSTwN0ptN0LHptN0LHl;
		output << m_mapSTptN0wN0LHptN0LHl;
		output << m_mapSTptN0ptN0LHwN0LHl;
		output << m_mapSTptN0ptN0LHptN0LHl;
		output << m_mapSTwN0ptN0LPptN0LPl;
		output << m_mapSTptN0wN0LPptN0LPl;
		output << m_mapSTptN0ptN0LPwN0LPl;
		output << m_mapSTptN0ptN0LPptN0LPl;

		// quar-gram
		// st + n0 + st right head + st right pred
		output << m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl;
		// st + n0 + st left pred + st left pred 2
		output << m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + n0 + st right pred + st right pred 2
		output << m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + n0 + n0 left head + n0 left pred
		output << m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st + n0 + n0 left pred + n0 left pred 2
		output << m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st2 + n0 + st2 left/right head/pred
		output << m_mapST2wN0ptST2LHptST2LHl;
		output << m_mapST2ptN0wST2LHptST2LHl;
		output << m_mapST2ptN0ptST2LHwST2LHl;
		output << m_mapST2ptN0ptST2LHptST2LHl;
		output << m_mapST2wN0ptST2LPptST2LPl;
		output << m_mapST2ptN0wST2LPptST2LPl;
		output << m_mapST2ptN0ptST2LPwST2LPl;
		output << m_mapST2ptN0ptST2LPptST2LPl;
		output << m_mapST2wN0ptST2RHptST2RHl;
		output << m_mapST2ptN0wST2RHptST2RHl;
		output << m_mapST2ptN0ptST2RHwST2RHl;
		output << m_mapST2ptN0ptST2RHptST2RHl;
		output << m_mapST2wN0ptST2RPptST2RPl;
		output << m_mapST2ptN0wST2RPptST2RPl;
		output << m_mapST2ptN0ptST2RPwST2RPl;
		output << m_mapST2ptN0ptST2RPptST2RPl;

		// st + n0 + n0 left head/pred
		output << m_mapST2wN0ptN0LHptN0LHl;
		output << m_mapST2ptN0wN0LHptN0LHl;
		output << m_mapST2ptN0ptN0LHwN0LHl;
		output << m_mapST2ptN0ptN0LHptN0LHl;
		output << m_mapST2wN0ptN0LPptN0LPl;
		output << m_mapST2ptN0wN0LPptN0LPl;
		output << m_mapST2ptN0ptN0LPwN0LPl;
		output << m_mapST2ptN0ptN0LPptN0LPl;

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		output << m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl;
		// st2 + n0 + st2 left pred + st2 left pred 2
		output << m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l;
		// st2 + n0 + st2 right pred + st2 right pred 2
		output << m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l;
		// st2 + n0 + n0 left head + n0 left pred
		output << m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st2 + n0 + n0 left pred + n0 left pred 2
		output << m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// sst + n0 + sst left/right head/pred
		output << m_mapSSTwN0ptSSTLHptSSTLHl;
		output << m_mapSSTptN0wSSTLHptSSTLHl;
		output << m_mapSSTptN0ptSSTLHwSSTLHl;
		output << m_mapSSTptN0ptSSTLHptSSTLHl;
		output << m_mapSSTwN0ptSSTLPptSSTLPl;
		output << m_mapSSTptN0wSSTLPptSSTLPl;
		output << m_mapSSTptN0ptSSTLPwSSTLPl;
		output << m_mapSSTptN0ptSSTLPptSSTLPl;
		output << m_mapSSTwN0ptSSTRHptSSTRHl;
		output << m_mapSSTptN0wSSTRHptSSTRHl;
		output << m_mapSSTptN0ptSSTRHwSSTRHl;
		output << m_mapSSTptN0ptSSTRHptSSTRHl;
		output << m_mapSSTwN0ptSSTRPptSSTRPl;
		output << m_mapSSTptN0wSSTRPptSSTRPl;
		output << m_mapSSTptN0ptSSTRPwSSTRPl;
		output << m_mapSSTptN0ptSSTRPptSSTRPl;

		// st + n0 + n0 left head/pred
		output << m_mapSSTwN0ptN0LHptN0LHl;
		output << m_mapSSTptN0wN0LHptN0LHl;
		output << m_mapSSTptN0ptN0LHwN0LHl;
		output << m_mapSSTptN0ptN0LHptN0LHl;
		output << m_mapSSTwN0ptN0LPptN0LPl;
		output << m_mapSSTptN0wN0LPptN0LPl;
		output << m_mapSSTptN0ptN0LPwN0LPl;
		output << m_mapSSTptN0ptN0LPptN0LPl;

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		output << m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl;
		// sst + n0 + sst left pred + sst left pred 2
		output << m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l;
		// sst + n0 + sst right pred + sst right pred 2
		output << m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l;
		// sst + n0 + n0 left head + n0 left pred
		output << m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// sst + n0 + n0 left pred + n0 left pred 2
		output << m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st + n0 + label set
		output << m_mapSTwSTll;
		output << m_mapSTwSTrl;
		output << m_mapN0wN0ll;
		output << m_mapSTptSTll;
		output << m_mapSTptSTrl;
		output << m_mapN0ptN0ll;
		output << m_mapSTptN0ptSTll;
		output << m_mapSTptN0ptSTrl;
		output << m_mapSTptN0ptN0ll;
		// st2 + n0 + label set
		output << m_mapST2wST2ll;
		output << m_mapST2wST2rl;
		output << m_mapST2ptST2ll;
		output << m_mapST2ptST2rl;
		output << m_mapST2ptN0ptST2ll;
		output << m_mapST2ptN0ptST2rl;
		output << m_mapST2ptN0ptN0ll;
		// sst + n0 + label set
		output << m_mapSSTwSSTll;
		output << m_mapSSTwSSTrl;
		output << m_mapSSTptSSTll;
		output << m_mapSSTptSSTrl;
		output << m_mapSSTptN0ptSSTll;
		output << m_mapSSTptN0ptSSTrl;
		output << m_mapSSTptN0ptN0ll;

		// char feature (for chinese)
		// st context char
		output << m_map1CharBeforeST;
		output << m_map2CharBeforeST;
		output << m_map3CharBeforeST;
		output << m_map1CharAfterST;
		output << m_map2CharAfterST;
		output << m_map3CharAfterST;
		// n0 context char
		output << m_map1CharBeforeN0;
		output << m_map2CharBeforeN0;
		output << m_map3CharBeforeN0;
		output << m_map1CharAfterN0;
		output << m_map2CharAfterN0;
		output << m_map3CharAfterN0;

		// path feature
		// st - n0
		output << m_mapSTPOSPath;
		output << m_mapSTFPOSPath;
		output << m_mapSTwN0ptSTsynhw;
		output << m_mapSTwN0wSTsynhpt;
		output << m_mapSTptN0wSTsynhw;
		output << m_mapSTwN0ptN0synhw;
		output << m_mapSTwN0wN0synhpt;
		output << m_mapSTptN0wN0synhw;
		// st2 - n0
		output << m_mapST2POSPath;
		output << m_mapST2FPOSPath;
		output << m_mapST2wN0ptST2synhw;
		output << m_mapST2wN0wST2synhpt;
		output << m_mapST2ptN0wST2synhw;
		output << m_mapST2wN0ptN0synhw;
		output << m_mapST2wN0wN0synhpt;
		output << m_mapST2ptN0wN0synhw;
		// sst - n0
		output << m_mapSSTPOSPath;
		output << m_mapSSTFPOSPath;
		output << m_mapSSTwN0ptSSTsynhw;
		output << m_mapSSTwN0wSSTsynhpt;
		output << m_mapSSTptN0wSSTsynhw;
		output << m_mapSSTwN0ptN0synhw;
		output << m_mapSSTwN0wN0synhpt;
		output << m_mapSSTptN0wN0synhw;

		// supertag feature
		output << m_mapSTst;
		output << m_mapST2st;
		output << m_mapSSTst;
		output << m_mapN0ist;
		output << m_mapSTstST2st;
		output << m_mapSTstSSTst;
		output << m_mapSTstN0w;
		output << m_mapSTstN0pt;
		output << m_mapSTstN0wpt;

		output.close();
	}

	template<class RET_TYPE>
	void Weight<RET_TYPE>::computeAverageFeatureWeights(const int & round) {

		// uni-gram
		// st, n0, st2, sst
		// st
		m_mapSTw.computeAverage(round);
		m_mapSTpt.computeAverage(round);
		// n0
		m_mapN0w.computeAverage(round);
		m_mapN0pt.computeAverage(round);
		// st2
		m_mapST2w.computeAverage(round);
		m_mapST2pt.computeAverage(round);
		// sst
		m_mapSSTw.computeAverage(round);
		m_mapSSTpt.computeAverage(round);

		// unigram context
		// st, n0
		// st context
		m_mapSTiw.computeAverage(round);
		m_mapSTiwSTjw.computeAverage(round);
		m_mapSTipt.computeAverage(round);
		m_mapSTiptSTjpt.computeAverage(round);
		// n0 context
		m_mapN0iw.computeAverage(round);
		m_mapN0iwN0jw.computeAverage(round);
		m_mapN0ipt.computeAverage(round);
		m_mapN0iptN0jpt.computeAverage(round);

		// unigram with label
		// st, n0, st2, sst
		// st
		m_mapSTwSTLHl.computeAverage(round);
		m_mapSTwSTLPl.computeAverage(round);
		m_mapSTwSTRHl.computeAverage(round);
		m_mapSTwSTRPl.computeAverage(round);
		// n0
		m_mapN0wN0LHl.computeAverage(round);
		m_mapN0wN0LPl.computeAverage(round);
		m_mapN0ptN0LHl.computeAverage(round);
		m_mapN0ptN0LPl.computeAverage(round);
		// st2
		m_mapST2wST2LHl.computeAverage(round);
		m_mapST2wST2LPl.computeAverage(round);
		m_mapST2wST2RHl.computeAverage(round);
		m_mapST2wST2RPl.computeAverage(round);
		// sst
		m_mapSSTwSSTLHl.computeAverage(round);
		m_mapSSTwSSTLPl.computeAverage(round);
		m_mapSSTwSSTRHl.computeAverage(round);
		m_mapSSTwSSTRPl.computeAverage(round);

		// unigram with arity
		// st, n0, st2, sst
		// st
		m_mapSTwSTLHi.computeAverage(round);
		m_mapSTwSTLPi.computeAverage(round);
		m_mapSTwSTRHi.computeAverage(round);
		m_mapSTwSTRPi.computeAverage(round);
		m_mapSTwSTHi.computeAverage(round);
		m_mapSTwSTPi.computeAverage(round);
		// n0
		m_mapN0wN0LHi.computeAverage(round);
		m_mapN0wN0LPi.computeAverage(round);
		m_mapN0ptN0LHi.computeAverage(round);
		m_mapN0ptN0LPi.computeAverage(round);
		// st2
		m_mapST2wST2LHi.computeAverage(round);
		m_mapST2wST2LPi.computeAverage(round);
		m_mapST2wST2RHi.computeAverage(round);
		m_mapST2wST2RPi.computeAverage(round);
		m_mapST2wST2Hi.computeAverage(round);
		m_mapST2wST2Pi.computeAverage(round);
		// sst
		m_mapSSTwSSTLHi.computeAverage(round);
		m_mapSSTwSSTLPi.computeAverage(round);
		m_mapSSTwSSTRHi.computeAverage(round);
		m_mapSSTwSSTRPi.computeAverage(round);
		m_mapSSTwSSTHi.computeAverage(round);
		m_mapSSTwSSTPi.computeAverage(round);

		// bi-gram
		// st + n0
		m_mapSTwN0w.computeAverage(round);
		m_mapSTwN0pt.computeAverage(round);
		m_mapSTptN0w.computeAverage(round);
		m_mapSTptN0pt.computeAverage(round);
		m_mapSTwptN0w.computeAverage(round);
		m_mapSTwN0wpt.computeAverage(round);
		m_mapSTwptN0pt.computeAverage(round);
		m_mapSTptN0wpt.computeAverage(round);
		m_mapSTwptN0wpt.computeAverage(round);
		// st + n0 + dis
		m_mapSTwN0wD.computeAverage(round);
		m_mapSTwN0ptD.computeAverage(round);
		m_mapSTptN0wD.computeAverage(round);
		m_mapSTptN0ptD.computeAverage(round);
		// st + n0 + st left/right head/pred
		m_mapSTwN0wSTLPi.computeAverage(round);
		m_mapSTwN0ptSTLPi.computeAverage(round);
		m_mapSTptN0wSTLPi.computeAverage(round);
		m_mapSTptN0ptSTLPi.computeAverage(round);
		m_mapSTwN0wSTRPi.computeAverage(round);
		m_mapSTwN0ptSTRPi.computeAverage(round);
		m_mapSTptN0wSTRPi.computeAverage(round);
		m_mapSTptN0ptSTRPi.computeAverage(round);
		m_mapSTwN0wN0LPi.computeAverage(round);
		m_mapSTwN0ptN0LPi.computeAverage(round);
		m_mapSTptN0wN0LPi.computeAverage(round);
		m_mapSTptN0ptN0LPi.computeAverage(round);

		// st2 + n0
		m_mapST2wN0w.computeAverage(round);
		m_mapST2wN0pt.computeAverage(round);
		m_mapST2ptN0w.computeAverage(round);
		m_mapST2ptN0pt.computeAverage(round);
		m_mapST2wptN0w.computeAverage(round);
		m_mapST2wN0wpt.computeAverage(round);
		m_mapST2wptN0pt.computeAverage(round);
		m_mapST2ptN0wpt.computeAverage(round);
		m_mapST2wptN0wpt.computeAverage(round);
		// st2 + n0 + dis
		m_mapST2wN0wD.computeAverage(round);
		m_mapST2wN0ptD.computeAverage(round);
		m_mapST2ptN0wD.computeAverage(round);
		m_mapST2ptN0ptD.computeAverage(round);
		// st2 + n0 + st2 left/right head/pred
		m_mapST2wN0wST2LPi.computeAverage(round);
		m_mapST2wN0ptST2LPi.computeAverage(round);
		m_mapST2ptN0wST2LPi.computeAverage(round);
		m_mapST2ptN0ptST2LPi.computeAverage(round);
		m_mapST2wN0wST2RPi.computeAverage(round);
		m_mapST2wN0ptST2RPi.computeAverage(round);
		m_mapST2ptN0wST2RPi.computeAverage(round);
		m_mapST2ptN0ptST2RPi.computeAverage(round);
		m_mapST2wN0wN0LPi.computeAverage(round);
		m_mapST2wN0ptN0LPi.computeAverage(round);
		m_mapST2ptN0wN0LPi.computeAverage(round);
		m_mapST2ptN0ptN0LPi.computeAverage(round);

		// sst + n0
		m_mapSSTwN0w.computeAverage(round);
		m_mapSSTwN0pt.computeAverage(round);
		m_mapSSTptN0w.computeAverage(round);
		m_mapSSTptN0pt.computeAverage(round);
		m_mapSSTwptN0w.computeAverage(round);
		m_mapSSTwN0wpt.computeAverage(round);
		m_mapSSTwptN0pt.computeAverage(round);
		m_mapSSTptN0wpt.computeAverage(round);
		m_mapSSTwptN0wpt.computeAverage(round);
		// sst + n0 + dis
		m_mapSSTwN0wD.computeAverage(round);
		m_mapSSTwN0ptD.computeAverage(round);
		m_mapSSTptN0wD.computeAverage(round);
		m_mapSSTptN0ptD.computeAverage(round);
		// st2 + n0 + st2 left/right head/pred
		m_mapSSTwN0wSSTLPi.computeAverage(round);
		m_mapSSTwN0ptSSTLPi.computeAverage(round);
		m_mapSSTptN0wSSTLPi.computeAverage(round);
		m_mapSSTptN0ptSSTLPi.computeAverage(round);
		m_mapSSTwN0wSSTRPi.computeAverage(round);
		m_mapSSTwN0ptSSTRPi.computeAverage(round);
		m_mapSSTptN0wSSTRPi.computeAverage(round);
		m_mapSSTptN0ptSSTRPi.computeAverage(round);
		m_mapSSTwN0wN0LPi.computeAverage(round);
		m_mapSSTwN0ptN0LPi.computeAverage(round);
		m_mapSSTptN0wN0LPi.computeAverage(round);
		m_mapSSTptN0ptN0LPi.computeAverage(round);

		// st + st2
		m_mapSTwST2w.computeAverage(round);
		m_mapSTwST2pt.computeAverage(round);
		m_mapSTptST2w.computeAverage(round);
		m_mapSTptST2pt.computeAverage(round);
		m_mapSTwptST2w.computeAverage(round);
		m_mapSTwST2wpt.computeAverage(round);
		m_mapSTwptST2pt.computeAverage(round);
		m_mapSTptST2wpt.computeAverage(round);
		m_mapSTwptST2wpt.computeAverage(round);

		// st + sst
		m_mapSTwSSTw.computeAverage(round);
		m_mapSTwSSTpt.computeAverage(round);
		m_mapSTptSSTw.computeAverage(round);
		m_mapSTptSSTpt.computeAverage(round);
		m_mapSTwptSSTw.computeAverage(round);
		m_mapSTwSSTwpt.computeAverage(round);
		m_mapSTwptSSTpt.computeAverage(round);
		m_mapSTptSSTwpt.computeAverage(round);
		m_mapSTwptSSTwpt.computeAverage(round);

		// tri-gram
		// st + n0 + st2
		m_mapSTwN0ptST2pt.computeAverage(round);
		m_mapSTptN0wST2pt.computeAverage(round);
		m_mapSTptN0ptST2w.computeAverage(round);
		m_mapSTptN0ptST2pt.computeAverage(round);
		// st + n0 + sst
		m_mapSTwN0ptSSTpt.computeAverage(round);
		m_mapSTptN0wSSTpt.computeAverage(round);
		m_mapSTptN0ptSSTw.computeAverage(round);
		m_mapSTptN0ptSSTpt.computeAverage(round);

		// st + n0 + st left/right head/pred
		m_mapSTwN0ptSTLHptSTLHl.computeAverage(round);
		m_mapSTptN0wSTLHptSTLHl.computeAverage(round);
		m_mapSTptN0ptSTLHwSTLHl.computeAverage(round);
		m_mapSTptN0ptSTLHptSTLHl.computeAverage(round);
		m_mapSTwN0ptSTLPptSTLPl.computeAverage(round);
		m_mapSTptN0wSTLPptSTLPl.computeAverage(round);
		m_mapSTptN0ptSTLPwSTLPl.computeAverage(round);
		m_mapSTptN0ptSTLPptSTLPl.computeAverage(round);
		m_mapSTwN0ptSTRHptSTRHl.computeAverage(round);
		m_mapSTptN0wSTRHptSTRHl.computeAverage(round);
		m_mapSTptN0ptSTRHwSTRHl.computeAverage(round);
		m_mapSTptN0ptSTRHptSTRHl.computeAverage(round);
		m_mapSTwN0ptSTRPptSTRPl.computeAverage(round);
		m_mapSTptN0wSTRPptSTRPl.computeAverage(round);
		m_mapSTptN0ptSTRPwSTRPl.computeAverage(round);
		m_mapSTptN0ptSTRPptSTRPl.computeAverage(round);

		// st + n0 + n0 left head/pred
		m_mapSTwN0ptN0LHptN0LHl.computeAverage(round);
		m_mapSTptN0wN0LHptN0LHl.computeAverage(round);
		m_mapSTptN0ptN0LHwN0LHl.computeAverage(round);
		m_mapSTptN0ptN0LHptN0LHl.computeAverage(round);
		m_mapSTwN0ptN0LPptN0LPl.computeAverage(round);
		m_mapSTptN0wN0LPptN0LPl.computeAverage(round);
		m_mapSTptN0ptN0LPwN0LPl.computeAverage(round);
		m_mapSTptN0ptN0LPptN0LPl.computeAverage(round);

		// quar-gram
		// st + n0 + st right head + st right pred
		m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl.computeAverage(round);
		// st + n0 + st left pred + st left pred 2
		m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l.computeAverage(round);
		// st + n0 + st right pred + st right pred 2
		m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l.computeAverage(round);
		// st + n0 + n0 left head + n0 left pred
		m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl.computeAverage(round);
		// st + n0 + n0 left pred + n0 left pred 2
		m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.computeAverage(round);

		// st2 + n0 + st2 left/right head/pred
		m_mapST2wN0ptST2LHptST2LHl.computeAverage(round);
		m_mapST2ptN0wST2LHptST2LHl.computeAverage(round);
		m_mapST2ptN0ptST2LHwST2LHl.computeAverage(round);
		m_mapST2ptN0ptST2LHptST2LHl.computeAverage(round);
		m_mapST2wN0ptST2LPptST2LPl.computeAverage(round);
		m_mapST2ptN0wST2LPptST2LPl.computeAverage(round);
		m_mapST2ptN0ptST2LPwST2LPl.computeAverage(round);
		m_mapST2ptN0ptST2LPptST2LPl.computeAverage(round);
		m_mapST2wN0ptST2RHptST2RHl.computeAverage(round);
		m_mapST2ptN0wST2RHptST2RHl.computeAverage(round);
		m_mapST2ptN0ptST2RHwST2RHl.computeAverage(round);
		m_mapST2ptN0ptST2RHptST2RHl.computeAverage(round);
		m_mapST2wN0ptST2RPptST2RPl.computeAverage(round);
		m_mapST2ptN0wST2RPptST2RPl.computeAverage(round);
		m_mapST2ptN0ptST2RPwST2RPl.computeAverage(round);
		m_mapST2ptN0ptST2RPptST2RPl.computeAverage(round);

		// st + n0 + n0 left head/pred
		m_mapST2wN0ptN0LHptN0LHl.computeAverage(round);
		m_mapST2ptN0wN0LHptN0LHl.computeAverage(round);
		m_mapST2ptN0ptN0LHwN0LHl.computeAverage(round);
		m_mapST2ptN0ptN0LHptN0LHl.computeAverage(round);
		m_mapST2wN0ptN0LPptN0LPl.computeAverage(round);
		m_mapST2ptN0wN0LPptN0LPl.computeAverage(round);
		m_mapST2ptN0ptN0LPwN0LPl.computeAverage(round);
		m_mapST2ptN0ptN0LPptN0LPl.computeAverage(round);

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl.computeAverage(round);
		// st2 + n0 + st2 left pred + st2 left pred 2
		m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l.computeAverage(round);
		// st2 + n0 + st2 right pred + st2 right pred 2
		m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l.computeAverage(round);
		// st2 + n0 + n0 left head + n0 left pred
		m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl.computeAverage(round);
		// st2 + n0 + n0 left pred + n0 left pred 2
		m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l.computeAverage(round);

		// sst + n0 + sst left/right head/pred
		m_mapSSTwN0ptSSTLHptSSTLHl.computeAverage(round);
		m_mapSSTptN0wSSTLHptSSTLHl.computeAverage(round);
		m_mapSSTptN0ptSSTLHwSSTLHl.computeAverage(round);
		m_mapSSTptN0ptSSTLHptSSTLHl.computeAverage(round);
		m_mapSSTwN0ptSSTLPptSSTLPl.computeAverage(round);
		m_mapSSTptN0wSSTLPptSSTLPl.computeAverage(round);
		m_mapSSTptN0ptSSTLPwSSTLPl.computeAverage(round);
		m_mapSSTptN0ptSSTLPptSSTLPl.computeAverage(round);
		m_mapSSTwN0ptSSTRHptSSTRHl.computeAverage(round);
		m_mapSSTptN0wSSTRHptSSTRHl.computeAverage(round);
		m_mapSSTptN0ptSSTRHwSSTRHl.computeAverage(round);
		m_mapSSTptN0ptSSTRHptSSTRHl.computeAverage(round);
		m_mapSSTwN0ptSSTRPptSSTRPl.computeAverage(round);
		m_mapSSTptN0wSSTRPptSSTRPl.computeAverage(round);
		m_mapSSTptN0ptSSTRPwSSTRPl.computeAverage(round);
		m_mapSSTptN0ptSSTRPptSSTRPl.computeAverage(round);

		// st + n0 + n0 left head/pred
		m_mapSSTwN0ptN0LHptN0LHl.computeAverage(round);
		m_mapSSTptN0wN0LHptN0LHl.computeAverage(round);
		m_mapSSTptN0ptN0LHwN0LHl.computeAverage(round);
		m_mapSSTptN0ptN0LHptN0LHl.computeAverage(round);
		m_mapSSTwN0ptN0LPptN0LPl.computeAverage(round);
		m_mapSSTptN0wN0LPptN0LPl.computeAverage(round);
		m_mapSSTptN0ptN0LPwN0LPl.computeAverage(round);
		m_mapSSTptN0ptN0LPptN0LPl.computeAverage(round);

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl.computeAverage(round);
		// sst + n0 + sst left pred + sst left pred 2
		m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l.computeAverage(round);
		// sst + n0 + sst right pred + sst right pred 2
		m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l.computeAverage(round);
		// sst + n0 + n0 left head + n0 left pred
		m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl.computeAverage(round);
		// sst + n0 + n0 left pred + n0 left pred 2
		m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.computeAverage(round);

		// st + n0 + label set
		m_mapSTwSTll.computeAverage(round);
		m_mapSTwSTrl.computeAverage(round);
		m_mapN0wN0ll.computeAverage(round);
		m_mapSTptSTll.computeAverage(round);
		m_mapSTptSTrl.computeAverage(round);
		m_mapN0ptN0ll.computeAverage(round);
		m_mapSTptN0ptSTll.computeAverage(round);
		m_mapSTptN0ptSTrl.computeAverage(round);
		m_mapSTptN0ptN0ll.computeAverage(round);
		// st2 + n0 + label set
		m_mapST2wST2ll.computeAverage(round);
		m_mapST2wST2rl.computeAverage(round);
		m_mapST2ptST2ll.computeAverage(round);
		m_mapST2ptST2rl.computeAverage(round);
		m_mapST2ptN0ptST2ll.computeAverage(round);
		m_mapST2ptN0ptST2rl.computeAverage(round);
		m_mapST2ptN0ptN0ll.computeAverage(round);
		// sst + n0 + label set
		m_mapSSTwSSTll.computeAverage(round);
		m_mapSSTwSSTrl.computeAverage(round);
		m_mapSSTptSSTll.computeAverage(round);
		m_mapSSTptSSTrl.computeAverage(round);
		m_mapSSTptN0ptSSTll.computeAverage(round);
		m_mapSSTptN0ptSSTrl.computeAverage(round);
		m_mapSSTptN0ptN0ll.computeAverage(round);

		// char feature (for chinese)
		// st context char
		m_map1CharBeforeST.computeAverage(round);
		m_map2CharBeforeST.computeAverage(round);
		m_map3CharBeforeST.computeAverage(round);
		m_map1CharAfterST.computeAverage(round);
		m_map2CharAfterST.computeAverage(round);
		m_map3CharAfterST.computeAverage(round);
		// n0 context char
		m_map1CharBeforeN0.computeAverage(round);
		m_map2CharBeforeN0.computeAverage(round);
		m_map3CharBeforeN0.computeAverage(round);
		m_map1CharAfterN0.computeAverage(round);
		m_map2CharAfterN0.computeAverage(round);
		m_map3CharAfterN0.computeAverage(round);

		// path feature
		// st - n0
		m_mapSTPOSPath.computeAverage(round);
		m_mapSTFPOSPath.computeAverage(round);
		m_mapSTwN0ptSTsynhw.computeAverage(round);
		m_mapSTwN0wSTsynhpt.computeAverage(round);
		m_mapSTptN0wSTsynhw.computeAverage(round);
		m_mapSTwN0ptN0synhw.computeAverage(round);
		m_mapSTwN0wN0synhpt.computeAverage(round);
		m_mapSTptN0wN0synhw.computeAverage(round);
		// st2 - n0
		m_mapST2POSPath.computeAverage(round);
		m_mapST2FPOSPath.computeAverage(round);
		m_mapST2wN0ptST2synhw.computeAverage(round);
		m_mapST2wN0wST2synhpt.computeAverage(round);
		m_mapST2ptN0wST2synhw.computeAverage(round);
		m_mapST2wN0ptN0synhw.computeAverage(round);
		m_mapST2wN0wN0synhpt.computeAverage(round);
		m_mapST2ptN0wN0synhw.computeAverage(round);
		// sst - n0
		m_mapSSTPOSPath.computeAverage(round);
		m_mapSSTFPOSPath.computeAverage(round);
		m_mapSSTwN0ptSSTsynhw.computeAverage(round);
		m_mapSSTwN0wSSTsynhpt.computeAverage(round);
		m_mapSSTptN0wSSTsynhw.computeAverage(round);
		m_mapSSTwN0ptN0synhw.computeAverage(round);
		m_mapSSTwN0wN0synhpt.computeAverage(round);
		m_mapSSTptN0wN0synhw.computeAverage(round);

		// supertag feature
		m_mapSTst.computeAverage(round);
		m_mapST2st.computeAverage(round);
		m_mapSSTst.computeAverage(round);
		m_mapN0ist.computeAverage(round);
		m_mapSTstST2st.computeAverage(round);
		m_mapSTstSSTst.computeAverage(round);
		m_mapSTstN0w.computeAverage(round);
		m_mapSTstN0pt.computeAverage(round);
		m_mapSTstN0wpt.computeAverage(round);
	}
}

#endif
