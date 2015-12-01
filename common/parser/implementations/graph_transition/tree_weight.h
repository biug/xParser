#ifndef _TREE_WEIGHT_H
#define _TREE_WEIGHT_H

#include "graph_transition_weight.h"
#include "graph_transition_depparser.h"
#include "include/learning/perceptron/packed_score.h"

namespace graph_transition {

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	class TreeWeight : public GraphWeightBase {
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

		WordMap<RET_TYPE> m_mapSTw;
		POSTagMap<RET_TYPE> m_mapSTt;
		WordPOSTagMap<RET_TYPE> m_mapSTwt;

		WordMap<RET_TYPE> m_mapN0w;
		POSTagMap<RET_TYPE> m_mapN0t;
		WordPOSTagMap<RET_TYPE> m_mapN0wt;

		WordMap<RET_TYPE> m_mapS1w;
		POSTagMap<RET_TYPE> m_mapS1t;
		WordPOSTagMap<RET_TYPE> m_mapS1wt;

		WordMap<RET_TYPE> m_mapS2w;
		POSTagMap<RET_TYPE> m_mapS2t;
		WordPOSTagMap<RET_TYPE> m_mapS2wt;

		WordMap<RET_TYPE> m_mapN1w;
		POSTagMap<RET_TYPE> m_mapN1t;
		WordPOSTagMap<RET_TYPE> m_mapN1wt;

		WordMap<RET_TYPE> m_mapN2w;
		POSTagMap<RET_TYPE> m_mapN2t;
		WordPOSTagMap<RET_TYPE> m_mapN2wt;

		WordMap<RET_TYPE> m_mapSTHw;
		POSTagMap<RET_TYPE> m_mapSTHt;
		IntMap<RET_TYPE> m_mapSTi;

		WordMap<RET_TYPE> m_mapSTHHw;
		POSTagMap<RET_TYPE> m_mapSTHHt;
		IntMap<RET_TYPE> m_mapSTHi;

		WordMap<RET_TYPE> m_mapSTLDw;
		POSTagMap<RET_TYPE> m_mapSTLDt;
		IntMap<RET_TYPE> m_mapSTLDi;

		WordMap<RET_TYPE> m_mapSTRDw;
		POSTagMap<RET_TYPE> m_mapSTRDt;
		IntMap<RET_TYPE> m_mapSTRDi;

		WordMap<RET_TYPE> m_mapN0LDw;
		POSTagMap<RET_TYPE> m_mapN0LDt;
		IntMap<RET_TYPE> m_mapN0LDi;

		WordMap<RET_TYPE> m_mapSTL2Dw;
		POSTagMap<RET_TYPE> m_mapSTL2Dt;
		IntMap<RET_TYPE> m_mapSTL2Di;

		WordMap<RET_TYPE> m_mapSTR2Dw;
		POSTagMap<RET_TYPE> m_mapSTR2Dt;
		IntMap<RET_TYPE> m_mapSTR2Di;

		WordMap<RET_TYPE> m_mapN0L2Dw;
		POSTagMap<RET_TYPE> m_mapN0L2Dt;
		IntMap<RET_TYPE> m_mapN0L2Di;

		WordMap<RET_TYPE> m_mapHTw;
		POSTagMap<RET_TYPE> m_mapHTt;
		WordPOSTagMap<RET_TYPE> m_mapHTwt;

		WordWordPOSTagPOSTagMap<RET_TYPE> m_mapSTwtN0wt;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwtN0w;
		WordWordPOSTagMap<RET_TYPE> m_mapSTwN0wt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTtN0wt;
		WordPOSTagPOSTagMap<RET_TYPE> m_mapSTwtN0t;
		TwoWordsMap<RET_TYPE> m_mapSTwN0w;

		POSTagSet2Map<RET_TYPE> m_mapSTtN0t;
		POSTagSet2Map<RET_TYPE> m_mapN0tN1t;
		POSTagSet3Map<RET_TYPE> m_mapN0tN1tN2t;
		POSTagSet3Map<RET_TYPE> m_mapSTtN0tN1t;
		POSTagSet3Map<RET_TYPE> m_mapSTtN0tN0LDt;
		POSTagSet3Map<RET_TYPE> m_mapN0tN0LDtN0L2Dt;
		POSTagSet3Map<RET_TYPE> m_mapSTHtSTtN0t;
		POSTagSet3Map<RET_TYPE> m_mapHTtHT2tN0t;
		POSTagSet3Map<RET_TYPE> m_mapSTHHtSTHtSTt;
		POSTagSet3Map<RET_TYPE> m_mapSTtSTLDtN0t;
		POSTagSet3Map<RET_TYPE> m_mapSTtSTLDtSTL2Dt;
		POSTagSet3Map<RET_TYPE> m_mapSTtSTRDtN0t;
		POSTagSet3Map<RET_TYPE> m_mapSTtSTRDtSTR2Dt;

		WordIntMap<RET_TYPE> m_mapSTwd;
		POSTagIntMap<RET_TYPE> m_mapSTtd;
		WordIntMap<RET_TYPE> m_mapN0wd;
		POSTagIntMap<RET_TYPE> m_mapN0td;
		TwoWordsIntMap<RET_TYPE> m_mapSTwN0wd;
		POSTagSet2IntMap<RET_TYPE> m_mapSTtN0td;

		WordIntMap<RET_TYPE> m_mapSTwra;
		POSTagIntMap<RET_TYPE> m_mapSTtra;
		WordIntMap<RET_TYPE> m_mapSTwla;
		POSTagIntMap<RET_TYPE> m_mapSTtla;
		WordIntMap<RET_TYPE> m_mapN0wla;
		POSTagIntMap<RET_TYPE> m_mapN0tla;

		WordTagsetMap<RET_TYPE> m_mapSTwrp;
		POSTagTagsetMap<RET_TYPE> m_mapSTtrp;
		WordTagsetMap<RET_TYPE> m_mapSTwlp;
		POSTagTagsetMap<RET_TYPE> m_mapSTtlp;
		WordTagsetMap<RET_TYPE> m_mapN0wlp;
		POSTagTagsetMap<RET_TYPE> m_mapN0tlp;

	public:
		TreeWeight(const std::string & sRead, const std::string & sRecord,
				DWord * pWords, DPOSTag * pPOSTags, DLabel * pLables, DSuperTag * pSuperTags,
				SuperTagCandidates * wordsCandidates, SuperTagCandidates * postagsCandidates);
		~TreeWeight();

		void loadScores() override;
		void saveScores() const override;
		void computeAverageFeatureWeights(const int & round) override;

		void getOrUpdateFeatureScores(GraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE> * parser, const STATE_TYPE & state, const ActionScoreIncrement & amount);
	};


	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::empty_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::start_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> WordPOSTag TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::end_taggedword = WordPOSTag();
	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE> TagSet TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::empty_tagset = TagSet();

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::TreeWeight(const std::string & sRead, const std::string & sRecord,
			DWord * pWords, DPOSTag * pPOSTags, DLabel * pLables, DSuperTag * pSuperTags,
			SuperTagCandidates * wordsCandidates, SuperTagCandidates * postagsCandidates) :
		GraphWeightBase(sRead, sRecord, pWords, pPOSTags, pLables, pSuperTags, wordsCandidates, postagsCandidates),
		m_mapSTw("StackWord"),
		m_mapSTt("StackTag"),
		m_mapSTwt("StackWordTag"),
		m_mapN0w("NextWord"),
		m_mapN0t("NextTag"),
		m_mapN0wt("NextWordTag"),
		m_mapS1w("Stack+1Word"),
		m_mapS1t("Stack+1Tag"),
		m_mapS1wt("Stack+1WordTag"),
		m_mapS2w("Stack+2Word"),
		m_mapS2t("Stack+2Tag"),
		m_mapS2wt("Stack+2WordTag"),
		m_mapN1w("Next+1Word"),
		m_mapN1t("Next+1Tag"),
		m_mapN1wt("Next+1WordTag"),
		m_mapN2w("Next+2Word"),
		m_mapN2t("Next+2Tag"),
		m_mapN2wt("Next+2WordTag"),
		m_mapSTHw("StackHeadWord"),
		m_mapSTHt("StackHeadTag"),
		m_mapSTi("StackLabel"),
		m_mapSTHHw("StackHeadHeadWord"),
		m_mapSTHHt("StackHeadHeadTag"),
		m_mapSTHi("StackLabel"),
		m_mapSTLDw("StackLDWord"),
		m_mapSTLDt("StackLDTag"),
		m_mapSTLDi("StackLDLabel"),
		m_mapSTRDw("StackRDWord"),
		m_mapSTRDt("StackRDTag"),
		m_mapSTRDi("StackRDLabel"),
		m_mapN0LDw("NextLDWord"),
		m_mapN0LDt("NextLDTag"),
		m_mapN0LDi("NextLDLabel"),
		m_mapSTL2Dw("StackL2DWord"),
		m_mapSTL2Dt("StackL2DTag"),
		m_mapSTL2Di("StackL2DLabel"),
		m_mapSTR2Dw("StackR2DWord"),
		m_mapSTR2Dt("StackR2DTag"),
		m_mapSTR2Di("StackR2DLabel"),
		m_mapN0L2Dw("NextL2DWord"),
		m_mapN0L2Dt("NextL2DTag"),
		m_mapN0L2Di("NextL2DLabel"),
		m_mapHTw("HeadStackWord"),
		m_mapHTt("HeadStackTag"),
		m_mapHTwt("HeadStackWordTag"),
		m_mapSTwtN0wt("StackWordTagNextWordTag"),
		m_mapSTwtN0w("StackWordTagNextWord"),
		m_mapSTwN0wt("StackWordNextWordTag"),
		m_mapSTtN0wt("StackTagNextWordTag"),
		m_mapSTwtN0t("StackWordTagNextTag"),
		m_mapSTwN0w("StackWordNextWord"),
		m_mapSTtN0t("StackTagNextTag"),
		m_mapN0tN1t("NextTagNext+1Tag"),
		m_mapN0tN1tN2t("NextTagTrigram"),
		m_mapSTtN0tN1t("StackTagNextTagNext+1Tag"),
		m_mapSTtN0tN0LDt("StackTagNextTagNextLDTag"),
		m_mapN0tN0LDtN0L2Dt("StackTagNextTagNextLDTagNextTagNextL2DTag"),
		m_mapSTHtSTtN0t("StackHeadTagStackTagNextTag"),
		m_mapHTtHT2tN0t("HeadStackTagHeadStack2TagNextTag"),
		m_mapSTHHtSTHtSTt("StackHeadHeadTagStackHeadTagStackTag"),
		m_mapSTtSTLDtN0t("StackTagStackLDTagNextTag"),
		m_mapSTtSTLDtSTL2Dt("StackTagStackLDTagStackL2DTag"),
		m_mapSTtSTRDtN0t("StackTagStackRDTagNextTag"),
		m_mapSTtSTRDtSTR2Dt("StackTagStackRDTagStackR2DTag"),
		m_mapSTwd("StackWordDist"),
		m_mapSTtd("StackTagDist"),
		m_mapN0wd("NextWordDist"),
		m_mapN0td("NextTagDist"),
		m_mapSTwN0wd("StackWordNextWordDist"),
		m_mapSTtN0td("StackTagNextTagDist"),
		m_mapSTwra("StackWordRightArity"),
		m_mapSTtra("StackTagRightArity"),
		m_mapSTwla("StackWordLeftArity"),
		m_mapSTtla("StackTagLeftArity"),
		m_mapN0wla("NextWordRightArity"),
		m_mapN0tla("NextTagRightArity"),
		m_mapSTwrp("StackWordRightSetoftags"),
		m_mapSTtrp("StackTagRightSetoftags"),
		m_mapSTwlp("StackWordLeftSetoftags"),
		m_mapSTtlp("StackTagLeftSetoftags"),
		m_mapN0wlp("Next0WordLeftSetoftags"),
		m_mapN0tlp("Next0TagLeftSetoftags")
	{

		empty_taggedword.refer(m_pWords->lookup(EMPTY_WORD), m_pPOSTags->lookup(EMPTY_POSTAG));
		start_taggedword.refer(m_pWords->lookup(START_WORD), m_pPOSTags->lookup(START_POSTAG));
		end_taggedword.refer(m_pWords->lookup(END_WORD), m_pPOSTags->lookup(END_POSTAG));

		loadScores();
		std::cout << "load complete." << std::endl;
	}

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::~TreeWeight() = default;

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::loadScores() {

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
		input >> m_mapSTt;
		input >> m_mapSTwt;
		input >> m_mapN0w;
		input >> m_mapN0t;
		input >> m_mapN0wt;
		input >> m_mapS1w;
		input >> m_mapS1t;
		input >> m_mapS1wt;
		input >> m_mapS2w;
		input >> m_mapS2t;
		input >> m_mapS2wt;
		input >> m_mapN1w;
		input >> m_mapN1t;
		input >> m_mapN1wt;
		input >> m_mapN2w;
		input >> m_mapN2t;
		input >> m_mapN2wt;
		input >> m_mapSTHw;
		input >> m_mapSTHt;
		input >> m_mapSTi;
		input >> m_mapSTHHw;
		input >> m_mapSTHHt;
		input >> m_mapSTHi;
		input >> m_mapSTLDw;
		input >> m_mapSTLDt;
		input >> m_mapSTLDi;
		input >> m_mapSTRDw;
		input >> m_mapSTRDt;
		input >> m_mapSTRDi;
		input >> m_mapN0LDw;
		input >> m_mapN0LDt;
		input >> m_mapN0LDi;
		input >> m_mapSTL2Dw;
		input >> m_mapSTL2Dt;
		input >> m_mapSTL2Di;
		input >> m_mapSTR2Dw;
		input >> m_mapSTR2Dt;
		input >> m_mapSTR2Di;
		input >> m_mapN0L2Dw;
		input >> m_mapN0L2Dt;
		input >> m_mapN0L2Di;
		input >> m_mapHTw;
		input >> m_mapHTt;
		input >> m_mapHTwt;
		input >> m_mapSTwtN0wt;
		input >> m_mapSTwtN0w;
		input >> m_mapSTwN0wt;
		input >> m_mapSTtN0wt;
		input >> m_mapSTwtN0t;
		input >> m_mapSTwN0w;
		input >> m_mapSTtN0t;
		input >> m_mapN0tN1t;
		input >> m_mapN0tN1tN2t;
		input >> m_mapSTtN0tN1t;
		input >> m_mapSTtN0tN0LDt;
		input >> m_mapN0tN0LDtN0L2Dt;
		input >> m_mapSTHtSTtN0t;
		input >> m_mapHTtHT2tN0t;
		input >> m_mapSTHHtSTHtSTt;
		input >> m_mapSTtSTLDtN0t;
		input >> m_mapSTtSTLDtSTL2Dt;
		input >> m_mapSTtSTRDtN0t;
		input >> m_mapSTtSTRDtSTR2Dt;
		input >> m_mapSTwd;
		input >> m_mapSTtd;
		input >> m_mapN0wd;
		input >> m_mapN0td;
		input >> m_mapSTwN0wd;
		input >> m_mapSTtN0td;
		input >> m_mapSTwra;
		input >> m_mapSTtra;
		input >> m_mapSTwla;
		input >> m_mapSTtla;
		input >> m_mapN0wla;
		input >> m_mapN0tla;
		input >> m_mapSTwrp;
		input >> m_mapSTtrp;
		input >> m_mapSTwlp;
		input >> m_mapSTtlp;
		input >> m_mapN0wlp;
		input >> m_mapN0tlp;

		input.close();
	}

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::saveScores() const {

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

		output << m_mapSTw;
		output << m_mapSTt;
		output << m_mapSTwt;
		output << m_mapN0w;
		output << m_mapN0t;
		output << m_mapN0wt;
		output << m_mapS1w;
		output << m_mapS1t;
		output << m_mapS1wt;
		output << m_mapS2w;
		output << m_mapS2t;
		output << m_mapS2wt;
		output << m_mapN1w;
		output << m_mapN1t;
		output << m_mapN1wt;
		output << m_mapN2w;
		output << m_mapN2t;
		output << m_mapN2wt;
		output << m_mapSTHw;
		output << m_mapSTHt;
		output << m_mapSTi;
		output << m_mapSTHHw;
		output << m_mapSTHHt;
		output << m_mapSTHi;
		output << m_mapSTLDw;
		output << m_mapSTLDt;
		output << m_mapSTLDi;
		output << m_mapSTRDw;
		output << m_mapSTRDt;
		output << m_mapSTRDi;
		output << m_mapN0LDw;
		output << m_mapN0LDt;
		output << m_mapN0LDi;
		output << m_mapSTL2Dw;
		output << m_mapSTL2Dt;
		output << m_mapSTL2Di;
		output << m_mapSTR2Dw;
		output << m_mapSTR2Dt;
		output << m_mapSTR2Di;
		output << m_mapN0L2Dw;
		output << m_mapN0L2Dt;
		output << m_mapN0L2Di;
		output << m_mapHTw;
		output << m_mapHTt;
		output << m_mapHTwt;
		output << m_mapSTwtN0wt;
		output << m_mapSTwtN0w;
		output << m_mapSTwN0wt;
		output << m_mapSTtN0wt;
		output << m_mapSTwtN0t;
		output << m_mapSTwN0w;
		output << m_mapSTtN0t;
		output << m_mapN0tN1t;
		output << m_mapN0tN1tN2t;
		output << m_mapSTtN0tN1t;
		output << m_mapSTtN0tN0LDt;
		output << m_mapN0tN0LDtN0L2Dt;
		output << m_mapSTHtSTtN0t;
		output << m_mapHTtHT2tN0t;
		output << m_mapSTHHtSTHtSTt;
		output << m_mapSTtSTLDtN0t;
		output << m_mapSTtSTLDtSTL2Dt;
		output << m_mapSTtSTRDtN0t;
		output << m_mapSTtSTRDtSTR2Dt;
		output << m_mapSTwd;
		output << m_mapSTtd;
		output << m_mapN0wd;
		output << m_mapN0td;
		output << m_mapSTwN0wd;
		output << m_mapSTtN0td;
		output << m_mapSTwra;
		output << m_mapSTtra;
		output << m_mapSTwla;
		output << m_mapSTtla;
		output << m_mapN0wla;
		output << m_mapN0tla;
		output << m_mapSTwrp;
		output << m_mapSTtrp;
		output << m_mapSTwlp;
		output << m_mapSTtlp;
		output << m_mapN0wlp;
		output << m_mapN0tlp;

		output.close();
	}

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::computeAverageFeatureWeights(const int & round) {
		m_mapSTw.computeAverage(round);
		m_mapSTt.computeAverage(round);
		m_mapSTwt.computeAverage(round);
		m_mapN0w.computeAverage(round);
		m_mapN0t.computeAverage(round);
		m_mapN0wt.computeAverage(round);
		m_mapS1w.computeAverage(round);
		m_mapS1t.computeAverage(round);
		m_mapS1wt.computeAverage(round);
		m_mapS2w.computeAverage(round);
		m_mapS2t.computeAverage(round);
		m_mapS2wt.computeAverage(round);
		m_mapN1w.computeAverage(round);
		m_mapN1t.computeAverage(round);
		m_mapN1wt.computeAverage(round);
		m_mapN2w.computeAverage(round);
		m_mapN2t.computeAverage(round);
		m_mapN2wt.computeAverage(round);
		m_mapSTHw.computeAverage(round);
		m_mapSTHt.computeAverage(round);
		m_mapSTi.computeAverage(round);
		m_mapSTHHw.computeAverage(round);
		m_mapSTHHt.computeAverage(round);
		m_mapSTHi.computeAverage(round);
		m_mapSTLDw.computeAverage(round);
		m_mapSTLDt.computeAverage(round);
		m_mapSTLDi.computeAverage(round);
		m_mapSTRDw.computeAverage(round);
		m_mapSTRDt.computeAverage(round);
		m_mapSTRDi.computeAverage(round);
		m_mapN0LDw.computeAverage(round);
		m_mapN0LDt.computeAverage(round);
		m_mapN0LDi.computeAverage(round);
		m_mapSTL2Dw.computeAverage(round);
		m_mapSTL2Dt.computeAverage(round);
		m_mapSTL2Di.computeAverage(round);
		m_mapSTR2Dw.computeAverage(round);
		m_mapSTR2Dt.computeAverage(round);
		m_mapSTR2Di.computeAverage(round);
		m_mapN0L2Dw.computeAverage(round);
		m_mapN0L2Dt.computeAverage(round);
		m_mapN0L2Di.computeAverage(round);
		m_mapHTw.computeAverage(round);
		m_mapHTt.computeAverage(round);
		m_mapHTwt.computeAverage(round);
		m_mapSTwtN0wt.computeAverage(round);
		m_mapSTwtN0w.computeAverage(round);
		m_mapSTwN0wt.computeAverage(round);
		m_mapSTtN0wt.computeAverage(round);
		m_mapSTwtN0t.computeAverage(round);
		m_mapSTwN0w.computeAverage(round);
		m_mapSTtN0t.computeAverage(round);
		m_mapN0tN1t.computeAverage(round);
		m_mapN0tN1tN2t.computeAverage(round);
		m_mapSTtN0tN1t.computeAverage(round);
		m_mapSTtN0tN0LDt.computeAverage(round);
		m_mapN0tN0LDtN0L2Dt.computeAverage(round);
		m_mapSTHtSTtN0t.computeAverage(round);
		m_mapHTtHT2tN0t.computeAverage(round);
		m_mapSTHHtSTHtSTt.computeAverage(round);
		m_mapSTtSTLDtN0t.computeAverage(round);
		m_mapSTtSTLDtSTL2Dt.computeAverage(round);
		m_mapSTtSTRDtN0t.computeAverage(round);
		m_mapSTtSTRDtSTR2Dt.computeAverage(round);
		m_mapSTwd.computeAverage(round);
		m_mapSTtd.computeAverage(round);
		m_mapN0wd.computeAverage(round);
		m_mapN0td.computeAverage(round);
		m_mapSTwN0wd.computeAverage(round);
		m_mapSTtN0td.computeAverage(round);
		m_mapSTwra.computeAverage(round);
		m_mapSTtra.computeAverage(round);
		m_mapSTwla.computeAverage(round);
		m_mapSTtla.computeAverage(round);
		m_mapN0wla.computeAverage(round);
		m_mapN0tla.computeAverage(round);
		m_mapSTwrp.computeAverage(round);
		m_mapSTtrp.computeAverage(round);
		m_mapSTwlp.computeAverage(round);
		m_mapSTtlp.computeAverage(round);
		m_mapN0wlp.computeAverage(round);
		m_mapN0tlp.computeAverage(round);
	}

