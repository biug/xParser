#ifndef _TWO_STACK_WEIGHT_H
#define _TWO_STACK_WEIGHT_H

#include "graph_transition_weight.h"
#include "graph_transition_depparser.h"
#include "include/learning/perceptron/packed_score.h"

namespace graph_transition {

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	class TwoStackWeight : public GraphWeightBase {
	public:

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
		TwoStackWeight(const std::string & sRead, const std::string & sRecord,
				DWord * pWords, DPOSTag * pPOSTags, DLabel * pLables, DSuperTag * pSuperTags,
				SuperTagCandidates * wordsCandidates, SuperTagCandidates * postagsCandidates);
		~TwoStackWeight();

		void loadScores() override;
		void saveScores() const override;
		void computeAverageFeatureWeights(const int & round) override;

		void getOrUpdateFeatureScores(GraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE> * parser, const STATE_TYPE & state, const int & sstIndex, const ActionScoreIncrement & amount);
	};

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::empty_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::start_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::end_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::middle_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> TagSet TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::empty_tagset = TagSet();

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::TwoStackWeight(const std::string & sRead, const std::string & sRecord,
			DWord * pWords, DPOSTag * pPOSTags, DLabel * pLables, DSuperTag * pSuperTags,
			SuperTagCandidates * wordsCandidates, SuperTagCandidates * postagsCandidates) :
		GraphWeightBase(sRead, sRecord, pWords, pPOSTags, pLables, pSuperTags, wordsCandidates, postagsCandidates),
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

		empty_taggedword.refer(m_pWords->lookup(EMPTY_WORD), m_pPOSTags->lookup(EMPTY_POSTAG));
		start_taggedword.refer(m_pWords->lookup(START_WORD), m_pPOSTags->lookup(START_POSTAG));
		middle_taggedword.refer(m_pWords->lookup(MIDDLE_WORD), m_pPOSTags->lookup(MIDDLE_POSTAG));
		end_taggedword.refer(m_pWords->lookup(END_WORD), m_pPOSTags->lookup(END_POSTAG));

