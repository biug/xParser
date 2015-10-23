#ifndef _BOTH_TWOSTACK_DEPPARSER_H
#define _BOTH_TWOSTACK_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "both_twostack_state.h"
#include "both_twostack_weight.h"
#include "common/parser/implementations/graph_transition_both/graph_transition_both_depparser_base.h"

namespace both_twostack {

	using graph_transition_both::GraphDepParserBase;

	template<class RET_TYPE>
	class DepParser : public GraphDepParserBase<StateItem> {
	protected:
		RET_TYPE m_lPackedScore;

		Unsigned poses_feature;
		Int uni_feature;
		TwoInts bi_features;
		ThreeInts tri_features;
		FourInts quar_features;
		IntTagSet uni_tagset;
		TwoIntsTagSet bi_tagset;

		void mem(const tscore & score);
		void arcMem(const tscore & score);
		void recall(const tscore & score);
		void arcRecall(const tscore & score);
		void reduce(const tscore & score);
		void arcReduce(const tscore & score);
		void shift(const tscore & score, const int & tokenId);
		void arcShift(const tscore & score, const int & tokenId);

		void getActionScores(const StateItem & item);
		void getOrUpdateFeatureScore(const StateItem & item, const ActionScoreIncrement & amount);

	public:
		DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, int nState, const bool & bChar, const bool & bPath, const bool & bSTag);
		~DepParser();

