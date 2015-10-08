#ifndef _STD_TITOV_WEIGHT_H
#define _STD_TITOV_WEIGHT_H

#include "std_titov_macros.h"
#include "common/parser/weight_base.h"
#include "include/learning/perceptron/packed_score.h"

#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

extern SuperTagCandidates g_mapSuperTagCandidatesOfWords;
extern SuperTagCandidates g_mapSuperTagCandidatesOfPOSTags;

namespace std_titov {
	template<class RET_TYPE>
	class Weight : public WeightBase {
	public:
		// uni-gram
		// st, n0, st2
		WordMap<RET_TYPE> m_mapSTw;
		POSTagMap<RET_TYPE> m_mapSTpt;
		WordMap<RET_TYPE> m_mapST2w;
		POSTagMap<RET_TYPE> m_mapST2pt;
		WordMap<RET_TYPE> m_mapN0w;
		POSTagMap<RET_TYPE> m_mapN0pt;

		// unigram context
		// st + sti, n0 + n0i
		WordIntMap<RET_TYPE> m_mapSTiw;
		TwoWordsIntMap<RET_TYPE> m_mapSTiwSTjw;
		WordIntMap<RET_TYPE> m_mapN0iw;
		TwoWordsIntMap<RET_TYPE> m_mapN0iwN0jw;
		POSTagIntMap<RET_TYPE> m_mapSTipt;
		POSTagSet2IntMap<RET_TYPE> m_mapSTiptSTjpt;
		POSTagIntMap<RET_TYPE> m_mapN0ipt;
		POSTagSet2IntMap<RET_TYPE> m_mapN0iptN0jpt;

		// unigram with label
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

		// unigram with arity
		// st, n0, st2
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

		// tri-gram
		// st + n0 + st2
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwN0ptST2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptN0wST2pt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTptN0ptST2w;
		POSTagSet3Map<RET_TYPE> m_mapSTptN0ptST2pt;

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

		// supertag feature
		SuperTagMap<RET_TYPE> m_mapSTst;
		SuperTagMap<RET_TYPE> m_mapST2st;
		SuperTagIntMap<RET_TYPE> m_mapN0ist;
		SuperTagSet2Map<RET_TYPE> m_mapSTstST2st;
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
		// st, n0, st2
		m_mapSTw("m_mapSTw"),
		m_mapSTpt("m_mapSTpt"),
		m_mapST2w("m_mapST2w"),
		m_mapST2pt("m_mapST2pt"),
		m_mapN0w("m_mapN0w"),
		m_mapN0pt("m_mapN0pt"),

		// unigram context
		// st + sti, n0 + n0i
		m_mapSTiw("m_mapSTiw"),
		m_mapSTiwSTjw("m_mapSTiwSTjw"),
		m_mapN0iw("m_mapN0iw"),
		m_mapN0iwN0jw("m_mapN0iwN0jw"),
		m_mapSTipt("m_mapSTipt"),
		m_mapSTiptSTjpt("m_mapSTiptSTjpt"),
		m_mapN0ipt("m_mapN0ipt"),
		m_mapN0iptN0jpt("m_mapN0iptN0jpt"),

		// unigram with label
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

		// unigram with arity
		// st, n0, st2
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

		// tri-gram
		// st + n0 + st2
		m_mapSTwN0ptST2pt("m_mapSTwN0ptST2pt"),
		m_mapSTptN0wST2pt("m_mapSTptN0wST2pt"),
		m_mapSTptN0ptST2w("m_mapSTptN0ptST2w"),
		m_mapSTptN0ptST2pt("m_mapSTptN0ptST2pt"),

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

		// supertag feature
		m_mapSTst("m_mapSTst"),
		m_mapST2st("m_mapST2st"),
		m_mapN0ist("m_mapN0ist"),
		m_mapSTstST2st("m_mapSTstST2st"),
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
		// st, n0, st2
		input >> m_mapSTw;
		input >> m_mapSTpt;
		input >> m_mapST2w;
		input >> m_mapST2pt;
		input >> m_mapN0w;
		input >> m_mapN0pt;