		loadScores();
		std::cout << "load complete." << std::endl;
	}

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::~TwoStackWeight() = default;

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::loadScores() {

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

		input >> *m_pWords;

		input >> *m_pPOSTags;

		input >> *m_pLabels;

		input >> *m_pSuperTags;

		input >> *m_pSuperTagCandidatesOfWords;

		input >> *m_pSuperTagCandidatesOfPOSTags;

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

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::saveScores() const {

		if (m_sRecordPath.empty()) {
			return;
		}
		std::ofstream output(m_sRecordPath);
		if (!output) {
			return;
		}

		output << *m_pWords;

		output << *m_pPOSTags;

		output << *m_pLabels;

		output << *m_pSuperTags;

		output << *m_pSuperTagCandidatesOfWords;

		output << *m_pSuperTagCandidatesOfPOSTags;

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

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::computeAverageFeatureWeights(const int & round) {

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

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void TwoStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::getOrUpdateFeatureScores(GraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE> * parser, const STATE_TYPE & item, const int & sstIndex, const ActionScoreIncrement & amount) {
		int dis = -1;

		const int outIndex = -1;
		const int emptyLabel = 0;
		const int emptyArity = -1;
		const int emptySuperTag = 0;

		WordPOSTag (&sentence)[MAX_SENTENCE_SIZE] = parser->m_lSentence;
		RET_TYPE & packedScore = parser->m_lPackedScore;
		const int & scoreIndex = parser->m_nScoreIndex;
		const int & trainingRound = parser->m_nTrainingRound;
		const int & sentenceLength = parser->m_nSentenceLength;

		// st
		const int & st_index = item.stackBack() == -1 ? outIndex : item.stackTop();
		const int & stlh_index = st_index == -1 ? outIndex : item.leftHead(st_index);
		const int & stlp_index = st_index == -1 ? outIndex : item.leftPred(st_index);
		const int & strh_index = st_index == -1 ? outIndex : item.rightHead(st_index);
		const int & strp_index = st_index == -1 ? outIndex : item.rightPred(st_index);
		// st
		const WordPOSTag & st_word_postag = st_index == -1 ? start_taggedword : sentence[st_index];
		const WordPOSTag & stlh_word_postag = stlh_index == -1 ? empty_taggedword : sentence[stlh_index];
		const WordPOSTag & stlp_word_postag = stlp_index == -1 ? empty_taggedword : sentence[stlp_index];
		const WordPOSTag & strh_word_postag = strh_index == -1 ? empty_taggedword : sentence[strh_index];
		const WordPOSTag & strp_word_postag = strp_index == -1 ? empty_taggedword : sentence[strp_index];
		// st
		const Word & st_word = st_word_postag.first();
		const POSTag & st_postag = st_word_postag.second();
		const TagSet & st_llabelset = st_index == -1 ? empty_tagset : item.leftPredLabelSet(st_index);
		const TagSet & st_rlabelset = st_index == -1 ? empty_tagset : item.rightPredLabelSet(st_index);
		const Word & stlh_word = stlh_word_postag.first();
		const POSTag & stlh_postag = stlh_word_postag.second();
		const int & stlh_label = st_index == -1 ? emptyLabel : item.leftHeadLabel(st_index);
		const int & stlh_arity = st_index == -1 ? emptyArity : item.leftHeadArity(st_index);
		const Word & stlp_word = stlp_word_postag.first();
		const POSTag & stlp_postag = stlp_word_postag.second();
		const POSTag & stlp2_postag = (st_index == -1 || item.leftSubPred(st_index) == -1) ? empty_taggedword.second() : sentence[item.leftSubPred(st_index)].second();
		const int & stlp_label = st_index == -1 ? emptyLabel : item.leftPredLabel(st_index);
		const int & stlp2_label = st_index == -1 ? emptyLabel : item.leftSubPredLabel(st_index);
		const int & stlp_arity = st_index == -1 ? emptyArity : item.leftPredArity(st_index);
		const Word & strh_word = strh_word_postag.first();
		const POSTag & strh_postag = strh_word_postag.second();
		const int & strh_label = st_index == -1 ? emptyLabel : item.rightHeadLabel(st_index);
		const int & strh_arity = st_index == -1 ? emptyArity : item.rightHeadArity(st_index);
		const Word & strp_word = strp_word_postag.first();
		const POSTag & strp_postag = strp_word_postag.second();
		const POSTag & strp2_postag = (st_index == -1 || item.rightSubPred(st_index) == -1) ? empty_taggedword.second() : sentence[item.rightSubPred(st_index)].second();
		const int & strp_label = st_index == -1 ? emptyLabel : item.rightPredLabel(st_index);
		const int & strp2_label = st_index == -1 ? emptyLabel : item.rightSubPredLabel(st_index);
		const int & strp_arity = st_index == -1 ? emptyArity : item.rightPredArity(st_index);

		// st2
		const int & st2_index = item.stackBack() <= 0 ? outIndex : item.stackSubTop();
		const int & st2lh_index = st2_index == -1 ? outIndex : item.leftHead(st2_index);
		const int & st2lp_index = st2_index == -1 ? outIndex : item.leftPred(st2_index);
		const int & st2rh_index = st2_index == -1 ? outIndex : item.rightHead(st2_index);
		const int & st2rp_index = st2_index == -1 ? outIndex : item.rightPred(st2_index);
		// st2
		const WordPOSTag & st2_word_postag = st2_index == -1 ? start_taggedword : sentence[st2_index];
		const WordPOSTag & st2lh_word_postag = st2lh_index == -1 ? empty_taggedword : sentence[st2lh_index];
		const WordPOSTag & st2lp_word_postag = st2lp_index == -1 ? empty_taggedword : sentence[st2lp_index];
		const WordPOSTag & st2rh_word_postag = st2rh_index == -1 ? empty_taggedword : sentence[st2rh_index];
		const WordPOSTag & st2rp_word_postag = st2rp_index == -1 ? empty_taggedword : sentence[st2rp_index];
		// st2
		const Word & st2_word = st2_word_postag.first();
		const POSTag & st2_postag = st2_word_postag.second();
		const TagSet & st2_llabelset = st2_index == -1 ? empty_tagset : item.leftPredLabelSet(st2_index);
		const TagSet & st2_rlabelset = st2_index == -1 ? empty_tagset : item.rightPredLabelSet(st2_index);
		const Word & st2lh_word = st2lh_word_postag.first();
		const POSTag & st2lh_postag = st2lh_word_postag.second();
		const int & st2lh_label = st2_index == -1 ? emptyLabel : item.leftHeadLabel(st2_index);
		const int & st2lh_arity = st2_index == -1 ? emptyArity : item.leftHeadArity(st2_index);
		const Word & st2lp_word = st2lp_word_postag.first();
		const POSTag & st2lp_postag = st2lp_word_postag.second();
		const POSTag & st2lp2_postag = (st2_index == -1 || item.leftSubPred(st2_index) == -1) ? empty_taggedword.second() : sentence[item.leftSubPred(st2_index)].second();
		const int & st2lp_label = st2_index == -1 ? emptyLabel : item.leftPredLabel(st2_index);
		const int & st2lp2_label = st2_index == -1 ? emptyLabel : item.leftSubPredLabel(st2_index);
		const int & st2lp_arity = st2_index == -1 ? emptyArity : item.leftPredArity(st2_index);
		const Word & st2rh_word = st2rh_word_postag.first();
		const POSTag & st2rh_postag = st2rh_word_postag.second();
		const int & st2rh_label = st2_index == -1 ? emptyLabel : item.rightHeadLabel(st2_index);
		const int & st2rh_arity = st2_index == -1 ? emptyArity : item.rightHeadArity(st2_index);
		const Word & st2rp_word = st2rp_word_postag.first();
		const POSTag & st2rp_postag = st2rp_word_postag.second();
		const POSTag & st2rp2_postag = (st2_index == -1 || item.rightSubPred(st2_index) == -1) ? empty_taggedword.second() : sentence[item.rightSubPred(st2_index)].second();
		const int & st2rp_label = st2_index == -1 ? emptyLabel : item.rightPredLabel(st2_index);
		const int & st2rp2_label = st2_index == -1 ? emptyLabel : item.rightSubPredLabel(st2_index);
		const int & st2rp_arity = st2_index == -1 ? emptyArity : item.rightPredArity(st2_index);

		// sst
		// notice sst index
		const int & sst_index = sstIndex;
		const int & sstlh_index = sst_index == -1 ? outIndex : item.leftHead(sst_index);
		const int & sstlp_index = sst_index == -1 ? outIndex : item.leftPred(sst_index);
		const int & sstrh_index = sst_index == -1 ? outIndex : item.rightHead(sst_index);
		const int & sstrp_index = sst_index == -1 ? outIndex : item.rightPred(sst_index);
		// sst
		const WordPOSTag & sst_word_postag = sst_index == -1 ? middle_taggedword : sentence[sst_index];
		const WordPOSTag & sstlh_word_postag = sstlh_index == -1 ? empty_taggedword : sentence[sstlh_index];
		const WordPOSTag & sstlp_word_postag = sstlp_index == -1 ? empty_taggedword : sentence[sstlp_index];
		const WordPOSTag & sstrh_word_postag = sstrh_index == -1 ? empty_taggedword : sentence[sstrh_index];
		const WordPOSTag & sstrp_word_postag = sstrp_index == -1 ? empty_taggedword : sentence[sstrp_index];
		// sst
		const Word & sst_word = sst_word_postag.first();
		const POSTag & sst_postag = sst_word_postag.second();
		const TagSet & sst_llabelset = sst_index == -1 ? empty_tagset : item.leftPredLabelSet(sst_index);
		const TagSet & sst_rlabelset = sst_index == -1 ? empty_tagset : item.rightPredLabelSet(sst_index);
		const Word & sstlh_word = sstlh_word_postag.first();
		const POSTag & sstlh_postag = sstlh_word_postag.second();
		const int & sstlh_label = sst_index == -1 ? emptyLabel : item.leftHeadLabel(sst_index);
		const int & sstlh_arity = sst_index == -1 ? emptyArity : item.leftHeadArity(sst_index);
		const Word & sstlp_word = sstlp_word_postag.first();
		const POSTag & sstlp_postag = sstlp_word_postag.second();
		const POSTag & sstlp2_postag = (sst_index == -1 || item.leftSubPred(sst_index) == -1) ? empty_taggedword.second() : sentence[item.leftSubPred(sst_index)].second();
		const int & sstlp_label = sst_index == -1 ? emptyLabel : item.leftPredLabel(sst_index);
		const int & sstlp2_label = sst_index == -1 ? emptyLabel : item.leftSubPredLabel(sst_index);
		const int & sstlp_arity = sst_index == -1 ? emptyArity : item.leftPredArity(sst_index);
		const Word & sstrh_word = sstrh_word_postag.first();
		const POSTag & sstrh_postag = sstrh_word_postag.second();
		const int & sstrh_label = sst_index == -1 ? emptyLabel : item.rightHeadLabel(sst_index);
		const int & sstrh_arity = sst_index == -1 ? emptyArity : item.rightHeadArity(sst_index);
		const Word & sstrp_word = sstrp_word_postag.first();
		const POSTag & sstrp_postag = sstrp_word_postag.second();
		const POSTag & sstrp2_postag = (sst_index == -1 || item.rightSubPred(sst_index) == -1) ? empty_taggedword.second() : sentence[item.rightSubPred(sst_index)].second();
		const int & sstrp_label = sst_index == -1 ? emptyLabel : item.rightPredLabel(sst_index);
		const int & sstrp2_label = sst_index == -1 ? emptyLabel : item.rightSubPredLabel(sst_index);
		const int & sstrp_arity = sst_index == -1 ? emptyArity : item.rightPredArity(sst_index);

		// n0
		const int & n0_index = item.size() < sentenceLength ? item.size() : outIndex;
		const int & n0lp_index = n0_index == -1 ? outIndex : item.leftPred(n0_index);
		const int & n0lh_index = n0_index == -1 ? outIndex : item.leftHead(n0_index);
		// n0
		const WordPOSTag & n0_word_postag = n0_index == -1 ? end_taggedword : sentence[n0_index];
		const WordPOSTag & n0lh_word_postag = n0lh_index == -1 ? empty_taggedword : sentence[n0lh_index];
		const WordPOSTag & n0lp_word_postag = n0lp_index == -1 ? empty_taggedword : sentence[n0lp_index];
		// n0
		const Word & n0_word = n0_word_postag.first();
		const POSTag & n0_postag = n0_word_postag.second();
		const TagSet & n0_llabelset = n0_index == -1 ? empty_tagset : item.leftPredLabelSet(n0_index);
		const Word & n0lh_word = n0lh_word_postag.first();
		const POSTag & n0lh_postag = n0lh_word_postag.second();
		const int & n0lh_label = n0_index == -1 ? emptyLabel : item.leftHeadLabel(n0_index);
		const int & n0lh_arity = n0_index == -1 ? emptyArity : item.leftHeadArity(n0_index);
		const Word & n0lp_word = n0lp_word_postag.first();
		const POSTag & n0lp_postag = n0lp_word_postag.second();
		const POSTag & n0lp2_postag = (n0_index == -1 || item.leftSubPred(n0_index) == -1) ? empty_taggedword.second() : sentence[item.leftSubPred(n0_index)].second();
		const int & n0lp_label = n0_index == -1 ? emptyLabel : item.leftPredLabel(n0_index);
		const int & n0lp2_label = n0_index == -1 ? emptyLabel : item.leftSubPredLabel(n0_index);
		const int & n0lp_arity = n0_index == -1 ? emptyArity : item.leftPredArity(n0_index);

		// n0
		m_mapN0w.getOrUpdateScore(packedScore, n0_word, scoreIndex, amount, trainingRound);
		m_mapN0pt.getOrUpdateScore(packedScore, n0_postag, scoreIndex, amount, trainingRound);
		// unigram with label
		bi_features.refer(n0_word, n0lh_label);
		m_mapN0wN0LHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0lp_label);
		m_mapN0wN0LPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0_postag, n0lh_label);
		m_mapN0ptN0LHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0lp_label);
		m_mapN0ptN0LPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// unigram with arity
		bi_features.refer(n0_word, n0lh_arity);
		m_mapN0wN0LHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0lp_arity);
		m_mapN0wN0LPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0_postag, n0lh_arity);
		m_mapN0ptN0LHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0lp_arity);
		m_mapN0ptN0LPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);

		// st
		m_mapSTw.getOrUpdateScore(packedScore, st_word, scoreIndex, amount, trainingRound);
		m_mapSTpt.getOrUpdateScore(packedScore, st_postag, scoreIndex, amount, trainingRound);
		// unigram with label
		bi_features.refer(st_word, stlh_label);
		m_mapSTwSTLHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(stlp_label);
		m_mapSTwSTLPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(strh_label);
		m_mapSTwSTRHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(strp_label);
		m_mapSTwSTRPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// unigram with arity
		bi_features.referLast(stlh_arity);
		m_mapSTwSTLHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(stlp_arity);
		m_mapSTwSTLPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(strh_arity);
		m_mapSTwSTRHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(strp_arity);
		m_mapSTwSTRPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(stlh_arity + strh_arity);
		m_mapSTwSTHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(stlp_arity + strp_arity);
		m_mapSTwSTPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// bigram st + n0
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, n0_postag);
		m_mapSTptN0pt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0_word);
		m_mapSTwN0w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0_postag);
		m_mapSTwN0pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapSTwptN0pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0_word, st_postag);
		m_mapSTptN0w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapSTptN0wpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st_word, n0_word, st_postag);
		m_mapSTwptN0w.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(n0_postag);
		m_mapSTwN0wpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(poses_feature);
		m_mapSTwptN0wpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// bigram with arity
		tri_features.referLast(strp_arity);
		m_mapSTwN0wSTRPi.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(n0lp_arity);
		m_mapSTwN0wN0LPi.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// bigram with dis
		dis = encodeLinkDistance(st_index, n0_index == -1 ? sentenceLength : n0_index);
		tri_features.referLast(dis);
		m_mapSTwN0wD.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st_word, n0_postag, dis);
		m_mapSTwN0ptD.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, st_postag, dis);
		m_mapSTptN0wD.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(poses_feature, dis);
		m_mapSTptN0ptD.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);

		// st2
		m_mapST2w.getOrUpdateScore(packedScore, st2_word, scoreIndex, amount, trainingRound);
		m_mapST2pt.getOrUpdateScore(packedScore, st2_postag, scoreIndex, amount, trainingRound);
		// unigram with label
		bi_features.refer(st2_word, st2lh_label);
		m_mapST2wST2LHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(st2lp_label);
		m_mapST2wST2LPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(st2rh_label);
		m_mapST2wST2RHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(st2rp_label);
		m_mapST2wST2RPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// unigram with arity
		bi_features.referLast(st2lh_arity);
		m_mapST2wST2LHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(st2lp_arity);
		m_mapST2wST2LPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(st2rh_arity);
		m_mapST2wST2RHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(st2rp_arity);
		m_mapST2wST2RPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(st2lh_arity + st2rh_arity);
		m_mapST2wST2Hi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(st2lp_arity + st2rp_arity);
		m_mapST2wST2Pi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// bigram st2 + n0
		poses_feature = ENCODE_POSTAG_SET_2(st2_postag, n0_postag);
		m_mapST2ptN0pt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0_word);
		m_mapST2wN0w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0_postag);
		m_mapST2wN0pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapST2wptN0pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0_word, st2_postag);
		m_mapST2ptN0w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapST2ptN0wpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2_word, n0_word, st2_postag);
		m_mapST2wptN0w.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(n0_postag);
		m_mapST2wN0wpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(poses_feature);
		m_mapST2wptN0wpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// bigram with arity
		tri_features.referLast(st2rp_arity);
		m_mapST2wN0wST2RPi.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(n0lp_arity);
		m_mapST2wN0wN0LPi.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// bigram with dis
		dis = encodeLinkDistance(st2_index, n0_index == -1 ? sentenceLength : n0_index);
		tri_features.referLast(dis);
		m_mapST2wN0wD.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2_word, n0_postag, dis);
		m_mapST2wN0ptD.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, st2_postag, dis);
		m_mapST2ptN0wD.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(poses_feature, dis);
		m_mapST2ptN0ptD.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// bigram st + st2
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, st2_postag);
		m_mapSTptST2pt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
		bi_features.refer(st_word, st2_word);
		m_mapSTwST2w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(st2_postag);
		m_mapSTwST2pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapSTwptST2pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(st2_word, st_postag);
		m_mapSTptST2w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapSTptST2wpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st_word, st2_word, st_postag);
		m_mapSTwptST2w.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(st2_postag);
		m_mapSTwST2wpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(poses_feature);
		m_mapSTwptST2wpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);

		// sst
		m_mapSSTw.getOrUpdateScore(packedScore, sst_word, scoreIndex, amount, trainingRound);
		m_mapSSTpt.getOrUpdateScore(packedScore, sst_postag, scoreIndex, amount, trainingRound);
		// unigram with label
		bi_features.refer(sst_word, sstlh_label);
		m_mapSSTwSSTLHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(sstlp_label);
		m_mapSSTwSSTLPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(sstrh_label);
		m_mapSSTwSSTRHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(sstrp_label);
		m_mapSSTwSSTRPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// unigram with arity
		// sst
		bi_features.referLast(sstlh_arity);
		m_mapSSTwSSTLHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(sstlp_arity);
		m_mapSSTwSSTLPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(sstrh_arity);
		m_mapSSTwSSTRHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(sstrp_arity);
		m_mapSSTwSSTRPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(sstlh_arity + sstrh_arity);
		m_mapSSTwSSTHi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(sstlp_arity + sstrp_arity);
		m_mapSSTwSSTPi.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// bigram sst + n0
		poses_feature = ENCODE_POSTAG_SET_2(sst_postag, n0_postag);
		m_mapSSTptN0pt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0_word);
		m_mapSSTwN0w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(n0_postag);
		m_mapSSTwN0pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapSSTwptN0pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0_word, sst_postag);
		m_mapSSTptN0w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapSSTptN0wpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sst_word, n0_word, sst_postag);
		m_mapSSTwptN0w.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(n0_postag);
		m_mapSSTwN0wpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(poses_feature);
		m_mapSSTwptN0wpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// bigram with arity
		tri_features.referLast(sstrp_arity);
		m_mapSSTwN0wSSTRPi.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(n0lp_arity);
		m_mapSSTwN0wN0LPi.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// bigram with dis
		dis = encodeLinkDistance(sst_index, n0_index == -1 ? sentenceLength : n0_index);
		tri_features.referLast(dis);
		m_mapSSTwN0wD.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sst_word, n0_postag, dis);
		m_mapSSTwN0ptD.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, sst_postag, dis);
		m_mapSSTptN0wD.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(poses_feature, dis);
		m_mapSSTptN0ptD.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// bigram st + sst
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, sst_postag);
		m_mapSTptSSTpt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
		bi_features.refer(st_word, sst_word);
		m_mapSTwSSTw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(sst_postag);
		m_mapSTwSSTpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapSTwptSSTpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(sst_word, st_postag);
		m_mapSTptSSTw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.referLast(poses_feature);
		m_mapSTptSSTwpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st_word, sst_word, st_postag);
		m_mapSTwptSSTw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(sst_postag);
		m_mapSTwSSTwpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.referLast(poses_feature);
		m_mapSTwptSSTwpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);

		// tri-gram
		// st + n0 + st2
		bi_features.refer(st_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag));
		m_mapSTwN0ptST2pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, st2_postag));
		m_mapSTptN0wST2pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(st2_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag));
		m_mapSTptN0ptST2w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		poses_feature = ENCODE_POSTAG_SET_3(st_postag, st2_postag, n0_postag);
		m_mapSTptN0ptST2pt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
		// st + n0 + sst
		bi_features.refer(st_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag));
		m_mapSTwN0ptSSTpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, sst_postag));
		m_mapSTptN0wSSTpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(sst_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag));
		m_mapSTptN0ptSSTw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		poses_feature = ENCODE_POSTAG_SET_3(st_postag, sst_postag, n0_postag);
		m_mapSTptN0ptSSTpt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);

		// st + n0 + st left/right head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlh_postag, n0_postag), stlh_label);
		m_mapSTwN0ptSTLHptSTLHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, stlh_postag), stlh_label);
		m_mapSTptN0wSTLHptSTLHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(stlh_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), stlh_label);
		m_mapSTptN0ptSTLHwSTLHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlh_postag, n0_postag), stlh_label);
		m_mapSTptN0ptSTLHptSTLHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlp_postag, n0_postag), stlp_label);
		m_mapSTwN0ptSTLPptSTLPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, stlp_postag), stlp_label);
		m_mapSTptN0wSTLPptSTLPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(stlp_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), stlp_label);
		m_mapSTptN0ptSTLPwSTLPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlp_postag, n0_postag), stlp_label);
		m_mapSTptN0ptSTLPptSTLPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strh_postag, n0_postag), strh_label);
		m_mapSTwN0ptSTRHptSTRHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, strh_postag), strh_label);
		m_mapSTptN0wSTRHptSTRHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(strh_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), strh_label);
		m_mapSTptN0ptSTRHwSTRHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strh_postag, n0_postag), strh_label);
		m_mapSTptN0ptSTRHptSTRHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strp_postag, n0_postag), strp_label);
		m_mapSTwN0ptSTRPptSTRPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, strp_postag), strp_label);
		m_mapSTptN0wSTRPptSTRPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(strp_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), strp_label);
		m_mapSTptN0ptSTRPwSTRPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strp_postag, n0_postag), strp_label);
		m_mapSTptN0ptSTRPptSTRPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);

		// st + n0 + n0 left head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(n0_postag, n0lh_postag), n0lh_label);
		m_mapSTwN0ptN0LHptN0LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, n0lh_postag), n0lh_label);
		m_mapSTptN0wN0LHptN0LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0lh_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), n0lh_label);
		m_mapSTptN0ptN0LHwN0LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, n0_postag, n0lh_postag), n0lh_label);
		m_mapSTptN0ptN0LHptN0LHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(n0_postag, n0lp_postag), n0lp_label);
		m_mapSTwN0ptN0LPptN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, n0lp_postag), n0lp_label);
		m_mapSTptN0wN0LPptN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0lp_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), n0lp_label);
		m_mapSTptN0ptN0LPwN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, n0_postag, n0lp_postag), n0lp_label);
		m_mapSTptN0ptN0LPptN0LPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);

		// quar-gram
		// st + n0 + st right head + st right pred
		tri_features.refer(encodePOSTagSet4(st_postag, strh_postag, strp_postag, n0_postag), strh_label, strp_label);
		m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// st + n0 + st left pred + st left pred 2
		tri_features.refer(encodePOSTagSet4(st_postag, stlp_postag, stlp2_postag, n0_postag), stlp_label, stlp2_label);
		m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// st + n0 + st right pred + st right pred 2
		tri_features.refer(encodePOSTagSet4(st_postag, strp_postag, strp2_postag, n0_postag), strp_label, strp2_label);
		m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// st + n0 + n0 left head + n0 left pred
		tri_features.refer(encodePOSTagSet4(st_postag, n0_postag, n0lh_postag, n0lp_postag), n0lh_label, n0lp_label);
		m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// st + n0 + n0 left pred + n0 left pred 2
		tri_features.refer(encodePOSTagSet4(st_postag, n0_postag, n0lp_postag, n0lp2_postag), n0lp_label, n0lp2_label);
		m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);

		// st2 + n0 + st2 left/right head/pred
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2lh_postag, n0_postag), st2lh_label);
		m_mapST2wN0ptST2LHptST2LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, st2lh_postag), st2lh_label);
		m_mapST2ptN0wST2LHptST2LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2lh_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2lh_label);
		m_mapST2ptN0ptST2LHwST2LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2lh_postag, n0_postag), st2lh_label);
		m_mapST2ptN0ptST2LHptST2LHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2lp_postag, n0_postag), st2lp_label);
		m_mapST2wN0ptST2LPptST2LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, st2lp_postag), st2lp_label);
		m_mapST2ptN0wST2LPptST2LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2lp_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2lp_label);
		m_mapST2ptN0ptST2LPwST2LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2lp_postag, n0_postag), st2lp_label);
		m_mapST2ptN0ptST2LPptST2LPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2rh_postag, n0_postag), st2rh_label);
		m_mapST2wN0ptST2RHptST2RHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, st2rh_postag), st2rh_label);
		m_mapST2ptN0wST2RHptST2RHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2rh_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2rh_label);
		m_mapST2ptN0ptST2RHwST2RHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2rh_postag, n0_postag), st2rh_label);
		m_mapST2ptN0ptST2RHptST2RHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2rp_postag, n0_postag), st2rp_label);
		m_mapST2wN0ptST2RPptST2RPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, st2rp_postag), st2rp_label);
		m_mapST2ptN0wST2RPptST2RPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2rp_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2rp_label);
		m_mapST2ptN0ptST2RPwST2RPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2rp_postag, n0_postag), st2rp_label);
		m_mapST2ptN0ptST2RPptST2RPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);

		// st2 + n0 + n0 left head/pred
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(n0_postag, n0lh_postag), n0lh_label);
		m_mapST2wN0ptN0LHptN0LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, n0lh_postag), n0lh_label);
		m_mapST2ptN0wN0LHptN0LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0lh_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), n0lh_label);
		m_mapST2ptN0ptN0LHwN0LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, n0_postag, n0lh_postag), n0lh_label);
		m_mapST2ptN0ptN0LHptN0LHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(n0_postag, n0lp_postag), n0lp_label);
		m_mapST2wN0ptN0LPptN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, n0lp_postag), n0lp_label);
		m_mapST2ptN0wN0LPptN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0lp_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), n0lp_label);
		m_mapST2ptN0ptN0LPwN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, n0_postag, n0lp_postag), n0lp_label);
		m_mapST2ptN0ptN0LPptN0LPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		tri_features.refer(encodePOSTagSet4(st2_postag, st2rh_postag, st2rp_postag, n0_postag), st2rh_label, st2rp_label);
		m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// st2 + n0 + st2 left pred + st2 left pred 2
		tri_features.refer(encodePOSTagSet4(st2_postag, st2lp_postag, st2lp2_postag, n0_postag), st2lp_label, st2lp2_label);
		m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// st2 + n0 + st2 right pred + st2 right pred 2
		tri_features.refer(encodePOSTagSet4(st2_postag, st2rp_postag, st2rp2_postag, n0_postag), st2rp_label, st2rp2_label);
		m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// st2 + n0 + n0 left head + n0 left pred
		tri_features.refer(encodePOSTagSet4(st2_postag, n0_postag, n0lh_postag, n0lp_postag), n0lh_label, n0lp_label);
		m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// st2 + n0 + n0 left pred + n0 left pred 2
		tri_features.refer(encodePOSTagSet4(st2_postag, n0_postag, n0lp_postag, n0lp2_postag), n0lp_label, n0lp2_label);
		m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);

		// sst + n0 + sst left/right head/pred
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstlh_postag, n0_postag), sstlh_label);
		m_mapSSTwN0ptSSTLHptSSTLHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, sstlh_postag), sstlh_label);
		m_mapSSTptN0wSSTLHptSSTLHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sstlh_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sstlh_label);
		m_mapSSTptN0ptSSTLHwSSTLHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstlh_postag, n0_postag), sstlh_label);
		m_mapSSTptN0ptSSTLHptSSTLHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstlp_postag, n0_postag), sstlp_label);
		m_mapSSTwN0ptSSTLPptSSTLPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, sstlp_postag), sstlp_label);
		m_mapSSTptN0wSSTLPptSSTLPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sstlp_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sstlp_label);
		m_mapSSTptN0ptSSTLPwSSTLPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstlp_postag, n0_postag), sstlp_label);
		m_mapSSTptN0ptSSTLPptSSTLPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstrh_postag, n0_postag), sstrh_label);
		m_mapSSTwN0ptSSTRHptSSTRHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, sstrh_postag), sstrh_label);
		m_mapSSTptN0wSSTRHptSSTRHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sstrh_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sstrh_label);
		m_mapSSTptN0ptSSTRHwSSTRHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstrh_postag, n0_postag), sstrh_label);
		m_mapSSTptN0ptSSTRHptSSTRHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstrp_postag, n0_postag), sstrp_label);
		m_mapSSTwN0ptSSTRPptSSTRPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, sstrp_postag), sstrp_label);
		m_mapSSTptN0wSSTRPptSSTRPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sstrp_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sstrp_label);
		m_mapSSTptN0ptSSTRPwSSTRPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstrp_postag, n0_postag), sstrp_label);
		m_mapSSTptN0ptSSTRPptSSTRPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);

		// sst + n0 + n0 left head/pred
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(n0_postag, n0lh_postag), n0lh_label);
		m_mapSSTwN0ptN0LHptN0LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, n0lh_postag), n0lh_label);
		m_mapSSTptN0wN0LHptN0LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0lh_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), n0lh_label);
		m_mapSSTptN0ptN0LHwN0LHl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, n0_postag, n0lh_postag), n0lh_label);
		m_mapSSTptN0ptN0LHptN0LHl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(n0_postag, n0lp_postag), n0lp_label);
		m_mapSSTwN0ptN0LPptN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, n0lp_postag), n0lp_label);
		m_mapSSTptN0wN0LPptN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0lp_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), n0lp_label);
		m_mapSSTptN0ptN0LPwN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, n0_postag, n0lp_postag), n0lp_label);
		m_mapSSTptN0ptN0LPptN0LPl.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		tri_features.refer(encodePOSTagSet4(sst_postag, sstrh_postag, sstrp_postag, n0_postag), sstrh_label, sstrp_label);
		m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// sst + n0 + sst left pred + sst left pred 2
		tri_features.refer(encodePOSTagSet4(sst_postag, sstlp_postag, sstlp2_postag, n0_postag), sstlp_label, sstlp2_label);
		m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// sst + n0 + sst right pred + sst right pred 2
		tri_features.refer(encodePOSTagSet4(sst_postag, sstrp_postag, sstrp2_postag, n0_postag), sstrp_label, sstrp2_label);
		m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// sst + n0 + n0 left head + n0 left pred
		tri_features.refer(encodePOSTagSet4(sst_postag, n0_postag, n0lh_postag, n0lp_postag), n0lh_label, n0lp_label);
		m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// sst + n0 + n0 left pred + n0 left pred 2
		tri_features.refer(encodePOSTagSet4(sst_postag, n0_postag, n0lp_postag, n0lp2_postag), n0lp_label, n0lp2_label);
		m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);

		// st + n0 + labelset
		uni_tagset.refer(st_word, st_llabelset.bits(0), st_llabelset.bits(1));
		m_mapSTwSTll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(st_word, st_rlabelset.bits(0), st_rlabelset.bits(1));
		m_mapSTwSTrl.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(n0_word, n0_llabelset.bits(0), n0_llabelset.bits(1));
		m_mapN0wN0ll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(st_postag, st_llabelset.bits(0), st_llabelset.bits(1));
		m_mapSTptSTll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(st_postag, st_rlabelset.bits(0), st_rlabelset.bits(1));
		m_mapSTptSTrl.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(n0_postag, n0_llabelset.bits(0), n0_llabelset.bits(1));
		m_mapN0ptN0ll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, n0_postag);
		uni_tagset.refer(poses_feature, st_llabelset.bits(0), st_llabelset.bits(1));
		m_mapSTptN0ptSTll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(poses_feature, st_rlabelset.bits(0), st_rlabelset.bits(1));
		m_mapSTptN0ptSTrl.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(poses_feature, n0_llabelset.bits(0), n0_llabelset.bits(1));
		m_mapSTptN0ptN0ll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);

		// st2 + n0 + labelset
		uni_tagset.refer(st2_word, st2_llabelset.bits(0), st2_llabelset.bits(1));
		m_mapST2wST2ll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(st2_word, st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		m_mapST2wST2rl.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(st2_postag, st2_llabelset.bits(0), st2_llabelset.bits(1));
		m_mapST2ptST2ll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(st2_postag, st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		m_mapST2ptST2rl.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st2_postag, n0_postag);
		uni_tagset.refer(poses_feature, st2_llabelset.bits(0), st2_llabelset.bits(1));
		m_mapST2ptN0ptST2ll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(poses_feature, st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		m_mapST2ptN0ptST2rl.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(poses_feature, n0_llabelset.bits(0), n0_llabelset.bits(1));
		m_mapST2ptN0ptN0ll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);

		// sst + n0 + labelset
		uni_tagset.refer(sst_word, sst_llabelset.bits(0), sst_llabelset.bits(1));
		m_mapSSTwSSTll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(sst_word, sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		m_mapSSTwSSTrl.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(sst_postag, sst_llabelset.bits(0), sst_llabelset.bits(1));
		m_mapSSTptSSTll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(sst_postag, sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		m_mapSSTptSSTrl.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(sst_postag, n0_postag);
		uni_tagset.refer(poses_feature, sst_llabelset.bits(0), sst_llabelset.bits(1));
		m_mapSSTptN0ptSSTll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(poses_feature, sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		m_mapSSTptN0ptSSTrl.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		uni_tagset.refer(poses_feature, n0_llabelset.bits(0), n0_llabelset.bits(1));
		m_mapSSTptN0ptN0ll.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);

		// context
		const int & stl2_index = st_index >= 2 ? st_index - 2 : outIndex;
		const int & stl1_index = st_index >= 1 ? st_index - 1 : outIndex;
		const int & str1_index = st_index + 1 < sentenceLength ? st_index + 1 : outIndex;
		const int & str2_index = st_index + 2 < sentenceLength ? st_index + 2 : outIndex;
		const int & n0l2_index = n0_index >= 2 ? n0_index - 2 : outIndex;
		const int & n0l1_index = n0_index >= 1 ? n0_index - 1 : outIndex;
		const int & n0r1_index = n0_index + 1 < sentenceLength ? n0_index + 1 : outIndex;
		const int & n0r2_index = n0_index + 2 < sentenceLength ? n0_index + 2 : outIndex;
		// context
		const WordPOSTag & stl2_word_postag = stl2_index == -1 ? start_taggedword : sentence[stl2_index];
		const WordPOSTag & stl1_word_postag = stl1_index == -1 ? start_taggedword : sentence[stl1_index];
		const WordPOSTag & str1_word_postag = str1_index == -1 ? end_taggedword : sentence[str1_index];
		const WordPOSTag & str2_word_postag = str2_index == -1 ? end_taggedword : sentence[str2_index];
		const WordPOSTag & n0l2_word_postag = n0l2_index == -1 ? start_taggedword : sentence[n0l2_index];
		const WordPOSTag & n0l1_word_postag = n0l1_index == -1 ? start_taggedword : sentence[n0l1_index];
		const WordPOSTag & n0r1_word_postag = n0r1_index == -1 ? end_taggedword : sentence[n0r1_index];
		const WordPOSTag & n0r2_word_postag = n0r2_index == -1 ? end_taggedword : sentence[n0r2_index];
		// context
		const Word & stl2_word = stl2_word_postag.first();
		const POSTag & stl2_postag = stl2_word_postag.second();
		const Word & stl1_word = stl1_word_postag.first();
		const POSTag & stl1_postag = stl1_word_postag.second();
		const Word & str1_word = str1_word_postag.first();
		const POSTag & str1_postag = str1_word_postag.second();
		const Word & str2_word = str2_word_postag.first();
		const POSTag & str2_postag = str2_word_postag.second();
		const Word & n0l2_word = n0l2_word_postag.first();
		const POSTag & n0l2_postag = n0l2_word_postag.second();
		const Word & n0l1_word = n0l1_word_postag.first();
		const POSTag & n0l1_postag = n0l1_word_postag.second();
		const Word & n0r1_word = n0r1_word_postag.first();
		const POSTag & n0r1_postag = n0r1_word_postag.second();
		const Word & n0r2_word = n0r2_word_postag.first();
		const POSTag & n0r2_postag = n0r2_word_postag.second();
		// unigram context
		// st + sti, n0 + n0i
		// st word
		bi_features.refer(stl2_word, -2);
		m_mapSTiw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(stl1_word, -1);
		m_mapSTiw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(str1_word, 1);
		m_mapSTiw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(str2_word, 2);
		m_mapSTiw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(stl2_word, stl1_word, -1);
		m_mapSTiwSTjw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(stl1_word, str1_word, 0);
		m_mapSTiwSTjw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(str1_word, str2_word, 1);
		m_mapSTiwSTjw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// n0 word
		bi_features.refer(n0l2_word, -2);
		m_mapN0iw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0l1_word, -1);
		m_mapN0iw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0r1_word, 1);
		m_mapN0iw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0r2_word, 2);
		m_mapN0iw.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0l2_word, n0l1_word, -1);
		m_mapN0iwN0jw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0l1_word, n0r1_word, 0);
		m_mapN0iwN0jw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		tri_features.refer(n0r1_word, n0r2_word, 1);
		m_mapN0iwN0jw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		// st pos
		bi_features.refer(stl2_postag, -2);
		m_mapSTipt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(stl1_postag, -1);
		m_mapSTipt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(str1_postag, 1);
		m_mapSTipt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(str2_postag, 2);
		m_mapSTipt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(stl2_postag, stl1_postag), -1);
		m_mapSTiptSTjpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(stl1_postag, str1_postag), 0);
		m_mapSTiptSTjpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(str1_postag, str2_postag), 1);
		m_mapSTiptSTjpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		// n0 pos
		bi_features.refer(n0l2_postag, -2);
		m_mapN0ipt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0l1_postag, -1);
		m_mapN0ipt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0r1_postag, 1);
		m_mapN0ipt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(n0r2_postag, 2);
		m_mapN0ipt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(n0l2_postag, n0l1_postag), -1);
		m_mapN0iptN0jpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(n0l1_postag, n0r1_postag), 0);
		m_mapN0iptN0jpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(n0r1_postag, n0r2_postag), 1);
		m_mapN0iptN0jpt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);

		if (parser->m_bChar) {
			const Sentence & charSentence = parser->m_sSentence;

			m_map1CharBeforeST.getOrUpdateScore(packedScore, nCharPrev(charSentence, st_index == -1 ? 0 : st_index, 1), scoreIndex, amount, trainingRound);
			m_map2CharBeforeST.getOrUpdateScore(packedScore, nCharPrev(charSentence, st_index == -1 ? 0 : st_index, 2), scoreIndex, amount, trainingRound);
			m_map3CharBeforeST.getOrUpdateScore(packedScore, nCharPrev(charSentence, st_index == -1 ? 0 : st_index, 3), scoreIndex, amount, trainingRound);
			m_map1CharAfterST.getOrUpdateScore(packedScore, nCharNext(charSentence, st_index == -1 ? 0 : st_index, 1), scoreIndex, amount, trainingRound);
			m_map2CharAfterST.getOrUpdateScore(packedScore, nCharNext(charSentence, st_index == -1 ? 0 : st_index, 2), scoreIndex, amount, trainingRound);
			m_map3CharAfterST.getOrUpdateScore(packedScore, nCharNext(charSentence, st_index == -1 ? 0 : st_index, 3), scoreIndex, amount, trainingRound);

			m_map1CharBeforeN0.getOrUpdateScore(packedScore, nCharPrev(charSentence, n0_index == -1 ? sentenceLength : n0_index, 1), scoreIndex, amount, trainingRound);
			m_map2CharBeforeN0.getOrUpdateScore(packedScore, nCharPrev(charSentence, n0_index == -1 ? sentenceLength : n0_index, 2), scoreIndex, amount, trainingRound);
			m_map3CharBeforeN0.getOrUpdateScore(packedScore, nCharPrev(charSentence, n0_index == -1 ? sentenceLength : n0_index, 3), scoreIndex, amount, trainingRound);
			m_map1CharAfterN0.getOrUpdateScore(packedScore, nCharNext(charSentence, n0_index == -1 ? sentenceLength : n0_index, 1), scoreIndex, amount, trainingRound);
			m_map2CharAfterN0.getOrUpdateScore(packedScore, nCharNext(charSentence, n0_index == -1 ? sentenceLength : n0_index, 2), scoreIndex, amount, trainingRound);
			m_map3CharAfterN0.getOrUpdateScore(packedScore, nCharNext(charSentence, n0_index == -1 ? sentenceLength : n0_index, 3), scoreIndex, amount, trainingRound);
		}

		if (parser->m_bPath) {

			LCA & analyzer = parser->m_lcaAnalyzer;
			const DependencyTree & syntaxTree = parser->m_dtSyntaxTree;
			const WordPOSTag & st_syn_head_word_postag = (st_index == -1 || TREENODE_HEAD(syntaxTree[st_index]) == -1 ? empty_taggedword : sentence[TREENODE_HEAD(syntaxTree[st_index])]);
			const WordPOSTag & n0_syn_head_word_postag = (n0_index == -1 || TREENODE_HEAD(syntaxTree[n0_index]) == -1 ? empty_taggedword : sentence[TREENODE_HEAD(syntaxTree[n0_index])]);
			const WordPOSTag & st2_syn_head_word_postag = (st2_index == -1 || TREENODE_HEAD(syntaxTree[st2_index]) == -1 ? empty_taggedword : sentence[TREENODE_HEAD(syntaxTree[st2_index])]);
			const WordPOSTag & sst_syn_head_word_postag = (sst_index == -1 || TREENODE_HEAD(syntaxTree[sst_index]) == -1 ? empty_taggedword : sentence[TREENODE_HEAD(syntaxTree[sst_index])]);

			if (st_index == -1 || n0_index == -1) {
				m_mapSTPOSPath.getOrUpdateScore(packedScore, "n#", scoreIndex, amount, trainingRound);
				m_mapSTFPOSPath.getOrUpdateScore(packedScore, "n#", scoreIndex, amount, trainingRound);
			}
			else {
				m_mapSTPOSPath.getOrUpdateScore(packedScore, analyzer.POSPath[st_index][n0_index], scoreIndex, amount, trainingRound);
				m_mapSTFPOSPath.getOrUpdateScore(packedScore, analyzer.FPOSPath[st_index][n0_index], scoreIndex, amount, trainingRound);
			}
			tri_features.refer(st_word, st_syn_head_word_postag.first(), n0_postag);
			m_mapSTwN0ptSTsynhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(st_word, n0_postag, st_syn_head_word_postag.second());
			m_mapSTwN0wSTsynhpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(n0_word, st_syn_head_word_postag.first(), st_postag);
			m_mapSTptN0wSTsynhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(st_word, n0_syn_head_word_postag.first(), n0_postag);
			m_mapSTwN0ptN0synhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(st_word, n0_postag, n0_syn_head_word_postag.second());
			m_mapSTwN0wN0synhpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(n0_word, n0_syn_head_word_postag.first(), st_postag);
			m_mapSTptN0wN0synhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);

			if (st2_index == -1 || n0_index == -1) {
				m_mapST2POSPath.getOrUpdateScore(packedScore, "n#", scoreIndex, amount, trainingRound);
				m_mapST2FPOSPath.getOrUpdateScore(packedScore, "n#", scoreIndex, amount, trainingRound);
			}
			else {
				m_mapST2POSPath.getOrUpdateScore(packedScore, analyzer.POSPath[st2_index][n0_index], scoreIndex, amount, trainingRound);
				m_mapST2FPOSPath.getOrUpdateScore(packedScore, analyzer.FPOSPath[st2_index][n0_index], scoreIndex, amount, trainingRound);
			}
			tri_features.refer(st2_word, st2_syn_head_word_postag.first(), n0_postag);
			m_mapST2wN0ptST2synhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(st2_word, n0_postag, st2_syn_head_word_postag.second());
			m_mapST2wN0wST2synhpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(n0_word, st2_syn_head_word_postag.first(), st2_postag);
			m_mapST2ptN0wST2synhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(st2_word, n0_syn_head_word_postag.first(), n0_postag);
			m_mapST2wN0ptN0synhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(st2_word, n0_postag, n0_syn_head_word_postag.second());
			m_mapST2wN0wN0synhpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(n0_word, n0_syn_head_word_postag.first(), st2_postag);
			m_mapST2ptN0wN0synhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);

			if (sst_index == -1 || n0_index == -1) {
				m_mapST2POSPath.getOrUpdateScore(packedScore, "n#", scoreIndex, amount, trainingRound);
				m_mapST2FPOSPath.getOrUpdateScore(packedScore, "n#", scoreIndex, amount, trainingRound);
			}
			else {
				m_mapSSTPOSPath.getOrUpdateScore(packedScore, analyzer.POSPath[sst_index][n0_index], scoreIndex, amount, trainingRound);
				m_mapSSTFPOSPath.getOrUpdateScore(packedScore, analyzer.FPOSPath[sst_index][n0_index], scoreIndex, amount, trainingRound);
			}
			tri_features.refer(sst_word, sst_syn_head_word_postag.first(), n0_postag);
			m_mapSSTwN0ptSSTsynhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(sst_word, n0_postag, sst_syn_head_word_postag.second());
			m_mapSSTwN0wSSTsynhpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(n0_word, sst_syn_head_word_postag.first(), sst_postag);
			m_mapSSTptN0wSSTsynhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(sst_word, n0_syn_head_word_postag.first(), n0_postag);
			m_mapSSTwN0ptN0synhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(sst_word, n0_postag, n0_syn_head_word_postag.second());
			m_mapSSTwN0wN0synhpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.refer(n0_word, n0_syn_head_word_postag.first(), sst_postag);
			m_mapSSTptN0wN0synhw.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);

		}

		if (parser->m_bSuperTag) {
			const SuperTag & st_supertag = st_index == -1 ? emptySuperTag : item.superTag(st_index);
			const SuperTag & st2_supertag = st2_index == -1 ? emptySuperTag : item.superTag(st2_index);
			const SuperTag & sst_supertag = sst_index == -1 ? emptySuperTag : item.superTag(sst_index);
			const SuperTag & n0l2_supertag = n0l2_index == -1 ? emptySuperTag : item.superTag(n0l2_index);
			const SuperTag & n0l1_supertag = n0l1_index == -1 ? emptySuperTag : item.superTag(n0l1_index);

			m_mapSTst.getOrUpdateScore(packedScore, st_supertag, scoreIndex, amount, trainingRound);
			m_mapST2st.getOrUpdateScore(packedScore, st2_supertag, scoreIndex, amount, trainingRound);
			m_mapSSTst.getOrUpdateScore(packedScore, sst_supertag, scoreIndex, amount, trainingRound);
			bi_features.refer(n0l2_supertag, -2);
			m_mapN0ist.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(n0l1_supertag, -1);
			m_mapN0ist.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_supertag, st2_supertag);
			m_mapSTstST2st.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_supertag, sst_supertag);
			m_mapSTstSSTst.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_supertag, n0_word);
			m_mapSTstN0w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_supertag, n0_postag);
			m_mapSTstN0pt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			tri_features.refer(st_supertag, n0_word, n0_postag);
			m_mapSTstN0wpt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
		}
	}
}

#endif
