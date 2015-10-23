#ifndef _BOTH_TWOSTACK_WEIGHT_H
#define _BOTH_TWOSTACK_WEIGHT_H

#include "both_twostack_macros.h"
#include "common/parser/weight_base.h"
#include "include/learning/perceptron/packed_score.h"

#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace both_twostack {
	template<class RET_TYPE>
	class Weight : public WeightBase {
	public:
		// uni-gram
		// st, st2, sst
		// bf, bf2, sbf
		// st
		WordMap<RET_TYPE> m_mapSTw;
		POSTagMap<RET_TYPE> m_mapSTpt;
		// st2
		WordMap<RET_TYPE> m_mapST2w;
		POSTagMap<RET_TYPE> m_mapST2pt;
		// sst
		WordMap<RET_TYPE> m_mapSSTw;
		POSTagMap<RET_TYPE> m_mapSSTpt;
		// bf
		WordMap<RET_TYPE> m_mapBFw;
		POSTagMap<RET_TYPE> m_mapBFpt;
		// bf2
		WordMap<RET_TYPE> m_mapBF2w;
		POSTagMap<RET_TYPE> m_mapBF2pt;
		// sbf
		WordMap<RET_TYPE> m_mapSBFw;
		POSTagMap<RET_TYPE> m_mapSBFpt;

		// unigram context
		// st, bf
		// st context
		WordIntMap<RET_TYPE> m_mapSTiw;
		TwoWordsIntMap<RET_TYPE> m_mapSTiwSTjw;
		POSTagIntMap<RET_TYPE> m_mapSTipt;
		POSTagSet2IntMap<RET_TYPE> m_mapSTiptSTjpt;
		// bf context
		WordIntMap<RET_TYPE> m_mapBFiw;
		TwoWordsIntMap<RET_TYPE> m_mapBFiwBFjw;
		POSTagIntMap<RET_TYPE> m_mapBFipt;
		POSTagSet2IntMap<RET_TYPE> m_mapBFiptBFjpt;

		// unigram with label
		// st, st2, sst
		// bf, bf2, sbf
		// st
		WordIntMap<RET_TYPE> m_mapSTwSTLHl;
		WordIntMap<RET_TYPE> m_mapSTwSTLPl;
		WordIntMap<RET_TYPE> m_mapSTwSTRHl;
		WordIntMap<RET_TYPE> m_mapSTwSTRPl;
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
		// bf
		WordIntMap<RET_TYPE> m_mapBFwBFLHl;
		WordIntMap<RET_TYPE> m_mapBFwBFLPl;
		WordIntMap<RET_TYPE> m_mapBFwBFRHl;
		WordIntMap<RET_TYPE> m_mapBFwBFRPl;
		// bf2
		WordIntMap<RET_TYPE> m_mapBF2wBF2LHl;
		WordIntMap<RET_TYPE> m_mapBF2wBF2LPl;
		WordIntMap<RET_TYPE> m_mapBF2wBF2RHl;
		WordIntMap<RET_TYPE> m_mapBF2wBF2RPl;
		// sbf
		WordIntMap<RET_TYPE> m_mapSBFwSBFLHl;
		WordIntMap<RET_TYPE> m_mapSBFwSBFLPl;
		WordIntMap<RET_TYPE> m_mapSBFwSBFRHl;
		WordIntMap<RET_TYPE> m_mapSBFwSBFRPl;

		// unigram with arity
		// st, st2, sst
		// bf, bf2, sbf
		// st
		WordIntMap<RET_TYPE> m_mapSTwSTLHi;
		WordIntMap<RET_TYPE> m_mapSTwSTLPi;
		WordIntMap<RET_TYPE> m_mapSTwSTRHi;
		WordIntMap<RET_TYPE> m_mapSTwSTRPi;
		WordIntMap<RET_TYPE> m_mapSTwSTHi;
		WordIntMap<RET_TYPE> m_mapSTwSTPi;
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
		// bf
		WordIntMap<RET_TYPE> m_mapBFwBFLHi;
		WordIntMap<RET_TYPE> m_mapBFwBFLPi;
		WordIntMap<RET_TYPE> m_mapBFwBFRHi;
		WordIntMap<RET_TYPE> m_mapBFwBFRPi;
		WordIntMap<RET_TYPE> m_mapBFwBFHi;
		WordIntMap<RET_TYPE> m_mapBFwBFPi;
		// bf2
		WordIntMap<RET_TYPE> m_mapBF2wBF2LHi;
		WordIntMap<RET_TYPE> m_mapBF2wBF2LPi;
		WordIntMap<RET_TYPE> m_mapBF2wBF2RHi;
		WordIntMap<RET_TYPE> m_mapBF2wBF2RPi;
		WordIntMap<RET_TYPE> m_mapBF2wBF2Hi;
		WordIntMap<RET_TYPE> m_mapBF2wBF2Pi;
		// sbf
		WordIntMap<RET_TYPE> m_mapSBFwSBFLHi;
		WordIntMap<RET_TYPE> m_mapSBFwSBFLPi;
		WordIntMap<RET_TYPE> m_mapSBFwSBFRHi;
		WordIntMap<RET_TYPE> m_mapSBFwSBFRPi;
		WordIntMap<RET_TYPE> m_mapSBFwSBFHi;
		WordIntMap<RET_TYPE> m_mapSBFwSBFPi;

		// bi-gram
		// st + bf
		TwoWordsMap<RET_TYPE> m_mapSTwBFw;
		WordPOSTagMap<RET_TYPE> m_mapSTwBFpt;
		WordPOSTagMap<RET_TYPE> m_mapSTptBFw;
		POSTagSet2Map<RET_TYPE> m_mapSTptBFpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwptBFw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBFwpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptBFpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBFwpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptBFwpt;
		// st + bf + dis
		TwoWordsIntMap<RET_TYPE> m_mapSTwBFwD;
		WordPOSTagIntMap<RET_TYPE> m_mapSTwBFptD;
		WordPOSTagIntMap<RET_TYPE> m_mapSTptBFwD;
		POSTagSet2IntMap<RET_TYPE> m_mapSTptBFptD;
		// st + bf + st left/right head/pred
		TwoWordsIntMap<RET_TYPE> m_mapSTwBFwSTRPi;
		TwoWordsIntMap<RET_TYPE> m_mapSTwBFwBFLPi;

		// bi-gram
		// st + bf2
		TwoWordsMap<RET_TYPE> m_mapSTwBF2w;
		WordPOSTagMap<RET_TYPE> m_mapSTwBF2pt;
		WordPOSTagMap<RET_TYPE> m_mapSTptBF2w;
		POSTagSet2Map<RET_TYPE> m_mapSTptBF2pt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwptBF2w;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBF2wpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptBF2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBF2wpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptBF2wpt;
		// st + bf2 + dis
		TwoWordsIntMap<RET_TYPE> m_mapSTwBF2wD;
		WordPOSTagIntMap<RET_TYPE> m_mapSTwBF2ptD;
		WordPOSTagIntMap<RET_TYPE> m_mapSTptBF2wD;
		POSTagSet2IntMap<RET_TYPE> m_mapSTptBF2ptD;
		// st + bf2 + st left/right head/pred
		TwoWordsIntMap<RET_TYPE> m_mapSTwBF2wSTRPi;
		TwoWordsIntMap<RET_TYPE> m_mapSTwBF2wBF2LPi;

		// bi-gram
		// st + sbf
		TwoWordsMap<RET_TYPE> m_mapSTwSBFw;
		WordPOSTagMap<RET_TYPE> m_mapSTwSBFpt;
		WordPOSTagMap<RET_TYPE> m_mapSTptSBFw;
		POSTagSet2Map<RET_TYPE> m_mapSTptSBFpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwptSBFw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwSBFwpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptSBFpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptSBFwpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapSTwptSBFwpt;
		// st + sbf + dis
		TwoWordsIntMap<RET_TYPE> m_mapSTwSBFwD;
		WordPOSTagIntMap<RET_TYPE> m_mapSTwSBFptD;
		WordPOSTagIntMap<RET_TYPE> m_mapSTptSBFwD;
		POSTagSet2IntMap<RET_TYPE> m_mapSTptSBFptD;
		// st + sbf + st left/right head/pred
		TwoWordsIntMap<RET_TYPE> m_mapSTwSBFwSTRPi;
		TwoWordsIntMap<RET_TYPE> m_mapSTwSBFwSBFLPi;

		// st2 + bf
		TwoWordsMap<RET_TYPE> m_mapST2wBFw;
		WordPOSTagMap<RET_TYPE> m_mapST2wBFpt;
		WordPOSTagMap<RET_TYPE> m_mapST2ptBFw;
		POSTagSet2Map<RET_TYPE> m_mapST2ptBFpt;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wptBFw;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wBFwpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapST2wptBFpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapST2ptBFwpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapST2wptBFwpt;
		// st2 + bf + dis
		TwoWordsIntMap<RET_TYPE> m_mapST2wBFwD;
		WordPOSTagIntMap<RET_TYPE> m_mapST2wBFptD;
		WordPOSTagIntMap<RET_TYPE> m_mapST2ptBFwD;
		POSTagSet2IntMap<RET_TYPE> m_mapST2ptBFptD;
		// st2 + bf + st2 left/right head/pred
		TwoWordsIntMap<RET_TYPE> m_mapST2wBFwST2RPi;
		TwoWordsIntMap<RET_TYPE> m_mapST2wBFwBFLPi;

		// sst + bf
		TwoWordsMap<RET_TYPE> m_mapSSTwBFw;
		WordPOSTagMap<RET_TYPE> m_mapSSTwBFpt;
		WordPOSTagMap<RET_TYPE> m_mapSSTptBFw;
		POSTagSet2Map<RET_TYPE> m_mapSSTptBFpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwptBFw;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwBFwpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSSTwptBFpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSSTptBFwpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapSSTwptBFwpt;
		// sst + bf + dis
		TwoWordsIntMap<RET_TYPE> m_mapSSTwBFwD;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTwBFptD;
		WordPOSTagIntMap<RET_TYPE> m_mapSSTptBFwD;
		POSTagSet2IntMap<RET_TYPE> m_mapSSTptBFptD;
		// st2 + bf + st2 left/right head/pred
		TwoWordsIntMap<RET_TYPE> m_mapSSTwBFwSSTRPi;
		TwoWordsIntMap<RET_TYPE> m_mapSSTwBFwBFLPi;

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

		// bf + bf2
		TwoWordsMap<RET_TYPE> m_mapBFwBF2w;
		WordPOSTagMap<RET_TYPE> m_mapBFwBF2pt;
		WordPOSTagMap<RET_TYPE> m_mapBFptBF2w;
		POSTagSet2Map<RET_TYPE> m_mapBFptBF2pt;
		WordWordPOSTagMap<RET_TYPE> m_mapBFwptBF2w;
		WordWordPOSTagMap<RET_TYPE> m_mapBFwBF2wpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapBFwptBF2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapBFptBF2wpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapBFwptBF2wpt;

		// bf + sbf
		TwoWordsMap<RET_TYPE> m_mapBFwSBFw;
		WordPOSTagMap<RET_TYPE> m_mapBFwSBFpt;
		WordPOSTagMap<RET_TYPE> m_mapBFptSBFw;
		POSTagSet2Map<RET_TYPE> m_mapBFptSBFpt;
		WordWordPOSTagMap<RET_TYPE> m_mapBFwptSBFw;
		WordWordPOSTagMap<RET_TYPE> m_mapBFwSBFwpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapBFwptSBFpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapBFptSBFwpt;
		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapBFwptSBFwpt;

		// tri-gram
		// st + bf + st2
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwBFptST2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBFwST2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBFptST2w;
		POSTagSet3Map<RET_TYPE> m_mapSTptBFptST2pt;
		// st + bf + sst
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwBFptSSTpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBFwSSTpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBFptSSTw;
		POSTagSet3Map<RET_TYPE> m_mapSTptBFptSSTpt;
		// st + bf + bf2
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwBFptBF2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBFwBF2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBFptBF2w;
		POSTagSet3Map<RET_TYPE> m_mapSTptBFptBF2pt;
		// st + bf + sbf
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwBFptSBFpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBFwSBFpt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptBFptSBFw;
		POSTagSet3Map<RET_TYPE> m_mapSTptBFptSBFpt;

		// st + bf + st left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBFptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFwSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFptSTLHwSTLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBFptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBFptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFwSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFptSTLPwSTLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBFptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBFptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFwSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFptSTRHwSTRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBFptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBFptSTRPptSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFwSTRPptSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFptSTRPwSTRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBFptSTRPptSTRPl;

		// st + bf + bf left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBFptBFLHptBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFwBFLHptBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFptBFLHwBFLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBFptBFLHptBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBFptBFLPptBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFwBFLPptBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFptBFLPwBFLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBFptBFLPptBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBFptBFRHptBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFwBFRHptBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFptBFRHwBFRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBFptBFRHptBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBFptBFRPptBFRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFwBFRPptBFRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBFptBFRPwBFRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBFptBFRPptBFRPl;

		// quar-gram
		// st + bf + st right head + st right pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBFptSTRHptSTRPptSTRHlSTRPl;
		// st + bf + st left pred + st left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBFptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + bf + st right pred + st right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBFptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + bf + bf left head + bf left pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBFptBFLHptBFLPptBFLHlBFLPl;
		// st + bf + bf left pred + bf left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBFptBFLPptBFLP2ptBFLPlBFLP2l;
		// st + bf + bf right pred + bf right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBFptBFRPptBFRP2ptBFRPlBFRP2l;

		// st + bf2 + st left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBF2ptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2wSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2ptSTLHwSTLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBF2ptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBF2ptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2wSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2ptSTLPwSTLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBF2ptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBF2ptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2wSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2ptSTRHwSTRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBF2ptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBF2ptSTRPptSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2wSTRPptSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2ptSTRPwSTRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBF2ptSTRPptSTRPl;

		// st + bf2 + bf2 left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBF2ptBF2LHptBF2LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2wBF2LHptBF2LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2ptBF2LHwBF2LHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBF2ptBF2LHptBF2LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBF2ptBF2LPptBF2LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2wBF2LPptBF2LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2ptBF2LPwBF2LPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBF2ptBF2LPptBF2LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBF2ptBF2RHptBF2RHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2wBF2RHptBF2RHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2ptBF2RHwBF2RHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBF2ptBF2RHptBF2RHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwBF2ptBF2RPptBF2RPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2wBF2RPptBF2RPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptBF2ptBF2RPwBF2RPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptBF2ptBF2RPptBF2RPl;

		// quar-gram
		// st + bf2 + st right head + st right pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBF2ptSTRHptSTRPptSTRHlSTRPl;
		// st + bf2 + st left pred + st left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBF2ptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + bf2 + st right pred + st right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBF2ptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + bf2 + bf2 left head + bf2 left pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBF2ptBF2LHptBF2LPptBF2LHlBF2LPl;
		// st + bf2 + bf2 left pred + bf2 left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBF2ptBF2LPptBF2LP2ptBF2LPlBF2LP2l;
		// st + bf2 + bf2 right pred + bf2 right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptBF2ptBF2RPptBF2RP2ptBF2RPlBF2RP2l;

		// st + sbf + st left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwSBFptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFwSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFptSTLHwSTLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptSBFptSTLHptSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwSBFptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFwSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFptSTLPwSTLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptSBFptSTLPptSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwSBFptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFwSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFptSTRHwSTRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptSBFptSTRHptSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwSBFptSTRPptSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFwSTRPptSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFptSTRPwSTRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptSBFptSTRPptSTRPl;

		// st + sbf + sbf left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwSBFptSBFLHptSBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFwSBFLHptSBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFptSBFLHwSBFLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptSBFptSBFLHptSBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwSBFptSBFLPptSBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFwSBFLPptSBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFptSBFLPwSBFLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptSBFptSBFLPptSBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwSBFptSBFRHptSBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFwSBFRHptSBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFptSBFRHwSBFRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptSBFptSBFRHptSBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTwSBFptSBFRPptSBFRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFwSBFRPptSBFRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSTptSBFptSBFRPwSBFRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSTptSBFptSBFRPptSBFRPl;

		// quar-gram
		// st + sbf + st right head + st right pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptSBFptSTRHptSTRPptSTRHlSTRPl;
		// st + sbf + st left pred + st left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptSBFptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + sbf + st right pred + st right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptSBFptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + sbf + sbf left head + sbf left pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptSBFptSBFLHptSBFLPptSBFLHlSBFLPl;
		// st + sbf + sbf left pred + sbf left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptSBFptSBFLPptSBFLP2ptSBFLPlSBFLP2l;
		// st + sbf + sbf right pred + sbf right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSTptSBFptSBFRPptSBFRP2ptSBFRPlSBFRP2l;

		// st2 + bf + st2 left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wBFptST2LHptST2LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFwST2LHptST2LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFptST2LHwST2LHl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptBFptST2LHptST2LHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wBFptST2LPptST2LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFwST2LPptST2LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFptST2LPwST2LPl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptBFptST2LPptST2LPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wBFptST2RHptST2RHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFwST2RHptST2RHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFptST2RHwST2RHl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptBFptST2RHptST2RHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wBFptST2RPptST2RPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFwST2RPptST2RPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFptST2RPwST2RPl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptBFptST2RPptST2RPl;

		// st2 + bf + bf left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wBFptBFLHptBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFwBFLHptBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFptBFLHwBFLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptBFptBFLHptBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wBFptBFLPptBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFwBFLPptBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFptBFLPwBFLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptBFptBFLPptBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wBFptBFRHptBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFwBFRHptBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFptBFRHwBFRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptBFptBFRHptBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2wBFptBFRPptBFRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFwBFRPptBFRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapST2ptBFptBFRPwBFRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapST2ptBFptBFRPptBFRPl;

		// quar-gram
		// st2 + bf + st2 right head + st2 right pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptBFptST2RHptST2RPptST2RHlST2RPl;
		// st2 + bf + st2 left pred + st2 left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptBFptST2LPptST2LP2ptST2LPlST2LP2l;
		// st2 + bf + st2 right pred + st2 right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptBFptST2RPptST2RP2ptST2RPlST2RP2l;
		// st2 + bf + bf left head + bf left pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptBFptBFLHptBFLPptBFLHlBFLPl;
		// st2 + bf + bf left pred + bf left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptBFptBFLPptBFLP2ptBFLPlBFLP2l;
		// st2 + bf + bf right pred + bf right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapST2ptBFptBFRPptBFRP2ptBFRPlBFRP2l;

		// sst + bf + sst left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwBFptSSTLHptSSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFwSSTLHptSSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFptSSTLHwSSTLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptBFptSSTLHptSSTLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwBFptSSTLPptSSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFwSSTLPptSSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFptSSTLPwSSTLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptBFptSSTLPptSSTLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwBFptSSTRHptSSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFwSSTRHptSSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFptSSTRHwSSTRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptBFptSSTRHptSSTRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwBFptSSTRPptSSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFwSSTRPptSSTRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFptSSTRPwSSTRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptBFptSSTRPptSSTRPl;

		// st + bf + bf left/right head/pred
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwBFptBFLHptBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFwBFLHptBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFptBFLHwBFLHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptBFptBFLHptBFLHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwBFptBFLPptBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFwBFLPptBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFptBFLPwBFLPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptBFptBFLPptBFLPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwBFptBFRHptBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFwBFRHptBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFptBFRHwBFRHl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptBFptBFRHptBFRHl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTwBFptBFRPptBFRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFwBFRPptBFRPl;
		WordPOSTagPOSTagIntMap<RET_TYPE> m_mapSSTptBFptBFRPwBFRPl;
		POSTagSet3IntMap<RET_TYPE> m_mapSSTptBFptBFRPptBFRPl;

		// quar-gram
		// sst + bf + sst right head + sst right pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptBFptSSTRHptSSTRPptSSTRHlSSTRPl;
		// sst + bf + sst left pred + sst left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptBFptSSTLPptSSTLP2ptSSTLPlSSTLP2l;
		// sst + bf + sst right pred + sst right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptBFptSSTRPptSSTRP2ptSSTRPlSSTRP2l;
		// sst + bf + bf left head + bf left pred
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptBFptBFLHptBFLPptBFLHlBFLPl;
		// sst + bf + bf left pred + bf left pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptBFptBFLPptBFLP2ptBFLPlBFLP2l;
		// sst + bf + bf right pred + bf right pred 2
		POSTagSet4TwoIntsMap<RET_TYPE> m_mapSSTptBFptBFRPptBFRP2ptBFRPlBFRP2l;

		// st + bf + label set
		WordTagsetMap<RET_TYPE> m_mapSTwSTll;
		WordTagsetMap<RET_TYPE> m_mapSTwSTrl;
		WordTagsetMap<RET_TYPE> m_mapBFwBFll;
		WordTagsetMap<RET_TYPE> m_mapBFwBFrl;
		POSTagTagsetMap<RET_TYPE> m_mapSTptSTll;
		POSTagTagsetMap<RET_TYPE> m_mapSTptSTrl;
		POSTagTagsetMap<RET_TYPE> m_mapBFptBFll;
		POSTagTagsetMap<RET_TYPE> m_mapBFptBFrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptBFptSTll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptBFptSTrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptBFptBFll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptBFptBFrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptBF2ptSTll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptBF2ptSTrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptBF2ptBF2ll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptBF2ptBF2rl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptSBFptSTll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptSBFptSTrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptSBFptSBFll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSTptSBFptSBFrl;
		// st2 + bf + label set
		WordTagsetMap<RET_TYPE> m_mapST2wST2ll;
		WordTagsetMap<RET_TYPE> m_mapST2wST2rl;
		WordTagsetMap<RET_TYPE> m_mapBF2wBF2ll;
		WordTagsetMap<RET_TYPE> m_mapBF2wBF2rl;
		POSTagTagsetMap<RET_TYPE> m_mapST2ptST2ll;
		POSTagTagsetMap<RET_TYPE> m_mapST2ptST2rl;
		POSTagTagsetMap<RET_TYPE> m_mapBF2ptBF2ll;
		POSTagTagsetMap<RET_TYPE> m_mapBF2ptBF2rl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapST2ptBFptST2ll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapST2ptBFptST2rl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapST2ptBFptBFll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapST2ptBFptBFrl;
		// sst + bf + label set
		WordTagsetMap<RET_TYPE> m_mapSSTwSSTll;
		WordTagsetMap<RET_TYPE> m_mapSSTwSSTrl;
		WordTagsetMap<RET_TYPE> m_mapSBFwSBFll;
		WordTagsetMap<RET_TYPE> m_mapSBFwSBFrl;
		POSTagTagsetMap<RET_TYPE> m_mapSSTptSSTll;
		POSTagTagsetMap<RET_TYPE> m_mapSSTptSSTrl;
		POSTagTagsetMap<RET_TYPE> m_mapSBFptSBFll;
		POSTagTagsetMap<RET_TYPE> m_mapSBFptSBFrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSSTptBFptSSTll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSSTptBFptSSTrl;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSSTptBFptBFll;
		POSTagSet2TagsetMap<RET_TYPE> m_mapSSTptBFptBFrl;

		// char feature (for chinese)
		// st context char
		StringMap<RET_TYPE> m_map1CharBeforeST;
		StringMap<RET_TYPE> m_map2CharBeforeST;
		StringMap<RET_TYPE> m_map3CharBeforeST;
		StringMap<RET_TYPE> m_map1CharAfterST;
		StringMap<RET_TYPE> m_map2CharAfterST;
		StringMap<RET_TYPE> m_map3CharAfterST;
		// bf context char
		StringMap<RET_TYPE> m_map1CharBeforeBF;
		StringMap<RET_TYPE> m_map2CharBeforeBF;
		StringMap<RET_TYPE> m_map3CharBeforeBF;
		StringMap<RET_TYPE> m_map1CharAfterBF;
		StringMap<RET_TYPE> m_map2CharAfterBF;
		StringMap<RET_TYPE> m_map3CharAfterBF;

		// path feature
		// st - bf
		StringMap<RET_TYPE> m_mapSTPOSPath;
		StringMap<RET_TYPE> m_mapSTFPOSPath;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBFptSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBFwSTsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTptBFwSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBFptBFsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBFwBFsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTptBFwBFsynhw;
		// st - bf2
		StringMap<RET_TYPE> m_mapBF2POSPath;
		StringMap<RET_TYPE> m_mapBF2FPOSPath;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBF2ptSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBF2wSTsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTptBF2wSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBF2ptBF2synhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwBF2wBF2synhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTptBF2wBF2synhw;
		// st - sbf
		StringMap<RET_TYPE> m_mapSBFPOSPath;
		StringMap<RET_TYPE> m_mapSBFFPOSPath;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwSBFptSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwSBFwSTsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTptSBFwSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwSBFptSBFsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwSBFwSBFsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTptSBFwSBFsynhw;
		// st2 - bf
		StringMap<RET_TYPE> m_mapST2POSPath;
		StringMap<RET_TYPE> m_mapST2FPOSPath;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wBFptST2synhw;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wBFwST2synhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapST2ptBFwST2synhw;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wBFptBFsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapST2wBFwBFsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapST2ptBFwBFsynhw;
		// sst - bf
		StringMap<RET_TYPE> m_mapSSTPOSPath;
		StringMap<RET_TYPE> m_mapSSTFPOSPath;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwBFptSSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwBFwSSTsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTptBFwSSTsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwBFptBFsynhw;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTwBFwBFsynhpt;
		WordWordPOSTagMap<RET_TYPE> m_mapSSTptBFwBFsynhw;

		// supertag feature
		SuperTagMap<RET_TYPE> m_mapSTst;
		SuperTagMap<RET_TYPE> m_mapST2st;
		SuperTagMap<RET_TYPE> m_mapSSTst;
		SuperTagIntMap<RET_TYPE> m_mapBFist;
		SuperTagSet2Map<RET_TYPE> m_mapSTstST2st;
		SuperTagSet2Map<RET_TYPE> m_mapSTstSSTst;
		WordSuperTagMap<RET_TYPE> m_mapSTstBFw;
		POSTagSuperTagMap<RET_TYPE> m_mapSTstBFpt;
		WordPOSTagSuperTagMap<RET_TYPE> m_mapSTstBFwpt;

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
		// st, st2, sst
		// bf, bf2, sbf
		// st
		m_mapSTw("m_mapSTw"),
		m_mapSTpt("m_mapSTpt"),
		// st2
		m_mapST2w("m_mapST2w"),
		m_mapST2pt("m_mapST2pt"),
		// sst
		m_mapSSTw("m_mapSSTw"),
		m_mapSSTpt("m_mapSSTpt"),
		// bf
		m_mapBFw("m_mapBFw"),
		m_mapBFpt("m_mapBFpt"),
		// bf2
		m_mapBF2w("m_mapBF2w"),
		m_mapBF2pt("m_mapBF2pt"),
		// sbf
		m_mapSBFw("m_mapSBFw"),
		m_mapSBFpt("m_mapSBFpt"),

		// unigram context
		// st, bf
		// st context
		m_mapSTiw("m_mapSTiw"),
		m_mapSTiwSTjw("m_mapSTiwSTjw"),
		m_mapSTipt("m_mapSTipt"),
		m_mapSTiptSTjpt("m_mapSTiptSTjpt"),
		// bf context
		m_mapBFiw("m_mapBFiw"),
		m_mapBFiwBFjw("m_mapBFiwBFjw"),
		m_mapBFipt("m_mapBFipt"),
		m_mapBFiptBFjpt("m_mapBFiptBFjpt"),

		// unigram with label
		// st, st2, sst
		// bf, bf2, sbf
		// st
		m_mapSTwSTLHl("m_mapSTwSTLHl"),
		m_mapSTwSTLPl("m_mapSTwSTLPl"),
		m_mapSTwSTRHl("m_mapSTwSTRHl"),
		m_mapSTwSTRPl("m_mapSTwSTRPl"),
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
		// bf
		m_mapBFwBFLHl("m_mapBFwBFLHl"),
		m_mapBFwBFLPl("m_mapBFwBFLPl"),
		m_mapBFwBFRHl("m_mapBFwBFRHl"),
		m_mapBFwBFRPl("m_mapBFwBFRPl"),
		// bf2
		m_mapBF2wBF2LHl("m_mapBF2wBF2LHl"),
		m_mapBF2wBF2LPl("m_mapBF2wBF2LPl"),
		m_mapBF2wBF2RHl("m_mapBF2wBF2RHl"),
		m_mapBF2wBF2RPl("m_mapBF2wBF2RPl"),
		// sbf
		m_mapSBFwSBFLHl("m_mapSBFwSBFLHl"),
		m_mapSBFwSBFLPl("m_mapSBFwSBFLPl"),
		m_mapSBFwSBFRHl("m_mapSBFwSBFRHl"),
		m_mapSBFwSBFRPl("m_mapSBFwSBFRPl"),

		// unigram with arity
		// st, st2, sst
		// bf, bf2, sbf
		// st
		m_mapSTwSTLHi("m_mapSTwSTLHi"),
		m_mapSTwSTLPi("m_mapSTwSTLPi"),
		m_mapSTwSTRHi("m_mapSTwSTRHi"),
		m_mapSTwSTRPi("m_mapSTwSTRPi"),
		m_mapSTwSTHi("m_mapSTwSTHi"),
		m_mapSTwSTPi("m_mapSTwSTPi"),
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
		// bf
		m_mapBFwBFLHi("m_mapBFwBFLHi"),
		m_mapBFwBFLPi("m_mapBFwBFLPi"),
		m_mapBFwBFRHi("m_mapBFwBFRHi"),
		m_mapBFwBFRPi("m_mapBFwBFRPi"),
		m_mapBFwBFHi("m_mapBFwBFHi"),
		m_mapBFwBFPi("m_mapBFwBFPi"),
		// bf2
		m_mapBF2wBF2LHi("m_mapBF2wBF2LHi"),
		m_mapBF2wBF2LPi("m_mapBF2wBF2LPi"),
		m_mapBF2wBF2RHi("m_mapBF2wBF2RHi"),
		m_mapBF2wBF2RPi("m_mapBF2wBF2RPi"),
		m_mapBF2wBF2Hi("m_mapBF2wBF2Hi"),
		m_mapBF2wBF2Pi("m_mapBF2wBF2Pi"),
		// sbf
		m_mapSBFwSBFLHi("m_mapSBFwSBFLHi"),
		m_mapSBFwSBFLPi("m_mapSBFwSBFLPi"),
		m_mapSBFwSBFRHi("m_mapSBFwSBFRHi"),
		m_mapSBFwSBFRPi("m_mapSBFwSBFRPi"),
		m_mapSBFwSBFHi("m_mapSBFwSBFHi"),
		m_mapSBFwSBFPi("m_mapSBFwSBFPi"),

		// bi-gram
		// st + bf
		m_mapSTwBFw("m_mapSTwBFw"),
		m_mapSTwBFpt("m_mapSTwBFpt"),
		m_mapSTptBFw("m_mapSTptBFw"),
		m_mapSTptBFpt("m_mapSTptBFpt"),
		m_mapSTwptBFw("m_mapSTwptBFw"),
		m_mapSTwBFwpt("m_mapSTwBFwpt"),
		m_mapSTwptBFpt("m_mapSTwptBFpt"),
		m_mapSTptBFwpt("m_mapSTptBFwpt"),
		m_mapSTwptBFwpt("m_mapSTwptBFwpt"),
		// st + bf + dis
		m_mapSTwBFwD("m_mapSTwBFwD"),
		m_mapSTwBFptD("m_mapSTwBFptD"),
		m_mapSTptBFwD("m_mapSTptBFwD"),
		m_mapSTptBFptD("m_mapSTptBFptD"),
		// st + bf + st left/right head/pred
		m_mapSTwBFwSTRPi("m_mapSTwBFwSTRPi"),
		m_mapSTwBFwBFLPi("m_mapSTwBFwBFLPi"),

		// bi-gram
		// st + bf2
		m_mapSTwBF2w("m_mapSTwBF2w"),
		m_mapSTwBF2pt("m_mapSTwBF2pt"),
		m_mapSTptBF2w("m_mapSTptBF2w"),
		m_mapSTptBF2pt("m_mapSTptBF2pt"),
		m_mapSTwptBF2w("m_mapSTwptBF2w"),
		m_mapSTwBF2wpt("m_mapSTwBF2wpt"),
		m_mapSTwptBF2pt("m_mapSTwptBF2pt"),
		m_mapSTptBF2wpt("m_mapSTptBF2wpt"),
		m_mapSTwptBF2wpt("m_mapSTwptBF2wpt"),
		// st + bf2 + dis
		m_mapSTwBF2wD("m_mapSTwBF2wD"),
		m_mapSTwBF2ptD("m_mapSTwBF2ptD"),
		m_mapSTptBF2wD("m_mapSTptBF2wD"),
		m_mapSTptBF2ptD("m_mapSTptBF2ptD"),
		// st + bf2 + st left/right head/pred
		m_mapSTwBF2wSTRPi("m_mapSTwBF2wSTRPi"),
		m_mapSTwBF2wBF2LPi("m_mapSTwBF2wBF2LPi"),

		// bi-gram
		// st + sbf
		m_mapSTwSBFw("m_mapSTwSBFw"),
		m_mapSTwSBFpt("m_mapSTwSBFpt"),
		m_mapSTptSBFw("m_mapSTptSBFw"),
		m_mapSTptSBFpt("m_mapSTptSBFpt"),
		m_mapSTwptSBFw("m_mapSTwptSBFw"),
		m_mapSTwSBFwpt("m_mapSTwSBFwpt"),
		m_mapSTwptSBFpt("m_mapSTwptSBFpt"),
		m_mapSTptSBFwpt("m_mapSTptSBFwpt"),
		m_mapSTwptSBFwpt("m_mapSTwptSBFwpt"),
		// st + sbf + dis
		m_mapSTwSBFwD("m_mapSTwSBFwD"),
		m_mapSTwSBFptD("m_mapSTwSBFptD"),
		m_mapSTptSBFwD("m_mapSTptSBFwD"),
		m_mapSTptSBFptD("m_mapSTptSBFptD"),
		// st + sbf + st left/right head/pred
		m_mapSTwSBFwSTRPi("m_mapSTwSBFwSTRPi"),
		m_mapSTwSBFwSBFLPi("m_mapSTwSBFwSBFLPi"),

		// st2 + bf
		m_mapST2wBFw("m_mapST2wBFw"),
		m_mapST2wBFpt("m_mapST2wBFpt"),
		m_mapST2ptBFw("m_mapST2ptBFw"),
		m_mapST2ptBFpt("m_mapST2ptBFpt"),
		m_mapST2wptBFw("m_mapST2wptBFw"),
		m_mapST2wBFwpt("m_mapST2wBFwpt"),
		m_mapST2wptBFpt("m_mapST2wptBFpt"),
		m_mapST2ptBFwpt("m_mapST2ptBFwpt"),
		m_mapST2wptBFwpt("m_mapST2wptBFwpt"),
		// st2 + bf + dis
		m_mapST2wBFwD("m_mapST2wBFwD"),
		m_mapST2wBFptD("m_mapST2wBFptD"),
		m_mapST2ptBFwD("m_mapST2ptBFwD"),
		m_mapST2ptBFptD("m_mapST2ptBFptD"),
		// st2 + bf + st2 left/right head/pred
		m_mapST2wBFwST2RPi("m_mapST2wBFwST2RPi"),
		m_mapST2wBFwBFLPi("m_mapST2wBFwBFLPi"),

		// sst + bf
		m_mapSSTwBFw("m_mapSSTwBFw"),
		m_mapSSTwBFpt("m_mapSSTwBFpt"),
		m_mapSSTptBFw("m_mapSSTptBFw"),
		m_mapSSTptBFpt("m_mapSSTptBFpt"),
		m_mapSSTwptBFw("m_mapSSTwptBFw"),
		m_mapSSTwBFwpt("m_mapSSTwBFwpt"),
		m_mapSSTwptBFpt("m_mapSSTwptBFpt"),
		m_mapSSTptBFwpt("m_mapSSTptBFwpt"),
		m_mapSSTwptBFwpt("m_mapSSTwptBFwpt"),
		// sst + bf + dis
		m_mapSSTwBFwD("m_mapSSTwBFwD"),
		m_mapSSTwBFptD("m_mapSSTwBFptD"),
		m_mapSSTptBFwD("m_mapSSTptBFwD"),
		m_mapSSTptBFptD("m_mapSSTptBFptD"),
		// st2 + bf + st2 left/right head/pred
		m_mapSSTwBFwSSTRPi("m_mapSSTwBFwSSTRPi"),
		m_mapSSTwBFwBFLPi("m_mapSSTwBFwBFLPi"),

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

		// bf + bf2
		m_mapBFwBF2w("m_mapBFwBF2w"),
		m_mapBFwBF2pt("m_mapBFwBF2pt"),
		m_mapBFptBF2w("m_mapBFptBF2w"),
		m_mapBFptBF2pt("m_mapBFptBF2pt"),
		m_mapBFwptBF2w("m_mapBFwptBF2w"),
		m_mapBFwBF2wpt("m_mapBFwBF2wpt"),
		m_mapBFwptBF2pt("m_mapBFwptBF2pt"),
		m_mapBFptBF2wpt("m_mapBFptBF2wpt"),
		m_mapBFwptBF2wpt("m_mapBFwptBF2wpt"),

		// bf + sbf
		m_mapBFwSBFw("m_mapBFwSBFw"),
		m_mapBFwSBFpt("m_mapBFwSBFpt"),
		m_mapBFptSBFw("m_mapBFptSBFw"),
		m_mapBFptSBFpt("m_mapBFptSBFpt"),
		m_mapBFwptSBFw("m_mapBFwptSBFw"),
		m_mapBFwSBFwpt("m_mapBFwSBFwpt"),
		m_mapBFwptSBFpt("m_mapBFwptSBFpt"),
		m_mapBFptSBFwpt("m_mapBFptSBFwpt"),
		m_mapBFwptSBFwpt("m_mapBFwptSBFwpt"),

		// tri-gram
		// st + bf + st2
		m_mapSTwBFptST2pt("m_mapSTwBFptST2pt"),
		m_mapSTptBFwST2pt("m_mapSTptBFwST2pt"),
		m_mapSTptBFptST2w("m_mapSTptBFptST2w"),
		m_mapSTptBFptST2pt("m_mapSTptBFptST2pt"),
		// st + bf + sst
		m_mapSTwBFptSSTpt("m_mapSTwBFptSSTpt"),
		m_mapSTptBFwSSTpt("m_mapSTptBFwSSTpt"),
		m_mapSTptBFptSSTw("m_mapSTptBFptSSTw"),
		m_mapSTptBFptSSTpt("m_mapSTptBFptSSTpt"),
		// st + bf + bf2
		m_mapSTwBFptBF2pt("m_mapSTwBFptBF2pt"),
		m_mapSTptBFwBF2pt("m_mapSTptBFwBF2pt"),
		m_mapSTptBFptBF2w("m_mapSTptBFptBF2w"),
		m_mapSTptBFptBF2pt("m_mapSTptBFptBF2pt"),
		// st + bf + sbf
		m_mapSTwBFptSBFpt("m_mapSTwBFptSBFpt"),
		m_mapSTptBFwSBFpt("m_mapSTptBFwSBFpt"),
		m_mapSTptBFptSBFw("m_mapSTptBFptSBFw"),
		m_mapSTptBFptSBFpt("m_mapSTptBFptSBFpt"),

		// st + bf + st left/right head/pred
		m_mapSTwBFptSTLHptSTLHl("m_mapSTwBFptSTLHptSTLHl"),
		m_mapSTptBFwSTLHptSTLHl("m_mapSTptBFwSTLHptSTLHl"),
		m_mapSTptBFptSTLHwSTLHl("m_mapSTptBFptSTLHwSTLHl"),
		m_mapSTptBFptSTLHptSTLHl("m_mapSTptBFptSTLHptSTLHl"),
		m_mapSTwBFptSTLPptSTLPl("m_mapSTwBFptSTLPptSTLPl"),
		m_mapSTptBFwSTLPptSTLPl("m_mapSTptBFwSTLPptSTLPl"),
		m_mapSTptBFptSTLPwSTLPl("m_mapSTptBFptSTLPwSTLPl"),
		m_mapSTptBFptSTLPptSTLPl("m_mapSTptBFptSTLPptSTLPl"),
		m_mapSTwBFptSTRHptSTRHl("m_mapSTwBFptSTRHptSTRHl"),
		m_mapSTptBFwSTRHptSTRHl("m_mapSTptBFwSTRHptSTRHl"),
		m_mapSTptBFptSTRHwSTRHl("m_mapSTptBFptSTRHwSTRHl"),
		m_mapSTptBFptSTRHptSTRHl("m_mapSTptBFptSTRHptSTRHl"),
		m_mapSTwBFptSTRPptSTRPl("m_mapSTwBFptSTRPptSTRPl"),
		m_mapSTptBFwSTRPptSTRPl("m_mapSTptBFwSTRPptSTRPl"),
		m_mapSTptBFptSTRPwSTRPl("m_mapSTptBFptSTRPwSTRPl"),
		m_mapSTptBFptSTRPptSTRPl("m_mapSTptBFptSTRPptSTRPl"),

		// st + bf + bf left/right head/pred
		m_mapSTwBFptBFLHptBFLHl("m_mapSTwBFptBFLHptBFLHl"),
		m_mapSTptBFwBFLHptBFLHl("m_mapSTptBFwBFLHptBFLHl"),
		m_mapSTptBFptBFLHwBFLHl("m_mapSTptBFptBFLHwBFLHl"),
		m_mapSTptBFptBFLHptBFLHl("m_mapSTptBFptBFLHptBFLHl"),
		m_mapSTwBFptBFLPptBFLPl("m_mapSTwBFptBFLPptBFLPl"),
		m_mapSTptBFwBFLPptBFLPl("m_mapSTptBFwBFLPptBFLPl"),
		m_mapSTptBFptBFLPwBFLPl("m_mapSTptBFptBFLPwBFLPl"),
		m_mapSTptBFptBFLPptBFLPl("m_mapSTptBFptBFLPptBFLPl"),
		m_mapSTwBFptBFRHptBFRHl("m_mapSTwBFptBFRHptBFRHl"),
		m_mapSTptBFwBFRHptBFRHl("m_mapSTptBFwBFRHptBFRHl"),
		m_mapSTptBFptBFRHwBFRHl("m_mapSTptBFptBFRHwBFRHl"),
		m_mapSTptBFptBFRHptBFRHl("m_mapSTptBFptBFRHptBFRHl"),
		m_mapSTwBFptBFRPptBFRPl("m_mapSTwBFptBFRPptBFRPl"),
		m_mapSTptBFwBFRPptBFRPl("m_mapSTptBFwBFRPptBFRPl"),
		m_mapSTptBFptBFRPwBFRPl("m_mapSTptBFptBFRPwBFRPl"),
		m_mapSTptBFptBFRPptBFRPl("m_mapSTptBFptBFRPptBFRPl"),

		// quar-gram
		// st + bf + st right head + st right pred
		m_mapSTptBFptSTRHptSTRPptSTRHlSTRPl("m_mapSTptBFptSTRHptSTRPptSTRHlSTRPl"),
		// st + bf + st left pred + st left pred 2
		m_mapSTptBFptSTLPptSTLP2ptSTLPlSTLP2l("m_mapSTptBFptSTLPptSTLP2ptSTLPlSTLP2l"),
		// st + bf + st right pred + st right pred 2
		m_mapSTptBFptSTRPptSTRP2ptSTRPlSTRP2l("m_mapSTptBFptSTRPptSTRP2ptSTRPlSTRP2l"),
		// st + bf + bf left head + bf left pred
		m_mapSTptBFptBFLHptBFLPptBFLHlBFLPl("m_mapSTptBFptBFLHptBFLPptBFLHlBFLPl"),
		// st + bf + bf left pred + bf left pred 2
		m_mapSTptBFptBFLPptBFLP2ptBFLPlBFLP2l("m_mapSTptBFptBFLPptBFLP2ptBFLPlBFLP2l"),
		// st + bf + bf right pred + bf right pred 2
		m_mapSTptBFptBFRPptBFRP2ptBFRPlBFRP2l("m_mapSTptBFptBFRPptBFRP2ptBFRPlBFRP2l"),

		// st + bf2 + st left/right head/pred
		m_mapSTwBF2ptSTLHptSTLHl("m_mapSTwBF2ptSTLHptSTLHl"),
		m_mapSTptBF2wSTLHptSTLHl("m_mapSTptBF2wSTLHptSTLHl"),
		m_mapSTptBF2ptSTLHwSTLHl("m_mapSTptBF2ptSTLHwSTLHl"),
		m_mapSTptBF2ptSTLHptSTLHl("m_mapSTptBF2ptSTLHptSTLHl"),
		m_mapSTwBF2ptSTLPptSTLPl("m_mapSTwBF2ptSTLPptSTLPl"),
		m_mapSTptBF2wSTLPptSTLPl("m_mapSTptBF2wSTLPptSTLPl"),
		m_mapSTptBF2ptSTLPwSTLPl("m_mapSTptBF2ptSTLPwSTLPl"),
		m_mapSTptBF2ptSTLPptSTLPl("m_mapSTptBF2ptSTLPptSTLPl"),
		m_mapSTwBF2ptSTRHptSTRHl("m_mapSTwBF2ptSTRHptSTRHl"),
		m_mapSTptBF2wSTRHptSTRHl("m_mapSTptBF2wSTRHptSTRHl"),
		m_mapSTptBF2ptSTRHwSTRHl("m_mapSTptBF2ptSTRHwSTRHl"),
		m_mapSTptBF2ptSTRHptSTRHl("m_mapSTptBF2ptSTRHptSTRHl"),
		m_mapSTwBF2ptSTRPptSTRPl("m_mapSTwBF2ptSTRPptSTRPl"),
		m_mapSTptBF2wSTRPptSTRPl("m_mapSTptBF2wSTRPptSTRPl"),
		m_mapSTptBF2ptSTRPwSTRPl("m_mapSTptBF2ptSTRPwSTRPl"),
		m_mapSTptBF2ptSTRPptSTRPl("m_mapSTptBF2ptSTRPptSTRPl"),

		// st + bf2 + bf2 left/right head/pred
		m_mapSTwBF2ptBF2LHptBF2LHl("m_mapSTwBF2ptBF2LHptBF2LHl"),
		m_mapSTptBF2wBF2LHptBF2LHl("m_mapSTptBF2wBF2LHptBF2LHl"),
		m_mapSTptBF2ptBF2LHwBF2LHl("m_mapSTptBF2ptBF2LHwBF2LHl"),
		m_mapSTptBF2ptBF2LHptBF2LHl("m_mapSTptBF2ptBF2LHptBF2LHl"),
		m_mapSTwBF2ptBF2LPptBF2LPl("m_mapSTwBF2ptBF2LPptBF2LPl"),
		m_mapSTptBF2wBF2LPptBF2LPl("m_mapSTptBF2wBF2LPptBF2LPl"),
		m_mapSTptBF2ptBF2LPwBF2LPl("m_mapSTptBF2ptBF2LPwBF2LPl"),
		m_mapSTptBF2ptBF2LPptBF2LPl("m_mapSTptBF2ptBF2LPptBF2LPl"),
		m_mapSTwBF2ptBF2RHptBF2RHl("m_mapSTwBF2ptBF2RHptBF2RHl"),
		m_mapSTptBF2wBF2RHptBF2RHl("m_mapSTptBF2wBF2RHptBF2RHl"),
		m_mapSTptBF2ptBF2RHwBF2RHl("m_mapSTptBF2ptBF2RHwBF2RHl"),
		m_mapSTptBF2ptBF2RHptBF2RHl("m_mapSTptBF2ptBF2RHptBF2RHl"),
		m_mapSTwBF2ptBF2RPptBF2RPl("m_mapSTwBF2ptBF2RPptBF2RPl"),
		m_mapSTptBF2wBF2RPptBF2RPl("m_mapSTptBF2wBF2RPptBF2RPl"),
		m_mapSTptBF2ptBF2RPwBF2RPl("m_mapSTptBF2ptBF2RPwBF2RPl"),
		m_mapSTptBF2ptBF2RPptBF2RPl("m_mapSTptBF2ptBF2RPptBF2RPl"),

		// quar-gram
		// st + bf2 + st right head + st right pred
		m_mapSTptBF2ptSTRHptSTRPptSTRHlSTRPl("m_mapSTptBF2ptSTRHptSTRPptSTRHlSTRPl"),
		// st + bf2 + st left pred + st left pred 2
		m_mapSTptBF2ptSTLPptSTLP2ptSTLPlSTLP2l("m_mapSTptBF2ptSTLPptSTLP2ptSTLPlSTLP2l"),
		// st + bf2 + st right pred + st right pred 2
		m_mapSTptBF2ptSTRPptSTRP2ptSTRPlSTRP2l("m_mapSTptBF2ptSTRPptSTRP2ptSTRPlSTRP2l"),
		// st + bf2 + bf2 left head + bf2 left pred
		m_mapSTptBF2ptBF2LHptBF2LPptBF2LHlBF2LPl("m_mapSTptBF2ptBF2LHptBF2LPptBF2LHlBF2LPl"),
		// st + bf2 + bf2 left pred + bf2 left pred 2
		m_mapSTptBF2ptBF2LPptBF2LP2ptBF2LPlBF2LP2l("m_mapSTptBF2ptBF2LPptBF2LP2ptBF2LPlBF2LP2l"),
		// st + bf2 + bf2 right pred + bf2 right pred 2
		m_mapSTptBF2ptBF2RPptBF2RP2ptBF2RPlBF2RP2l("m_mapSTptBF2ptBF2RPptBF2RP2ptBF2RPlBF2RP2l"),

		// st + sbf + st left/right head/pred
		m_mapSTwSBFptSTLHptSTLHl("m_mapSTwSBFptSTLHptSTLHl"),
		m_mapSTptSBFwSTLHptSTLHl("m_mapSTptSBFwSTLHptSTLHl"),
		m_mapSTptSBFptSTLHwSTLHl("m_mapSTptSBFptSTLHwSTLHl"),
		m_mapSTptSBFptSTLHptSTLHl("m_mapSTptSBFptSTLHptSTLHl"),
		m_mapSTwSBFptSTLPptSTLPl("m_mapSTwSBFptSTLPptSTLPl"),
		m_mapSTptSBFwSTLPptSTLPl("m_mapSTptSBFwSTLPptSTLPl"),
		m_mapSTptSBFptSTLPwSTLPl("m_mapSTptSBFptSTLPwSTLPl"),
		m_mapSTptSBFptSTLPptSTLPl("m_mapSTptSBFptSTLPptSTLPl"),
		m_mapSTwSBFptSTRHptSTRHl("m_mapSTwSBFptSTRHptSTRHl"),
		m_mapSTptSBFwSTRHptSTRHl("m_mapSTptSBFwSTRHptSTRHl"),
		m_mapSTptSBFptSTRHwSTRHl("m_mapSTptSBFptSTRHwSTRHl"),
		m_mapSTptSBFptSTRHptSTRHl("m_mapSTptSBFptSTRHptSTRHl"),
		m_mapSTwSBFptSTRPptSTRPl("m_mapSTwSBFptSTRPptSTRPl"),
		m_mapSTptSBFwSTRPptSTRPl("m_mapSTptSBFwSTRPptSTRPl"),
		m_mapSTptSBFptSTRPwSTRPl("m_mapSTptSBFptSTRPwSTRPl"),
		m_mapSTptSBFptSTRPptSTRPl("m_mapSTptSBFptSTRPptSTRPl"),

		// st + sbf + sbf left/right head/pred
		m_mapSTwSBFptSBFLHptSBFLHl("m_mapSTwSBFptSBFLHptSBFLHl"),
		m_mapSTptSBFwSBFLHptSBFLHl("m_mapSTptSBFwSBFLHptSBFLHl"),
		m_mapSTptSBFptSBFLHwSBFLHl("m_mapSTptSBFptSBFLHwSBFLHl"),
		m_mapSTptSBFptSBFLHptSBFLHl("m_mapSTptSBFptSBFLHptSBFLHl"),
		m_mapSTwSBFptSBFLPptSBFLPl("m_mapSTwSBFptSBFLPptSBFLPl"),
		m_mapSTptSBFwSBFLPptSBFLPl("m_mapSTptSBFwSBFLPptSBFLPl"),
		m_mapSTptSBFptSBFLPwSBFLPl("m_mapSTptSBFptSBFLPwSBFLPl"),
		m_mapSTptSBFptSBFLPptSBFLPl("m_mapSTptSBFptSBFLPptSBFLPl"),
		m_mapSTwSBFptSBFRHptSBFRHl("m_mapSTwSBFptSBFRHptSBFRHl"),
		m_mapSTptSBFwSBFRHptSBFRHl("m_mapSTptSBFwSBFRHptSBFRHl"),
		m_mapSTptSBFptSBFRHwSBFRHl("m_mapSTptSBFptSBFRHwSBFRHl"),
		m_mapSTptSBFptSBFRHptSBFRHl("m_mapSTptSBFptSBFRHptSBFRHl"),
		m_mapSTwSBFptSBFRPptSBFRPl("m_mapSTwSBFptSBFRPptSBFRPl"),
		m_mapSTptSBFwSBFRPptSBFRPl("m_mapSTptSBFwSBFRPptSBFRPl"),
		m_mapSTptSBFptSBFRPwSBFRPl("m_mapSTptSBFptSBFRPwSBFRPl"),
		m_mapSTptSBFptSBFRPptSBFRPl("m_mapSTptSBFptSBFRPptSBFRPl"),

		// quar-gram
		// st + sbf + st right head + st right pred
		m_mapSTptSBFptSTRHptSTRPptSTRHlSTRPl("m_mapSTptSBFptSTRHptSTRPptSTRHlSTRPl"),
		// st + sbf + st left pred + st left pred 2
		m_mapSTptSBFptSTLPptSTLP2ptSTLPlSTLP2l("m_mapSTptSBFptSTLPptSTLP2ptSTLPlSTLP2l"),
		// st + sbf + st right pred + st right pred 2
		m_mapSTptSBFptSTRPptSTRP2ptSTRPlSTRP2l("m_mapSTptSBFptSTRPptSTRP2ptSTRPlSTRP2l"),
		// st + sbf + sbf left head + sbf left pred
		m_mapSTptSBFptSBFLHptSBFLPptSBFLHlSBFLPl("m_mapSTptSBFptSBFLHptSBFLPptSBFLHlSBFLPl"),
		// st + sbf + sbf left pred + sbf left pred 2
		m_mapSTptSBFptSBFLPptSBFLP2ptSBFLPlSBFLP2l("m_mapSTptSBFptSBFLPptSBFLP2ptSBFLPlSBFLP2l"),
		// st + sbf + sbf right pred + sbf right pred 2
		m_mapSTptSBFptSBFRPptSBFRP2ptSBFRPlSBFRP2l("m_mapSTptSBFptSBFRPptSBFRP2ptSBFRPlSBFRP2l"),

		// st2 + bf + st2 left/right head/pred
		m_mapST2wBFptST2LHptST2LHl("m_mapST2wBFptST2LHptST2LHl"),
		m_mapST2ptBFwST2LHptST2LHl("m_mapST2ptBFwST2LHptST2LHl"),
		m_mapST2ptBFptST2LHwST2LHl("m_mapST2ptBFptST2LHwST2LHl"),
		m_mapST2ptBFptST2LHptST2LHl("m_mapST2ptBFptST2LHptST2LHl"),
		m_mapST2wBFptST2LPptST2LPl("m_mapST2wBFptST2LPptST2LPl"),
		m_mapST2ptBFwST2LPptST2LPl("m_mapST2ptBFwST2LPptST2LPl"),
		m_mapST2ptBFptST2LPwST2LPl("m_mapST2ptBFptST2LPwST2LPl"),
		m_mapST2ptBFptST2LPptST2LPl("m_mapST2ptBFptST2LPptST2LPl"),
		m_mapST2wBFptST2RHptST2RHl("m_mapST2wBFptST2RHptST2RHl"),
		m_mapST2ptBFwST2RHptST2RHl("m_mapST2ptBFwST2RHptST2RHl"),
		m_mapST2ptBFptST2RHwST2RHl("m_mapST2ptBFptST2RHwST2RHl"),
		m_mapST2ptBFptST2RHptST2RHl("m_mapST2ptBFptST2RHptST2RHl"),
		m_mapST2wBFptST2RPptST2RPl("m_mapST2wBFptST2RPptST2RPl"),
		m_mapST2ptBFwST2RPptST2RPl("m_mapST2ptBFwST2RPptST2RPl"),
		m_mapST2ptBFptST2RPwST2RPl("m_mapST2ptBFptST2RPwST2RPl"),
		m_mapST2ptBFptST2RPptST2RPl("m_mapST2ptBFptST2RPptST2RPl"),

		// st2 + bf + bf left/right head/pred
		m_mapST2wBFptBFLHptBFLHl("m_mapST2wBFptBFLHptBFLHl"),
		m_mapST2ptBFwBFLHptBFLHl("m_mapST2ptBFwBFLHptBFLHl"),
		m_mapST2ptBFptBFLHwBFLHl("m_mapST2ptBFptBFLHwBFLHl"),
		m_mapST2ptBFptBFLHptBFLHl("m_mapST2ptBFptBFLHptBFLHl"),
		m_mapST2wBFptBFLPptBFLPl("m_mapST2wBFptBFLPptBFLPl"),
		m_mapST2ptBFwBFLPptBFLPl("m_mapST2ptBFwBFLPptBFLPl"),
		m_mapST2ptBFptBFLPwBFLPl("m_mapST2ptBFptBFLPwBFLPl"),
		m_mapST2ptBFptBFLPptBFLPl("m_mapST2ptBFptBFLPptBFLPl"),
		m_mapST2wBFptBFRHptBFRHl("m_mapST2wBFptBFRHptBFRHl"),
		m_mapST2ptBFwBFRHptBFRHl("m_mapST2ptBFwBFRHptBFRHl"),
		m_mapST2ptBFptBFRHwBFRHl("m_mapST2ptBFptBFRHwBFRHl"),
		m_mapST2ptBFptBFRHptBFRHl("m_mapST2ptBFptBFRHptBFRHl"),
		m_mapST2wBFptBFRPptBFRPl("m_mapST2wBFptBFRPptBFRPl"),
		m_mapST2ptBFwBFRPptBFRPl("m_mapST2ptBFwBFRPptBFRPl"),
		m_mapST2ptBFptBFRPwBFRPl("m_mapST2ptBFptBFRPwBFRPl"),
		m_mapST2ptBFptBFRPptBFRPl("m_mapST2ptBFptBFRPptBFRPl"),

		// quar-gram
		// st2 + bf + st2 right head + st2 right pred
		m_mapST2ptBFptST2RHptST2RPptST2RHlST2RPl("m_mapST2ptBFptST2RHptST2RPptST2RHlST2RPl"),
		// st2 + bf + st2 left pred + st2 left pred 2
		m_mapST2ptBFptST2LPptST2LP2ptST2LPlST2LP2l("m_mapST2ptBFptST2LPptST2LP2ptST2LPlST2LP2l"),
		// st2 + bf + st2 right pred + st2 right pred 2
		m_mapST2ptBFptST2RPptST2RP2ptST2RPlST2RP2l("m_mapST2ptBFptST2RPptST2RP2ptST2RPlST2RP2l"),
		// st2 + bf + bf left head + bf left pred
		m_mapST2ptBFptBFLHptBFLPptBFLHlBFLPl("m_mapST2ptBFptBFLHptBFLPptBFLHlBFLPl"),
		// st2 + bf + bf left pred + bf left pred 2
		m_mapST2ptBFptBFLPptBFLP2ptBFLPlBFLP2l("m_mapST2ptBFptBFLPptBFLP2ptBFLPlBFLP2l"),
		// st2 + bf + bf right pred + bf right pred 2
		m_mapST2ptBFptBFRPptBFRP2ptBFRPlBFRP2l("m_mapST2ptBFptBFRPptBFRP2ptBFRPlBFRP2l"),

		// sst + bf + sst left/right head/pred
		m_mapSSTwBFptSSTLHptSSTLHl("m_mapSSTwBFptSSTLHptSSTLHl"),
		m_mapSSTptBFwSSTLHptSSTLHl("m_mapSSTptBFwSSTLHptSSTLHl"),
		m_mapSSTptBFptSSTLHwSSTLHl("m_mapSSTptBFptSSTLHwSSTLHl"),
		m_mapSSTptBFptSSTLHptSSTLHl("m_mapSSTptBFptSSTLHptSSTLHl"),
		m_mapSSTwBFptSSTLPptSSTLPl("m_mapSSTwBFptSSTLPptSSTLPl"),
		m_mapSSTptBFwSSTLPptSSTLPl("m_mapSSTptBFwSSTLPptSSTLPl"),
		m_mapSSTptBFptSSTLPwSSTLPl("m_mapSSTptBFptSSTLPwSSTLPl"),
		m_mapSSTptBFptSSTLPptSSTLPl("m_mapSSTptBFptSSTLPptSSTLPl"),
		m_mapSSTwBFptSSTRHptSSTRHl("m_mapSSTwBFptSSTRHptSSTRHl"),
		m_mapSSTptBFwSSTRHptSSTRHl("m_mapSSTptBFwSSTRHptSSTRHl"),
		m_mapSSTptBFptSSTRHwSSTRHl("m_mapSSTptBFptSSTRHwSSTRHl"),
		m_mapSSTptBFptSSTRHptSSTRHl("m_mapSSTptBFptSSTRHptSSTRHl"),
		m_mapSSTwBFptSSTRPptSSTRPl("m_mapSSTwBFptSSTRPptSSTRPl"),
		m_mapSSTptBFwSSTRPptSSTRPl("m_mapSSTptBFwSSTRPptSSTRPl"),
		m_mapSSTptBFptSSTRPwSSTRPl("m_mapSSTptBFptSSTRPwSSTRPl"),
		m_mapSSTptBFptSSTRPptSSTRPl("m_mapSSTptBFptSSTRPptSSTRPl"),

		// st + bf + bf left/right head/pred
		m_mapSSTwBFptBFLHptBFLHl("m_mapSSTwBFptBFLHptBFLHl"),
		m_mapSSTptBFwBFLHptBFLHl("m_mapSSTptBFwBFLHptBFLHl"),
		m_mapSSTptBFptBFLHwBFLHl("m_mapSSTptBFptBFLHwBFLHl"),
		m_mapSSTptBFptBFLHptBFLHl("m_mapSSTptBFptBFLHptBFLHl"),
		m_mapSSTwBFptBFLPptBFLPl("m_mapSSTwBFptBFLPptBFLPl"),
		m_mapSSTptBFwBFLPptBFLPl("m_mapSSTptBFwBFLPptBFLPl"),
		m_mapSSTptBFptBFLPwBFLPl("m_mapSSTptBFptBFLPwBFLPl"),
		m_mapSSTptBFptBFLPptBFLPl("m_mapSSTptBFptBFLPptBFLPl"),
		m_mapSSTwBFptBFRHptBFRHl("m_mapSSTwBFptBFRHptBFRHl"),
		m_mapSSTptBFwBFRHptBFRHl("m_mapSSTptBFwBFRHptBFRHl"),
		m_mapSSTptBFptBFRHwBFRHl("m_mapSSTptBFptBFRHwBFRHl"),
		m_mapSSTptBFptBFRHptBFRHl("m_mapSSTptBFptBFRHptBFRHl"),
		m_mapSSTwBFptBFRPptBFRPl("m_mapSSTwBFptBFRPptBFRPl"),
		m_mapSSTptBFwBFRPptBFRPl("m_mapSSTptBFwBFRPptBFRPl"),
		m_mapSSTptBFptBFRPwBFRPl("m_mapSSTptBFptBFRPwBFRPl"),
		m_mapSSTptBFptBFRPptBFRPl("m_mapSSTptBFptBFRPptBFRPl"),

		// quar-gram
		// sst + bf + sst right head + sst right pred
		m_mapSSTptBFptSSTRHptSSTRPptSSTRHlSSTRPl("m_mapSSTptBFptSSTRHptSSTRPptSSTRHlSSTRPl"),
		// sst + bf + sst left pred + sst left pred 2
		m_mapSSTptBFptSSTLPptSSTLP2ptSSTLPlSSTLP2l("m_mapSSTptBFptSSTLPptSSTLP2ptSSTLPlSSTLP2l"),
		// sst + bf + sst right pred + sst right pred 2
		m_mapSSTptBFptSSTRPptSSTRP2ptSSTRPlSSTRP2l("m_mapSSTptBFptSSTRPptSSTRP2ptSSTRPlSSTRP2l"),
		// sst + bf + bf left head + bf left pred
		m_mapSSTptBFptBFLHptBFLPptBFLHlBFLPl("m_mapSSTptBFptBFLHptBFLPptBFLHlBFLPl"),
		// sst + bf + bf left pred + bf left pred 2
		m_mapSSTptBFptBFLPptBFLP2ptBFLPlBFLP2l("m_mapSSTptBFptBFLPptBFLP2ptBFLPlBFLP2l"),
		// sst + bf + bf right pred + bf right pred 2
		m_mapSSTptBFptBFRPptBFRP2ptBFRPlBFRP2l("m_mapSSTptBFptBFRPptBFRP2ptBFRPlBFRP2l"),

		// st + bf + label set
		m_mapSTwSTll("m_mapSTwSTll"),
		m_mapSTwSTrl("m_mapSTwSTrl"),
		m_mapBFwBFll("m_mapBFwBFll"),
		m_mapBFwBFrl("m_mapBFwBFrl"),
		m_mapSTptSTll("m_mapSTptSTll"),
		m_mapSTptSTrl("m_mapSTptSTrl"),
		m_mapBFptBFll("m_mapBFptBFll"),
		m_mapBFptBFrl("m_mapBFptBFrl"),
		m_mapSTptBFptSTll("m_mapSTptBFptSTll"),
		m_mapSTptBFptSTrl("m_mapSTptBFptSTrl"),
		m_mapSTptBFptBFll("m_mapSTptBFptBFll"),
		m_mapSTptBFptBFrl("m_mapSTptBFptBFrl"),
		m_mapSTptBF2ptSTll("m_mapSTptBF2ptSTll"),
		m_mapSTptBF2ptSTrl("m_mapSTptBF2ptSTrl"),
		m_mapSTptBF2ptBF2ll("m_mapSTptBF2ptBF2ll"),
		m_mapSTptBF2ptBF2rl("m_mapSTptBF2ptBF2rl"),
		m_mapSTptSBFptSTll("m_mapSTptSBFptSTll"),
		m_mapSTptSBFptSTrl("m_mapSTptSBFptSTrl"),
		m_mapSTptSBFptSBFll("m_mapSTptSBFptSBFll"),
		m_mapSTptSBFptSBFrl("m_mapSTptSBFptSBFrl"),
		// st2 + bf + label set
		m_mapST2wST2ll("m_mapST2wST2ll"),
		m_mapST2wST2rl("m_mapST2wST2rl"),
		m_mapBF2wBF2ll("m_mapBF2wBF2ll"),
		m_mapBF2wBF2rl("m_mapBF2wBF2rl"),
		m_mapST2ptST2ll("m_mapST2ptST2ll"),
		m_mapST2ptST2rl("m_mapST2ptST2rl"),
		m_mapBF2ptBF2ll("m_mapBF2ptBF2ll"),
		m_mapBF2ptBF2rl("m_mapBF2ptBF2rl"),
		m_mapST2ptBFptST2ll("m_mapST2ptBFptST2ll"),
		m_mapST2ptBFptST2rl("m_mapST2ptBFptST2rl"),
		m_mapST2ptBFptBFll("m_mapST2ptBFptBFll"),
		m_mapST2ptBFptBFrl("m_mapST2ptBFptBFrl"),
		// sst + bf + label set
		m_mapSSTwSSTll("m_mapSSTwSSTll"),
		m_mapSSTwSSTrl("m_mapSSTwSSTrl"),
		m_mapSBFwSBFll("m_mapSBFwSBFll"),
		m_mapSBFwSBFrl("m_mapSBFwSBFrl"),
		m_mapSSTptSSTll("m_mapSSTptSSTll"),
		m_mapSSTptSSTrl("m_mapSSTptSSTrl"),
		m_mapSBFptSBFll("m_mapSBFptSBFll"),
		m_mapSBFptSBFrl("m_mapSBFptSBFrl"),
		m_mapSSTptBFptSSTll("m_mapSSTptBFptSSTll"),
		m_mapSSTptBFptSSTrl("m_mapSSTptBFptSSTrl"),
		m_mapSSTptBFptBFll("m_mapSSTptBFptBFll"),
		m_mapSSTptBFptBFrl("m_mapSSTptBFptBFrl"),

		// char feature (for chinese)
		// st context char
		m_map1CharBeforeST("m_map1CharBeforeST"),
		m_map2CharBeforeST("m_map2CharBeforeST"),
		m_map3CharBeforeST("m_map3CharBeforeST"),
		m_map1CharAfterST("m_map1CharAfterST"),
		m_map2CharAfterST("m_map2CharAfterST"),
		m_map3CharAfterST("m_map3CharAfterST"),
		// bf context char
		m_map1CharBeforeBF("m_map1CharBeforeBF"),
		m_map2CharBeforeBF("m_map2CharBeforeBF"),
		m_map3CharBeforeBF("m_map3CharBeforeBF"),
		m_map1CharAfterBF("m_map1CharAfterBF"),
		m_map2CharAfterBF("m_map2CharAfterBF"),
		m_map3CharAfterBF("m_map3CharAfterBF"),

		// path feature
		// st - bf
		m_mapSTPOSPath("m_mapSTPOSPath"),
		m_mapSTFPOSPath("m_mapSTFPOSPath"),
		m_mapSTwBFptSTsynhw("m_mapSTwBFptSTsynhw"),
		m_mapSTwBFwSTsynhpt("m_mapSTwBFwSTsynhpt"),
		m_mapSTptBFwSTsynhw("m_mapSTptBFwSTsynhw"),
		m_mapSTwBFptBFsynhw("m_mapSTwBFptBFsynhw"),
		m_mapSTwBFwBFsynhpt("m_mapSTwBFwBFsynhpt"),
		m_mapSTptBFwBFsynhw("m_mapSTptBFwBFsynhw"),
		// st - bf2
		m_mapBF2POSPath("m_mapBF2POSPath"),
		m_mapBF2FPOSPath("m_mapBF2FPOSPath"),
		m_mapSTwBF2ptSTsynhw("m_mapSTwBF2ptSTsynhw"),
		m_mapSTwBF2wSTsynhpt("m_mapSTwBF2wSTsynhpt"),
		m_mapSTptBF2wSTsynhw("m_mapSTptBF2wSTsynhw"),
		m_mapSTwBF2ptBF2synhw("m_mapSTwBF2ptBF2synhw"),
		m_mapSTwBF2wBF2synhpt("m_mapSTwBF2wBF2synhpt"),
		m_mapSTptBF2wBF2synhw("m_mapSTptBF2wBF2synhw"),
		// st - sbf
		m_mapSBFPOSPath("m_mapSBFPOSPath"),
		m_mapSBFFPOSPath("m_mapSBFFPOSPath"),
		m_mapSTwSBFptSTsynhw("m_mapSTwSBFptSTsynhw"),
		m_mapSTwSBFwSTsynhpt("m_mapSTwSBFwSTsynhpt"),
		m_mapSTptSBFwSTsynhw("m_mapSTptSBFwSTsynhw"),
		m_mapSTwSBFptSBFsynhw("m_mapSTwSBFptSBFsynhw"),
		m_mapSTwSBFwSBFsynhpt("m_mapSTwSBFwSBFsynhpt"),
		m_mapSTptSBFwSBFsynhw("m_mapSTptSBFwSBFsynhw"),
		// st2 - bf
		m_mapST2POSPath("m_mapST2POSPath"),
		m_mapST2FPOSPath("m_mapST2FPOSPath"),
		m_mapST2wBFptST2synhw("m_mapST2wBFptST2synhw"),
		m_mapST2wBFwST2synhpt("m_mapST2wBFwST2synhpt"),
		m_mapST2ptBFwST2synhw("m_mapST2ptBFwST2synhw"),
		m_mapST2wBFptBFsynhw("m_mapST2wBFptBFsynhw"),
		m_mapST2wBFwBFsynhpt("m_mapST2wBFwBFsynhpt"),
		m_mapST2ptBFwBFsynhw("m_mapST2ptBFwBFsynhw"),
		// sst - bf
		m_mapSSTPOSPath("m_mapSSTPOSPath"),
		m_mapSSTFPOSPath("m_mapSSTFPOSPath"),
		m_mapSSTwBFptSSTsynhw("m_mapSSTwBFptSSTsynhw"),
		m_mapSSTwBFwSSTsynhpt("m_mapSSTwBFwSSTsynhpt"),
		m_mapSSTptBFwSSTsynhw("m_mapSSTptBFwSSTsynhw"),
		m_mapSSTwBFptBFsynhw("m_mapSSTwBFptBFsynhw"),
		m_mapSSTwBFwBFsynhpt("m_mapSSTwBFwBFsynhpt"),
		m_mapSSTptBFwBFsynhw("m_mapSSTptBFwBFsynhw"),

		// supertag feature
		m_mapSTst("m_mapSTst"),
		m_mapST2st("m_mapST2st"),
		m_mapSSTst("m_mapSSTst"),
		m_mapBFist("m_mapBFist"),
		m_mapSTstST2st("m_mapSTstST2st"),
		m_mapSTstSSTst("m_mapSTstSSTst"),
		m_mapSTstBFw("m_mapSTstBFw"),
		m_mapSTstBFpt("m_mapSTstBFpt"),
		m_mapSTstBFwpt("m_mapSTstBFwpt")
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
		// st, st2, sst
		// bf, bf2, sbf
		// st
		input >> m_mapSTw;
		input >> m_mapSTpt;
		// st2
		input >> m_mapST2w;
		input >> m_mapST2pt;
		// sst
		input >> m_mapSSTw;
		input >> m_mapSSTpt;
		// bf
		input >> m_mapBFw;
		input >> m_mapBFpt;
		// bf2
		input >> m_mapBF2w;
		input >> m_mapBF2pt;
		// sbf
		input >> m_mapSBFw;
		input >> m_mapSBFpt;

		// unigram context
		// st, bf
		// st context
		input >> m_mapSTiw;
		input >> m_mapSTiwSTjw;
		input >> m_mapSTipt;
		input >> m_mapSTiptSTjpt;
		// bf context
		input >> m_mapBFiw;
		input >> m_mapBFiwBFjw;
		input >> m_mapBFipt;
		input >> m_mapBFiptBFjpt;

		// unigram with label
		// st, st2, sst
		// bf, bf2, sbf
		// st
		input >> m_mapSTwSTLHl;
		input >> m_mapSTwSTLPl;
		input >> m_mapSTwSTRHl;
		input >> m_mapSTwSTRPl;
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
		// bf
		input >> m_mapBFwBFLHl;
		input >> m_mapBFwBFLPl;
		input >> m_mapBFwBFRHl;
		input >> m_mapBFwBFRPl;
		// bf2
		input >> m_mapBF2wBF2LHl;
		input >> m_mapBF2wBF2LPl;
		input >> m_mapBF2wBF2RHl;
		input >> m_mapBF2wBF2RPl;
		// sbf
		input >> m_mapSBFwSBFLHl;
		input >> m_mapSBFwSBFLPl;
		input >> m_mapSBFwSBFRHl;
		input >> m_mapSBFwSBFRPl;

		// unigram with arity
		// st, st2, sst
		// bf, bf2, sbf
		// st
		input >> m_mapSTwSTLHi;
		input >> m_mapSTwSTLPi;
		input >> m_mapSTwSTRHi;
		input >> m_mapSTwSTRPi;
		input >> m_mapSTwSTHi;
		input >> m_mapSTwSTPi;
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
		// bf
		input >> m_mapBFwBFLHi;
		input >> m_mapBFwBFLPi;
		input >> m_mapBFwBFRHi;
		input >> m_mapBFwBFRPi;
		input >> m_mapBFwBFHi;
		input >> m_mapBFwBFPi;
		// bf2
		input >> m_mapBF2wBF2LHi;
		input >> m_mapBF2wBF2LPi;
		input >> m_mapBF2wBF2RHi;
		input >> m_mapBF2wBF2RPi;
		input >> m_mapBF2wBF2Hi;
		input >> m_mapBF2wBF2Pi;
		// sbf
		input >> m_mapSBFwSBFLHi;
		input >> m_mapSBFwSBFLPi;
		input >> m_mapSBFwSBFRHi;
		input >> m_mapSBFwSBFRPi;
		input >> m_mapSBFwSBFHi;
		input >> m_mapSBFwSBFPi;

		// bi-gram
		// st + bf
		input >> m_mapSTwBFw;
		input >> m_mapSTwBFpt;
		input >> m_mapSTptBFw;
		input >> m_mapSTptBFpt;
		input >> m_mapSTwptBFw;
		input >> m_mapSTwBFwpt;
		input >> m_mapSTwptBFpt;
		input >> m_mapSTptBFwpt;
		input >> m_mapSTwptBFwpt;
		// st + bf + dis
		input >> m_mapSTwBFwD;
		input >> m_mapSTwBFptD;
		input >> m_mapSTptBFwD;
		input >> m_mapSTptBFptD;
		// st + bf + st left/right head/pred
		input >> m_mapSTwBFwSTRPi;
		input >> m_mapSTwBFwBFLPi;

		// bi-gram
		// st + bf2
		input >> m_mapSTwBF2w;
		input >> m_mapSTwBF2pt;
		input >> m_mapSTptBF2w;
		input >> m_mapSTptBF2pt;
		input >> m_mapSTwptBF2w;
		input >> m_mapSTwBF2wpt;
		input >> m_mapSTwptBF2pt;
		input >> m_mapSTptBF2wpt;
		input >> m_mapSTwptBF2wpt;
		// st + bf2 + dis
		input >> m_mapSTwBF2wD;
		input >> m_mapSTwBF2ptD;
		input >> m_mapSTptBF2wD;
		input >> m_mapSTptBF2ptD;
		// st + bf2 + st left/right head/pred
		input >> m_mapSTwBF2wSTRPi;
		input >> m_mapSTwBF2wBF2LPi;

		// bi-gram
		// st + sbf
		input >> m_mapSTwSBFw;
		input >> m_mapSTwSBFpt;
		input >> m_mapSTptSBFw;
		input >> m_mapSTptSBFpt;
		input >> m_mapSTwptSBFw;
		input >> m_mapSTwSBFwpt;
		input >> m_mapSTwptSBFpt;
		input >> m_mapSTptSBFwpt;
		input >> m_mapSTwptSBFwpt;
		// st + sbf + dis
		input >> m_mapSTwSBFwD;
		input >> m_mapSTwSBFptD;
		input >> m_mapSTptSBFwD;
		input >> m_mapSTptSBFptD;
		// st + sbf + st left/right head/pred
		input >> m_mapSTwSBFwSTRPi;
		input >> m_mapSTwSBFwSBFLPi;

		// st2 + bf
		input >> m_mapST2wBFw;
		input >> m_mapST2wBFpt;
		input >> m_mapST2ptBFw;
		input >> m_mapST2ptBFpt;
		input >> m_mapST2wptBFw;
		input >> m_mapST2wBFwpt;
		input >> m_mapST2wptBFpt;
		input >> m_mapST2ptBFwpt;
		input >> m_mapST2wptBFwpt;
		// st2 + bf + dis
		input >> m_mapST2wBFwD;
		input >> m_mapST2wBFptD;
		input >> m_mapST2ptBFwD;
		input >> m_mapST2ptBFptD;
		// st2 + bf + st2 left/right head/pred
		input >> m_mapST2wBFwST2RPi;
		input >> m_mapST2wBFwBFLPi;

		// sst + bf
		input >> m_mapSSTwBFw;
		input >> m_mapSSTwBFpt;
		input >> m_mapSSTptBFw;
		input >> m_mapSSTptBFpt;
		input >> m_mapSSTwptBFw;
		input >> m_mapSSTwBFwpt;
		input >> m_mapSSTwptBFpt;
		input >> m_mapSSTptBFwpt;
		input >> m_mapSSTwptBFwpt;
		// sst + bf + dis
		input >> m_mapSSTwBFwD;
		input >> m_mapSSTwBFptD;
		input >> m_mapSSTptBFwD;
		input >> m_mapSSTptBFptD;
		// st2 + bf + st2 left/right head/pred
		input >> m_mapSSTwBFwSSTRPi;
		input >> m_mapSSTwBFwBFLPi;

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

		// bf + bf2
		input >> m_mapBFwBF2w;
		input >> m_mapBFwBF2pt;
		input >> m_mapBFptBF2w;
		input >> m_mapBFptBF2pt;
		input >> m_mapBFwptBF2w;
		input >> m_mapBFwBF2wpt;
		input >> m_mapBFwptBF2pt;
		input >> m_mapBFptBF2wpt;
		input >> m_mapBFwptBF2wpt;

		// bf + sbf
		input >> m_mapBFwSBFw;
		input >> m_mapBFwSBFpt;
		input >> m_mapBFptSBFw;
		input >> m_mapBFptSBFpt;
		input >> m_mapBFwptSBFw;
		input >> m_mapBFwSBFwpt;
		input >> m_mapBFwptSBFpt;
		input >> m_mapBFptSBFwpt;
		input >> m_mapBFwptSBFwpt;

		// tri-gram
		// st + bf + st2
		input >> m_mapSTwBFptST2pt;
		input >> m_mapSTptBFwST2pt;
		input >> m_mapSTptBFptST2w;
		input >> m_mapSTptBFptST2pt;
		// st + bf + sst
		input >> m_mapSTwBFptSSTpt;
		input >> m_mapSTptBFwSSTpt;
		input >> m_mapSTptBFptSSTw;
		input >> m_mapSTptBFptSSTpt;
		// st + bf + bf2
		input >> m_mapSTwBFptBF2pt;
		input >> m_mapSTptBFwBF2pt;
		input >> m_mapSTptBFptBF2w;
		input >> m_mapSTptBFptBF2pt;
		// st + bf + sbf
		input >> m_mapSTwBFptSBFpt;
		input >> m_mapSTptBFwSBFpt;
		input >> m_mapSTptBFptSBFw;
		input >> m_mapSTptBFptSBFpt;

		// st + bf + st left/right head/pred
		input >> m_mapSTwBFptSTLHptSTLHl;
		input >> m_mapSTptBFwSTLHptSTLHl;
		input >> m_mapSTptBFptSTLHwSTLHl;
		input >> m_mapSTptBFptSTLHptSTLHl;
		input >> m_mapSTwBFptSTLPptSTLPl;
		input >> m_mapSTptBFwSTLPptSTLPl;
		input >> m_mapSTptBFptSTLPwSTLPl;
		input >> m_mapSTptBFptSTLPptSTLPl;
		input >> m_mapSTwBFptSTRHptSTRHl;
		input >> m_mapSTptBFwSTRHptSTRHl;
		input >> m_mapSTptBFptSTRHwSTRHl;
		input >> m_mapSTptBFptSTRHptSTRHl;
		input >> m_mapSTwBFptSTRPptSTRPl;
		input >> m_mapSTptBFwSTRPptSTRPl;
		input >> m_mapSTptBFptSTRPwSTRPl;
		input >> m_mapSTptBFptSTRPptSTRPl;

		// st + bf + bf left/right head/pred
		input >> m_mapSTwBFptBFLHptBFLHl;
		input >> m_mapSTptBFwBFLHptBFLHl;
		input >> m_mapSTptBFptBFLHwBFLHl;
		input >> m_mapSTptBFptBFLHptBFLHl;
		input >> m_mapSTwBFptBFLPptBFLPl;
		input >> m_mapSTptBFwBFLPptBFLPl;
		input >> m_mapSTptBFptBFLPwBFLPl;
		input >> m_mapSTptBFptBFLPptBFLPl;
		input >> m_mapSTwBFptBFRHptBFRHl;
		input >> m_mapSTptBFwBFRHptBFRHl;
		input >> m_mapSTptBFptBFRHwBFRHl;
		input >> m_mapSTptBFptBFRHptBFRHl;
		input >> m_mapSTwBFptBFRPptBFRPl;
		input >> m_mapSTptBFwBFRPptBFRPl;
		input >> m_mapSTptBFptBFRPwBFRPl;
		input >> m_mapSTptBFptBFRPptBFRPl;

		// quar-gram
		// st + bf + st right head + st right pred
		input >> m_mapSTptBFptSTRHptSTRPptSTRHlSTRPl;
		// st + bf + st left pred + st left pred 2
		input >> m_mapSTptBFptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + bf + st right pred + st right pred 2
		input >> m_mapSTptBFptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + bf + bf left head + bf left pred
		input >> m_mapSTptBFptBFLHptBFLPptBFLHlBFLPl;
		// st + bf + bf left pred + bf left pred 2
		input >> m_mapSTptBFptBFLPptBFLP2ptBFLPlBFLP2l;
		// st + bf + bf right pred + bf right pred 2
		input >> m_mapSTptBFptBFRPptBFRP2ptBFRPlBFRP2l;

		// st + bf2 + st left/right head/pred
		input >> m_mapSTwBF2ptSTLHptSTLHl;
		input >> m_mapSTptBF2wSTLHptSTLHl;
		input >> m_mapSTptBF2ptSTLHwSTLHl;
		input >> m_mapSTptBF2ptSTLHptSTLHl;
		input >> m_mapSTwBF2ptSTLPptSTLPl;
		input >> m_mapSTptBF2wSTLPptSTLPl;
		input >> m_mapSTptBF2ptSTLPwSTLPl;
		input >> m_mapSTptBF2ptSTLPptSTLPl;
		input >> m_mapSTwBF2ptSTRHptSTRHl;
		input >> m_mapSTptBF2wSTRHptSTRHl;
		input >> m_mapSTptBF2ptSTRHwSTRHl;
		input >> m_mapSTptBF2ptSTRHptSTRHl;
		input >> m_mapSTwBF2ptSTRPptSTRPl;
		input >> m_mapSTptBF2wSTRPptSTRPl;
		input >> m_mapSTptBF2ptSTRPwSTRPl;
		input >> m_mapSTptBF2ptSTRPptSTRPl;

		// st + bf2 + bf2 left/right head/pred
		input >> m_mapSTwBF2ptBF2LHptBF2LHl;
		input >> m_mapSTptBF2wBF2LHptBF2LHl;
		input >> m_mapSTptBF2ptBF2LHwBF2LHl;
		input >> m_mapSTptBF2ptBF2LHptBF2LHl;
		input >> m_mapSTwBF2ptBF2LPptBF2LPl;
		input >> m_mapSTptBF2wBF2LPptBF2LPl;
		input >> m_mapSTptBF2ptBF2LPwBF2LPl;
		input >> m_mapSTptBF2ptBF2LPptBF2LPl;
		input >> m_mapSTwBF2ptBF2RHptBF2RHl;
		input >> m_mapSTptBF2wBF2RHptBF2RHl;
		input >> m_mapSTptBF2ptBF2RHwBF2RHl;
		input >> m_mapSTptBF2ptBF2RHptBF2RHl;
		input >> m_mapSTwBF2ptBF2RPptBF2RPl;
		input >> m_mapSTptBF2wBF2RPptBF2RPl;
		input >> m_mapSTptBF2ptBF2RPwBF2RPl;
		input >> m_mapSTptBF2ptBF2RPptBF2RPl;

		// quar-gram
		// st + bf2 + st right head + st right pred
		input >> m_mapSTptBF2ptSTRHptSTRPptSTRHlSTRPl;
		// st + bf2 + st left pred + st left pred 2
		input >> m_mapSTptBF2ptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + bf2 + st right pred + st right pred 2
		input >> m_mapSTptBF2ptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + bf2 + bf2 left head + bf2 left pred
		input >> m_mapSTptBF2ptBF2LHptBF2LPptBF2LHlBF2LPl;
		// st + bf2 + bf2 left pred + bf2 left pred 2
		input >> m_mapSTptBF2ptBF2LPptBF2LP2ptBF2LPlBF2LP2l;
		// st + bf2 + bf2 right pred + bf2 right pred 2
		input >> m_mapSTptBF2ptBF2RPptBF2RP2ptBF2RPlBF2RP2l;

		// st + sbf + st left/right head/pred
		input >> m_mapSTwSBFptSTLHptSTLHl;
		input >> m_mapSTptSBFwSTLHptSTLHl;
		input >> m_mapSTptSBFptSTLHwSTLHl;
		input >> m_mapSTptSBFptSTLHptSTLHl;
		input >> m_mapSTwSBFptSTLPptSTLPl;
		input >> m_mapSTptSBFwSTLPptSTLPl;
		input >> m_mapSTptSBFptSTLPwSTLPl;
		input >> m_mapSTptSBFptSTLPptSTLPl;
		input >> m_mapSTwSBFptSTRHptSTRHl;
		input >> m_mapSTptSBFwSTRHptSTRHl;
		input >> m_mapSTptSBFptSTRHwSTRHl;
		input >> m_mapSTptSBFptSTRHptSTRHl;
		input >> m_mapSTwSBFptSTRPptSTRPl;
		input >> m_mapSTptSBFwSTRPptSTRPl;
		input >> m_mapSTptSBFptSTRPwSTRPl;
		input >> m_mapSTptSBFptSTRPptSTRPl;

		// st + sbf + sbf left/right head/pred
		input >> m_mapSTwSBFptSBFLHptSBFLHl;
		input >> m_mapSTptSBFwSBFLHptSBFLHl;
		input >> m_mapSTptSBFptSBFLHwSBFLHl;
		input >> m_mapSTptSBFptSBFLHptSBFLHl;
		input >> m_mapSTwSBFptSBFLPptSBFLPl;
		input >> m_mapSTptSBFwSBFLPptSBFLPl;
		input >> m_mapSTptSBFptSBFLPwSBFLPl;
		input >> m_mapSTptSBFptSBFLPptSBFLPl;
		input >> m_mapSTwSBFptSBFRHptSBFRHl;
		input >> m_mapSTptSBFwSBFRHptSBFRHl;
		input >> m_mapSTptSBFptSBFRHwSBFRHl;
		input >> m_mapSTptSBFptSBFRHptSBFRHl;
		input >> m_mapSTwSBFptSBFRPptSBFRPl;
		input >> m_mapSTptSBFwSBFRPptSBFRPl;
		input >> m_mapSTptSBFptSBFRPwSBFRPl;
		input >> m_mapSTptSBFptSBFRPptSBFRPl;

		// quar-gram
		// st + sbf + st right head + st right pred
		input >> m_mapSTptSBFptSTRHptSTRPptSTRHlSTRPl;
		// st + sbf + st left pred + st left pred 2
		input >> m_mapSTptSBFptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + sbf + st right pred + st right pred 2
		input >> m_mapSTptSBFptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + sbf + sbf left head + sbf left pred
		input >> m_mapSTptSBFptSBFLHptSBFLPptSBFLHlSBFLPl;
		// st + sbf + sbf left pred + sbf left pred 2
		input >> m_mapSTptSBFptSBFLPptSBFLP2ptSBFLPlSBFLP2l;
		// st + sbf + sbf right pred + sbf right pred 2
		input >> m_mapSTptSBFptSBFRPptSBFRP2ptSBFRPlSBFRP2l;

		// st2 + bf + st2 left/right head/pred
		input >> m_mapST2wBFptST2LHptST2LHl;
		input >> m_mapST2ptBFwST2LHptST2LHl;
		input >> m_mapST2ptBFptST2LHwST2LHl;
		input >> m_mapST2ptBFptST2LHptST2LHl;
		input >> m_mapST2wBFptST2LPptST2LPl;
		input >> m_mapST2ptBFwST2LPptST2LPl;
		input >> m_mapST2ptBFptST2LPwST2LPl;
		input >> m_mapST2ptBFptST2LPptST2LPl;
		input >> m_mapST2wBFptST2RHptST2RHl;
		input >> m_mapST2ptBFwST2RHptST2RHl;
		input >> m_mapST2ptBFptST2RHwST2RHl;
		input >> m_mapST2ptBFptST2RHptST2RHl;
		input >> m_mapST2wBFptST2RPptST2RPl;
		input >> m_mapST2ptBFwST2RPptST2RPl;
		input >> m_mapST2ptBFptST2RPwST2RPl;
		input >> m_mapST2ptBFptST2RPptST2RPl;

		// st2 + bf + bf left/right head/pred
		input >> m_mapST2wBFptBFLHptBFLHl;
		input >> m_mapST2ptBFwBFLHptBFLHl;
		input >> m_mapST2ptBFptBFLHwBFLHl;
		input >> m_mapST2ptBFptBFLHptBFLHl;
		input >> m_mapST2wBFptBFLPptBFLPl;
		input >> m_mapST2ptBFwBFLPptBFLPl;
		input >> m_mapST2ptBFptBFLPwBFLPl;
		input >> m_mapST2ptBFptBFLPptBFLPl;
		input >> m_mapST2wBFptBFRHptBFRHl;
		input >> m_mapST2ptBFwBFRHptBFRHl;
		input >> m_mapST2ptBFptBFRHwBFRHl;
		input >> m_mapST2ptBFptBFRHptBFRHl;
		input >> m_mapST2wBFptBFRPptBFRPl;
		input >> m_mapST2ptBFwBFRPptBFRPl;
		input >> m_mapST2ptBFptBFRPwBFRPl;
		input >> m_mapST2ptBFptBFRPptBFRPl;

		// quar-gram
		// st2 + bf + st2 right head + st2 right pred
		input >> m_mapST2ptBFptST2RHptST2RPptST2RHlST2RPl;
		// st2 + bf + st2 left pred + st2 left pred 2
		input >> m_mapST2ptBFptST2LPptST2LP2ptST2LPlST2LP2l;
		// st2 + bf + st2 right pred + st2 right pred 2
		input >> m_mapST2ptBFptST2RPptST2RP2ptST2RPlST2RP2l;
		// st2 + bf + bf left head + bf left pred
		input >> m_mapST2ptBFptBFLHptBFLPptBFLHlBFLPl;
		// st2 + bf + bf left pred + bf left pred 2
		input >> m_mapST2ptBFptBFLPptBFLP2ptBFLPlBFLP2l;
		// st2 + bf + bf right pred + bf right pred 2
		input >> m_mapST2ptBFptBFRPptBFRP2ptBFRPlBFRP2l;

		// sst + bf + sst left/right head/pred
		input >> m_mapSSTwBFptSSTLHptSSTLHl;
		input >> m_mapSSTptBFwSSTLHptSSTLHl;
		input >> m_mapSSTptBFptSSTLHwSSTLHl;
		input >> m_mapSSTptBFptSSTLHptSSTLHl;
		input >> m_mapSSTwBFptSSTLPptSSTLPl;
		input >> m_mapSSTptBFwSSTLPptSSTLPl;
		input >> m_mapSSTptBFptSSTLPwSSTLPl;
		input >> m_mapSSTptBFptSSTLPptSSTLPl;
		input >> m_mapSSTwBFptSSTRHptSSTRHl;
		input >> m_mapSSTptBFwSSTRHptSSTRHl;
		input >> m_mapSSTptBFptSSTRHwSSTRHl;
		input >> m_mapSSTptBFptSSTRHptSSTRHl;
		input >> m_mapSSTwBFptSSTRPptSSTRPl;
		input >> m_mapSSTptBFwSSTRPptSSTRPl;
		input >> m_mapSSTptBFptSSTRPwSSTRPl;
		input >> m_mapSSTptBFptSSTRPptSSTRPl;

		// st + bf + bf left/right head/pred
		input >> m_mapSSTwBFptBFLHptBFLHl;
		input >> m_mapSSTptBFwBFLHptBFLHl;
		input >> m_mapSSTptBFptBFLHwBFLHl;
		input >> m_mapSSTptBFptBFLHptBFLHl;
		input >> m_mapSSTwBFptBFLPptBFLPl;
		input >> m_mapSSTptBFwBFLPptBFLPl;
		input >> m_mapSSTptBFptBFLPwBFLPl;
		input >> m_mapSSTptBFptBFLPptBFLPl;
		input >> m_mapSSTwBFptBFRHptBFRHl;
		input >> m_mapSSTptBFwBFRHptBFRHl;
		input >> m_mapSSTptBFptBFRHwBFRHl;
		input >> m_mapSSTptBFptBFRHptBFRHl;
		input >> m_mapSSTwBFptBFRPptBFRPl;
		input >> m_mapSSTptBFwBFRPptBFRPl;
		input >> m_mapSSTptBFptBFRPwBFRPl;
		input >> m_mapSSTptBFptBFRPptBFRPl;

		// quar-gram
		// sst + bf + sst right head + sst right pred
		input >> m_mapSSTptBFptSSTRHptSSTRPptSSTRHlSSTRPl;
		// sst + bf + sst left pred + sst left pred 2
		input >> m_mapSSTptBFptSSTLPptSSTLP2ptSSTLPlSSTLP2l;
		// sst + bf + sst right pred + sst right pred 2
		input >> m_mapSSTptBFptSSTRPptSSTRP2ptSSTRPlSSTRP2l;
		// sst + bf + bf left head + bf left pred
		input >> m_mapSSTptBFptBFLHptBFLPptBFLHlBFLPl;
		// sst + bf + bf left pred + bf left pred 2
		input >> m_mapSSTptBFptBFLPptBFLP2ptBFLPlBFLP2l;
		// sst + bf + bf right pred + bf right pred 2
		input >> m_mapSSTptBFptBFRPptBFRP2ptBFRPlBFRP2l;

		// st + bf + label set
		input >> m_mapSTwSTll;
		input >> m_mapSTwSTrl;
		input >> m_mapBFwBFll;
		input >> m_mapBFwBFrl;
		input >> m_mapSTptSTll;
		input >> m_mapSTptSTrl;
		input >> m_mapBFptBFll;
		input >> m_mapBFptBFrl;
		input >> m_mapSTptBFptSTll;
		input >> m_mapSTptBFptSTrl;
		input >> m_mapSTptBFptBFll;
		input >> m_mapSTptBFptBFrl;
		input >> m_mapSTptBF2ptSTll;
		input >> m_mapSTptBF2ptSTrl;
		input >> m_mapSTptBF2ptBF2ll;
		input >> m_mapSTptBF2ptBF2rl;
		input >> m_mapSTptSBFptSTll;
		input >> m_mapSTptSBFptSTrl;
		input >> m_mapSTptSBFptSBFll;
		input >> m_mapSTptSBFptSBFrl;
		// st2 + bf + label set
		input >> m_mapST2wST2ll;
		input >> m_mapST2wST2rl;
		input >> m_mapBF2wBF2ll;
		input >> m_mapBF2wBF2rl;
		input >> m_mapST2ptST2ll;
		input >> m_mapST2ptST2rl;
		input >> m_mapBF2ptBF2ll;
		input >> m_mapBF2ptBF2rl;
		input >> m_mapST2ptBFptST2ll;
		input >> m_mapST2ptBFptST2rl;
		input >> m_mapST2ptBFptBFll;
		input >> m_mapST2ptBFptBFrl;
		// sst + bf + label set
		input >> m_mapSSTwSSTll;
		input >> m_mapSSTwSSTrl;
		input >> m_mapSBFwSBFll;
		input >> m_mapSBFwSBFrl;
		input >> m_mapSSTptSSTll;
		input >> m_mapSSTptSSTrl;
		input >> m_mapSBFptSBFll;
		input >> m_mapSBFptSBFrl;
		input >> m_mapSSTptBFptSSTll;
		input >> m_mapSSTptBFptSSTrl;
		input >> m_mapSSTptBFptBFll;
		input >> m_mapSSTptBFptBFrl;

		// char feature (for chinese)
		// st context char
		input >> m_map1CharBeforeST;
		input >> m_map2CharBeforeST;
		input >> m_map3CharBeforeST;
		input >> m_map1CharAfterST;
		input >> m_map2CharAfterST;
		input >> m_map3CharAfterST;
		// bf context char
		input >> m_map1CharBeforeBF;
		input >> m_map2CharBeforeBF;
		input >> m_map3CharBeforeBF;
		input >> m_map1CharAfterBF;
		input >> m_map2CharAfterBF;
		input >> m_map3CharAfterBF;

		// path feature
		// st - bf
		input >> m_mapSTPOSPath;
		input >> m_mapSTFPOSPath;
		input >> m_mapSTwBFptSTsynhw;
		input >> m_mapSTwBFwSTsynhpt;
		input >> m_mapSTptBFwSTsynhw;
		input >> m_mapSTwBFptBFsynhw;
		input >> m_mapSTwBFwBFsynhpt;
		input >> m_mapSTptBFwBFsynhw;
		// st - bf2
		input >> m_mapBF2POSPath;
		input >> m_mapBF2FPOSPath;
		input >> m_mapSTwBF2ptSTsynhw;
		input >> m_mapSTwBF2wSTsynhpt;
		input >> m_mapSTptBF2wSTsynhw;
		input >> m_mapSTwBF2ptBF2synhw;
		input >> m_mapSTwBF2wBF2synhpt;
		input >> m_mapSTptBF2wBF2synhw;
		// st - sbf
		input >> m_mapSBFPOSPath;
		input >> m_mapSBFFPOSPath;
		input >> m_mapSTwSBFptSTsynhw;
		input >> m_mapSTwSBFwSTsynhpt;
		input >> m_mapSTptSBFwSTsynhw;
		input >> m_mapSTwSBFptSBFsynhw;
		input >> m_mapSTwSBFwSBFsynhpt;
		input >> m_mapSTptSBFwSBFsynhw;
		// st2 - bf
		input >> m_mapST2POSPath;
		input >> m_mapST2FPOSPath;
		input >> m_mapST2wBFptST2synhw;
		input >> m_mapST2wBFwST2synhpt;
		input >> m_mapST2ptBFwST2synhw;
		input >> m_mapST2wBFptBFsynhw;
		input >> m_mapST2wBFwBFsynhpt;
		input >> m_mapST2ptBFwBFsynhw;
		// sst - bf
		input >> m_mapSSTPOSPath;
		input >> m_mapSSTFPOSPath;
		input >> m_mapSSTwBFptSSTsynhw;
		input >> m_mapSSTwBFwSSTsynhpt;
		input >> m_mapSSTptBFwSSTsynhw;
		input >> m_mapSSTwBFptBFsynhw;
		input >> m_mapSSTwBFwBFsynhpt;
		input >> m_mapSSTptBFwBFsynhw;

		// supertag feature
		input >> m_mapSTst;
		input >> m_mapST2st;
		input >> m_mapSSTst;
		input >> m_mapBFist;
		input >> m_mapSTstST2st;
		input >> m_mapSTstSSTst;
		input >> m_mapSTstBFw;
		input >> m_mapSTstBFpt;
		input >> m_mapSTstBFwpt;

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
		// st, st2, sst
		// bf, bf2, sbf
		// st
		output << m_mapSTw;
		output << m_mapSTpt;
		// st2
		output << m_mapST2w;
		output << m_mapST2pt;
		// sst
		output << m_mapSSTw;
		output << m_mapSSTpt;
		// bf
		output << m_mapBFw;
		output << m_mapBFpt;
		// bf2
		output << m_mapBF2w;
		output << m_mapBF2pt;
		// sbf
		output << m_mapSBFw;
		output << m_mapSBFpt;

		// unigram context
		// st, bf
		// st context
		output << m_mapSTiw;
		output << m_mapSTiwSTjw;
		output << m_mapSTipt;
		output << m_mapSTiptSTjpt;
		// bf context
		output << m_mapBFiw;
		output << m_mapBFiwBFjw;
		output << m_mapBFipt;
		output << m_mapBFiptBFjpt;

		// unigram with label
		// st, st2, sst
		// bf, bf2, sbf
		// st
		output << m_mapSTwSTLHl;
		output << m_mapSTwSTLPl;
		output << m_mapSTwSTRHl;
		output << m_mapSTwSTRPl;
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
		// bf
		output << m_mapBFwBFLHl;
		output << m_mapBFwBFLPl;
		output << m_mapBFwBFRHl;
		output << m_mapBFwBFRPl;
		// bf2
		output << m_mapBF2wBF2LHl;
		output << m_mapBF2wBF2LPl;
		output << m_mapBF2wBF2RHl;
		output << m_mapBF2wBF2RPl;
		// sbf
		output << m_mapSBFwSBFLHl;
		output << m_mapSBFwSBFLPl;
		output << m_mapSBFwSBFRHl;
		output << m_mapSBFwSBFRPl;

		// unigram with arity
		// st, st2, sst
		// bf, bf2, sbf
		// st
		output << m_mapSTwSTLHi;
		output << m_mapSTwSTLPi;
		output << m_mapSTwSTRHi;
		output << m_mapSTwSTRPi;
		output << m_mapSTwSTHi;
		output << m_mapSTwSTPi;
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
		// bf
		output << m_mapBFwBFLHi;
		output << m_mapBFwBFLPi;
		output << m_mapBFwBFRHi;
		output << m_mapBFwBFRPi;
		output << m_mapBFwBFHi;
		output << m_mapBFwBFPi;
		// bf2
		output << m_mapBF2wBF2LHi;
		output << m_mapBF2wBF2LPi;
		output << m_mapBF2wBF2RHi;
		output << m_mapBF2wBF2RPi;
		output << m_mapBF2wBF2Hi;
		output << m_mapBF2wBF2Pi;
		// sbf
		output << m_mapSBFwSBFLHi;
		output << m_mapSBFwSBFLPi;
		output << m_mapSBFwSBFRHi;
		output << m_mapSBFwSBFRPi;
		output << m_mapSBFwSBFHi;
		output << m_mapSBFwSBFPi;

		// bi-gram
		// st + bf
		output << m_mapSTwBFw;
		output << m_mapSTwBFpt;
		output << m_mapSTptBFw;
		output << m_mapSTptBFpt;
		output << m_mapSTwptBFw;
		output << m_mapSTwBFwpt;
		output << m_mapSTwptBFpt;
		output << m_mapSTptBFwpt;
		output << m_mapSTwptBFwpt;
		// st + bf + dis
		output << m_mapSTwBFwD;
		output << m_mapSTwBFptD;
		output << m_mapSTptBFwD;
		output << m_mapSTptBFptD;
		// st + bf + st left/right head/pred
		output << m_mapSTwBFwSTRPi;
		output << m_mapSTwBFwBFLPi;

		// bi-gram
		// st + bf2
		output << m_mapSTwBF2w;
		output << m_mapSTwBF2pt;
		output << m_mapSTptBF2w;
		output << m_mapSTptBF2pt;
		output << m_mapSTwptBF2w;
		output << m_mapSTwBF2wpt;
		output << m_mapSTwptBF2pt;
		output << m_mapSTptBF2wpt;
		output << m_mapSTwptBF2wpt;
		// st + bf2 + dis
		output << m_mapSTwBF2wD;
		output << m_mapSTwBF2ptD;
		output << m_mapSTptBF2wD;
		output << m_mapSTptBF2ptD;
		// st + bf2 + st left/right head/pred
		output << m_mapSTwBF2wSTRPi;
		output << m_mapSTwBF2wBF2LPi;

		// bi-gram
		// st + sbf
		output << m_mapSTwSBFw;
		output << m_mapSTwSBFpt;
		output << m_mapSTptSBFw;
		output << m_mapSTptSBFpt;
		output << m_mapSTwptSBFw;
		output << m_mapSTwSBFwpt;
		output << m_mapSTwptSBFpt;
		output << m_mapSTptSBFwpt;
		output << m_mapSTwptSBFwpt;
		// st + sbf + dis
		output << m_mapSTwSBFwD;
		output << m_mapSTwSBFptD;
		output << m_mapSTptSBFwD;
		output << m_mapSTptSBFptD;
		// st + sbf + st left/right head/pred
		output << m_mapSTwSBFwSTRPi;
		output << m_mapSTwSBFwSBFLPi;

		// st2 + bf
		output << m_mapST2wBFw;
		output << m_mapST2wBFpt;
		output << m_mapST2ptBFw;
		output << m_mapST2ptBFpt;
		output << m_mapST2wptBFw;
		output << m_mapST2wBFwpt;
		output << m_mapST2wptBFpt;
		output << m_mapST2ptBFwpt;
		output << m_mapST2wptBFwpt;
		// st2 + bf + dis
		output << m_mapST2wBFwD;
		output << m_mapST2wBFptD;
		output << m_mapST2ptBFwD;
		output << m_mapST2ptBFptD;
		// st2 + bf + st2 left/right head/pred
		output << m_mapST2wBFwST2RPi;
		output << m_mapST2wBFwBFLPi;

		// sst + bf
		output << m_mapSSTwBFw;
		output << m_mapSSTwBFpt;
		output << m_mapSSTptBFw;
		output << m_mapSSTptBFpt;
		output << m_mapSSTwptBFw;
		output << m_mapSSTwBFwpt;
		output << m_mapSSTwptBFpt;
		output << m_mapSSTptBFwpt;
		output << m_mapSSTwptBFwpt;
		// sst + bf + dis
		output << m_mapSSTwBFwD;
		output << m_mapSSTwBFptD;
		output << m_mapSSTptBFwD;
		output << m_mapSSTptBFptD;
		// st2 + bf + st2 left/right head/pred
		output << m_mapSSTwBFwSSTRPi;
		output << m_mapSSTwBFwBFLPi;

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

		// bf + bf2
		output << m_mapBFwBF2w;
		output << m_mapBFwBF2pt;
		output << m_mapBFptBF2w;
		output << m_mapBFptBF2pt;
		output << m_mapBFwptBF2w;
		output << m_mapBFwBF2wpt;
		output << m_mapBFwptBF2pt;
		output << m_mapBFptBF2wpt;
		output << m_mapBFwptBF2wpt;

		// bf + sbf
		output << m_mapBFwSBFw;
		output << m_mapBFwSBFpt;
		output << m_mapBFptSBFw;
		output << m_mapBFptSBFpt;
		output << m_mapBFwptSBFw;
		output << m_mapBFwSBFwpt;
		output << m_mapBFwptSBFpt;
		output << m_mapBFptSBFwpt;
		output << m_mapBFwptSBFwpt;

		// tri-gram
		// st + bf + st2
		output << m_mapSTwBFptST2pt;
		output << m_mapSTptBFwST2pt;
		output << m_mapSTptBFptST2w;
		output << m_mapSTptBFptST2pt;
		// st + bf + sst
		output << m_mapSTwBFptSSTpt;
		output << m_mapSTptBFwSSTpt;
		output << m_mapSTptBFptSSTw;
		output << m_mapSTptBFptSSTpt;
		// st + bf + bf2
		output << m_mapSTwBFptBF2pt;
		output << m_mapSTptBFwBF2pt;
		output << m_mapSTptBFptBF2w;
		output << m_mapSTptBFptBF2pt;
		// st + bf + sbf
		output << m_mapSTwBFptSBFpt;
		output << m_mapSTptBFwSBFpt;
		output << m_mapSTptBFptSBFw;
		output << m_mapSTptBFptSBFpt;

		// st + bf + st left/right head/pred
		output << m_mapSTwBFptSTLHptSTLHl;
		output << m_mapSTptBFwSTLHptSTLHl;
		output << m_mapSTptBFptSTLHwSTLHl;
		output << m_mapSTptBFptSTLHptSTLHl;
		output << m_mapSTwBFptSTLPptSTLPl;
		output << m_mapSTptBFwSTLPptSTLPl;
		output << m_mapSTptBFptSTLPwSTLPl;
		output << m_mapSTptBFptSTLPptSTLPl;
		output << m_mapSTwBFptSTRHptSTRHl;
		output << m_mapSTptBFwSTRHptSTRHl;
		output << m_mapSTptBFptSTRHwSTRHl;
		output << m_mapSTptBFptSTRHptSTRHl;
		output << m_mapSTwBFptSTRPptSTRPl;
		output << m_mapSTptBFwSTRPptSTRPl;
		output << m_mapSTptBFptSTRPwSTRPl;
		output << m_mapSTptBFptSTRPptSTRPl;

		// st + bf + bf left/right head/pred
		output << m_mapSTwBFptBFLHptBFLHl;
		output << m_mapSTptBFwBFLHptBFLHl;
		output << m_mapSTptBFptBFLHwBFLHl;
		output << m_mapSTptBFptBFLHptBFLHl;
		output << m_mapSTwBFptBFLPptBFLPl;
		output << m_mapSTptBFwBFLPptBFLPl;
		output << m_mapSTptBFptBFLPwBFLPl;
		output << m_mapSTptBFptBFLPptBFLPl;
		output << m_mapSTwBFptBFRHptBFRHl;
		output << m_mapSTptBFwBFRHptBFRHl;
		output << m_mapSTptBFptBFRHwBFRHl;
		output << m_mapSTptBFptBFRHptBFRHl;
		output << m_mapSTwBFptBFRPptBFRPl;
		output << m_mapSTptBFwBFRPptBFRPl;
		output << m_mapSTptBFptBFRPwBFRPl;
		output << m_mapSTptBFptBFRPptBFRPl;

		// quar-gram
		// st + bf + st right head + st right pred
		output << m_mapSTptBFptSTRHptSTRPptSTRHlSTRPl;
		// st + bf + st left pred + st left pred 2
		output << m_mapSTptBFptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + bf + st right pred + st right pred 2
		output << m_mapSTptBFptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + bf + bf left head + bf left pred
		output << m_mapSTptBFptBFLHptBFLPptBFLHlBFLPl;
		// st + bf + bf left pred + bf left pred 2
		output << m_mapSTptBFptBFLPptBFLP2ptBFLPlBFLP2l;
		// st + bf + bf right pred + bf right pred 2
		output << m_mapSTptBFptBFRPptBFRP2ptBFRPlBFRP2l;

		// st + bf2 + st left/right head/pred
		output << m_mapSTwBF2ptSTLHptSTLHl;
		output << m_mapSTptBF2wSTLHptSTLHl;
		output << m_mapSTptBF2ptSTLHwSTLHl;
		output << m_mapSTptBF2ptSTLHptSTLHl;
		output << m_mapSTwBF2ptSTLPptSTLPl;
		output << m_mapSTptBF2wSTLPptSTLPl;
		output << m_mapSTptBF2ptSTLPwSTLPl;
		output << m_mapSTptBF2ptSTLPptSTLPl;
		output << m_mapSTwBF2ptSTRHptSTRHl;
		output << m_mapSTptBF2wSTRHptSTRHl;
		output << m_mapSTptBF2ptSTRHwSTRHl;
		output << m_mapSTptBF2ptSTRHptSTRHl;
		output << m_mapSTwBF2ptSTRPptSTRPl;
		output << m_mapSTptBF2wSTRPptSTRPl;
		output << m_mapSTptBF2ptSTRPwSTRPl;
		output << m_mapSTptBF2ptSTRPptSTRPl;

		// st + bf2 + bf2 left/right head/pred
		output << m_mapSTwBF2ptBF2LHptBF2LHl;
		output << m_mapSTptBF2wBF2LHptBF2LHl;
		output << m_mapSTptBF2ptBF2LHwBF2LHl;
		output << m_mapSTptBF2ptBF2LHptBF2LHl;
		output << m_mapSTwBF2ptBF2LPptBF2LPl;
		output << m_mapSTptBF2wBF2LPptBF2LPl;
		output << m_mapSTptBF2ptBF2LPwBF2LPl;
		output << m_mapSTptBF2ptBF2LPptBF2LPl;
		output << m_mapSTwBF2ptBF2RHptBF2RHl;
		output << m_mapSTptBF2wBF2RHptBF2RHl;
		output << m_mapSTptBF2ptBF2RHwBF2RHl;
		output << m_mapSTptBF2ptBF2RHptBF2RHl;
		output << m_mapSTwBF2ptBF2RPptBF2RPl;
		output << m_mapSTptBF2wBF2RPptBF2RPl;
		output << m_mapSTptBF2ptBF2RPwBF2RPl;
		output << m_mapSTptBF2ptBF2RPptBF2RPl;

		// quar-gram
		// st + bf2 + st right head + st right pred
		output << m_mapSTptBF2ptSTRHptSTRPptSTRHlSTRPl;
		// st + bf2 + st left pred + st left pred 2
		output << m_mapSTptBF2ptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + bf2 + st right pred + st right pred 2
		output << m_mapSTptBF2ptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + bf2 + bf2 left head + bf2 left pred
		output << m_mapSTptBF2ptBF2LHptBF2LPptBF2LHlBF2LPl;
		// st + bf2 + bf2 left pred + bf2 left pred 2
		output << m_mapSTptBF2ptBF2LPptBF2LP2ptBF2LPlBF2LP2l;
		// st + bf2 + bf2 right pred + bf2 right pred 2
		output << m_mapSTptBF2ptBF2RPptBF2RP2ptBF2RPlBF2RP2l;

		// st + sbf + st left/right head/pred
		output << m_mapSTwSBFptSTLHptSTLHl;
		output << m_mapSTptSBFwSTLHptSTLHl;
		output << m_mapSTptSBFptSTLHwSTLHl;
		output << m_mapSTptSBFptSTLHptSTLHl;
		output << m_mapSTwSBFptSTLPptSTLPl;
		output << m_mapSTptSBFwSTLPptSTLPl;
		output << m_mapSTptSBFptSTLPwSTLPl;
		output << m_mapSTptSBFptSTLPptSTLPl;
		output << m_mapSTwSBFptSTRHptSTRHl;
		output << m_mapSTptSBFwSTRHptSTRHl;
		output << m_mapSTptSBFptSTRHwSTRHl;
		output << m_mapSTptSBFptSTRHptSTRHl;
		output << m_mapSTwSBFptSTRPptSTRPl;
		output << m_mapSTptSBFwSTRPptSTRPl;
		output << m_mapSTptSBFptSTRPwSTRPl;
		output << m_mapSTptSBFptSTRPptSTRPl;

		// st + sbf + sbf left/right head/pred
		output << m_mapSTwSBFptSBFLHptSBFLHl;
		output << m_mapSTptSBFwSBFLHptSBFLHl;
		output << m_mapSTptSBFptSBFLHwSBFLHl;
		output << m_mapSTptSBFptSBFLHptSBFLHl;
		output << m_mapSTwSBFptSBFLPptSBFLPl;
		output << m_mapSTptSBFwSBFLPptSBFLPl;
		output << m_mapSTptSBFptSBFLPwSBFLPl;
		output << m_mapSTptSBFptSBFLPptSBFLPl;
		output << m_mapSTwSBFptSBFRHptSBFRHl;
		output << m_mapSTptSBFwSBFRHptSBFRHl;
		output << m_mapSTptSBFptSBFRHwSBFRHl;
		output << m_mapSTptSBFptSBFRHptSBFRHl;
		output << m_mapSTwSBFptSBFRPptSBFRPl;
		output << m_mapSTptSBFwSBFRPptSBFRPl;
		output << m_mapSTptSBFptSBFRPwSBFRPl;
		output << m_mapSTptSBFptSBFRPptSBFRPl;

		// quar-gram
		// st + sbf + st right head + st right pred
		output << m_mapSTptSBFptSTRHptSTRPptSTRHlSTRPl;
		// st + sbf + st left pred + st left pred 2
		output << m_mapSTptSBFptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + sbf + st right pred + st right pred 2
		output << m_mapSTptSBFptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + sbf + sbf left head + sbf left pred
		output << m_mapSTptSBFptSBFLHptSBFLPptSBFLHlSBFLPl;
		// st + sbf + sbf left pred + sbf left pred 2
		output << m_mapSTptSBFptSBFLPptSBFLP2ptSBFLPlSBFLP2l;
		// st + sbf + sbf right pred + sbf right pred 2
		output << m_mapSTptSBFptSBFRPptSBFRP2ptSBFRPlSBFRP2l;

		// st2 + bf + st2 left/right head/pred
		output << m_mapST2wBFptST2LHptST2LHl;
		output << m_mapST2ptBFwST2LHptST2LHl;
		output << m_mapST2ptBFptST2LHwST2LHl;
		output << m_mapST2ptBFptST2LHptST2LHl;
		output << m_mapST2wBFptST2LPptST2LPl;
		output << m_mapST2ptBFwST2LPptST2LPl;
		output << m_mapST2ptBFptST2LPwST2LPl;
		output << m_mapST2ptBFptST2LPptST2LPl;
		output << m_mapST2wBFptST2RHptST2RHl;
		output << m_mapST2ptBFwST2RHptST2RHl;
		output << m_mapST2ptBFptST2RHwST2RHl;
		output << m_mapST2ptBFptST2RHptST2RHl;
		output << m_mapST2wBFptST2RPptST2RPl;
		output << m_mapST2ptBFwST2RPptST2RPl;
		output << m_mapST2ptBFptST2RPwST2RPl;
		output << m_mapST2ptBFptST2RPptST2RPl;

		// st2 + bf + bf left/right head/pred
		output << m_mapST2wBFptBFLHptBFLHl;
		output << m_mapST2ptBFwBFLHptBFLHl;
		output << m_mapST2ptBFptBFLHwBFLHl;
		output << m_mapST2ptBFptBFLHptBFLHl;
		output << m_mapST2wBFptBFLPptBFLPl;
		output << m_mapST2ptBFwBFLPptBFLPl;
		output << m_mapST2ptBFptBFLPwBFLPl;
		output << m_mapST2ptBFptBFLPptBFLPl;
		output << m_mapST2wBFptBFRHptBFRHl;
		output << m_mapST2ptBFwBFRHptBFRHl;
		output << m_mapST2ptBFptBFRHwBFRHl;
		output << m_mapST2ptBFptBFRHptBFRHl;
		output << m_mapST2wBFptBFRPptBFRPl;
		output << m_mapST2ptBFwBFRPptBFRPl;
		output << m_mapST2ptBFptBFRPwBFRPl;
		output << m_mapST2ptBFptBFRPptBFRPl;

		// quar-gram
		// st2 + bf + st2 right head + st2 right pred
		output << m_mapST2ptBFptST2RHptST2RPptST2RHlST2RPl;
		// st2 + bf + st2 left pred + st2 left pred 2
		output << m_mapST2ptBFptST2LPptST2LP2ptST2LPlST2LP2l;
		// st2 + bf + st2 right pred + st2 right pred 2
		output << m_mapST2ptBFptST2RPptST2RP2ptST2RPlST2RP2l;
		// st2 + bf + bf left head + bf left pred
		output << m_mapST2ptBFptBFLHptBFLPptBFLHlBFLPl;
		// st2 + bf + bf left pred + bf left pred 2
		output << m_mapST2ptBFptBFLPptBFLP2ptBFLPlBFLP2l;
		// st2 + bf + bf right pred + bf right pred 2
		output << m_mapST2ptBFptBFRPptBFRP2ptBFRPlBFRP2l;

		// sst + bf + sst left/right head/pred
		output << m_mapSSTwBFptSSTLHptSSTLHl;
		output << m_mapSSTptBFwSSTLHptSSTLHl;
		output << m_mapSSTptBFptSSTLHwSSTLHl;
		output << m_mapSSTptBFptSSTLHptSSTLHl;
		output << m_mapSSTwBFptSSTLPptSSTLPl;
		output << m_mapSSTptBFwSSTLPptSSTLPl;
		output << m_mapSSTptBFptSSTLPwSSTLPl;
		output << m_mapSSTptBFptSSTLPptSSTLPl;
		output << m_mapSSTwBFptSSTRHptSSTRHl;
		output << m_mapSSTptBFwSSTRHptSSTRHl;
		output << m_mapSSTptBFptSSTRHwSSTRHl;
		output << m_mapSSTptBFptSSTRHptSSTRHl;
		output << m_mapSSTwBFptSSTRPptSSTRPl;
		output << m_mapSSTptBFwSSTRPptSSTRPl;
		output << m_mapSSTptBFptSSTRPwSSTRPl;
		output << m_mapSSTptBFptSSTRPptSSTRPl;

		// st + bf + bf left/right head/pred
		output << m_mapSSTwBFptBFLHptBFLHl;
		output << m_mapSSTptBFwBFLHptBFLHl;
		output << m_mapSSTptBFptBFLHwBFLHl;
		output << m_mapSSTptBFptBFLHptBFLHl;
		output << m_mapSSTwBFptBFLPptBFLPl;
		output << m_mapSSTptBFwBFLPptBFLPl;
		output << m_mapSSTptBFptBFLPwBFLPl;
		output << m_mapSSTptBFptBFLPptBFLPl;
		output << m_mapSSTwBFptBFRHptBFRHl;
		output << m_mapSSTptBFwBFRHptBFRHl;
		output << m_mapSSTptBFptBFRHwBFRHl;
		output << m_mapSSTptBFptBFRHptBFRHl;
		output << m_mapSSTwBFptBFRPptBFRPl;
		output << m_mapSSTptBFwBFRPptBFRPl;
		output << m_mapSSTptBFptBFRPwBFRPl;
		output << m_mapSSTptBFptBFRPptBFRPl;

		// quar-gram
		// sst + bf + sst right head + sst right pred
		output << m_mapSSTptBFptSSTRHptSSTRPptSSTRHlSSTRPl;
		// sst + bf + sst left pred + sst left pred 2
		output << m_mapSSTptBFptSSTLPptSSTLP2ptSSTLPlSSTLP2l;
		// sst + bf + sst right pred + sst right pred 2
		output << m_mapSSTptBFptSSTRPptSSTRP2ptSSTRPlSSTRP2l;
		// sst + bf + bf left head + bf left pred
		output << m_mapSSTptBFptBFLHptBFLPptBFLHlBFLPl;
		// sst + bf + bf left pred + bf left pred 2
		output << m_mapSSTptBFptBFLPptBFLP2ptBFLPlBFLP2l;
		// sst + bf + bf right pred + bf right pred 2
		output << m_mapSSTptBFptBFRPptBFRP2ptBFRPlBFRP2l;

		// st + bf + label set
		output << m_mapSTwSTll;
		output << m_mapSTwSTrl;
		output << m_mapBFwBFll;
		output << m_mapBFwBFrl;
		output << m_mapSTptSTll;
		output << m_mapSTptSTrl;
		output << m_mapBFptBFll;
		output << m_mapBFptBFrl;
		output << m_mapSTptBFptSTll;
		output << m_mapSTptBFptSTrl;
		output << m_mapSTptBFptBFll;
		output << m_mapSTptBFptBFrl;
		output << m_mapSTptBF2ptSTll;
		output << m_mapSTptBF2ptSTrl;
		output << m_mapSTptBF2ptBF2ll;
		output << m_mapSTptBF2ptBF2rl;
		output << m_mapSTptSBFptSTll;
		output << m_mapSTptSBFptSTrl;
		output << m_mapSTptSBFptSBFll;
		output << m_mapSTptSBFptSBFrl;
		// st2 + bf + label set
		output << m_mapST2wST2ll;
		output << m_mapST2wST2rl;
		output << m_mapBF2wBF2ll;
		output << m_mapBF2wBF2rl;
		output << m_mapST2ptST2ll;
		output << m_mapST2ptST2rl;
		output << m_mapBF2ptBF2ll;
		output << m_mapBF2ptBF2rl;
		output << m_mapST2ptBFptST2ll;
		output << m_mapST2ptBFptST2rl;
		output << m_mapST2ptBFptBFll;
		output << m_mapST2ptBFptBFrl;
		// sst + bf + label set
		output << m_mapSSTwSSTll;
		output << m_mapSSTwSSTrl;
		output << m_mapSBFwSBFll;
		output << m_mapSBFwSBFrl;
		output << m_mapSSTptSSTll;
		output << m_mapSSTptSSTrl;
		output << m_mapSBFptSBFll;
		output << m_mapSBFptSBFrl;
		output << m_mapSSTptBFptSSTll;
		output << m_mapSSTptBFptSSTrl;
		output << m_mapSSTptBFptBFll;
		output << m_mapSSTptBFptBFrl;

		// char feature (for chinese)
		// st context char
		output << m_map1CharBeforeST;
		output << m_map2CharBeforeST;
		output << m_map3CharBeforeST;
		output << m_map1CharAfterST;
		output << m_map2CharAfterST;
		output << m_map3CharAfterST;
		// bf context char
		output << m_map1CharBeforeBF;
		output << m_map2CharBeforeBF;
		output << m_map3CharBeforeBF;
		output << m_map1CharAfterBF;
		output << m_map2CharAfterBF;
		output << m_map3CharAfterBF;

		// path feature
		// st - bf
		output << m_mapSTPOSPath;
		output << m_mapSTFPOSPath;
		output << m_mapSTwBFptSTsynhw;
		output << m_mapSTwBFwSTsynhpt;
		output << m_mapSTptBFwSTsynhw;
		output << m_mapSTwBFptBFsynhw;
		output << m_mapSTwBFwBFsynhpt;
		output << m_mapSTptBFwBFsynhw;
		// st - bf2
		output << m_mapBF2POSPath;
		output << m_mapBF2FPOSPath;
		output << m_mapSTwBF2ptSTsynhw;
		output << m_mapSTwBF2wSTsynhpt;
		output << m_mapSTptBF2wSTsynhw;
		output << m_mapSTwBF2ptBF2synhw;
		output << m_mapSTwBF2wBF2synhpt;
		output << m_mapSTptBF2wBF2synhw;
		// st - sbf
		output << m_mapSBFPOSPath;
		output << m_mapSBFFPOSPath;
		output << m_mapSTwSBFptSTsynhw;
		output << m_mapSTwSBFwSTsynhpt;
		output << m_mapSTptSBFwSTsynhw;
		output << m_mapSTwSBFptSBFsynhw;
		output << m_mapSTwSBFwSBFsynhpt;
		output << m_mapSTptSBFwSBFsynhw;
		// st2 - bf
		output << m_mapST2POSPath;
		output << m_mapST2FPOSPath;
		output << m_mapST2wBFptST2synhw;
		output << m_mapST2wBFwST2synhpt;
		output << m_mapST2ptBFwST2synhw;
		output << m_mapST2wBFptBFsynhw;
		output << m_mapST2wBFwBFsynhpt;
		output << m_mapST2ptBFwBFsynhw;
		// sst - bf
		output << m_mapSSTPOSPath;
		output << m_mapSSTFPOSPath;
		output << m_mapSSTwBFptSSTsynhw;
		output << m_mapSSTwBFwSSTsynhpt;
		output << m_mapSSTptBFwSSTsynhw;
		output << m_mapSSTwBFptBFsynhw;
		output << m_mapSSTwBFwBFsynhpt;
		output << m_mapSSTptBFwBFsynhw;

		// supertag feature
		output << m_mapSTst;
		output << m_mapST2st;
		output << m_mapSSTst;
		output << m_mapBFist;
		output << m_mapSTstST2st;
		output << m_mapSTstSSTst;
		output << m_mapSTstBFw;
		output << m_mapSTstBFpt;
		output << m_mapSTstBFwpt;

		output.close();
	}

	template<class RET_TYPE>
	void Weight<RET_TYPE>::computeAverageFeatureWeights(const int & round) {

		// uni-gram
		// st, st2, sst
		// bf, bf2, sbf
		// st
		m_mapSTw.computeAverage(round);
		m_mapSTpt.computeAverage(round);
		// st2
		m_mapST2w.computeAverage(round);
		m_mapST2pt.computeAverage(round);
		// sst
		m_mapSSTw.computeAverage(round);
		m_mapSSTpt.computeAverage(round);
		// bf
		m_mapBFw.computeAverage(round);
		m_mapBFpt.computeAverage(round);
		// bf2
		m_mapBF2w.computeAverage(round);
		m_mapBF2pt.computeAverage(round);
		// sbf
		m_mapSBFw.computeAverage(round);
		m_mapSBFpt.computeAverage(round);

		// unigram context
		// st, bf
		// st context
		m_mapSTiw.computeAverage(round);
		m_mapSTiwSTjw.computeAverage(round);
		m_mapSTipt.computeAverage(round);
		m_mapSTiptSTjpt.computeAverage(round);
		// bf context
		m_mapBFiw.computeAverage(round);
		m_mapBFiwBFjw.computeAverage(round);
		m_mapBFipt.computeAverage(round);
		m_mapBFiptBFjpt.computeAverage(round);

		// unigram with label
		// st, st2, sst
		// bf, bf2, sbf
		// st
		m_mapSTwSTLHl.computeAverage(round);
		m_mapSTwSTLPl.computeAverage(round);
		m_mapSTwSTRHl.computeAverage(round);
		m_mapSTwSTRPl.computeAverage(round);
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
		// bf
		m_mapBFwBFLHl.computeAverage(round);
		m_mapBFwBFLPl.computeAverage(round);
		m_mapBFwBFRHl.computeAverage(round);
		m_mapBFwBFRPl.computeAverage(round);
		// bf2
		m_mapBF2wBF2LHl.computeAverage(round);
		m_mapBF2wBF2LPl.computeAverage(round);
		m_mapBF2wBF2RHl.computeAverage(round);
		m_mapBF2wBF2RPl.computeAverage(round);
		// sbf
		m_mapSBFwSBFLHl.computeAverage(round);
		m_mapSBFwSBFLPl.computeAverage(round);
		m_mapSBFwSBFRHl.computeAverage(round);
		m_mapSBFwSBFRPl.computeAverage(round);

		// unigram with arity
		// st, st2, sst
		// bf, bf2, sbf
		// st
		m_mapSTwSTLHi.computeAverage(round);
		m_mapSTwSTLPi.computeAverage(round);
		m_mapSTwSTRHi.computeAverage(round);
		m_mapSTwSTRPi.computeAverage(round);
		m_mapSTwSTHi.computeAverage(round);
		m_mapSTwSTPi.computeAverage(round);
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
		// bf
		m_mapBFwBFLHi.computeAverage(round);
		m_mapBFwBFLPi.computeAverage(round);
		m_mapBFwBFRHi.computeAverage(round);
		m_mapBFwBFRPi.computeAverage(round);
		m_mapBFwBFHi.computeAverage(round);
		m_mapBFwBFPi.computeAverage(round);
		// bf2
		m_mapBF2wBF2LHi.computeAverage(round);
		m_mapBF2wBF2LPi.computeAverage(round);
		m_mapBF2wBF2RHi.computeAverage(round);
		m_mapBF2wBF2RPi.computeAverage(round);
		m_mapBF2wBF2Hi.computeAverage(round);
		m_mapBF2wBF2Pi.computeAverage(round);
		// sbf
		m_mapSBFwSBFLHi.computeAverage(round);
		m_mapSBFwSBFLPi.computeAverage(round);
		m_mapSBFwSBFRHi.computeAverage(round);
		m_mapSBFwSBFRPi.computeAverage(round);
		m_mapSBFwSBFHi.computeAverage(round);
		m_mapSBFwSBFPi.computeAverage(round);

		// bi-gram
		// st + bf
		m_mapSTwBFw.computeAverage(round);
		m_mapSTwBFpt.computeAverage(round);
		m_mapSTptBFw.computeAverage(round);
		m_mapSTptBFpt.computeAverage(round);
		m_mapSTwptBFw.computeAverage(round);
		m_mapSTwBFwpt.computeAverage(round);
		m_mapSTwptBFpt.computeAverage(round);
		m_mapSTptBFwpt.computeAverage(round);
		m_mapSTwptBFwpt.computeAverage(round);
		// st + bf + dis
		m_mapSTwBFwD.computeAverage(round);
		m_mapSTwBFptD.computeAverage(round);
		m_mapSTptBFwD.computeAverage(round);
		m_mapSTptBFptD.computeAverage(round);
		// st + bf + st left/right head/pred
		m_mapSTwBFwSTRPi.computeAverage(round);
		m_mapSTwBFwBFLPi.computeAverage(round);

		// bi-gram
		// st + bf2
		m_mapSTwBF2w.computeAverage(round);
		m_mapSTwBF2pt.computeAverage(round);
		m_mapSTptBF2w.computeAverage(round);
		m_mapSTptBF2pt.computeAverage(round);
		m_mapSTwptBF2w.computeAverage(round);
		m_mapSTwBF2wpt.computeAverage(round);
		m_mapSTwptBF2pt.computeAverage(round);
		m_mapSTptBF2wpt.computeAverage(round);
		m_mapSTwptBF2wpt.computeAverage(round);
		// st + bf2 + dis
		m_mapSTwBF2wD.computeAverage(round);
		m_mapSTwBF2ptD.computeAverage(round);
		m_mapSTptBF2wD.computeAverage(round);
		m_mapSTptBF2ptD.computeAverage(round);
		// st + bf2 + st left/right head/pred
		m_mapSTwBF2wSTRPi.computeAverage(round);
		m_mapSTwBF2wBF2LPi.computeAverage(round);

		// bi-gram
		// st + sbf
		m_mapSTwSBFw.computeAverage(round);
		m_mapSTwSBFpt.computeAverage(round);
		m_mapSTptSBFw.computeAverage(round);
		m_mapSTptSBFpt.computeAverage(round);
		m_mapSTwptSBFw.computeAverage(round);
		m_mapSTwSBFwpt.computeAverage(round);
		m_mapSTwptSBFpt.computeAverage(round);
		m_mapSTptSBFwpt.computeAverage(round);
		m_mapSTwptSBFwpt.computeAverage(round);
		// st + sbf + dis
		m_mapSTwSBFwD.computeAverage(round);
		m_mapSTwSBFptD.computeAverage(round);
		m_mapSTptSBFwD.computeAverage(round);
		m_mapSTptSBFptD.computeAverage(round);
		// st + sbf + st left/right head/pred
		m_mapSTwSBFwSTRPi.computeAverage(round);
		m_mapSTwSBFwSBFLPi.computeAverage(round);

		// st2 + bf
		m_mapST2wBFw.computeAverage(round);
		m_mapST2wBFpt.computeAverage(round);
		m_mapST2ptBFw.computeAverage(round);
		m_mapST2ptBFpt.computeAverage(round);
		m_mapST2wptBFw.computeAverage(round);
		m_mapST2wBFwpt.computeAverage(round);
		m_mapST2wptBFpt.computeAverage(round);
		m_mapST2ptBFwpt.computeAverage(round);
		m_mapST2wptBFwpt.computeAverage(round);
		// st2 + bf + dis
		m_mapST2wBFwD.computeAverage(round);
		m_mapST2wBFptD.computeAverage(round);
		m_mapST2ptBFwD.computeAverage(round);
		m_mapST2ptBFptD.computeAverage(round);
		// st2 + bf + st2 left/right head/pred
		m_mapST2wBFwST2RPi.computeAverage(round);
		m_mapST2wBFwBFLPi.computeAverage(round);

		// sst + bf
		m_mapSSTwBFw.computeAverage(round);
		m_mapSSTwBFpt.computeAverage(round);
		m_mapSSTptBFw.computeAverage(round);
		m_mapSSTptBFpt.computeAverage(round);
		m_mapSSTwptBFw.computeAverage(round);
		m_mapSSTwBFwpt.computeAverage(round);
		m_mapSSTwptBFpt.computeAverage(round);
		m_mapSSTptBFwpt.computeAverage(round);
		m_mapSSTwptBFwpt.computeAverage(round);
		// sst + bf + dis
		m_mapSSTwBFwD.computeAverage(round);
		m_mapSSTwBFptD.computeAverage(round);
		m_mapSSTptBFwD.computeAverage(round);
		m_mapSSTptBFptD.computeAverage(round);
		// st2 + bf + st2 left/right head/pred
		m_mapSSTwBFwSSTRPi.computeAverage(round);
		m_mapSSTwBFwBFLPi.computeAverage(round);

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

		// bf + bf2
		m_mapBFwBF2w.computeAverage(round);
		m_mapBFwBF2pt.computeAverage(round);
		m_mapBFptBF2w.computeAverage(round);
		m_mapBFptBF2pt.computeAverage(round);
		m_mapBFwptBF2w.computeAverage(round);
		m_mapBFwBF2wpt.computeAverage(round);
		m_mapBFwptBF2pt.computeAverage(round);
		m_mapBFptBF2wpt.computeAverage(round);
		m_mapBFwptBF2wpt.computeAverage(round);

		// bf + sbf
		m_mapBFwSBFw.computeAverage(round);
		m_mapBFwSBFpt.computeAverage(round);
		m_mapBFptSBFw.computeAverage(round);
		m_mapBFptSBFpt.computeAverage(round);
		m_mapBFwptSBFw.computeAverage(round);
		m_mapBFwSBFwpt.computeAverage(round);
		m_mapBFwptSBFpt.computeAverage(round);
		m_mapBFptSBFwpt.computeAverage(round);
		m_mapBFwptSBFwpt.computeAverage(round);

		// tri-gram
		// st + bf + st2
		m_mapSTwBFptST2pt.computeAverage(round);
		m_mapSTptBFwST2pt.computeAverage(round);
		m_mapSTptBFptST2w.computeAverage(round);
		m_mapSTptBFptST2pt.computeAverage(round);
		// st + bf + sst
		m_mapSTwBFptSSTpt.computeAverage(round);
		m_mapSTptBFwSSTpt.computeAverage(round);
		m_mapSTptBFptSSTw.computeAverage(round);
		m_mapSTptBFptSSTpt.computeAverage(round);
		// st + bf + bf2
		m_mapSTwBFptBF2pt.computeAverage(round);
		m_mapSTptBFwBF2pt.computeAverage(round);
		m_mapSTptBFptBF2w.computeAverage(round);
		m_mapSTptBFptBF2pt.computeAverage(round);
		// st + bf + sbf
		m_mapSTwBFptSBFpt.computeAverage(round);
		m_mapSTptBFwSBFpt.computeAverage(round);
		m_mapSTptBFptSBFw.computeAverage(round);
		m_mapSTptBFptSBFpt.computeAverage(round);

		// st + bf + st left/right head/pred
		m_mapSTwBFptSTLHptSTLHl.computeAverage(round);
		m_mapSTptBFwSTLHptSTLHl.computeAverage(round);
		m_mapSTptBFptSTLHwSTLHl.computeAverage(round);
		m_mapSTptBFptSTLHptSTLHl.computeAverage(round);
		m_mapSTwBFptSTLPptSTLPl.computeAverage(round);
		m_mapSTptBFwSTLPptSTLPl.computeAverage(round);
		m_mapSTptBFptSTLPwSTLPl.computeAverage(round);
		m_mapSTptBFptSTLPptSTLPl.computeAverage(round);
		m_mapSTwBFptSTRHptSTRHl.computeAverage(round);
		m_mapSTptBFwSTRHptSTRHl.computeAverage(round);
		m_mapSTptBFptSTRHwSTRHl.computeAverage(round);
		m_mapSTptBFptSTRHptSTRHl.computeAverage(round);
		m_mapSTwBFptSTRPptSTRPl.computeAverage(round);
		m_mapSTptBFwSTRPptSTRPl.computeAverage(round);
		m_mapSTptBFptSTRPwSTRPl.computeAverage(round);
		m_mapSTptBFptSTRPptSTRPl.computeAverage(round);

		// st + bf + bf left/right head/pred
		m_mapSTwBFptBFLHptBFLHl.computeAverage(round);
		m_mapSTptBFwBFLHptBFLHl.computeAverage(round);
		m_mapSTptBFptBFLHwBFLHl.computeAverage(round);
		m_mapSTptBFptBFLHptBFLHl.computeAverage(round);
		m_mapSTwBFptBFLPptBFLPl.computeAverage(round);
		m_mapSTptBFwBFLPptBFLPl.computeAverage(round);
		m_mapSTptBFptBFLPwBFLPl.computeAverage(round);
		m_mapSTptBFptBFLPptBFLPl.computeAverage(round);
		m_mapSTwBFptBFRHptBFRHl.computeAverage(round);
		m_mapSTptBFwBFRHptBFRHl.computeAverage(round);
		m_mapSTptBFptBFRHwBFRHl.computeAverage(round);
		m_mapSTptBFptBFRHptBFRHl.computeAverage(round);
		m_mapSTwBFptBFRPptBFRPl.computeAverage(round);
		m_mapSTptBFwBFRPptBFRPl.computeAverage(round);
		m_mapSTptBFptBFRPwBFRPl.computeAverage(round);
		m_mapSTptBFptBFRPptBFRPl.computeAverage(round);

		// quar-gram
		// st + bf + st right head + st right pred
		m_mapSTptBFptSTRHptSTRPptSTRHlSTRPl.computeAverage(round);
		// st + bf + st left pred + st left pred 2
		m_mapSTptBFptSTLPptSTLP2ptSTLPlSTLP2l.computeAverage(round);
		// st + bf + st right pred + st right pred 2
		m_mapSTptBFptSTRPptSTRP2ptSTRPlSTRP2l.computeAverage(round);
		// st + bf + bf left head + bf left pred
		m_mapSTptBFptBFLHptBFLPptBFLHlBFLPl.computeAverage(round);
		// st + bf + bf left pred + bf left pred 2
		m_mapSTptBFptBFLPptBFLP2ptBFLPlBFLP2l.computeAverage(round);
		// st + bf + bf right pred + bf right pred 2
		m_mapSTptBFptBFRPptBFRP2ptBFRPlBFRP2l.computeAverage(round);

		// st + bf2 + st left/right head/pred
		m_mapSTwBF2ptSTLHptSTLHl.computeAverage(round);
		m_mapSTptBF2wSTLHptSTLHl.computeAverage(round);
		m_mapSTptBF2ptSTLHwSTLHl.computeAverage(round);
		m_mapSTptBF2ptSTLHptSTLHl.computeAverage(round);
		m_mapSTwBF2ptSTLPptSTLPl.computeAverage(round);
		m_mapSTptBF2wSTLPptSTLPl.computeAverage(round);
		m_mapSTptBF2ptSTLPwSTLPl.computeAverage(round);
		m_mapSTptBF2ptSTLPptSTLPl.computeAverage(round);
		m_mapSTwBF2ptSTRHptSTRHl.computeAverage(round);
		m_mapSTptBF2wSTRHptSTRHl.computeAverage(round);
		m_mapSTptBF2ptSTRHwSTRHl.computeAverage(round);
		m_mapSTptBF2ptSTRHptSTRHl.computeAverage(round);
		m_mapSTwBF2ptSTRPptSTRPl.computeAverage(round);
		m_mapSTptBF2wSTRPptSTRPl.computeAverage(round);
		m_mapSTptBF2ptSTRPwSTRPl.computeAverage(round);
		m_mapSTptBF2ptSTRPptSTRPl.computeAverage(round);

		// st + bf2 + bf2 left/right head/pred
		m_mapSTwBF2ptBF2LHptBF2LHl.computeAverage(round);
		m_mapSTptBF2wBF2LHptBF2LHl.computeAverage(round);
		m_mapSTptBF2ptBF2LHwBF2LHl.computeAverage(round);
		m_mapSTptBF2ptBF2LHptBF2LHl.computeAverage(round);
		m_mapSTwBF2ptBF2LPptBF2LPl.computeAverage(round);
		m_mapSTptBF2wBF2LPptBF2LPl.computeAverage(round);
		m_mapSTptBF2ptBF2LPwBF2LPl.computeAverage(round);
		m_mapSTptBF2ptBF2LPptBF2LPl.computeAverage(round);
		m_mapSTwBF2ptBF2RHptBF2RHl.computeAverage(round);
		m_mapSTptBF2wBF2RHptBF2RHl.computeAverage(round);
		m_mapSTptBF2ptBF2RHwBF2RHl.computeAverage(round);
		m_mapSTptBF2ptBF2RHptBF2RHl.computeAverage(round);
		m_mapSTwBF2ptBF2RPptBF2RPl.computeAverage(round);
		m_mapSTptBF2wBF2RPptBF2RPl.computeAverage(round);
		m_mapSTptBF2ptBF2RPwBF2RPl.computeAverage(round);
		m_mapSTptBF2ptBF2RPptBF2RPl.computeAverage(round);

		// quar-gram
		// st + bf2 + st right head + st right pred
		m_mapSTptBF2ptSTRHptSTRPptSTRHlSTRPl.computeAverage(round);
		// st + bf2 + st left pred + st left pred 2
		m_mapSTptBF2ptSTLPptSTLP2ptSTLPlSTLP2l.computeAverage(round);
		// st + bf2 + st right pred + st right pred 2
		m_mapSTptBF2ptSTRPptSTRP2ptSTRPlSTRP2l.computeAverage(round);
		// st + bf2 + bf2 left head + bf2 left pred
		m_mapSTptBF2ptBF2LHptBF2LPptBF2LHlBF2LPl.computeAverage(round);
		// st + bf2 + bf2 left pred + bf2 left pred 2
		m_mapSTptBF2ptBF2LPptBF2LP2ptBF2LPlBF2LP2l.computeAverage(round);
		// st + bf2 + bf2 right pred + bf2 right pred 2
		m_mapSTptBF2ptBF2RPptBF2RP2ptBF2RPlBF2RP2l.computeAverage(round);

		// st + sbf + st left/right head/pred
		m_mapSTwSBFptSTLHptSTLHl.computeAverage(round);
		m_mapSTptSBFwSTLHptSTLHl.computeAverage(round);
		m_mapSTptSBFptSTLHwSTLHl.computeAverage(round);
		m_mapSTptSBFptSTLHptSTLHl.computeAverage(round);
		m_mapSTwSBFptSTLPptSTLPl.computeAverage(round);
		m_mapSTptSBFwSTLPptSTLPl.computeAverage(round);
		m_mapSTptSBFptSTLPwSTLPl.computeAverage(round);
		m_mapSTptSBFptSTLPptSTLPl.computeAverage(round);
		m_mapSTwSBFptSTRHptSTRHl.computeAverage(round);
		m_mapSTptSBFwSTRHptSTRHl.computeAverage(round);
		m_mapSTptSBFptSTRHwSTRHl.computeAverage(round);
		m_mapSTptSBFptSTRHptSTRHl.computeAverage(round);
		m_mapSTwSBFptSTRPptSTRPl.computeAverage(round);
		m_mapSTptSBFwSTRPptSTRPl.computeAverage(round);
		m_mapSTptSBFptSTRPwSTRPl.computeAverage(round);
		m_mapSTptSBFptSTRPptSTRPl.computeAverage(round);

		// st + sbf + sbf left/right head/pred
		m_mapSTwSBFptSBFLHptSBFLHl.computeAverage(round);
		m_mapSTptSBFwSBFLHptSBFLHl.computeAverage(round);
		m_mapSTptSBFptSBFLHwSBFLHl.computeAverage(round);
		m_mapSTptSBFptSBFLHptSBFLHl.computeAverage(round);
		m_mapSTwSBFptSBFLPptSBFLPl.computeAverage(round);
		m_mapSTptSBFwSBFLPptSBFLPl.computeAverage(round);
		m_mapSTptSBFptSBFLPwSBFLPl.computeAverage(round);
		m_mapSTptSBFptSBFLPptSBFLPl.computeAverage(round);
		m_mapSTwSBFptSBFRHptSBFRHl.computeAverage(round);
		m_mapSTptSBFwSBFRHptSBFRHl.computeAverage(round);
		m_mapSTptSBFptSBFRHwSBFRHl.computeAverage(round);
		m_mapSTptSBFptSBFRHptSBFRHl.computeAverage(round);
		m_mapSTwSBFptSBFRPptSBFRPl.computeAverage(round);
		m_mapSTptSBFwSBFRPptSBFRPl.computeAverage(round);
		m_mapSTptSBFptSBFRPwSBFRPl.computeAverage(round);
		m_mapSTptSBFptSBFRPptSBFRPl.computeAverage(round);

		// quar-gram
		// st + sbf + st right head + st right pred
		m_mapSTptSBFptSTRHptSTRPptSTRHlSTRPl.computeAverage(round);
		// st + sbf + st left pred + st left pred 2
		m_mapSTptSBFptSTLPptSTLP2ptSTLPlSTLP2l.computeAverage(round);
		// st + sbf + st right pred + st right pred 2
		m_mapSTptSBFptSTRPptSTRP2ptSTRPlSTRP2l.computeAverage(round);
		// st + sbf + sbf left head + sbf left pred
		m_mapSTptSBFptSBFLHptSBFLPptSBFLHlSBFLPl.computeAverage(round);
		// st + sbf + sbf left pred + sbf left pred 2
		m_mapSTptSBFptSBFLPptSBFLP2ptSBFLPlSBFLP2l.computeAverage(round);
		// st + sbf + sbf right pred + sbf right pred 2
		m_mapSTptSBFptSBFRPptSBFRP2ptSBFRPlSBFRP2l.computeAverage(round);

		// st2 + bf + st2 left/right head/pred
		m_mapST2wBFptST2LHptST2LHl.computeAverage(round);
		m_mapST2ptBFwST2LHptST2LHl.computeAverage(round);
		m_mapST2ptBFptST2LHwST2LHl.computeAverage(round);
		m_mapST2ptBFptST2LHptST2LHl.computeAverage(round);
		m_mapST2wBFptST2LPptST2LPl.computeAverage(round);
		m_mapST2ptBFwST2LPptST2LPl.computeAverage(round);
		m_mapST2ptBFptST2LPwST2LPl.computeAverage(round);
		m_mapST2ptBFptST2LPptST2LPl.computeAverage(round);
		m_mapST2wBFptST2RHptST2RHl.computeAverage(round);
		m_mapST2ptBFwST2RHptST2RHl.computeAverage(round);
		m_mapST2ptBFptST2RHwST2RHl.computeAverage(round);
		m_mapST2ptBFptST2RHptST2RHl.computeAverage(round);
		m_mapST2wBFptST2RPptST2RPl.computeAverage(round);
		m_mapST2ptBFwST2RPptST2RPl.computeAverage(round);
		m_mapST2ptBFptST2RPwST2RPl.computeAverage(round);
		m_mapST2ptBFptST2RPptST2RPl.computeAverage(round);

		// st2 + bf + bf left/right head/pred
		m_mapST2wBFptBFLHptBFLHl.computeAverage(round);
		m_mapST2ptBFwBFLHptBFLHl.computeAverage(round);
		m_mapST2ptBFptBFLHwBFLHl.computeAverage(round);
		m_mapST2ptBFptBFLHptBFLHl.computeAverage(round);
		m_mapST2wBFptBFLPptBFLPl.computeAverage(round);
		m_mapST2ptBFwBFLPptBFLPl.computeAverage(round);
		m_mapST2ptBFptBFLPwBFLPl.computeAverage(round);
		m_mapST2ptBFptBFLPptBFLPl.computeAverage(round);
		m_mapST2wBFptBFRHptBFRHl.computeAverage(round);
		m_mapST2ptBFwBFRHptBFRHl.computeAverage(round);
		m_mapST2ptBFptBFRHwBFRHl.computeAverage(round);
		m_mapST2ptBFptBFRHptBFRHl.computeAverage(round);
		m_mapST2wBFptBFRPptBFRPl.computeAverage(round);
		m_mapST2ptBFwBFRPptBFRPl.computeAverage(round);
		m_mapST2ptBFptBFRPwBFRPl.computeAverage(round);
		m_mapST2ptBFptBFRPptBFRPl.computeAverage(round);

		// quar-gram
		// st2 + bf + st2 right head + st2 right pred
		m_mapST2ptBFptST2RHptST2RPptST2RHlST2RPl.computeAverage(round);
		// st2 + bf + st2 left pred + st2 left pred 2
		m_mapST2ptBFptST2LPptST2LP2ptST2LPlST2LP2l.computeAverage(round);
		// st2 + bf + st2 right pred + st2 right pred 2
		m_mapST2ptBFptST2RPptST2RP2ptST2RPlST2RP2l.computeAverage(round);
		// st2 + bf + bf left head + bf left pred
		m_mapST2ptBFptBFLHptBFLPptBFLHlBFLPl.computeAverage(round);
		// st2 + bf + bf left pred + bf left pred 2
		m_mapST2ptBFptBFLPptBFLP2ptBFLPlBFLP2l.computeAverage(round);
		// st2 + bf + bf right pred + bf right pred 2
		m_mapST2ptBFptBFRPptBFRP2ptBFRPlBFRP2l.computeAverage(round);

		// sst + bf + sst left/right head/pred
		m_mapSSTwBFptSSTLHptSSTLHl.computeAverage(round);
		m_mapSSTptBFwSSTLHptSSTLHl.computeAverage(round);
		m_mapSSTptBFptSSTLHwSSTLHl.computeAverage(round);
		m_mapSSTptBFptSSTLHptSSTLHl.computeAverage(round);
		m_mapSSTwBFptSSTLPptSSTLPl.computeAverage(round);
		m_mapSSTptBFwSSTLPptSSTLPl.computeAverage(round);
		m_mapSSTptBFptSSTLPwSSTLPl.computeAverage(round);
		m_mapSSTptBFptSSTLPptSSTLPl.computeAverage(round);
		m_mapSSTwBFptSSTRHptSSTRHl.computeAverage(round);
		m_mapSSTptBFwSSTRHptSSTRHl.computeAverage(round);
		m_mapSSTptBFptSSTRHwSSTRHl.computeAverage(round);
		m_mapSSTptBFptSSTRHptSSTRHl.computeAverage(round);
		m_mapSSTwBFptSSTRPptSSTRPl.computeAverage(round);
		m_mapSSTptBFwSSTRPptSSTRPl.computeAverage(round);
		m_mapSSTptBFptSSTRPwSSTRPl.computeAverage(round);
		m_mapSSTptBFptSSTRPptSSTRPl.computeAverage(round);

		// st + bf + bf left/right head/pred
		m_mapSSTwBFptBFLHptBFLHl.computeAverage(round);
		m_mapSSTptBFwBFLHptBFLHl.computeAverage(round);
		m_mapSSTptBFptBFLHwBFLHl.computeAverage(round);
		m_mapSSTptBFptBFLHptBFLHl.computeAverage(round);
		m_mapSSTwBFptBFLPptBFLPl.computeAverage(round);
		m_mapSSTptBFwBFLPptBFLPl.computeAverage(round);
		m_mapSSTptBFptBFLPwBFLPl.computeAverage(round);
		m_mapSSTptBFptBFLPptBFLPl.computeAverage(round);
		m_mapSSTwBFptBFRHptBFRHl.computeAverage(round);
		m_mapSSTptBFwBFRHptBFRHl.computeAverage(round);
		m_mapSSTptBFptBFRHwBFRHl.computeAverage(round);
		m_mapSSTptBFptBFRHptBFRHl.computeAverage(round);
		m_mapSSTwBFptBFRPptBFRPl.computeAverage(round);
		m_mapSSTptBFwBFRPptBFRPl.computeAverage(round);
		m_mapSSTptBFptBFRPwBFRPl.computeAverage(round);
		m_mapSSTptBFptBFRPptBFRPl.computeAverage(round);

		// quar-gram
		// sst + bf + sst right head + sst right pred
		m_mapSSTptBFptSSTRHptSSTRPptSSTRHlSSTRPl.computeAverage(round);
		// sst + bf + sst left pred + sst left pred 2
		m_mapSSTptBFptSSTLPptSSTLP2ptSSTLPlSSTLP2l.computeAverage(round);
		// sst + bf + sst right pred + sst right pred 2
		m_mapSSTptBFptSSTRPptSSTRP2ptSSTRPlSSTRP2l.computeAverage(round);
		// sst + bf + bf left head + bf left pred
		m_mapSSTptBFptBFLHptBFLPptBFLHlBFLPl.computeAverage(round);
		// sst + bf + bf left pred + bf left pred 2
		m_mapSSTptBFptBFLPptBFLP2ptBFLPlBFLP2l.computeAverage(round);
		// sst + bf + bf right pred + bf right pred 2
		m_mapSSTptBFptBFRPptBFRP2ptBFRPlBFRP2l.computeAverage(round);

		// st + bf + label set
		m_mapSTwSTll.computeAverage(round);
		m_mapSTwSTrl.computeAverage(round);
		m_mapBFwBFll.computeAverage(round);
		m_mapBFwBFrl.computeAverage(round);
		m_mapSTptSTll.computeAverage(round);
		m_mapSTptSTrl.computeAverage(round);
		m_mapBFptBFll.computeAverage(round);
		m_mapBFptBFrl.computeAverage(round);
		m_mapSTptBFptSTll.computeAverage(round);
		m_mapSTptBFptSTrl.computeAverage(round);
		m_mapSTptBFptBFll.computeAverage(round);
		m_mapSTptBFptBFrl.computeAverage(round);
		m_mapSTptBF2ptSTll.computeAverage(round);
		m_mapSTptBF2ptSTrl.computeAverage(round);
		m_mapSTptBF2ptBF2ll.computeAverage(round);
		m_mapSTptBF2ptBF2rl.computeAverage(round);
		m_mapSTptSBFptSTll.computeAverage(round);
		m_mapSTptSBFptSTrl.computeAverage(round);
		m_mapSTptSBFptSBFll.computeAverage(round);
		m_mapSTptSBFptSBFrl.computeAverage(round);
		// st2 + bf + label set
		m_mapST2wST2ll.computeAverage(round);
		m_mapST2wST2rl.computeAverage(round);
		m_mapBF2wBF2ll.computeAverage(round);
		m_mapBF2wBF2rl.computeAverage(round);
		m_mapST2ptST2ll.computeAverage(round);
		m_mapST2ptST2rl.computeAverage(round);
		m_mapBF2ptBF2ll.computeAverage(round);
		m_mapBF2ptBF2rl.computeAverage(round);
		m_mapST2ptBFptST2ll.computeAverage(round);
		m_mapST2ptBFptST2rl.computeAverage(round);
		m_mapST2ptBFptBFll.computeAverage(round);
		m_mapST2ptBFptBFrl.computeAverage(round);
		// sst + bf + label set
		m_mapSSTwSSTll.computeAverage(round);
		m_mapSSTwSSTrl.computeAverage(round);
		m_mapSBFwSBFll.computeAverage(round);
		m_mapSBFwSBFrl.computeAverage(round);
		m_mapSSTptSSTll.computeAverage(round);
		m_mapSSTptSSTrl.computeAverage(round);
		m_mapSBFptSBFll.computeAverage(round);
		m_mapSBFptSBFrl.computeAverage(round);
		m_mapSSTptBFptSSTll.computeAverage(round);
		m_mapSSTptBFptSSTrl.computeAverage(round);
		m_mapSSTptBFptBFll.computeAverage(round);
		m_mapSSTptBFptBFrl.computeAverage(round);

		// char feature (for chinese)
		// st context char
		m_map1CharBeforeST.computeAverage(round);
		m_map2CharBeforeST.computeAverage(round);
		m_map3CharBeforeST.computeAverage(round);
		m_map1CharAfterST.computeAverage(round);
		m_map2CharAfterST.computeAverage(round);
		m_map3CharAfterST.computeAverage(round);
		// bf context char
		m_map1CharBeforeBF.computeAverage(round);
		m_map2CharBeforeBF.computeAverage(round);
		m_map3CharBeforeBF.computeAverage(round);
		m_map1CharAfterBF.computeAverage(round);
		m_map2CharAfterBF.computeAverage(round);
		m_map3CharAfterBF.computeAverage(round);

		// path feature
		// st - bf
		m_mapSTPOSPath.computeAverage(round);
		m_mapSTFPOSPath.computeAverage(round);
		m_mapSTwBFptSTsynhw.computeAverage(round);
		m_mapSTwBFwSTsynhpt.computeAverage(round);
		m_mapSTptBFwSTsynhw.computeAverage(round);
		m_mapSTwBFptBFsynhw.computeAverage(round);
		m_mapSTwBFwBFsynhpt.computeAverage(round);
		m_mapSTptBFwBFsynhw.computeAverage(round);
		// st - bf2
		m_mapBF2POSPath.computeAverage(round);
		m_mapBF2FPOSPath.computeAverage(round);
		m_mapSTwBF2ptSTsynhw.computeAverage(round);
		m_mapSTwBF2wSTsynhpt.computeAverage(round);
		m_mapSTptBF2wSTsynhw.computeAverage(round);
		m_mapSTwBF2ptBF2synhw.computeAverage(round);
		m_mapSTwBF2wBF2synhpt.computeAverage(round);
		m_mapSTptBF2wBF2synhw.computeAverage(round);
		// st - sbf
		m_mapSBFPOSPath.computeAverage(round);
		m_mapSBFFPOSPath.computeAverage(round);
		m_mapSTwSBFptSTsynhw.computeAverage(round);
		m_mapSTwSBFwSTsynhpt.computeAverage(round);
		m_mapSTptSBFwSTsynhw.computeAverage(round);
		m_mapSTwSBFptSBFsynhw.computeAverage(round);
		m_mapSTwSBFwSBFsynhpt.computeAverage(round);
		m_mapSTptSBFwSBFsynhw.computeAverage(round);
		// st2 - bf
		m_mapST2POSPath.computeAverage(round);
		m_mapST2FPOSPath.computeAverage(round);
		m_mapST2wBFptST2synhw.computeAverage(round);
		m_mapST2wBFwST2synhpt.computeAverage(round);
		m_mapST2ptBFwST2synhw.computeAverage(round);
		m_mapST2wBFptBFsynhw.computeAverage(round);
		m_mapST2wBFwBFsynhpt.computeAverage(round);
		m_mapST2ptBFwBFsynhw.computeAverage(round);
		// sst - bf
		m_mapSSTPOSPath.computeAverage(round);
		m_mapSSTFPOSPath.computeAverage(round);
		m_mapSSTwBFptSSTsynhw.computeAverage(round);
		m_mapSSTwBFwSSTsynhpt.computeAverage(round);
		m_mapSSTptBFwSSTsynhw.computeAverage(round);
		m_mapSSTwBFptBFsynhw.computeAverage(round);
		m_mapSSTwBFwBFsynhpt.computeAverage(round);
		m_mapSSTptBFwBFsynhw.computeAverage(round);

		// supertag feature
		m_mapSTst.computeAverage(round);
		m_mapST2st.computeAverage(round);
		m_mapSSTst.computeAverage(round);
		m_mapBFist.computeAverage(round);
		m_mapSTstST2st.computeAverage(round);
		m_mapSTstSSTst.computeAverage(round);
		m_mapSTstBFw.computeAverage(round);
		m_mapSTstBFpt.computeAverage(round);
		m_mapSTstBFwpt.computeAverage(round);
	}
}

#endif