		void decode() override;
	};

	extern int A_MM_FIRST;
	extern int A_RC_FIRST;
	extern int A_RE_FIRST;
	extern int A_SH_FIRST;
	extern int SH_FIRST;

	extern int A_MM_END;
	extern int A_RC_END;
	extern int A_RE_END;
	extern int A_SH_END;
	extern int SH_END;

	template<class RET_TYPE>
	DepParser<RET_TYPE>::DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, int nState, const bool & bChar, const bool & bPath, const bool & bSTag) :
		GraphDepParserBase(sFeatureInput, nState, bChar, bPath, bSTag) {

		empty_taggedword.refer(TWord::code(EMPTY_WORD), TPOSTag::code(EMPTY_POSTAG));
		start_taggedword.refer(TWord::code(START_WORD), TPOSTag::code(START_POSTAG));
		middle_taggedword.refer(TWord::code(MIDDLE_WORD), TPOSTag::code(MIDDLE_POSTAG));
		end_taggedword.refer(TWord::code(END_WORD), TPOSTag::code(END_POSTAG));

		m_Weight = new Weight<RET_TYPE>(sFeatureInput, sFeatureOut);
	}

	template<class RET_TYPE>
	DepParser<RET_TYPE>::~DepParser() {
		delete m_Weight;
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::mem(const tscore & score) {
		m_abScores.insertItem(ScoredAction(MEM, score + m_lPackedScore[MEM]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::arcMem(const tscore & score) {
		for (int action = A_MM_FIRST; action < A_MM_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::recall(const tscore & score) {
		m_abScores.insertItem(ScoredAction(RECALL, score + m_lPackedScore[RECALL]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::arcRecall(const tscore & score) {
		for (int action = A_RC_FIRST; action < A_RC_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduce(const tscore & score) {
		m_abScores.insertItem(ScoredAction(REDUCE, score + m_lPackedScore[REDUCE]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::arcReduce(const tscore & score) {
		for (int action = A_RE_FIRST; action < A_RE_END; ++action) {
			m_abScores.insertItem(ScoredAction(action, score + m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	void DepParser<RET_TYPE>::shift(const tscore & score, const int & tokenId) {
		if (m_bSuperTag) {
			if (g_mapSuperTagCandidatesOfWords.find(m_lSentence[tokenId].first()) != g_mapSuperTagCandidatesOfWords.end()) {
				for (const auto & tag : g_mapSuperTagCandidatesOfWords[m_lSentence[tokenId].first()]) {
					m_abScores.insertItem(ScoredAction(SH_FIRST + tag, score + m_lPackedScore[SH_FIRST + tag]));
				}
			}
			else {
				for (const auto & tag : g_mapSuperTagCandidatesOfPOSTags[m_lSentence[tokenId].second()]) {
					m_abScores.insertItem(ScoredAction(SH_FIRST + tag, score + m_lPackedScore[SH_FIRST + tag]));
				}
			}
		}
		else {
			m_abScores.insertItem(ScoredAction(SH_FIRST, score + m_lPackedScore[SH_FIRST]));
		}
	}

	template<class RET_TYPE>
	void DepParser<RET_TYPE>::arcShift(const tscore & score, const int & tokenId) {
		if (m_bSuperTag) {
			if (g_mapSuperTagCandidatesOfWords.find(m_lSentence[tokenId].first()) != g_mapSuperTagCandidatesOfWords.end()) {
				for (const auto & tag : g_mapSuperTagCandidatesOfWords[m_lSentence[tokenId].first()]) {
					for (int action = A_SH_FIRST + tag * g_nGraphLabelCount, n = action + g_nGraphLabelCount; action < n; ++action) {
						m_abScores.insertItem(ScoredAction(action, score + m_lPackedScore[action]));
					}
				}
			}
			else {
				for (const auto & tag : g_mapSuperTagCandidatesOfPOSTags[m_lSentence[tokenId].second()]) {
					for (int action = A_SH_FIRST + tag * g_nGraphLabelCount, n = action + g_nGraphLabelCount; action < n; ++action) {
						m_abScores.insertItem(ScoredAction(action, score + m_lPackedScore[action]));
					}
				}
			}
		}
		else {
			for (int action = A_SH_FIRST, n = A_SH_FIRST + g_nGraphLabelCount; action < n; ++action) {
				m_abScores.insertItem(ScoredAction(action, score + m_lPackedScore[action]));
			}
		}
	}


	template<class RET_TYPE>
	void DepParser<RET_TYPE>::getActionScores(const StateItem & item) {
		memset(m_lPackedScore, 0, sizeof(m_lPackedScore));
		getOrUpdateFeatureScore(item, ActionScoreIncrement(ACTION_START, 0));
	}

	template<class RET_TYPE>
	void DepParser<RET_TYPE>::decode() {

		m_pGenerated->clear();

		for (const auto & iGenerator : *m_pGenerator) {
			m_abScores.clear();
			getActionScores(*iGenerator);

			const tscore & score = iGenerator->getScore();

			if (iGenerator->stackBack() >= 0) {
				reduce(score);
			}

			if (iGenerator->size() < m_nSentenceLength) {
				if (iGenerator->canArc()) {
					arcReduce(score);
					if (iGenerator->canShift()) {
						arcShift(score, iGenerator->size());
					}
					if (iGenerator->canMem()) {
						arcMem(score);
					}
					if (iGenerator->canRecall()) {
						arcRecall(score);
					}
				}
				if (iGenerator->canShift()) {
					shift(score, iGenerator->size());
				}
			}

			if (iGenerator->canMem()) {
				mem(score);
			}

			if (iGenerator->canRecall()) {
				recall(score);
			}

			for (const auto & saScore : m_abScores) {
				m_iCandidate = *iGenerator;
				m_iCandidate.setScore(saScore->getScore());
				m_iCandidate.move(saScore->getAction());
				m_pGenerated->insertItem(m_iCandidate);
			}

			if (m_abScores.size() == 0) {
				m_abFinished.insertItem(*iGenerator);
			}
		}
	}

	template<class RET_TYPE>
	void DepParser<RET_TYPE>::getOrUpdateFeatureScore(const StateItem & item, const ActionScoreIncrement & amount) {

		Weight<RET_TYPE> * cweight = (Weight<RET_TYPE>*)m_Weight;
		const StateItem & rItem = m_iReverse[item.size()];

		// st
		const int & st_index = item.stackBack() == -1 ? -1 : item.stackTop();
		const int & stlh_index = st_index == -1 ? -1 : item.leftHead(st_index);
		const int & stlp_index = st_index == -1 ? -1 : item.leftPred(st_index);
		const int & strh_index = st_index == -1 ? -1 : item.rightHead(st_index);
		const int & strp_index = st_index == -1 ? -1 : item.rightPred(st_index);
		// st2
		const int & st2_index = item.stackBack() <= 0 ? -1 : item.stackSubTop();
		const int & st2lh_index = st2_index == -1 ? -1 : item.leftHead(st2_index);
		const int & st2lp_index = st2_index == -1 ? -1 : item.leftPred(st2_index);
		const int & st2rh_index = st2_index == -1 ? -1 : item.rightHead(st2_index);
		const int & st2rp_index = st2_index == -1 ? -1 : item.rightPred(st2_index);
		// sst
		const int & sst_index = item.secondStackBack() == -1 ? -1 : item.secondStackTop();
		const int & sstlh_index = sst_index == -1 ? -1 : item.leftHead(sst_index);
		const int & sstlp_index = sst_index == -1 ? -1 : item.leftPred(sst_index);
		const int & sstrh_index = sst_index == -1 ? -1 : item.rightHead(sst_index);
		const int & sstrp_index = sst_index == -1 ? -1 : item.rightPred(sst_index);
		// bf
		const int & bf_index = item.size() < m_nSentenceLength ? item.size() : -1;
		const int & bflh_index = bf_index == -1 ? -1 : item.leftHead(bf_index);
		const int & bflp_index = bf_index == -1 ? -1 : item.leftPred(bf_index);
		const int & bfrh_index = bf_index == -1 ? -1 : rItem.rightHead(bf_index);
		const int & bfrp_index = bf_index == -1 ? -1 : rItem.rightPred(bf_index);
		// bf2
		const int & bf2_index = rItem.stackBack() <= 0 ? -1 : rItem.stackSubTop();
		const int & bf2lh_index = bf2_index == -1 ? -1 : rItem.leftHead(bf2_index);
		const int & bf2lp_index = bf2_index == -1 ? -1 : rItem.leftPred(bf2_index);
		const int & bf2rh_index = bf2_index == -1 ? -1 : rItem.rightHead(bf2_index);
		const int & bf2rp_index = bf2_index == -1 ? -1 : rItem.rightPred(bf2_index);
		// sst
		const int & sbf_index = rItem.secondStackBack() == -1 ? -1 : rItem.secondStackTop();
		const int & sbflh_index = sbf_index == -1 ? -1 : item.leftHead(sbf_index);
		const int & sbflp_index = sbf_index == -1 ? -1 : item.leftPred(sbf_index);
		const int & sbfrh_index = sbf_index == -1 ? -1 : item.rightHead(sbf_index);
		const int & sbfrp_index = sbf_index == -1 ? -1 : item.rightPred(sbf_index);
		// context
		const int & stl2_index = st_index >= 2 ? st_index - 2 : -1;
		const int & stl1_index = st_index >= 1 ? st_index - 1 : -1;
		const int & str1_index = st_index + 1 < m_nSentenceLength ? st_index + 1 : -1;
		const int & str2_index = st_index + 2 < m_nSentenceLength ? st_index + 2 : -1;
		const int & bfl2_index = bf_index >= 2 ? bf_index - 2 : -1;
		const int & bfl1_index = bf_index >= 1 ? bf_index - 1 : -1;
		const int & bfr1_index = bf_index + 1 < m_nSentenceLength ? bf_index + 1 : -1;
		const int & bfr2_index = bf_index + 2 < m_nSentenceLength ? bf_index + 2 : -1;

		// st
		const WordPOSTag & st_word_postag = st_index == -1 ? start_taggedword : m_lSentence[st_index];
		const WordPOSTag & stlh_word_postag = stlh_index == -1 ? empty_taggedword : m_lSentence[stlh_index];
		const WordPOSTag & stlp_word_postag = stlp_index == -1 ? empty_taggedword : m_lSentence[stlp_index];
		const WordPOSTag & strh_word_postag = strh_index == -1 ? empty_taggedword : m_lSentence[strh_index];
		const WordPOSTag & strp_word_postag = strp_index == -1 ? empty_taggedword : m_lSentence[strp_index];
		// st2
		const WordPOSTag & st2_word_postag = st2_index == -1 ? start_taggedword : m_lSentence[st2_index];
		const WordPOSTag & st2lh_word_postag = st2lh_index == -1 ? empty_taggedword : m_lSentence[st2lh_index];
		const WordPOSTag & st2lp_word_postag = st2lp_index == -1 ? empty_taggedword : m_lSentence[st2lp_index];
		const WordPOSTag & st2rh_word_postag = st2rh_index == -1 ? empty_taggedword : m_lSentence[st2rh_index];
		const WordPOSTag & st2rp_word_postag = st2rp_index == -1 ? empty_taggedword : m_lSentence[st2rp_index];
		// sst
		const WordPOSTag & sst_word_postag = sst_index == -1 ? middle_taggedword : m_lSentence[sst_index];
		const WordPOSTag & sstlh_word_postag = sstlh_index == -1 ? empty_taggedword : m_lSentence[sstlh_index];
		const WordPOSTag & sstlp_word_postag = sstlp_index == -1 ? empty_taggedword : m_lSentence[sstlp_index];
		const WordPOSTag & sstrh_word_postag = sstrh_index == -1 ? empty_taggedword : m_lSentence[sstrh_index];
		const WordPOSTag & sstrp_word_postag = sstrp_index == -1 ? empty_taggedword : m_lSentence[sstrp_index];
		// bf
		const WordPOSTag & bf_word_postag = bf_index == -1 ? end_taggedword : m_lSentence[bf_index];
		const WordPOSTag & bflh_word_postag = bflh_index == -1 ? empty_taggedword : m_lSentence[bflh_index];
		const WordPOSTag & bflp_word_postag = bflp_index == -1 ? empty_taggedword : m_lSentence[bflp_index];
		const WordPOSTag & bfrh_word_postag = bfrh_index == -1 ? empty_taggedword : m_lSentence[bfrh_index];
		const WordPOSTag & bfrp_word_postag = bfrp_index == -1 ? empty_taggedword : m_lSentence[bfrp_index];
		// bf2
		const WordPOSTag & bf2_word_postag = bf2_index == -1 ? end_taggedword : m_lSentence[bf2_index];
		const WordPOSTag & bf2lh_word_postag = bf2lh_index == -1 ? empty_taggedword : m_lSentence[bf2lh_index];
		const WordPOSTag & bf2lp_word_postag = bf2lp_index == -1 ? empty_taggedword : m_lSentence[bf2lp_index];
		const WordPOSTag & bf2rh_word_postag = bf2rh_index == -1 ? empty_taggedword : m_lSentence[bf2rh_index];
		const WordPOSTag & bf2rp_word_postag = bf2rp_index == -1 ? empty_taggedword : m_lSentence[bf2rp_index];
		// sbf
		const WordPOSTag & sbf_word_postag = sbf_index == -1 ? middle_taggedword : m_lSentence[sbf_index];
		const WordPOSTag & sbflh_word_postag = sbflh_index == -1 ? empty_taggedword : m_lSentence[sbflh_index];
		const WordPOSTag & sbflp_word_postag = sbflp_index == -1 ? empty_taggedword : m_lSentence[sbflp_index];
		const WordPOSTag & sbfrh_word_postag = sbfrh_index == -1 ? empty_taggedword : m_lSentence[sbfrh_index];
		const WordPOSTag & sbfrp_word_postag = sbfrp_index == -1 ? empty_taggedword : m_lSentence[sbfrp_index];
		// context
		const WordPOSTag & stl2_word_postag = stl2_index == -1 ? start_taggedword : m_lSentence[stl2_index];
		const WordPOSTag & stl1_word_postag = stl1_index == -1 ? start_taggedword : m_lSentence[stl1_index];
		const WordPOSTag & str1_word_postag = str1_index == -1 ? end_taggedword : m_lSentence[str1_index];
		const WordPOSTag & str2_word_postag = str2_index == -1 ? end_taggedword : m_lSentence[str2_index];
		const WordPOSTag & bfl2_word_postag = bfl2_index == -1 ? start_taggedword : m_lSentence[bfl2_index];
		const WordPOSTag & bfl1_word_postag = bfl1_index == -1 ? start_taggedword : m_lSentence[bfl1_index];
		const WordPOSTag & bfr1_word_postag = bfr1_index == -1 ? end_taggedword : m_lSentence[bfr1_index];
		const WordPOSTag & bfr2_word_postag = bfr2_index == -1 ? end_taggedword : m_lSentence[bfr2_index];

		// st
		const Word & st_word = st_word_postag.first();
		const POSTag & st_postag = st_word_postag.second();
		const TagSet & st_llabelset = st_index == -1 ? empty_tagset : item.leftPredLabelSet(st_index);
		const TagSet & st_rlabelset = st_index == -1 ? empty_tagset : item.rightPredLabelSet(st_index);
		const Word & stlh_word = stlh_word_postag.first();
		const POSTag & stlh_postag = stlh_word_postag.second();
		const int & stlh_label = st_index == -1 ? 0 : item.leftHeadLabel(st_index);
		const int & stlh_arity = st_index == -1 ? -1 : item.leftHeadArity(st_index);
		const Word & stlp_word = stlp_word_postag.first();
		const POSTag & stlp_postag = stlp_word_postag.second();
		const POSTag & stlp2_postag = (st_index == -1 || item.leftSubPred(st_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(st_index)].second();
		const int & stlp_label = st_index == -1 ? 0 : item.leftPredLabel(st_index);
		const int & stlp2_label = st_index == -1 ? 0 : item.leftSubPredLabel(st_index);
		const int & stlp_arity = st_index == -1 ? -1 : item.leftPredArity(st_index);
		const Word & strh_word = strh_word_postag.first();
		const POSTag & strh_postag = strh_word_postag.second();
		const int & strh_label = st_index == -1 ? 0 : item.rightHeadLabel(st_index);
		const int & strh_arity = st_index == -1 ? -1 : item.rightHeadArity(st_index);
		const Word & strp_word = strp_word_postag.first();
		const POSTag & strp_postag = strp_word_postag.second();
		const POSTag & strp2_postag = (st_index == -1 || item.rightSubPred(st_index) == -1) ? empty_taggedword.second() : m_lSentence[item.rightSubPred(st_index)].second();
		const int & strp_label = st_index == -1 ? 0 : item.rightPredLabel(st_index);
		const int & strp2_label = st_index == -1 ? 0 : item.rightSubPredLabel(st_index);
		const int & strp_arity = st_index == -1 ? -1 : item.rightPredArity(st_index);
		// st2
		const Word & st2_word = st2_word_postag.first();
		const POSTag & st2_postag = st2_word_postag.second();
		const TagSet & st2_llabelset = st2_index == -1 ? empty_tagset : item.leftPredLabelSet(st2_index);
		const TagSet & st2_rlabelset = st2_index == -1 ? empty_tagset : item.rightPredLabelSet(st2_index);
		const Word & st2lh_word = st2lh_word_postag.first();
		const POSTag & st2lh_postag = st2lh_word_postag.second();
		const int & st2lh_label = st2_index == -1 ? 0 : item.leftHeadLabel(st2_index);
		const int & st2lh_arity = st2_index == -1 ? -1 : item.leftHeadArity(st2_index);
		const Word & st2lp_word = st2lp_word_postag.first();
		const POSTag & st2lp_postag = st2lp_word_postag.second();
		const POSTag & st2lp2_postag = (st2_index == -1 || item.leftSubPred(st2_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(st2_index)].second();
		const int & st2lp_label = st2_index == -1 ? 0 : item.leftPredLabel(st2_index);
		const int & st2lp2_label = st2_index == -1 ? 0 : item.leftSubPredLabel(st2_index);
		const int & st2lp_arity = st2_index == -1 ? -1 : item.leftPredArity(st2_index);
		const Word & st2rh_word = st2rh_word_postag.first();
		const POSTag & st2rh_postag = st2rh_word_postag.second();
		const int & st2rh_label = st2_index == -1 ? 0 : item.rightHeadLabel(st2_index);
		const int & st2rh_arity = st2_index == -1 ? -1 : item.rightHeadArity(st2_index);
		const Word & st2rp_word = st2rp_word_postag.first();
		const POSTag & st2rp_postag = st2rp_word_postag.second();
		const POSTag & st2rp2_postag = (st2_index == -1 || item.rightSubPred(st2_index) == -1) ? empty_taggedword.second() : m_lSentence[item.rightSubPred(st2_index)].second();
		const int & st2rp_label = st2_index == -1 ? 0 : item.rightPredLabel(st2_index);
		const int & st2rp2_label = st2_index == -1 ? 0 : item.rightSubPredLabel(st2_index);
		const int & st2rp_arity = st2_index == -1 ? -1 : item.rightPredArity(st2_index);
		// sst
		const Word & sst_word = sst_word_postag.first();
		const POSTag & sst_postag = sst_word_postag.second();
		const TagSet & sst_llabelset = sst_index == -1 ? empty_tagset : item.leftPredLabelSet(sst_index);
		const TagSet & sst_rlabelset = sst_index == -1 ? empty_tagset : item.rightPredLabelSet(sst_index);
		const Word & sstlh_word = sstlh_word_postag.first();
		const POSTag & sstlh_postag = sstlh_word_postag.second();
		const int & sstlh_label = sst_index == -1 ? 0 : item.leftHeadLabel(sst_index);
		const int & sstlh_arity = sst_index == -1 ? -1 : item.leftHeadArity(sst_index);
		const Word & sstlp_word = sstlp_word_postag.first();
		const POSTag & sstlp_postag = sstlp_word_postag.second();
		const POSTag & sstlp2_postag = (sst_index == -1 || item.leftSubPred(sst_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(sst_index)].second();
		const int & sstlp_label = sst_index == -1 ? 0 : item.leftPredLabel(sst_index);
		const int & sstlp2_label = sst_index == -1 ? 0 : item.leftSubPredLabel(sst_index);
		const int & sstlp_arity = sst_index == -1 ? -1 : item.leftPredArity(sst_index);
		const Word & sstrh_word = sstrh_word_postag.first();
		const POSTag & sstrh_postag = sstrh_word_postag.second();
		const int & sstrh_label = sst_index == -1 ? 0 : item.rightHeadLabel(sst_index);
		const int & sstrh_arity = sst_index == -1 ? -1 : item.rightHeadArity(sst_index);
		const Word & sstrp_word = sstrp_word_postag.first();
		const POSTag & sstrp_postag = sstrp_word_postag.second();
		const POSTag & sstrp2_postag = (sst_index == -1 || item.rightSubPred(sst_index) == -1) ? empty_taggedword.second() : m_lSentence[item.rightSubPred(sst_index)].second();
		const int & sstrp_label = sst_index == -1 ? 0 : item.rightPredLabel(sst_index);
		const int & sstrp2_label = sst_index == -1 ? 0 : item.rightSubPredLabel(sst_index);
		const int & sstrp_arity = sst_index == -1 ? -1 : item.rightPredArity(sst_index);
		// bf
		const Word & bf_word = bf_word_postag.first();
		const POSTag & bf_postag = bf_word_postag.second();
		const TagSet & bf_llabelset = bf_index == -1 ? empty_tagset : item.leftPredLabelSet(bf_index);
		const TagSet & bf_rlabelset = bf_index == -1 ? empty_tagset : rItem.rightPredLabelSet(bf_index);
		const Word & bflh_word = bflh_word_postag.first();
		const POSTag & bflh_postag = bflh_word_postag.second();
		const int & bflh_label = bf_index == -1 ? 0 : item.leftHeadLabel(bf_index);
		const int & bflh_arity = bf_index == -1 ? -1 : item.leftHeadArity(bf_index);
		const Word & bflp_word = bflp_word_postag.first();
		const POSTag & bflp_postag = bflp_word_postag.second();
		const POSTag & bflp2_postag = (bf_index == -1 || item.leftSubPred(bf_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(bf_index)].second();
		const int & bflp_label = bf_index == -1 ? 0 : item.leftPredLabel(bf_index);
		const int & bflp2_label = bf_index == -1 ? 0 : item.leftSubPredLabel(bf_index);
		const int & bflp_arity = bf_index == -1 ? -1 : item.leftPredArity(bf_index);
		const Word & bfrh_word = bfrh_word_postag.first();
		const POSTag & bfrh_postag = bfrh_word_postag.second();
		const int & bfrh_label = bf_index == -1 ? 0 : rItem.rightHeadLabel(bf_index);
		const int & bfrh_arity = bf_index == -1 ? -1 : rItem.rightHeadArity(bf_index);
		const Word & bfrp_word = bfrp_word_postag.first();
		const POSTag & bfrp_postag = bfrp_word_postag.second();
		const POSTag & bfrp2_postag = (bf_index == -1 || rItem.rightSubPred(bf_index) == -1) ? empty_taggedword.second() : m_lSentence[rItem.rightSubPred(bf_index)].second();
		const int & bfrp_label = bf_index == -1 ? 0 : rItem.rightPredLabel(bf_index);
		const int & bfrp2_label = bf_index == -1 ? 0 : rItem.rightSubPredLabel(bf_index);
		const int & bfrp_arity = bf_index == -1 ? -1 : rItem.rightPredArity(bf_index);
		// bf2
		const Word & bf2_word = bf2_word_postag.first();
		const POSTag & bf2_postag = bf2_word_postag.second();
		const TagSet & bf2_llabelset = bf2_index == -1 ? empty_tagset : item.leftPredLabelSet(bf2_index);
		const TagSet & bf2_rlabelset = bf2_index == -1 ? empty_tagset : rItem.rightPredLabelSet(bf2_index);
		const Word & bf2lh_word = bf2lh_word_postag.first();
		const POSTag & bf2lh_postag = bf2lh_word_postag.second();
		const int & bf2lh_label = bf2_index == -1 ? 0 : item.leftHeadLabel(bf2_index);
		const int & bf2lh_arity = bf2_index == -1 ? -1 : item.leftHeadArity(bf2_index);
		const Word & bf2lp_word = bf2lp_word_postag.first();
		const POSTag & bf2lp_postag = bf2lp_word_postag.second();
		const POSTag & bf2lp2_postag = (bf2_index == -1 || item.leftSubPred(bf2_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(bf2_index)].second();
		const int & bf2lp_label = bf2_index == -1 ? 0 : item.leftPredLabel(bf2_index);
		const int & bf2lp2_label = bf2_index == -1 ? 0 : item.leftSubPredLabel(bf2_index);
		const int & bf2lp_arity = bf2_index == -1 ? -1 : item.leftPredArity(bf2_index);
		const Word & bf2rh_word = bf2rh_word_postag.first();
		const POSTag & bf2rh_postag = bf2rh_word_postag.second();
		const int & bf2rh_label = bf2_index == -1 ? 0 : rItem.rightHeadLabel(bf2_index);
		const int & bf2rh_arity = bf2_index == -1 ? -1 : rItem.rightHeadArity(bf2_index);
		const Word & bf2rp_word = bf2rp_word_postag.first();
		const POSTag & bf2rp_postag = bf2rp_word_postag.second();
		const POSTag & bf2rp2_postag = (bf2_index == -1 || rItem.rightSubPred(bf2_index) == -1) ? empty_taggedword.second() : m_lSentence[rItem.rightSubPred(bf2_index)].second();
		const int & bf2rp_label = bf2_index == -1 ? 0 : rItem.rightPredLabel(bf2_index);
		const int & bf2rp2_label = bf2_index == -1 ? 0 : rItem.rightSubPredLabel(bf2_index);
		const int & bf2rp_arity = bf2_index == -1 ? -1 : rItem.rightPredArity(bf2_index);
		// sbf
		const Word & sbf_word = sbf_word_postag.first();
		const POSTag & sbf_postag = sbf_word_postag.second();
		const TagSet & sbf_llabelset = sbf_index == -1 ? empty_tagset : item.leftPredLabelSet(sbf_index);
		const TagSet & sbf_rlabelset = sbf_index == -1 ? empty_tagset : rItem.rightPredLabelSet(sbf_index);
		const Word & sbflh_word = sbflh_word_postag.first();
		const POSTag & sbflh_postag = sbflh_word_postag.second();
		const int & sbflh_label = sbf_index == -1 ? 0 : item.leftHeadLabel(sbf_index);
		const int & sbflh_arity = sbf_index == -1 ? -1 : item.leftHeadArity(sbf_index);
		const Word & sbflp_word = sbflp_word_postag.first();
		const POSTag & sbflp_postag = sbflp_word_postag.second();
		const POSTag & sbflp2_postag = (sbf_index == -1 || item.leftSubPred(sbf_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(sbf_index)].second();
		const int & sbflp_label = sbf_index == -1 ? 0 : item.leftPredLabel(sbf_index);
		const int & sbflp2_label = sbf_index == -1 ? 0 : item.leftSubPredLabel(sbf_index);
		const int & sbflp_arity = sbf_index == -1 ? -1 : item.leftPredArity(sbf_index);
		const Word & sbfrh_word = sbfrh_word_postag.first();
		const POSTag & sbfrh_postag = sbfrh_word_postag.second();
		const int & sbfrh_label = sbf_index == -1 ? 0 : rItem.rightHeadLabel(sbf_index);
		const int & sbfrh_arity = sbf_index == -1 ? -1 : rItem.rightHeadArity(sbf_index);
		const Word & sbfrp_word = sbfrp_word_postag.first();
		const POSTag & sbfrp_postag = sbfrp_word_postag.second();
		const POSTag & sbfrp2_postag = (sbf_index == -1 || rItem.rightSubPred(sbf_index) == -1) ? empty_taggedword.second() : m_lSentence[rItem.rightSubPred(sbf_index)].second();
		const int & sbfrp_label = sbf_index == -1 ? 0 : rItem.rightPredLabel(sbf_index);
		const int & sbfrp2_label = sbf_index == -1 ? 0 : rItem.rightSubPredLabel(sbf_index);
		const int & sbfrp_arity = sbf_index == -1 ? -1 : rItem.rightPredArity(sbf_index);

		const Word & stl2_word = stl2_word_postag.first();
		const POSTag & stl2_postag = stl2_word_postag.second();
		const Word & stl1_word = stl1_word_postag.first();
		const POSTag & stl1_postag = stl1_word_postag.second();
		const Word & str1_word = str1_word_postag.first();
		const POSTag & str1_postag = str1_word_postag.second();
		const Word & str2_word = str2_word_postag.first();
		const POSTag & str2_postag = str2_word_postag.second();
		const Word & bfl2_word = bfl2_word_postag.first();
		const POSTag & bfl2_postag = bfl2_word_postag.second();
		const Word & bfl1_word = bfl1_word_postag.first();
		const POSTag & bfl1_postag = bfl1_word_postag.second();
		const Word & bfr1_word = bfr1_word_postag.first();
		const POSTag & bfr1_postag = bfr1_word_postag.second();
		const Word & bfr2_word = bfr2_word_postag.first();
		const POSTag & bfr2_postag = bfr2_word_postag.second();

		int dis = -1;

		// uni-gram
		// st, bf, st2, sst
		cweight->m_mapSTw.getOrUpdateScore(m_lPackedScore, st_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSTpt.getOrUpdateScore(m_lPackedScore, st_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapST2w.getOrUpdateScore(m_lPackedScore, st2_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapST2pt.getOrUpdateScore(m_lPackedScore, st2_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSSTw.getOrUpdateScore(m_lPackedScore, sst_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSSTpt.getOrUpdateScore(m_lPackedScore, sst_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapBFw.getOrUpdateScore(m_lPackedScore, bf_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapBFpt.getOrUpdateScore(m_lPackedScore, bf_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapBF2w.getOrUpdateScore(m_lPackedScore, bf2_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapBF2pt.getOrUpdateScore(m_lPackedScore, bf2_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSBFw.getOrUpdateScore(m_lPackedScore, sbf_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSBFpt.getOrUpdateScore(m_lPackedScore, sbf_postag, m_nScoreIndex, amount, m_nTrainingRound);

		// unigram context
		// st + sti, bf + bfi
		// st
		bi_features.refer(stl2_word, -2);
		cweight->m_mapSTiw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(stl1_word, -1);
		cweight->m_mapSTiw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(str1_word, 1);
		cweight->m_mapSTiw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(str2_word, 2);
		cweight->m_mapSTiw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stl2_word, stl1_word, -1);
		cweight->m_mapSTiwSTjw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stl1_word, str1_word, 0);
		cweight->m_mapSTiwSTjw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(str1_word, str2_word, 1);
		cweight->m_mapSTiwSTjw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bf
		bi_features.refer(bfl2_word, -2);
		cweight->m_mapBFiw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bfl1_word, -1);
		cweight->m_mapBFiw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bfr1_word, 1);
		cweight->m_mapBFiw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bfr2_word, 2);
		cweight->m_mapBFiw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bfl2_word, bfl1_word, -1);
		cweight->m_mapBFiwBFjw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bfl1_word, bfr1_word, 0);
		cweight->m_mapBFiwBFjw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bfr1_word, bfr2_word, 1);
		cweight->m_mapBFiwBFjw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st
		bi_features.refer(stl2_postag, -2);
		cweight->m_mapSTipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(stl1_postag, -1);
		cweight->m_mapSTipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(str1_postag, 1);
		cweight->m_mapSTipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(str2_postag, 2);
		cweight->m_mapSTipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(stl2_postag, stl1_postag), -1);
		cweight->m_mapSTiptSTjpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(stl1_postag, str1_postag), 0);
		cweight->m_mapSTiptSTjpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(str1_postag, str2_postag), 1);
		cweight->m_mapSTiptSTjpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bf
		bi_features.refer(bfl2_postag, -2);
		cweight->m_mapBFipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bfl1_postag, -1);
		cweight->m_mapBFipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bfr1_postag, 1);
		cweight->m_mapBFipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bfr2_postag, 2);
		cweight->m_mapBFipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(bfl2_postag, bfl1_postag), -1);
		cweight->m_mapBFiptBFjpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(bfl1_postag, bfr1_postag), 0);
		cweight->m_mapBFiptBFjpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(bfr1_postag, bfr2_postag), 1);
		cweight->m_mapBFiptBFjpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// unigram with label
		// st
		bi_features.refer(st_word, stlh_label);
		cweight->m_mapSTwSTLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, stlp_label);
		cweight->m_mapSTwSTLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, strh_label);
		cweight->m_mapSTwSTRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, strp_label);
		cweight->m_mapSTwSTRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2
		bi_features.refer(st2_word, st2lh_label);
		cweight->m_mapST2wST2LHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st2lp_label);
		cweight->m_mapST2wST2LPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st2rh_label);
		cweight->m_mapST2wST2RHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st2rp_label);
		cweight->m_mapST2wST2RPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst
		bi_features.refer(sst_word, sstlh_label);
		cweight->m_mapSSTwSSTLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, sstlp_label);
		cweight->m_mapSSTwSSTLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, sstrh_label);
		cweight->m_mapSSTwSSTRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, sstrp_label);
		cweight->m_mapSSTwSSTRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bf
		bi_features.refer(bf_word, bflh_label);
		cweight->m_mapBFwBFLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, bflp_label);
		cweight->m_mapBFwBFLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_postag, bflh_label);
		cweight->m_mapBFwBFRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_postag, bflp_label);
		cweight->m_mapBFwBFRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bf2
		bi_features.refer(bf2_word, bf2lh_label);
		cweight->m_mapBF2wBF2LHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, bf2lp_label);
		cweight->m_mapBF2wBF2LPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_postag, bf2lh_label);
		cweight->m_mapBF2wBF2RHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_postag, bf2lp_label);
		cweight->m_mapBF2wBF2RPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sbf
		bi_features.refer(sbf_word, sbflh_label);
		cweight->m_mapSBFwSBFLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, sbflp_label);
		cweight->m_mapSBFwSBFLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_postag, sbflh_label);
		cweight->m_mapSBFwSBFRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_postag, sbflp_label);
		cweight->m_mapSBFwSBFRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// unigram with arity
		// st, bf
		// st
		bi_features.refer(st_word, stlh_arity);
		cweight->m_mapSTwSTLHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, stlp_arity);
		cweight->m_mapSTwSTLPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, strh_arity);
		cweight->m_mapSTwSTRHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, strp_arity);
		cweight->m_mapSTwSTRPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, stlh_arity + strh_arity);
		cweight->m_mapSTwSTHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, stlp_arity + strp_arity);
		cweight->m_mapSTwSTPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2
		bi_features.refer(st2_word, st2lh_arity);
		cweight->m_mapST2wST2LHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st2lp_arity);
		cweight->m_mapST2wST2LPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st2rh_arity);
		cweight->m_mapST2wST2RHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st2rp_arity);
		cweight->m_mapST2wST2RPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st2lh_arity + st2rh_arity);
		cweight->m_mapST2wST2Hi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st2lp_arity + st2rp_arity);
		cweight->m_mapST2wST2Pi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst
		bi_features.refer(sst_word, sstlh_arity);
		cweight->m_mapSSTwSSTLHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, sstlp_arity);
		cweight->m_mapSSTwSSTLPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, sstrh_arity);
		cweight->m_mapSSTwSSTRHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, sstrp_arity);
		cweight->m_mapSSTwSSTRPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, sstlh_arity + sstrh_arity);
		cweight->m_mapSSTwSSTHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, sstlp_arity + sstrp_arity);
		cweight->m_mapSSTwSSTPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_postag, stlh_arity);
		// bf
		bi_features.refer(bf_word, bflh_arity);
		cweight->m_mapBFwBFLHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, bflp_arity);
		cweight->m_mapBFwBFLPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, bfrh_arity);
		cweight->m_mapBFwBFRHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, bfrp_arity);
		cweight->m_mapBFwBFRPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, bflh_arity + bfrh_arity);
		cweight->m_mapBFwBFHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, bflp_arity + bfrp_arity);
		cweight->m_mapBFwBFPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bf2
		bi_features.refer(bf2_word, bf2lh_arity);
		cweight->m_mapBF2wBF2LHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, bf2lp_arity);
		cweight->m_mapBF2wBF2LPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, bf2rh_arity);
		cweight->m_mapBF2wBF2RHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, bf2rp_arity);
		cweight->m_mapBF2wBF2RPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, bf2lh_arity + bf2rh_arity);
		cweight->m_mapBF2wBF2Hi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, bf2lp_arity + bf2rp_arity);
		cweight->m_mapBF2wBF2Pi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sbf
		bi_features.refer(sbf_word, sbflh_arity);
		cweight->m_mapSBFwSBFLHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, sbflp_arity);
		cweight->m_mapSBFwSBFLPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, sbfrh_arity);
		cweight->m_mapSBFwSBFRHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, sbfrp_arity);
		cweight->m_mapSBFwSBFRPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, sbflh_arity + sbfrh_arity);
		cweight->m_mapSBFwSBFHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, sbflp_arity + sbfrp_arity);
		cweight->m_mapSBFwSBFPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// bi-gram
		// st + bf
		bi_features.refer(st_word, bf_word);
		cweight->m_mapSTwBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, bf_postag);
		cweight->m_mapSTwBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, st_postag);
		cweight->m_mapSTptBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, bf_postag);
		cweight->m_mapSTptBFpt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf_word, st_postag);
		cweight->m_mapSTwptBFw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf_word, bf_postag);
		cweight->m_mapSTwBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, poses_feature);
		cweight->m_mapSTwptBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, poses_feature);
		cweight->m_mapSTptBFwpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf_word, poses_feature);
		cweight->m_mapSTwptBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf + dis
		dis = encodeLinkDistance(st_index, bf_index == -1 ? m_nSentenceLength : bf_index);
		tri_features.refer(st_word, bf_word, dis);
		cweight->m_mapSTwBFwD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf_postag, dis);
		cweight->m_mapSTwBFptD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, st_postag, dis);
		cweight->m_mapSTptBFwD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(poses_feature, dis);
		cweight->m_mapSTptBFptD.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf + st left/right head/pred
		tri_features.refer(st_word, bf_word, strp_arity);
		cweight->m_mapSTwBFwSTRPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf_word, bflp_arity);
		cweight->m_mapSTwBFwBFLPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + bf2
		bi_features.refer(st_word, bf2_word);
		cweight->m_mapSTwBF2w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, bf2_postag);
		cweight->m_mapSTwBF2pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, st_postag);
		cweight->m_mapSTptBF2w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, bf2_postag);
		cweight->m_mapSTptBF2pt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf2_word, st_postag);
		cweight->m_mapSTwptBF2w.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf2_word, bf2_postag);
		cweight->m_mapSTwBF2wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, poses_feature);
		cweight->m_mapSTwptBF2pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, poses_feature);
		cweight->m_mapSTptBF2wpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf2_word, poses_feature);
		cweight->m_mapSTwptBF2wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf2 + dis
		dis = encodeLinkDistance(st_index, bf2_index == -1 ? m_nSentenceLength : bf2_index);
		tri_features.refer(st_word, bf2_word, dis);
		cweight->m_mapSTwBF2wD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf2_postag, dis);
		cweight->m_mapSTwBF2ptD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2_word, st_postag, dis);
		cweight->m_mapSTptBF2wD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(poses_feature, dis);
		cweight->m_mapSTptBF2ptD.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf2 + st left/right head/pred
		tri_features.refer(st_word, bf2_word, strp_arity);
		cweight->m_mapSTwBF2wSTRPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, bf2_word, bf2lp_arity);
		cweight->m_mapSTwBF2wBF2LPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + sbf
		bi_features.refer(st_word, sbf_word);
		cweight->m_mapSTwSBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, sbf_postag);
		cweight->m_mapSTwSBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, st_postag);
		cweight->m_mapSTptSBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, sbf_postag);
		cweight->m_mapSTptSBFpt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, sbf_word, st_postag);
		cweight->m_mapSTwptSBFw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, sbf_word, sbf_postag);
		cweight->m_mapSTwSBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, poses_feature);
		cweight->m_mapSTwptSBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, poses_feature);
		cweight->m_mapSTptSBFwpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, sbf_word, poses_feature);
		cweight->m_mapSTwptSBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + sbf + dis
		dis = encodeLinkDistance(st_index, sbf_index == -1 ? m_nSentenceLength : sbf_index);
		tri_features.refer(st_word, sbf_word, dis);
		cweight->m_mapSTwSBFwD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, sbf_postag, dis);
		cweight->m_mapSTwSBFptD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbf_word, st_postag, dis);
		cweight->m_mapSTptSBFwD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(poses_feature, dis);
		cweight->m_mapSTptSBFptD.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + sbf + st left/right head/pred
		tri_features.refer(st_word, sbf_word, strp_arity);
		cweight->m_mapSTwSBFwSTRPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, sbf_word, sbflp_arity);
		cweight->m_mapSTwSBFwSBFLPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + bf
		bi_features.refer(st2_word, bf_word);
		cweight->m_mapST2wBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, bf_postag);
		cweight->m_mapST2wBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, st2_postag);
		cweight->m_mapST2ptBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st2_postag, bf_postag);
		cweight->m_mapST2ptBFpt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, bf_word, st2_postag);
		cweight->m_mapST2wptBFw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, bf_word, bf_postag);
		cweight->m_mapST2wBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, poses_feature);
		cweight->m_mapST2wptBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, poses_feature);
		cweight->m_mapST2ptBFwpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, bf_word, poses_feature);
		cweight->m_mapST2wptBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + bf + dis
		dis = encodeLinkDistance(st2_index, bf_index == -1 ? m_nSentenceLength : bf_index);
		tri_features.refer(st2_word, bf_word, dis);
		cweight->m_mapST2wBFwD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, bf_postag, dis);
		cweight->m_mapST2wBFptD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, st2_postag, dis);
		cweight->m_mapST2ptBFwD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(poses_feature, dis);
		cweight->m_mapST2ptBFptD.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + bf + st2 left/right head/pred
		tri_features.refer(st2_word, bf_word, st2rp_arity);
		cweight->m_mapST2wBFwST2RPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, bf_word, bflp_arity);
		cweight->m_mapST2wBFwBFLPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + bf
		bi_features.refer(sst_word, bf_word);
		cweight->m_mapSSTwBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, bf_postag);
		cweight->m_mapSSTwBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, sst_postag);
		cweight->m_mapSSTptBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(sst_postag, bf_postag);
		cweight->m_mapSSTptBFpt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, bf_word, sst_postag);
		cweight->m_mapSSTwptBFw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, bf_word, bf_postag);
		cweight->m_mapSSTwBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, poses_feature);
		cweight->m_mapSSTwptBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, poses_feature);
		cweight->m_mapSSTptBFwpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, bf_word, poses_feature);
		cweight->m_mapSSTwptBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + bf + dis
		dis = encodeLinkDistance(sst_index, bf_index == -1 ? m_nSentenceLength : bf_index);
		tri_features.refer(sst_word, bf_word, dis);
		cweight->m_mapSSTwBFwD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, bf_postag, dis);
		cweight->m_mapSSTwBFptD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, sst_postag, dis);
		cweight->m_mapSSTptBFwD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(poses_feature, dis);
		cweight->m_mapSSTptBFptD.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + bf + sst left/right head/pred
		tri_features.refer(sst_word, bf_word, sstrp_arity);
		cweight->m_mapSSTwBFwSSTRPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, bf_word, bflp_arity);
		cweight->m_mapSSTwBFwBFLPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + st2
		bi_features.refer(st_word, st2_word);
		cweight->m_mapSTwST2w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, st2_postag);
		cweight->m_mapSTwST2pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st_postag);
		cweight->m_mapSTptST2w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, st2_postag);
		cweight->m_mapSTptST2pt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, st2_word, st_postag);
		cweight->m_mapSTwptST2w.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, st2_word, st2_postag);
		cweight->m_mapSTwST2wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, poses_feature);
		cweight->m_mapSTwptST2pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, poses_feature);
		cweight->m_mapSTptST2wpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, st2_word, poses_feature);
		cweight->m_mapSTwptST2wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + sst
		bi_features.refer(st_word, sst_word);
		cweight->m_mapSTwSSTw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, sst_postag);
		cweight->m_mapSTwSSTpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, st_postag);
		cweight->m_mapSTptSSTw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, sst_postag);
		cweight->m_mapSTptSSTpt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, sst_word, st_postag);
		cweight->m_mapSTwptSSTw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, sst_word, sst_postag);
		cweight->m_mapSTwSSTwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, poses_feature);
		cweight->m_mapSTwptSSTpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, poses_feature);
		cweight->m_mapSTptSSTwpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, sst_word, poses_feature);
		cweight->m_mapSTwptSSTwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// bf + bf2
		bi_features.refer(bf_word, bf2_word);
		cweight->m_mapBFwBF2w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, bf2_postag);
		cweight->m_mapBFwBF2pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, bf_postag);
		cweight->m_mapBFptBF2w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(bf_postag, bf2_postag);
		cweight->m_mapBFptBF2pt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, bf2_word, bf_postag);
		cweight->m_mapBFwptBF2w.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, bf2_word, bf2_postag);
		cweight->m_mapBFwBF2wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, poses_feature);
		cweight->m_mapBFwptBF2pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf2_word, poses_feature);
		cweight->m_mapBFptBF2wpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, bf2_word, poses_feature);
		cweight->m_mapBFwptBF2wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// bf + sbf
		bi_features.refer(bf_word, sbf_word);
		cweight->m_mapBFwSBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, sbf_postag);
		cweight->m_mapBFwSBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, bf_postag);
		cweight->m_mapBFptSBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(bf_postag, sbf_postag);
		cweight->m_mapBFptSBFpt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, sbf_word, bf_postag);
		cweight->m_mapBFwptSBFw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, sbf_word, sbf_postag);
		cweight->m_mapBFwSBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(bf_word, poses_feature);
		cweight->m_mapBFwptSBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sbf_word, poses_feature);
		cweight->m_mapBFptSBFwpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, sbf_word, poses_feature);
		cweight->m_mapBFwptSBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + bf + st left/right head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlh_postag, bf_postag), stlh_label);
		cweight->m_mapSTwBFptSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st_postag, stlh_postag), stlh_label);
		cweight->m_mapSTptBFwSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stlh_word, ENCODE_POSTAG_SET_2(st_postag, bf_postag), stlh_label);
		cweight->m_mapSTptBFptSTLHwSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlh_postag, bf_postag), stlh_label);
		cweight->m_mapSTptBFptSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlp_postag, bf_postag), stlp_label);
		cweight->m_mapSTwBFptSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st_postag, stlp_postag), stlp_label);
		cweight->m_mapSTptBFwSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stlp_word, ENCODE_POSTAG_SET_2(st_postag, bf_postag), stlp_label);
		cweight->m_mapSTptBFptSTLPwSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlp_postag, bf_postag), stlp_label);
		cweight->m_mapSTptBFptSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strh_postag, bf_postag), strh_label);
		cweight->m_mapSTwBFptSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st_postag, strh_postag), strh_label);
		cweight->m_mapSTptBFwSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(strh_word, ENCODE_POSTAG_SET_2(st_postag, bf_postag), strh_label);
		cweight->m_mapSTptBFptSTRHwSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strh_postag, bf_postag), strh_label);
		cweight->m_mapSTptBFptSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strp_postag, bf_postag), strp_label);
		cweight->m_mapSTwBFptSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st_postag, strp_postag), strp_label);
		cweight->m_mapSTptBFwSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(strp_word, ENCODE_POSTAG_SET_2(st_postag, bf_postag), strp_label);
		cweight->m_mapSTptBFptSTRPwSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strp_postag, bf_postag), strp_label);
		cweight->m_mapSTptBFptSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + bf + bf left head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(bf_postag, bflh_postag), bflh_label);
		cweight->m_mapSTwBFptBFLHptBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st_postag, bflh_postag), bflh_label);
		cweight->m_mapSTptBFwBFLHptBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bflh_word, ENCODE_POSTAG_SET_2(st_postag, bf_postag), bflh_label);
		cweight->m_mapSTptBFptBFLHwBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, bf_postag, bflh_postag), bflh_label);
		cweight->m_mapSTptBFptBFLHptBFLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(bf_postag, bflp_postag), bflp_label);
		cweight->m_mapSTwBFptBFLPptBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st_postag, bflp_postag), bflp_label);
		cweight->m_mapSTptBFwBFLPptBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bflp_word, ENCODE_POSTAG_SET_2(st_postag, bf_postag), bflp_label);
		cweight->m_mapSTptBFptBFLPwBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, bf_postag, bflp_postag), bflp_label);
		cweight->m_mapSTptBFptBFLPptBFLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(bf_postag, bfrh_postag), bfrh_label);
		cweight->m_mapSTwBFptBFRHptBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st_postag, bfrh_postag), bfrh_label);
		cweight->m_mapSTptBFwBFRHptBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bfrh_word, ENCODE_POSTAG_SET_2(st_postag, bf_postag), bfrh_label);
		cweight->m_mapSTptBFptBFRHwBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, bf_postag, bfrh_postag), bfrh_label);
		cweight->m_mapSTptBFptBFRHptBFRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(bf_postag, bfrp_postag), bfrp_label);
		cweight->m_mapSTwBFptBFRPptBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st_postag, bfrp_postag), bfrp_label);
		cweight->m_mapSTptBFwBFRPptBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bfrp_word, ENCODE_POSTAG_SET_2(st_postag, bf_postag), bfrp_label);
		cweight->m_mapSTptBFptBFRPwBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, bf_postag, bfrp_postag), bfrp_label);
		cweight->m_mapSTptBFptBFRPptBFRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// st + bf + st right head + st right pred
		poses_feature = encodePOSTagSet4(st_postag, strh_postag, strp_postag, bf_postag);
		tri_features.refer(poses_feature, strh_label, strp_label);
		cweight->m_mapSTptBFptSTRHptSTRPptSTRHlSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf + st left pred + st left pred 2
		poses_feature = encodePOSTagSet4(st_postag, stlp_postag, stlp2_postag, bf_postag);
		tri_features.refer(poses_feature, stlp_label, stlp2_label);
		cweight->m_mapSTptBFptSTLPptSTLP2ptSTLPlSTLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf + st right pred + st right pred 2
		poses_feature = encodePOSTagSet4(st_postag, strp_postag, strp2_postag, bf_postag);
		tri_features.refer(poses_feature, strp_label, strp2_label);
		cweight->m_mapSTptBFptSTRPptSTRP2ptSTRPlSTRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf + bf left head + bf left pred
		poses_feature = encodePOSTagSet4(st_postag, bf_postag, bflh_postag, bflp_postag);
		tri_features.refer(poses_feature, bflh_label, bflp_label);
		cweight->m_mapSTptBFptBFLHptBFLPptBFLHlBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf + bf left pred + bf left pred 2
		poses_feature = encodePOSTagSet4(st_postag, bf_postag, bflp_postag, bflp2_postag);
		tri_features.refer(poses_feature, bflp_label, bflp2_label);
		cweight->m_mapSTptBFptBFLPptBFLP2ptBFLPlBFLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf + bf right pred + bf right pred 2
		poses_feature = encodePOSTagSet4(st_postag, bf_postag, bfrp_postag, bfrp2_postag);
		tri_features.refer(poses_feature, bfrp_label, bfrp2_label);
		cweight->m_mapSTptBFptBFRPptBFRP2ptBFRPlBFRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + bf2 + st left/right head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlh_postag, bf2_postag), stlh_label);
		cweight->m_mapSTwBF2ptSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2_word, ENCODE_POSTAG_SET_2(st_postag, stlh_postag), stlh_label);
		cweight->m_mapSTptBF2wSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stlh_word, ENCODE_POSTAG_SET_2(st_postag, bf2_postag), stlh_label);
		cweight->m_mapSTptBF2ptSTLHwSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlh_postag, bf2_postag), stlh_label);
		cweight->m_mapSTptBF2ptSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlp_postag, bf2_postag), stlp_label);
		cweight->m_mapSTwBF2ptSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2_word, ENCODE_POSTAG_SET_2(st_postag, stlp_postag), stlp_label);
		cweight->m_mapSTptBF2wSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stlp_word, ENCODE_POSTAG_SET_2(st_postag, bf2_postag), stlp_label);
		cweight->m_mapSTptBF2ptSTLPwSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlp_postag, bf2_postag), stlp_label);
		cweight->m_mapSTptBF2ptSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strh_postag, bf2_postag), strh_label);
		cweight->m_mapSTwBF2ptSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2_word, ENCODE_POSTAG_SET_2(st_postag, strh_postag), strh_label);
		cweight->m_mapSTptBF2wSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(strh_word, ENCODE_POSTAG_SET_2(st_postag, bf2_postag), strh_label);
		cweight->m_mapSTptBF2ptSTRHwSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strh_postag, bf2_postag), strh_label);
		cweight->m_mapSTptBF2ptSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strp_postag, bf2_postag), strp_label);
		cweight->m_mapSTwBF2ptSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2_word, ENCODE_POSTAG_SET_2(st_postag, strp_postag), strp_label);
		cweight->m_mapSTptBF2wSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(strp_word, ENCODE_POSTAG_SET_2(st_postag, bf2_postag), strp_label);
		cweight->m_mapSTptBF2ptSTRPwSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strp_postag, bf2_postag), strp_label);
		cweight->m_mapSTptBF2ptSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + bf2 + bf2 left head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(bf2_postag, bf2lh_postag), bf2lh_label);
		cweight->m_mapSTwBF2ptBF2LHptBF2LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2_word, ENCODE_POSTAG_SET_2(st_postag, bf2lh_postag), bf2lh_label);
		cweight->m_mapSTptBF2wBF2LHptBF2LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2lh_word, ENCODE_POSTAG_SET_2(st_postag, bf2_postag), bf2lh_label);
		cweight->m_mapSTptBF2ptBF2LHwBF2LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, bf2_postag, bf2lh_postag), bf2lh_label);
		cweight->m_mapSTptBF2ptBF2LHptBF2LHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(bf2_postag, bf2lp_postag), bf2lp_label);
		cweight->m_mapSTwBF2ptBF2LPptBF2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2_word, ENCODE_POSTAG_SET_2(st_postag, bf2lp_postag), bf2lp_label);
		cweight->m_mapSTptBF2wBF2LPptBF2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2lp_word, ENCODE_POSTAG_SET_2(st_postag, bf2_postag), bf2lp_label);
		cweight->m_mapSTptBF2ptBF2LPwBF2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, bf2_postag, bf2lp_postag), bf2lp_label);
		cweight->m_mapSTptBF2ptBF2LPptBF2LPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(bf2_postag, bf2rh_postag), bf2rh_label);
		cweight->m_mapSTwBF2ptBF2RHptBF2RHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2_word, ENCODE_POSTAG_SET_2(st_postag, bf2rh_postag), bf2rh_label);
		cweight->m_mapSTptBF2wBF2RHptBF2RHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2rh_word, ENCODE_POSTAG_SET_2(st_postag, bf2_postag), bf2rh_label);
		cweight->m_mapSTptBF2ptBF2RHwBF2RHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, bf2_postag, bf2rh_postag), bf2rh_label);
		cweight->m_mapSTptBF2ptBF2RHptBF2RHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(bf2_postag, bf2rp_postag), bf2rp_label);
		cweight->m_mapSTwBF2ptBF2RPptBF2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2_word, ENCODE_POSTAG_SET_2(st_postag, bf2rp_postag), bf2rp_label);
		cweight->m_mapSTptBF2wBF2RPptBF2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf2rp_word, ENCODE_POSTAG_SET_2(st_postag, bf2_postag), bf2rp_label);
		cweight->m_mapSTptBF2ptBF2RPwBF2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, bf2_postag, bf2rp_postag), bf2rp_label);
		cweight->m_mapSTptBF2ptBF2RPptBF2RPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// st + bf2 + st right head + st right pred
		poses_feature = encodePOSTagSet4(st_postag, strh_postag, strp_postag, bf2_postag);
		tri_features.refer(poses_feature, strh_label, strp_label);
		cweight->m_mapSTptBF2ptSTRHptSTRPptSTRHlSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf2 + st left pred + st left pred 2
		poses_feature = encodePOSTagSet4(st_postag, stlp_postag, stlp2_postag, bf2_postag);
		tri_features.refer(poses_feature, stlp_label, stlp2_label);
		cweight->m_mapSTptBF2ptSTLPptSTLP2ptSTLPlSTLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf2 + st right pred + st right pred 2
		poses_feature = encodePOSTagSet4(st_postag, strp_postag, strp2_postag, bf2_postag);
		tri_features.refer(poses_feature, strp_label, strp2_label);
		cweight->m_mapSTptBF2ptSTRPptSTRP2ptSTRPlSTRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf2 + bf2 left head + bf2 left pred
		poses_feature = encodePOSTagSet4(st_postag, bf2_postag, bf2lh_postag, bf2lp_postag);
		tri_features.refer(poses_feature, bf2lh_label, bf2lp_label);
		cweight->m_mapSTptBF2ptBF2LHptBF2LPptBF2LHlBF2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf2 + bf2 left pred + bf2 left pred 2
		poses_feature = encodePOSTagSet4(st_postag, bf2_postag, bf2lp_postag, bf2lp2_postag);
		tri_features.refer(poses_feature, bf2lp_label, bf2lp2_label);
		cweight->m_mapSTptBF2ptBF2LPptBF2LP2ptBF2LPlBF2LP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + bf2 + bf2 right pred + bf2 right pred 2
		poses_feature = encodePOSTagSet4(st_postag, bf2_postag, bf2rp_postag, bf2rp2_postag);
		tri_features.refer(poses_feature, bf2rp_label, bf2rp2_label);
		cweight->m_mapSTptBF2ptBF2RPptBF2RP2ptBF2RPlBF2RP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + sbf + st left/right head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlh_postag, sbf_postag), stlh_label);
		cweight->m_mapSTwSBFptSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbf_word, ENCODE_POSTAG_SET_2(st_postag, stlh_postag), stlh_label);
		cweight->m_mapSTptSBFwSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stlh_word, ENCODE_POSTAG_SET_2(st_postag, sbf_postag), stlh_label);
		cweight->m_mapSTptSBFptSTLHwSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlh_postag, sbf_postag), stlh_label);
		cweight->m_mapSTptSBFptSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlp_postag, sbf_postag), stlp_label);
		cweight->m_mapSTwSBFptSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbf_word, ENCODE_POSTAG_SET_2(st_postag, stlp_postag), stlp_label);
		cweight->m_mapSTptSBFwSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stlp_word, ENCODE_POSTAG_SET_2(st_postag, sbf_postag), stlp_label);
		cweight->m_mapSTptSBFptSTLPwSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlp_postag, sbf_postag), stlp_label);
		cweight->m_mapSTptSBFptSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strh_postag, sbf_postag), strh_label);
		cweight->m_mapSTwSBFptSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbf_word, ENCODE_POSTAG_SET_2(st_postag, strh_postag), strh_label);
		cweight->m_mapSTptSBFwSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(strh_word, ENCODE_POSTAG_SET_2(st_postag, sbf_postag), strh_label);
		cweight->m_mapSTptSBFptSTRHwSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strh_postag, sbf_postag), strh_label);
		cweight->m_mapSTptSBFptSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strp_postag, sbf_postag), strp_label);
		cweight->m_mapSTwSBFptSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbf_word, ENCODE_POSTAG_SET_2(st_postag, strp_postag), strp_label);
		cweight->m_mapSTptSBFwSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(strp_word, ENCODE_POSTAG_SET_2(st_postag, sbf_postag), strp_label);
		cweight->m_mapSTptSBFptSTRPwSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strp_postag, sbf_postag), strp_label);
		cweight->m_mapSTptSBFptSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + sbf + sbf left head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(sbf_postag, sbflh_postag), sbflh_label);
		cweight->m_mapSTwSBFptSBFLHptSBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbf_word, ENCODE_POSTAG_SET_2(st_postag, sbflh_postag), sbflh_label);
		cweight->m_mapSTptSBFwSBFLHptSBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbflh_word, ENCODE_POSTAG_SET_2(st_postag, sbf_postag), sbflh_label);
		cweight->m_mapSTptSBFptSBFLHwSBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, sbf_postag, sbflh_postag), sbflh_label);
		cweight->m_mapSTptSBFptSBFLHptSBFLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(sbf_postag, sbflp_postag), sbflp_label);
		cweight->m_mapSTwSBFptSBFLPptSBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbf_word, ENCODE_POSTAG_SET_2(st_postag, sbflp_postag), sbflp_label);
		cweight->m_mapSTptSBFwSBFLPptSBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbflp_word, ENCODE_POSTAG_SET_2(st_postag, sbf_postag), sbflp_label);
		cweight->m_mapSTptSBFptSBFLPwSBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, sbf_postag, sbflp_postag), sbflp_label);
		cweight->m_mapSTptSBFptSBFLPptSBFLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(sbf_postag, sbfrh_postag), sbfrh_label);
		cweight->m_mapSTwSBFptSBFRHptSBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbf_word, ENCODE_POSTAG_SET_2(st_postag, sbfrh_postag), sbfrh_label);
		cweight->m_mapSTptSBFwSBFRHptSBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbfrh_word, ENCODE_POSTAG_SET_2(st_postag, sbf_postag), sbfrh_label);
		cweight->m_mapSTptSBFptSBFRHwSBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, sbf_postag, sbfrh_postag), sbfrh_label);
		cweight->m_mapSTptSBFptSBFRHptSBFRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(sbf_postag, sbfrp_postag), sbfrp_label);
		cweight->m_mapSTwSBFptSBFRPptSBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbf_word, ENCODE_POSTAG_SET_2(st_postag, sbfrp_postag), sbfrp_label);
		cweight->m_mapSTptSBFwSBFRPptSBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sbfrp_word, ENCODE_POSTAG_SET_2(st_postag, sbf_postag), sbfrp_label);
		cweight->m_mapSTptSBFptSBFRPwSBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, sbf_postag, sbfrp_postag), sbfrp_label);
		cweight->m_mapSTptSBFptSBFRPptSBFRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// st + sbf + st right head + st right pred
		poses_feature = encodePOSTagSet4(st_postag, strh_postag, strp_postag, sbf_postag);
		tri_features.refer(poses_feature, strh_label, strp_label);
		cweight->m_mapSTptSBFptSTRHptSTRPptSTRHlSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + sbf + st left pred + st left pred 2
		poses_feature = encodePOSTagSet4(st_postag, stlp_postag, stlp2_postag, sbf_postag);
		tri_features.refer(poses_feature, stlp_label, stlp2_label);
		cweight->m_mapSTptSBFptSTLPptSTLP2ptSTLPlSTLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + sbf + st right pred + st right pred 2
		poses_feature = encodePOSTagSet4(st_postag, strp_postag, strp2_postag, sbf_postag);
		tri_features.refer(poses_feature, strp_label, strp2_label);
		cweight->m_mapSTptSBFptSTRPptSTRP2ptSTRPlSTRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + sbf + sbf left head + sbf left pred
		poses_feature = encodePOSTagSet4(st_postag, sbf_postag, sbflh_postag, sbflp_postag);
		tri_features.refer(poses_feature, sbflh_label, sbflp_label);
		cweight->m_mapSTptSBFptSBFLHptSBFLPptSBFLHlSBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + sbf + sbf left pred + sbf left pred 2
		poses_feature = encodePOSTagSet4(st_postag, sbf_postag, sbflp_postag, sbflp2_postag);
		tri_features.refer(poses_feature, sbflp_label, sbflp2_label);
		cweight->m_mapSTptSBFptSBFLPptSBFLP2ptSBFLPlSBFLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + sbf + sbf right pred + sbf right pred 2
		poses_feature = encodePOSTagSet4(st_postag, sbf_postag, sbfrp_postag, sbfrp2_postag);
		tri_features.refer(poses_feature, sbfrp_label, sbfrp2_label);
		cweight->m_mapSTptSBFptSBFRPptSBFRP2ptSBFRPlSBFRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + bf + st2 left/right head/pred
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2lh_postag, bf_postag), st2lh_label);
		cweight->m_mapST2wBFptST2LHptST2LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st2_postag, st2lh_postag), st2lh_label);
		cweight->m_mapST2ptBFwST2LHptST2LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2lh_word, ENCODE_POSTAG_SET_2(st2_postag, bf_postag), st2lh_label);
		cweight->m_mapST2ptBFptST2LHwST2LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2lh_postag, bf_postag), st2lh_label);
		cweight->m_mapST2ptBFptST2LHptST2LHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2lp_postag, bf_postag), st2lp_label);
		cweight->m_mapST2wBFptST2LPptST2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st2_postag, st2lp_postag), st2lp_label);
		cweight->m_mapST2ptBFwST2LPptST2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2lp_word, ENCODE_POSTAG_SET_2(st2_postag, bf_postag), st2lp_label);
		cweight->m_mapST2ptBFptST2LPwST2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2lp_postag, bf_postag), st2lp_label);
		cweight->m_mapST2ptBFptST2LPptST2LPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2rh_postag, bf_postag), st2rh_label);
		cweight->m_mapST2wBFptST2RHptST2RHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st2_postag, st2rh_postag), st2rh_label);
		cweight->m_mapST2ptBFwST2RHptST2RHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2rh_word, ENCODE_POSTAG_SET_2(st2_postag, bf_postag), st2rh_label);
		cweight->m_mapST2ptBFptST2RHwST2RHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2rh_postag, bf_postag), st2rh_label);
		cweight->m_mapST2ptBFptST2RHptST2RHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2rp_postag, bf_postag), st2rp_label);
		cweight->m_mapST2wBFptST2RPptST2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st2_postag, st2rp_postag), st2rp_label);
		cweight->m_mapST2ptBFwST2RPptST2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2rp_word, ENCODE_POSTAG_SET_2(st2_postag, bf_postag), st2rp_label);
		cweight->m_mapST2ptBFptST2RPwST2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2rp_postag, bf_postag), st2rp_label);
		cweight->m_mapST2ptBFptST2RPptST2RPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + bf + bf left head/pred
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(bf_postag, bflh_postag), bflh_label);
		cweight->m_mapST2wBFptBFLHptBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st2_postag, bflh_postag), bflh_label);
		cweight->m_mapST2ptBFwBFLHptBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bflh_word, ENCODE_POSTAG_SET_2(st2_postag, bf_postag), bflh_label);
		cweight->m_mapST2ptBFptBFLHwBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, bf_postag, bflh_postag), bflh_label);
		cweight->m_mapST2ptBFptBFLHptBFLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(bf_postag, bflp_postag), bflp_label);
		cweight->m_mapST2wBFptBFLPptBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st2_postag, bflp_postag), bflp_label);
		cweight->m_mapST2ptBFwBFLPptBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bflp_word, ENCODE_POSTAG_SET_2(st2_postag, bf_postag), bflp_label);
		cweight->m_mapST2ptBFptBFLPwBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, bf_postag, bflp_postag), bflp_label);
		cweight->m_mapST2ptBFptBFLPptBFLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(bf_postag, bfrh_postag), bfrh_label);
		cweight->m_mapST2wBFptBFRHptBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st2_postag, bfrh_postag), bfrh_label);
		cweight->m_mapST2ptBFwBFRHptBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bfrh_word, ENCODE_POSTAG_SET_2(st2_postag, bf_postag), bfrh_label);
		cweight->m_mapST2ptBFptBFRHwBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, bf_postag, bfrh_postag), bfrh_label);
		cweight->m_mapST2ptBFptBFRHptBFRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(bf_postag, bfrp_postag), bfrp_label);
		cweight->m_mapST2wBFptBFRPptBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(st2_postag, bfrp_postag), bfrp_label);
		cweight->m_mapST2ptBFwBFRPptBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bfrp_word, ENCODE_POSTAG_SET_2(st2_postag, bf_postag), bfrp_label);
		cweight->m_mapST2ptBFptBFRPwBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, bf_postag, bfrp_postag), bfrp_label);
		cweight->m_mapST2ptBFptBFRPptBFRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// st2 + bf + st2 right head + st2 right pred
		poses_feature = encodePOSTagSet4(st2_postag, st2rh_postag, st2rp_postag, bf_postag);
		tri_features.refer(poses_feature, st2rh_label, st2rp_label);
		cweight->m_mapST2ptBFptST2RHptST2RPptST2RHlST2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + bf + st2 left pred + st2 left pred 2
		poses_feature = encodePOSTagSet4(st2_postag, st2lp_postag, st2lp2_postag, bf_postag);
		tri_features.refer(poses_feature, st2lp_label, st2lp2_label);
		cweight->m_mapST2ptBFptST2LPptST2LP2ptST2LPlST2LP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + bf + st2 right pred + st2 right pred 2
		poses_feature = encodePOSTagSet4(st2_postag, st2rp_postag, st2rp2_postag, bf_postag);
		tri_features.refer(poses_feature, st2rp_label, st2rp2_label);
		cweight->m_mapST2ptBFptST2RPptST2RP2ptST2RPlST2RP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + bf + bf left head + bf left pred
		poses_feature = encodePOSTagSet4(st2_postag, bf_postag, bflh_postag, bflp_postag);
		tri_features.refer(poses_feature, bflh_label, bflp_label);
		cweight->m_mapST2ptBFptBFLHptBFLPptBFLHlBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + bf + bf left pred + bf left pred 2
		poses_feature = encodePOSTagSet4(st2_postag, bf_postag, bflp_postag, bflp2_postag);
		tri_features.refer(poses_feature, bflp_label, bflp2_label);
		cweight->m_mapST2ptBFptBFLPptBFLP2ptBFLPlBFLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + bf + bf right pred + bf right pred 2
		poses_feature = encodePOSTagSet4(st2_postag, bf_postag, bfrp_postag, bfrp2_postag);
		tri_features.refer(poses_feature, bfrp_label, bfrp2_label);
		cweight->m_mapST2ptBFptBFRPptBFRP2ptBFRPlBFRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + bf + sst left/right head/pred
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstlh_postag, bf_postag), sstlh_label);
		cweight->m_mapSSTwBFptSSTLHptSSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(sst_postag, sstlh_postag), sstlh_label);
		cweight->m_mapSSTptBFwSSTLHptSSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sstlh_word, ENCODE_POSTAG_SET_2(sst_postag, bf_postag), sstlh_label);
		cweight->m_mapSSTptBFptSSTLHwSSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstlh_postag, bf_postag), sstlh_label);
		cweight->m_mapSSTptBFptSSTLHptSSTLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstlp_postag, bf_postag), sstlp_label);
		cweight->m_mapSSTwBFptSSTLPptSSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(sst_postag, sstlp_postag), sstlp_label);
		cweight->m_mapSSTptBFwSSTLPptSSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sstlp_word, ENCODE_POSTAG_SET_2(sst_postag, bf_postag), sstlp_label);
		cweight->m_mapSSTptBFptSSTLPwSSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstlp_postag, bf_postag), sstlp_label);
		cweight->m_mapSSTptBFptSSTLPptSSTLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstrh_postag, bf_postag), sstrh_label);
		cweight->m_mapSSTwBFptSSTRHptSSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(sst_postag, sstrh_postag), sstrh_label);
		cweight->m_mapSSTptBFwSSTRHptSSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sstrh_word, ENCODE_POSTAG_SET_2(sst_postag, bf_postag), sstrh_label);
		cweight->m_mapSSTptBFptSSTRHwSSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstrh_postag, bf_postag), sstrh_label);
		cweight->m_mapSSTptBFptSSTRHptSSTRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstrp_postag, bf_postag), sstrp_label);
		cweight->m_mapSSTwBFptSSTRPptSSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(sst_postag, sstrp_postag), sstrp_label);
		cweight->m_mapSSTptBFwSSTRPptSSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sstrp_word, ENCODE_POSTAG_SET_2(sst_postag, bf_postag), sstrp_label);
		cweight->m_mapSSTptBFptSSTRPwSSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstrp_postag, bf_postag), sstrp_label);
		cweight->m_mapSSTptBFptSSTRPptSSTRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + bf + bf left head/pred
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(bf_postag, bflh_postag), bflh_label);
		cweight->m_mapSSTwBFptBFLHptBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(sst_postag, bflh_postag), bflh_label);
		cweight->m_mapSSTptBFwBFLHptBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bflh_word, ENCODE_POSTAG_SET_2(sst_postag, bf_postag), bflh_label);
		cweight->m_mapSSTptBFptBFLHwBFLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, bf_postag, bflh_postag), bflh_label);
		cweight->m_mapSSTptBFptBFLHptBFLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(bf_postag, bflp_postag), bflp_label);
		cweight->m_mapSSTwBFptBFLPptBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(sst_postag, bflp_postag), bflp_label);
		cweight->m_mapSSTptBFwBFLPptBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bflp_word, ENCODE_POSTAG_SET_2(sst_postag, bf_postag), bflp_label);
		cweight->m_mapSSTptBFptBFLPwBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, bf_postag, bflp_postag), bflp_label);
		cweight->m_mapSSTptBFptBFLPptBFLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(bf_postag, bfrh_postag), bfrh_label);
		cweight->m_mapSSTwBFptBFRHptBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(sst_postag, bfrh_postag), bfrh_label);
		cweight->m_mapSSTptBFwBFRHptBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bfrh_word, ENCODE_POSTAG_SET_2(sst_postag, bf_postag), bfrh_label);
		cweight->m_mapSSTptBFptBFRHwBFRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, bf_postag, bfrh_postag), bfrh_label);
		cweight->m_mapSSTptBFptBFRHptBFRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(bf_postag, bfrp_postag), bfrp_label);
		cweight->m_mapSSTwBFptBFRPptBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bf_word, ENCODE_POSTAG_SET_2(sst_postag, bfrp_postag), bfrp_label);
		cweight->m_mapSSTptBFwBFRPptBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(bfrp_word, ENCODE_POSTAG_SET_2(sst_postag, bf_postag), bfrp_label);
		cweight->m_mapSSTptBFptBFRPwBFRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, bf_postag, bfrp_postag), bfrp_label);
		cweight->m_mapSSTptBFptBFRPptBFRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// sst + bf + sst right head + sst right pred
		poses_feature = encodePOSTagSet4(sst_postag, sstrh_postag, sstrp_postag, bf_postag);
		tri_features.refer(poses_feature, sstrh_label, sstrp_label);
		cweight->m_mapSSTptBFptSSTRHptSSTRPptSSTRHlSSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + bf + sst left pred + sst left pred 2
		poses_feature = encodePOSTagSet4(sst_postag, sstlp_postag, sstlp2_postag, bf_postag);
		tri_features.refer(poses_feature, sstlp_label, sstlp2_label);
		cweight->m_mapSSTptBFptSSTLPptSSTLP2ptSSTLPlSSTLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + bf + sst right pred + sst right pred 2
		poses_feature = encodePOSTagSet4(sst_postag, sstrp_postag, sstrp2_postag, bf_postag);
		tri_features.refer(poses_feature, sstrp_label, sstrp2_label);
		cweight->m_mapSSTptBFptSSTRPptSSTRP2ptSSTRPlSSTRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + bf + bf left head + bf left pred
		poses_feature = encodePOSTagSet4(sst_postag, bf_postag, bflh_postag, bflp_postag);
		tri_features.refer(poses_feature, bflh_label, bflp_label);
		cweight->m_mapSSTptBFptBFLHptBFLPptBFLHlBFLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + bf + bf left pred + bf left pred 2
		poses_feature = encodePOSTagSet4(sst_postag, bf_postag, bflp_postag, bflp2_postag);
		tri_features.refer(poses_feature, bflp_label, bflp2_label);
		cweight->m_mapSSTptBFptBFLPptBFLP2ptBFLPlBFLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + bf + bf left pred + bf left pred 2
		poses_feature = encodePOSTagSet4(sst_postag, bf_postag, bfrp_postag, bfrp2_postag);
		tri_features.refer(poses_feature, bfrp_label, bfrp2_label);
		cweight->m_mapSSTptBFptBFRPptBFRP2ptBFRPlBFRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + bf + labelset
		uni_tagset.refer(st_word, st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTwSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st_word, st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTwSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(bf_word, bf_llabelset.bits(0), bf_llabelset.bits(1));
		cweight->m_mapBFwBFll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(bf_word, bf_rlabelset.bits(0), bf_rlabelset.bits(1));
		cweight->m_mapBFwBFrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st_postag, st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTptSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st_postag, st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTptSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(bf_postag, bf_llabelset.bits(0), bf_llabelset.bits(1));
		cweight->m_mapBFptBFll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(bf_postag, bf_rlabelset.bits(0), bf_rlabelset.bits(1));
		cweight->m_mapBFptBFrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, bf_postag), st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTptBFptSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, bf_postag), st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTptBFptSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, bf_postag), bf_llabelset.bits(0), bf_llabelset.bits(1));
		cweight->m_mapSTptBFptBFll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, bf_postag), bf_rlabelset.bits(0), bf_rlabelset.bits(1));
		cweight->m_mapSTptBFptBFrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, bf2_postag), st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTptBF2ptSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, bf2_postag), st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTptBF2ptSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, bf2_postag), bf2_llabelset.bits(0), bf2_llabelset.bits(1));
		cweight->m_mapSTptBF2ptBF2ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, bf2_postag), bf2_rlabelset.bits(0), bf2_rlabelset.bits(1));
		cweight->m_mapSTptBF2ptBF2rl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, sbf_postag), st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTptSBFptSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, sbf_postag), st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTptSBFptSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, sbf_postag), sbf_llabelset.bits(0), sbf_llabelset.bits(1));
		cweight->m_mapSTptSBFptSBFll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, sbf_postag), sbf_rlabelset.bits(0), sbf_rlabelset.bits(1));
		cweight->m_mapSTptSBFptSBFrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + bf + labelset
		uni_tagset.refer(st2_word, st2_llabelset.bits(0), st2_llabelset.bits(1));
		cweight->m_mapST2wST2ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st2_word, st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		cweight->m_mapST2wST2rl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st2_postag, st2_llabelset.bits(0), st2_llabelset.bits(1));
		cweight->m_mapST2ptST2ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st2_postag, st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		cweight->m_mapST2ptST2rl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(bf2_word, bf2_llabelset.bits(0), bf2_llabelset.bits(1));
		cweight->m_mapBF2wBF2ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(bf2_word, bf2_rlabelset.bits(0), bf2_rlabelset.bits(1));
		cweight->m_mapBF2wBF2rl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(bf2_postag, bf2_llabelset.bits(0), bf2_llabelset.bits(1));
		cweight->m_mapBF2ptBF2ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(bf2_postag, bf2_rlabelset.bits(0), bf2_rlabelset.bits(1));
		cweight->m_mapBF2ptBF2rl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st2_postag, bf_postag), st2_llabelset.bits(0), st2_llabelset.bits(1));
		cweight->m_mapST2ptBFptST2ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st2_postag, bf_postag), st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		cweight->m_mapST2ptBFptST2rl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st2_postag, bf_postag), bf_llabelset.bits(0), bf_llabelset.bits(1));
		cweight->m_mapST2ptBFptBFll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st2_postag, bf_postag), bf_rlabelset.bits(0), bf_rlabelset.bits(1));
		cweight->m_mapST2ptBFptBFrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + bf + labelset
		uni_tagset.refer(sst_word, sst_llabelset.bits(0), sst_llabelset.bits(1));
		cweight->m_mapSSTwSSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sst_word, sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		cweight->m_mapSSTwSSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sst_postag, sst_llabelset.bits(0), sst_llabelset.bits(1));
		cweight->m_mapSSTptSSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sst_postag, sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		cweight->m_mapSSTptSSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sbf_word, sbf_llabelset.bits(0), sbf_llabelset.bits(1));
		cweight->m_mapSBFwSBFll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sbf_word, sbf_rlabelset.bits(0), sbf_rlabelset.bits(1));
		cweight->m_mapSBFwSBFrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sbf_postag, sbf_llabelset.bits(0), sbf_llabelset.bits(1));
		cweight->m_mapSBFptSBFll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sbf_postag, sbf_rlabelset.bits(0), sbf_rlabelset.bits(1));
		cweight->m_mapSBFptSBFrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(sst_postag, bf_postag), sst_llabelset.bits(0), sst_llabelset.bits(1));
		cweight->m_mapSSTptBFptSSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(sst_postag, bf_postag), sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		cweight->m_mapSSTptBFptSSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(sst_postag, bf_postag), bf_llabelset.bits(0), bf_llabelset.bits(1));
		cweight->m_mapSSTptBFptBFll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(sst_postag, bf_postag), bf_rlabelset.bits(0), bf_rlabelset.bits(1));
		cweight->m_mapSSTptBFptBFrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);

		if (m_bChar) {
			cweight->m_map1CharBeforeST.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, st_index == -1 ? 0 : st_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharBeforeST.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, st_index == -1 ? 0 : st_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharBeforeST.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, st_index == -1 ? 0 : st_index, 3), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map1CharAfterST.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, st_index == -1 ? 0 : st_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharAfterST.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, st_index == -1 ? 0 : st_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharAfterST.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, st_index == -1 ? 0 : st_index, 3), m_nScoreIndex, amount, m_nTrainingRound);

			cweight->m_map1CharBeforeBF.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, bf_index == -1 ? m_nSentenceLength : bf_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharBeforeBF.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, bf_index == -1 ? m_nSentenceLength : bf_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharBeforeBF.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, bf_index == -1 ? m_nSentenceLength : bf_index, 3), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map1CharAfterBF.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, bf_index == -1 ? m_nSentenceLength : bf_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharAfterBF.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, bf_index == -1 ? m_nSentenceLength : bf_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharAfterBF.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, bf_index == -1 ? m_nSentenceLength : bf_index, 3), m_nScoreIndex, amount, m_nTrainingRound);
		}

		if (m_bPath) {

			const WordPOSTag & st_syn_head_word_postag = (st_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[st_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[st_index])]);
			const WordPOSTag & st2_syn_head_word_postag = (st2_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[st2_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[st2_index])]);
			const WordPOSTag & sst_syn_head_word_postag = (sst_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[sst_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[sst_index])]);
			const WordPOSTag & bf_syn_head_word_postag = (bf_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[bf_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[bf_index])]);
			const WordPOSTag & bf2_syn_head_word_postag = (bf2_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[bf2_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[bf2_index])]);
			const WordPOSTag & sbf_syn_head_word_postag = (sbf_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[sbf_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[sbf_index])]);

			if (st_index == -1 || bf_index == -1) {
				cweight->m_mapSTPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSTFPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapSTPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.POSPath[st_index][bf_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSTFPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.FPOSPath[st_index][bf_index], m_nScoreIndex, amount, m_nTrainingRound);
			}
			tri_features.refer(st_word, st_syn_head_word_postag.first(), bf_postag);
			cweight->m_mapSTwBFptSTsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, bf_postag, st_syn_head_word_postag.second());
			cweight->m_mapSTwBFwSTsynhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(bf_word, st_syn_head_word_postag.first(), st_postag);
			cweight->m_mapSTptBFwSTsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, bf_syn_head_word_postag.first(), bf_postag);
			cweight->m_mapSTwBFptBFsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, bf_postag, bf_syn_head_word_postag.second());
			cweight->m_mapSTwBFwBFsynhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(bf_word, bf_syn_head_word_postag.first(), st_postag);
			cweight->m_mapSTptBFwBFsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

			if (st_index == -1 || bf2_index == -1) {
				cweight->m_mapBF2POSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapBF2FPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapBF2POSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.POSPath[st_index][bf2_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapBF2FPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.FPOSPath[st_index][bf2_index], m_nScoreIndex, amount, m_nTrainingRound);
			}
			tri_features.refer(st_word, st_syn_head_word_postag.first(), bf2_postag);
			cweight->m_mapSTwBF2ptSTsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, bf2_postag, st_syn_head_word_postag.second());
			cweight->m_mapSTwBF2wSTsynhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(bf2_word, st_syn_head_word_postag.first(), st_postag);
			cweight->m_mapSTptBF2wSTsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, bf2_syn_head_word_postag.first(), bf2_postag);
			cweight->m_mapSTwBF2ptBF2synhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, bf2_postag, bf2_syn_head_word_postag.second());
			cweight->m_mapSTwBF2wBF2synhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(bf2_word, bf2_syn_head_word_postag.first(), st_postag);
			cweight->m_mapSTptBF2wBF2synhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

			if (st_index == -1 || sbf_index == -1) {
				cweight->m_mapSBFPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSBFFPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapSBFPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.POSPath[st_index][sbf_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSBFFPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.FPOSPath[st_index][sbf_index], m_nScoreIndex, amount, m_nTrainingRound);
			}
			tri_features.refer(st_word, st_syn_head_word_postag.first(), sbf_postag);
			cweight->m_mapSTwSBFptSTsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, sbf_postag, st_syn_head_word_postag.second());
			cweight->m_mapSTwSBFwSTsynhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(sbf_word, st_syn_head_word_postag.first(), st_postag);
			cweight->m_mapSTptSBFwSTsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, sbf_syn_head_word_postag.first(), sbf_postag);
			cweight->m_mapSTwSBFptSBFsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, sbf_postag, sbf_syn_head_word_postag.second());
			cweight->m_mapSTwSBFwSBFsynhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(sbf_word, sbf_syn_head_word_postag.first(), st_postag);
			cweight->m_mapSTptSBFwSBFsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

			if (st2_index == -1 || bf_index == -1) {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.POSPath[st2_index][bf_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.FPOSPath[st2_index][bf_index], m_nScoreIndex, amount, m_nTrainingRound);
			}
			tri_features.refer(st2_word, st2_syn_head_word_postag.first(), bf_postag);
			cweight->m_mapST2wBFptST2synhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st2_word, bf_postag, st2_syn_head_word_postag.second());
			cweight->m_mapST2wBFwST2synhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(bf_word, st2_syn_head_word_postag.first(), st2_postag);
			cweight->m_mapST2ptBFwST2synhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st2_word, bf_syn_head_word_postag.first(), bf_postag);
			cweight->m_mapST2wBFptBFsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st2_word, bf_postag, bf_syn_head_word_postag.second());
			cweight->m_mapST2wBFwBFsynhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(bf_word, bf_syn_head_word_postag.first(), st2_postag);
			cweight->m_mapST2ptBFwBFsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

			if (sst_index == -1 || bf_index == -1) {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapSSTPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.POSPath[sst_index][bf_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSSTFPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.FPOSPath[sst_index][bf_index], m_nScoreIndex, amount, m_nTrainingRound);
			}
			tri_features.refer(sst_word, sst_syn_head_word_postag.first(), bf_postag);
			cweight->m_mapSSTwBFptSSTsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(sst_word, bf_postag, sst_syn_head_word_postag.second());
			cweight->m_mapSSTwBFwSSTsynhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(bf_word, sst_syn_head_word_postag.first(), sst_postag);
			cweight->m_mapSSTptBFwSSTsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(sst_word, bf_syn_head_word_postag.first(), bf_postag);
			cweight->m_mapSSTwBFptBFsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(sst_word, bf_postag, bf_syn_head_word_postag.second());
			cweight->m_mapSSTwBFwBFsynhpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(bf_word, bf_syn_head_word_postag.first(), sst_postag);
			cweight->m_mapSSTptBFwBFsynhw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		}

		if (m_bSuperTag) {
			const SuperTag & st_supertag = st_index == -1 ? 0 : item.superTag(st_index);
			const SuperTag & st2_supertag = st2_index == -1 ? 0 : item.superTag(st2_index);
			const SuperTag & sst_supertag = sst_index == -1 ? 0 : item.superTag(sst_index);
			const SuperTag & bfl2_supertag = bfl2_index == -1 ? 0 : item.superTag(bfl2_index);
			const SuperTag & bfl1_supertag = bfl1_index == -1 ? 0 : item.superTag(bfl1_index);

			cweight->m_mapSTst.getOrUpdateScore(m_lPackedScore, st_supertag, m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_mapST2st.getOrUpdateScore(m_lPackedScore, st2_supertag, m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_mapSSTst.getOrUpdateScore(m_lPackedScore, sst_supertag, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(bfl2_supertag, -2);
			cweight->m_mapBFist.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(bfl1_supertag, -1);
			cweight->m_mapBFist.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(st_supertag, st2_supertag);
			cweight->m_mapSTstST2st.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(st_supertag, sst_supertag);
			cweight->m_mapSTstSSTst.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(st_supertag, bf_word);
			cweight->m_mapSTstBFw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(st_supertag, bf_postag);
			cweight->m_mapSTstBFpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_supertag, bf_word, bf_postag);
			cweight->m_mapSTstBFwpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		}
	}
}

#endif