	template<class RET_TYPE, class STATE_TYPE, class ACTION_TYPE>
	void TreeWeight<RET_TYPE, STATE_TYPE, ACTION_TYPE>::getOrUpdateFeatureScores(GraphDepParserBase<RET_TYPE, STATE_TYPE, ACTION_TYPE> * parser, const STATE_TYPE & item, const ActionScoreIncrement & amount) {

		const int outIndex = -1;
		const int emptyLabel = 0;
		const int emptyArity = -1;

		WordPOSTag (&sentence)[MAX_SENTENCE_SIZE] = parser->m_lSentence;
		RET_TYPE & packedScore = parser->m_lPackedScore;
		const int & scoreIndex = parser->m_nScoreIndex;
		const int & trainingRound = parser->m_nTrainingRound;
		const int & sentenceLength = parser->m_nSentenceLength;

		const int & st_index = item.stackEmpty() ? outIndex : item.stackTop();
		const int & sth_index = st_index == -1 ? outIndex : item.head(st_index);
		const int & sthh_index = sth_index == -1 ? outIndex : item.head(sth_index);
		const int & stld_index = st_index == -1 ? outIndex : item.leftPred(st_index);
		const int & strd_index = st_index == -1 ? outIndex : item.rightPred(st_index);
		const int & stl2d_index = stld_index == -1 ? outIndex : item.leftSubPred(st_index);
		const int & str2d_index = strd_index == -1 ? outIndex : item.rightSubPred(st_index);
		const int & n0_index = ((item.size() == sentenceLength) ? outIndex : item.size());
		const int & n0ld_index = n0_index == -1 ? outIndex : item.leftPred(n0_index);
		const int & n0l2d_index = n0ld_index == -1 ? outIndex : item.leftSubPred(n0_index);
		const int & s1_index = ((st_index + 1 < sentenceLength) ? st_index + 1 : -1);
		const int & s2_index = ((st_index + 2 < sentenceLength) ? st_index + 2 : -1);
		const int & n1_index = ((n0_index + 1 < sentenceLength) ? n0_index + 1 : -1);
		const int & n2_index = ((n0_index + 2 < sentenceLength) ? n0_index + 2 : -1);

		const WordPOSTag & st_word_tag = st_index == -1 ? empty_taggedword : sentence[st_index];
		const WordPOSTag & sth_word_tag = sth_index == -1 ? empty_taggedword : sentence[sth_index];
		const WordPOSTag & sthh_word_tag = sthh_index == -1 ? empty_taggedword : sentence[sthh_index];
		const WordPOSTag & stld_word_tag = stld_index == -1 ? empty_taggedword : sentence[stld_index];
		const WordPOSTag & strd_word_tag = strd_index == -1 ? empty_taggedword : sentence[strd_index];
		const WordPOSTag & stl2d_word_tag = stl2d_index == -1 ? empty_taggedword : sentence[stl2d_index];
		const WordPOSTag & str2d_word_tag = str2d_index == -1 ? empty_taggedword : sentence[str2d_index];
		const WordPOSTag & n0_word_tag = n0_index == -1 ? empty_taggedword : sentence[n0_index];
		const WordPOSTag & n0ld_word_tag = n0ld_index == -1 ? empty_taggedword : sentence[n0ld_index];
		const WordPOSTag & n0l2d_word_tag = n0l2d_index == -1 ? empty_taggedword : sentence[n0l2d_index];
		const WordPOSTag & s1_word_tag = s1_index == -1 ? empty_taggedword : sentence[s1_index];
		const WordPOSTag & s2_word_tag = s2_index == -1 ? empty_taggedword : sentence[s2_index];
		const WordPOSTag & n1_word_tag = n1_index == -1 ? empty_taggedword : sentence[n1_index];
		const WordPOSTag & n2_word_tag = n2_index == -1 ? empty_taggedword : sentence[n2_index];

		const Word & st_word = st_word_tag.first();
		const Word & sth_word = sth_word_tag.first();
		const Word & sthh_word = sthh_word_tag.first();
		const Word & stld_word = stld_word_tag.first();
		const Word & strd_word = strd_word_tag.first();
		const Word & stl2d_word = stl2d_word_tag.first();
		const Word & str2d_word = str2d_word_tag.first();
		const Word & n0_word = n0_word_tag.first();
		const Word & n0ld_word = n0ld_word_tag.first();
		const Word & n0l2d_word = n0l2d_word_tag.first();
		const Word & s1_word = s1_word_tag.first();
		const Word & s2_word = s2_word_tag.first();
		const Word & n1_word = n1_word_tag.first();
		const Word & n2_word = n2_word_tag.first();

		const POSTag & st_tag = st_word_tag.second();
		const POSTag & sth_tag = sth_word_tag.second();
		const POSTag & sthh_tag = sthh_word_tag.second();
		const POSTag & stld_tag = stld_word_tag.second();
		const POSTag & strd_tag = strd_word_tag.second();
		const POSTag & stl2d_tag = stl2d_word_tag.second();
		const POSTag & str2d_tag = str2d_word_tag.second();
		const POSTag & n0_tag = n0_word_tag.second();
		const POSTag & n0ld_tag = n0ld_word_tag.second();
		const POSTag & n0l2d_tag = n0l2d_word_tag.second();
		const POSTag & s1_tag = s1_word_tag.second();
		const POSTag & s2_tag = s2_word_tag.second();
		const POSTag & n1_tag = n1_word_tag.second();
		const POSTag & n2_tag = n2_word_tag.second();

		const int & st_label = st_index == -1 ? emptyLabel : item.label(st_index);
		const int & sth_label = sth_index == -1 ? emptyLabel : item.label(sth_index);
		const int & stld_label = stld_index == -1 ? emptyLabel : item.label(stld_index);
		const int & strd_label = strd_index == -1 ? emptyLabel : item.label(strd_index);
		const int & stl2d_label = stl2d_index == -1 ? emptyLabel : item.label(stl2d_index);
		const int & str2d_label = str2d_index == -1 ? emptyLabel : item.label(str2d_index); //PROBLEM!
		const int & n0ld_label = n0ld_index == -1 ? emptyLabel : item.label(n0ld_index);
		const int & n0l2d_label = n0l2d_index == -1 ? emptyLabel : item.label(n0l2d_index);

		const int & st_n0_dist = encodeLinkDistanceOrDirection(st_index, n0_index, false);

		const int & st_rarity = st_index == -1 ? emptyArity : item.rightPredArity(st_index);
		const int & st_larity = st_index == -1 ? emptyArity : item.leftPredArity(st_index);
		const int & n0_larity = n0_index == -1 ? emptyArity : item.leftPredArity(n0_index);

		const TagSet & st_rtagset = st_index == -1 ? empty_tagset : item.rightPredLabelSet(st_index);
		const TagSet & st_ltagset = st_index == -1 ? empty_tagset : item.leftPredLabelSet(st_index);
		const TagSet & n0_ltagset = n0_index == -1 ? empty_tagset : item.leftPredLabelSet(n0_index);

		if (st_index != -1) {
			m_mapSTw.getOrUpdateScore(packedScore, st_word, scoreIndex, amount, trainingRound);
			m_mapSTt.getOrUpdateScore(packedScore, st_tag, scoreIndex, amount, trainingRound);
			bi_features.refer(st_word, st_tag);
			m_mapSTwt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (n0_index != -1) {
			m_mapN0w.getOrUpdateScore(packedScore, n0_word, scoreIndex, amount, trainingRound);
			m_mapN0t.getOrUpdateScore(packedScore, n0_tag, scoreIndex, amount, trainingRound);
			bi_features.refer(n0_word, n0_tag);
			m_mapN0wt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (s1_index != -1) {
			m_mapS1w.getOrUpdateScore(packedScore, s1_word, scoreIndex, amount, trainingRound);
			m_mapS1t.getOrUpdateScore(packedScore, s1_tag, scoreIndex, amount, trainingRound);
			bi_features.refer(s1_word, s1_tag);
			m_mapS1wt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (s2_index != -1) {
			m_mapS2w.getOrUpdateScore(packedScore, s2_word, scoreIndex, amount, trainingRound);
			m_mapS2t.getOrUpdateScore(packedScore, s2_tag, scoreIndex, amount, trainingRound);
			bi_features.refer(s2_word, s2_tag);
			m_mapS2wt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (n1_index != -1) {
			m_mapN1w.getOrUpdateScore(packedScore, n1_word, scoreIndex, amount, trainingRound);
			m_mapN1t.getOrUpdateScore(packedScore, n1_tag, scoreIndex, amount, trainingRound);
			bi_features.refer(n1_word, n1_tag);
			m_mapN1wt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (n2_index != -1) {
			m_mapN2w.getOrUpdateScore(packedScore, n2_word, scoreIndex, amount, trainingRound);
			m_mapN2t.getOrUpdateScore(packedScore, n2_tag, scoreIndex, amount, trainingRound);
			bi_features.refer(n2_word, n2_tag);
			m_mapN2wt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (sth_index != -1) {
			m_mapSTHw.getOrUpdateScore(packedScore, sth_word, scoreIndex, amount, trainingRound);
			m_mapSTHt.getOrUpdateScore(packedScore, sth_tag, scoreIndex, amount, trainingRound);
			m_mapSTi.getOrUpdateScore(packedScore, st_label, scoreIndex, amount, trainingRound);
		}

		if (sthh_index != -1) {
			m_mapSTHHw.getOrUpdateScore(packedScore, sthh_word, scoreIndex, amount, trainingRound);
			m_mapSTHHt.getOrUpdateScore(packedScore, sthh_tag, scoreIndex, amount, trainingRound);
			m_mapSTHi.getOrUpdateScore(packedScore, sth_label, scoreIndex, amount, trainingRound);
		}

		if (stld_index != -1) {
			m_mapSTLDw.getOrUpdateScore(packedScore, stld_word, scoreIndex, amount, trainingRound);
			m_mapSTLDt.getOrUpdateScore(packedScore, stld_tag, scoreIndex, amount, trainingRound);
			m_mapSTLDi.getOrUpdateScore(packedScore, stld_label, scoreIndex, amount, trainingRound);
		}

		if (strd_index != -1) {
			m_mapSTRDw.getOrUpdateScore(packedScore, strd_word, scoreIndex, amount, trainingRound);
			m_mapSTRDt.getOrUpdateScore(packedScore, strd_tag, scoreIndex, amount, trainingRound);
			m_mapSTRDi.getOrUpdateScore(packedScore, strd_label, scoreIndex, amount, trainingRound);
		}

		if (n0ld_index != -1) {
			m_mapN0LDw.getOrUpdateScore(packedScore, n0ld_word, scoreIndex, amount, trainingRound);
			m_mapN0LDt.getOrUpdateScore(packedScore, n0ld_tag, scoreIndex, amount, trainingRound);
			m_mapN0LDi.getOrUpdateScore(packedScore, n0ld_label, scoreIndex, amount, trainingRound);
		}

		if (stl2d_index != -1) {
			m_mapSTL2Dw.getOrUpdateScore(packedScore, stl2d_word, scoreIndex, amount, trainingRound);
			m_mapSTL2Dt.getOrUpdateScore(packedScore, stl2d_tag, scoreIndex, amount, trainingRound);
			m_mapSTL2Di.getOrUpdateScore(packedScore, stl2d_label, scoreIndex, amount, trainingRound);
		}

		if (str2d_index != -1) {
			m_mapSTR2Dw.getOrUpdateScore(packedScore, str2d_word, scoreIndex, amount, trainingRound);
			m_mapSTR2Dt.getOrUpdateScore(packedScore, str2d_tag, scoreIndex, amount, trainingRound);
			m_mapSTR2Di.getOrUpdateScore(packedScore, str2d_label, scoreIndex, amount, trainingRound);
		}

		if (n0l2d_index != -1) {
			m_mapN0L2Dw.getOrUpdateScore(packedScore, n0l2d_word, scoreIndex, amount, trainingRound);
			m_mapN0L2Dt.getOrUpdateScore(packedScore, n0l2d_tag, scoreIndex, amount, trainingRound);
			m_mapN0L2Di.getOrUpdateScore(packedScore, n0l2d_label, scoreIndex, amount, trainingRound);
		}

		if (st_index != -1) {
			poses_feature = ENCODE_POSTAG_SET_2(st_tag, n0_tag);
			m_mapSTtN0t.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			tri_features.refer(st_word, n0_word, poses_feature);
			m_mapSTwtN0wt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.referLast(st_tag);
			m_mapSTwtN0w.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			tri_features.referLast(n0_tag);
			m_mapSTwN0wt.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_word, poses_feature);
			m_mapSTwtN0t.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(n0_word, poses_feature);
			m_mapSTtN0wt.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_word, n0_word);
			m_mapSTwN0w.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (st_index != -1 && n0_index != -1) {
			poses_feature = ENCODE_POSTAG_SET_2(n0_tag, n1_tag);
			m_mapN0tN1t.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			poses_feature = ENCODE_POSTAG_SET_3(n0_tag, n1_tag, n2_tag);
			m_mapN0tN1tN2t.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			poses_feature = ENCODE_POSTAG_SET_3(st_tag, n0_tag, n1_tag);
			m_mapSTtN0tN1t.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			poses_feature = ENCODE_POSTAG_SET_3(st_tag, n0_tag, n0ld_tag);
			m_mapSTtN0tN0LDt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			poses_feature = ENCODE_POSTAG_SET_3(n0_tag, n0ld_tag, n0l2d_tag);
			m_mapN0tN0LDtN0L2Dt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
		}

		if (st_index != -1) {
			poses_feature = ENCODE_POSTAG_SET_3(sth_tag, st_tag, n0_tag);
			m_mapSTHtSTtN0t.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			poses_feature = ENCODE_POSTAG_SET_3(sthh_tag, sth_tag, st_tag);
			m_mapSTHHtSTHtSTt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			poses_feature = ENCODE_POSTAG_SET_3(st_tag, stld_tag, n0_tag);
			m_mapSTtSTLDtN0t.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			poses_feature = ENCODE_POSTAG_SET_3(st_tag, stld_tag, stl2d_tag);
			m_mapSTtSTLDtSTL2Dt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			poses_feature = ENCODE_POSTAG_SET_3(st_tag, strd_tag, n0_tag);
			m_mapSTtSTRDtN0t.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
			poses_feature = ENCODE_POSTAG_SET_3(st_tag, strd_tag, str2d_tag);
			m_mapSTtSTRDtSTR2Dt.getOrUpdateScore(packedScore, poses_feature, scoreIndex, amount, trainingRound);
		}

		if (st_index != -1 && n0_index != -1) {
			bi_features.refer(st_word, st_n0_dist);
			m_mapSTwd.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_tag, st_n0_dist);
			m_mapSTtd.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(n0_word, st_n0_dist);
			m_mapN0wd.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(n0_tag, st_n0_dist);
			m_mapN0td.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			tri_features.refer(st_word, n0_word, st_n0_dist);
			m_mapSTwN0wd.getOrUpdateScore(packedScore, tri_features, scoreIndex, amount, trainingRound);
			bi_features.refer(ENCODE_POSTAG_SET_2(st_tag, n0_tag), st_n0_dist);
			m_mapSTtN0td.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (st_index != -1) {
			bi_features.refer(st_word, st_rarity);
			m_mapSTwra.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_tag, st_rarity);
			m_mapSTtra.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_word, st_larity);
			m_mapSTwla.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(st_tag, st_larity);
			m_mapSTtla.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (n0_index != -1) {
			bi_features.refer(n0_word, n0_larity);
			m_mapN0wla.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
			bi_features.refer(n0_tag, n0_larity);
			m_mapN0tla.getOrUpdateScore(packedScore, bi_features, scoreIndex, amount, trainingRound);
		}

		if (st_index != -1){
			uni_tagset.refer(st_word, st_rtagset.bits(0), st_rtagset.bits(1));
			m_mapSTwrp.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
			uni_tagset.refer(st_tag, st_rtagset.bits(0), st_rtagset.bits(1));
			m_mapSTtrp.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
			uni_tagset.refer(st_word, st_ltagset.bits(0), st_ltagset.bits(1));
			m_mapSTwlp.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
			uni_tagset.refer(st_tag, st_ltagset.bits(0), st_ltagset.bits(1));
			m_mapSTtlp.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		}

		if (n0_index != -1){
			uni_tagset.refer(n0_word, n0_ltagset.bits(0), n0_ltagset.bits(1));
			m_mapN0wlp.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
			uni_tagset.refer(n0_tag, n0_ltagset.bits(0), n0_ltagset.bits(1));
			m_mapN0tlp.getOrUpdateScore(packedScore, uni_tagset, scoreIndex, amount, trainingRound);
		}
	}
}

#endif
