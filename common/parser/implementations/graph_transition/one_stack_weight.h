#ifndef _ONE_STACK_WEIGHT_H
#define _ONE_STACK_WEIGHT_H

#include "graph_transition_weight.h"
#include "graph_transition_depparser.h"
#include "include/learning/perceptron/packed_score.h"

namespace graph_transition {

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	class OneStackWeight : public GraphWeightBase {
	public:

		static WordPOSTag empty_taggedword;
		static WordPOSTag start_taggedword;
		static WordPOSTag end_taggedword;
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
		OneStackWeight(const std::string & sRead, const std::string & sRecord,
				DWord * pWords, DPOSTag * pPOSTags, DLabel * pLables, DSuperTag * pSuperTags,
				SuperTagCandidates * wordsCandidates, SuperTagCandidates * postagsCandidates);
		~OneStackWeight();

		void loadScores() override;
		void saveScores() const override;
		void computeAverageFeatureWeights(const int & round) override;

		void getOrUpdateFeatureScores(GraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE> * parser, const STATE_TYPE & state, const ActionScoreIncrement & amount);
	};


	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::empty_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::start_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::end_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> TagSet OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::empty_tagset = TagSet();

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::OneStackWeight(const std::string & sRead, const std::string & sRecord,
			DWord * pWords, DPOSTag * pPOSTags, DLabel * pLables, DSuperTag * pSuperTags,
			SuperTagCandidates * wordsCandidates, SuperTagCandidates * postagsCandidates) :
		GraphWeightBase(sRead, sRecord, pWords, pPOSTags, pLables, pSuperTags, wordsCandidates, postagsCandidates),
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

		empty_taggedword.refer(m_pWords->lookup(EMPTY_WORD), m_pPOSTags->lookup(EMPTY_POSTAG));
		start_taggedword.refer(m_pWords->lookup(START_WORD), m_pPOSTags->lookup(START_POSTAG));
		end_taggedword.refer(m_pWords->lookup(END_WORD), m_pPOSTags->lookup(END_POSTAG));

		loadScores();
		std::cout << "load complete." << std::endl;
	}

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::~OneStackWeight() = default;

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::loadScores() {

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

		input >> *m_pSuperTagCandidatesOfWords;

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

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::saveScores() const {

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

		output << *m_pSuperTagCandidatesOfWords;

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

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::computeAverageFeatureWeights(const int & round) {

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

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void OneStackWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::getOrUpdateFeatureScores(GraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE> * parser, const STATE_TYPE & item, const ActionScoreIncrement & amount) {
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

		}

		if (parser->m_bSuperTag) {
			const SuperTag & st_supertag = st_index == -1 ? emptySuperTag : item.superTag(st_index);
			const SuperTag & st2_supertag = st2_index == -1 ? emptySuperTag : item.superTag(st2_index);
			const SuperTag & n0l2_supertag = n0l2_index == -1 ? emptySuperTag : item.superTag(n0l2_index);
			const SuperTag & n0l1_supertag = n0l1_index == -1 ? emptySuperTag : item.superTag(n0l1_index);

			m_mapSTst.getOrUpdateScore(packedScore, st_supertag, scoreIndex, amount, trainingRound);
			m_mapST2st.getOrUpdateScore(packedScore, st2_supertag, scoreIndex, amount, trainingRound);
			bi_features.refer(n0l2_supertag, -2);
			m_mapN0ist.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(n0l1_supertag, -1);
			m_mapN0ist.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_supertag, st2_supertag);
			m_mapSTstST2st.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
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