		// unigram context
		// st + sti, n0 + n0i
		input >> m_mapSTiw;
		input >> m_mapSTiwSTjw;
		input >> m_mapN0iw;
		input >> m_mapN0iwN0jw;
		input >> m_mapSTipt;
		input >> m_mapSTiptSTjpt;
		input >> m_mapN0ipt;
		input >> m_mapN0iptN0jpt;

		// unigram with label
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

		// unigram with arity
		// st, n0, st2
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

		// tri-gram
		// st + n0 + st2
		input >> m_mapSTwN0ptST2pt;
		input >> m_mapSTptN0wST2pt;
		input >> m_mapSTptN0ptST2w;
		input >> m_mapSTptN0ptST2pt;

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

		// supertag feature
		input >> m_mapSTst;
		input >> m_mapST2st;
		input >> m_mapN0ist;
		input >> m_mapSTstST2st;
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
		// st, n0, st2
		output << m_mapSTw;
		output << m_mapSTpt;
		output << m_mapST2w;
		output << m_mapST2pt;
		output << m_mapN0w;
		output << m_mapN0pt;

		// unigram context
		// st + sti, n0 + n0i
		output << m_mapSTiw;
		output << m_mapSTiwSTjw;
		output << m_mapN0iw;
		output << m_mapN0iwN0jw;
		output << m_mapSTipt;
		output << m_mapSTiptSTjpt;
		output << m_mapN0ipt;
		output << m_mapN0iptN0jpt;

		// unigram with label
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

		// unigram with arity
		// st, n0, st2
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

		// tri-gram
		// st + n0 + st2
		output << m_mapSTwN0ptST2pt;
		output << m_mapSTptN0wST2pt;
		output << m_mapSTptN0ptST2w;
		output << m_mapSTptN0ptST2pt;

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

		// supertag feature
		output << m_mapSTst;
		output << m_mapST2st;
		output << m_mapN0ist;
		output << m_mapSTstST2st;
		output << m_mapSTstN0w;
		output << m_mapSTstN0pt;
		output << m_mapSTstN0wpt;

		output.close();
	}

	template<class RET_TYPE>
	void Weight<RET_TYPE>::computeAverageFeatureWeights(const int & round) {

		// uni-gram
		// st, n0, st2
		m_mapSTw.computeAverage(round);
		m_mapSTpt.computeAverage(round);
		m_mapST2w.computeAverage(round);
		m_mapST2pt.computeAverage(round);
		m_mapN0w.computeAverage(round);
		m_mapN0pt.computeAverage(round);

		// unigram context
		// st + sti, n0 + n0i
		m_mapSTiw.computeAverage(round);
		m_mapSTiwSTjw.computeAverage(round);
		m_mapN0iw.computeAverage(round);
		m_mapN0iwN0jw.computeAverage(round);
		m_mapSTipt.computeAverage(round);
		m_mapSTiptSTjpt.computeAverage(round);
		m_mapN0ipt.computeAverage(round);
		m_mapN0iptN0jpt.computeAverage(round);

		// unigram with label
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

		// unigram with arity
		// st, n0, st2
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

		// tri-gram
		// st + n0 + st2
		m_mapSTwN0ptST2pt.computeAverage(round);
		m_mapSTptN0wST2pt.computeAverage(round);
		m_mapSTptN0ptST2w.computeAverage(round);
		m_mapSTptN0ptST2pt.computeAverage(round);

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

		// supertag feature
		m_mapSTst.computeAverage(round);
		m_mapST2st.computeAverage(round);
		m_mapN0ist.computeAverage(round);
		m_mapSTstST2st.computeAverage(round);
		m_mapSTstN0w.computeAverage(round);
		m_mapSTstN0pt.computeAverage(round);
		m_mapSTstN0wpt.computeAverage(round);
	}
}

#endif
