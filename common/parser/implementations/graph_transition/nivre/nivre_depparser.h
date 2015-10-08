#ifndef _NIVRE_DEPPARSER_H
#define _NIVRE_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "nivre_state.h"
#include "nivre_weight.h"
#include "common/parser/implementations/graph_transition/graph_transition_depparser_base.h"

namespace nivre {

	template<class RET_TYPE>
	class DepParser : public GraphDepParserBase<StateItem> {
	protected:
		RET_TYPE m_lPackedScore;

		UNSIGNED poses_feature;
		Int uni_feature;
		TwoInts bi_features;
		ThreeInts tri_features;
		FourInts quar_features;
		IntTagSet uni_tagset;
		TwoIntsTagSet bi_tagset;

		void swap(const tscore & score);
		void arcSwap(const tscore & score);
		void reduce(const tscore & score);
		void arcReduce(const tscore & score);
		void shift(const tscore & score, const int & tokenId);
		void arcShift(const tscore & score, const int & tokenId);

		void getActionScores(const StateItem & item);
		void getOrUpdateFeatureScore(const StateItem & item, const AddScoreType & amount);

	public:
		DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, int nState, const bool & bChar, const bool & bPath, const bool & bSTag);
		~DepParser();

		void decode() override;
	};

	extern int A_SW_FIRST;
	extern int A_RE_FIRST;
	extern int A_SH_FIRST;
	extern int SH_FIRST;

	extern int A_SW_END;
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
	inline void DepParser<RET_TYPE>::swap(const tscore & score) {
		m_abScores.insertItem(ScoredAction(SWAP, score + m_lPackedScore[SWAP]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::arcSwap(const tscore & score) {
		for (int action = A_SW_FIRST; action < A_SW_END; ++action) {
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
					for (int action = A_SH_FIRST + tag * LABEL_COUNT, n = action + LABEL_COUNT; action < n; ++action) {
						m_abScores.insertItem(ScoredAction(action, score + m_lPackedScore[action]));
					}
				}
			}
			else {
				for (const auto & tag : g_mapSuperTagCandidatesOfPOSTags[m_lSentence[tokenId].second()]) {
					for (int action = A_SH_FIRST + tag * LABEL_COUNT, n = action + LABEL_COUNT; action < n; ++action) {
						m_abScores.insertItem(ScoredAction(action, score + m_lPackedScore[action]));
					}
				}
			}
		}
		else {
			for (int action = A_SH_FIRST, n = A_SH_FIRST + LABEL_COUNT; action < n; ++action) {
				m_abScores.insertItem(ScoredAction(action, score + m_lPackedScore[action]));
			}
		}
	}


	template<class RET_TYPE>
	void DepParser<RET_TYPE>::getActionScores(const StateItem & item) {
		memset(m_lPackedScore, 0, sizeof(m_lPackedScore));
		getOrUpdateFeatureScore(item, AddScoreType(ACTION_START, 0));
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
					arcShift(score, iGenerator->size());
					if (iGenerator->canSwap()) {
						arcSwap(score);
					}
				}
				shift(score, iGenerator->size());
			}

			if (iGenerator->canSwap()) {
				swap(score);
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
	void DepParser<RET_TYPE>::getOrUpdateFeatureScore(const StateItem & item, const AddScoreType & amount) {

		Weight<RET_TYPE> * cweight = (Weight<RET_TYPE>*)m_Weight;

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
		const int & sst_index = item.bufferTop() == item.size() ? -1 : item.bufferTop();
		const int & sstlh_index = sst_index == -1 ? -1 : item.leftHead(sst_index);
		const int & sstlp_index = sst_index == -1 ? -1 : item.leftPred(sst_index);
		const int & sstrh_index = sst_index == -1 ? -1 : item.rightHead(sst_index);
		const int & sstrp_index = sst_index == -1 ? -1 : item.rightPred(sst_index);
		// n0
		const int & n0_index = item.size() < m_nSentenceLength ? item.size() : -1;
		const int & n0lp_index = n0_index == -1 ? -1 : item.leftPred(n0_index);
		const int & n0lh_index = n0_index == -1 ? -1 : item.leftHead(n0_index);
		// context
		const int & stl2_index = st_index >= 2 ? st_index - 2 : -1;
		const int & stl1_index = st_index >= 1 ? st_index - 1 : -1;
		const int & str1_index = st_index + 1 < m_nSentenceLength ? st_index + 1 : -1;
		const int & str2_index = st_index + 2 < m_nSentenceLength ? st_index + 2 : -1;
		const int & n0l2_index = n0_index >= 2 ? n0_index - 2 : -1;
		const int & n0l1_index = n0_index >= 1 ? n0_index - 1 : -1;
		const int & n0r1_index = n0_index + 1 < m_nSentenceLength ? n0_index + 1 : -1;
		const int & n0r2_index = n0_index + 2 < m_nSentenceLength ? n0_index + 2 : -1;

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
		// n0
		const WordPOSTag & n0_word_postag = n0_index == -1 ? end_taggedword : m_lSentence[n0_index];
		const WordPOSTag & n0lh_word_postag = n0lh_index == -1 ? empty_taggedword : m_lSentence[n0lh_index];
		const WordPOSTag & n0lp_word_postag = n0lp_index == -1 ? empty_taggedword : m_lSentence[n0lp_index];
		// context
		const WordPOSTag & stl2_word_postag = stl2_index == -1 ? start_taggedword : m_lSentence[stl2_index];
		const WordPOSTag & stl1_word_postag = stl1_index == -1 ? start_taggedword : m_lSentence[stl1_index];
		const WordPOSTag & str1_word_postag = str1_index == -1 ? end_taggedword : m_lSentence[str1_index];
		const WordPOSTag & str2_word_postag = str2_index == -1 ? end_taggedword : m_lSentence[str2_index];
		const WordPOSTag & n0l2_word_postag = n0l2_index == -1 ? start_taggedword : m_lSentence[n0l2_index];
		const WordPOSTag & n0l1_word_postag = n0l1_index == -1 ? start_taggedword : m_lSentence[n0l1_index];
		const WordPOSTag & n0r1_word_postag = n0r1_index == -1 ? end_taggedword : m_lSentence[n0r1_index];
		const WordPOSTag & n0r2_word_postag = n0r2_index == -1 ? end_taggedword : m_lSentence[n0r2_index];

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
		// n0
		const Word & n0_word = n0_word_postag.first();
		const POSTag & n0_postag = n0_word_postag.second();
		const TagSet & n0_llabelset = n0_index == -1 ? empty_tagset : item.leftPredLabelSet(n0_index);
		const Word & n0lh_word = n0lh_word_postag.first();
		const POSTag & n0lh_postag = n0lh_word_postag.second();
		const int & n0lh_label = n0_index == -1 ? 0 : item.leftHeadLabel(n0_index);
		const int & n0lh_arity = n0_index == -1 ? -1 : item.leftHeadArity(n0_index);
		const Word & n0lp_word = n0lp_word_postag.first();
		const POSTag & n0lp_postag = n0lp_word_postag.second();
		const POSTag & n0lp2_postag = (n0_index == -1 || item.leftSubPred(n0_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(n0_index)].second();
		const int & n0lp_label = n0_index == -1 ? 0 : item.leftPredLabel(n0_index);
		const int & n0lp2_label = n0_index == -1 ? 0 : item.leftSubPredLabel(n0_index);
		const int & n0lp_arity = n0_index == -1 ? -1 : item.leftPredArity(n0_index);
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

		int dis = -1;

		// uni-gram
		// st, n0, st2, sst
		cweight->m_mapSTw.getOrUpdateScore(m_lPackedScore, st_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSTpt.getOrUpdateScore(m_lPackedScore, st_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapST2w.getOrUpdateScore(m_lPackedScore, st2_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapST2pt.getOrUpdateScore(m_lPackedScore, st2_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSSTw.getOrUpdateScore(m_lPackedScore, sst_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSSTpt.getOrUpdateScore(m_lPackedScore, sst_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN0w.getOrUpdateScore(m_lPackedScore, n0_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN0pt.getOrUpdateScore(m_lPackedScore, n0_postag, m_nScoreIndex, amount, m_nTrainingRound);

		// unigram context
		// st + sti, n0 + n0i
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
		// n0
		bi_features.refer(n0l2_word, -2);
		cweight->m_mapN0iw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0l1_word, -1);
		cweight->m_mapN0iw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0r1_word, 1);
		cweight->m_mapN0iw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0r2_word, 2);
		cweight->m_mapN0iw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0l2_word, n0l1_word, -1);
		cweight->m_mapN0iwN0jw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0l1_word, n0r1_word, 0);
		cweight->m_mapN0iwN0jw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0r1_word, n0r2_word, 1);
		cweight->m_mapN0iwN0jw.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

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
		// n0
		bi_features.refer(n0l2_postag, -2);
		cweight->m_mapN0ipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0l1_postag, -1);
		cweight->m_mapN0ipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0r1_postag, 1);
		cweight->m_mapN0ipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0r2_postag, 2);
		cweight->m_mapN0ipt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(n0l2_postag, n0l1_postag), -1);
		cweight->m_mapN0iptN0jpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(n0l1_postag, n0r1_postag), 0);
		cweight->m_mapN0iptN0jpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(n0r1_postag, n0r2_postag), 1);
		cweight->m_mapN0iptN0jpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

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
		// n0
		bi_features.refer(n0_word, n0lh_label);
		cweight->m_mapN0wN0LHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, n0lp_label);
		cweight->m_mapN0wN0LPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
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

		// unigram with arity
		// st, n0
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
		bi_features.refer(st_postag, stlh_arity);
		// n0
		bi_features.refer(n0_word, n0lh_arity);
		cweight->m_mapN0wN0LHi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, n0lp_arity);
		cweight->m_mapN0wN0LPi.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
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

		// bi-gram
		// st + n0
		bi_features.refer(st_word, n0_word);
		cweight->m_mapSTwN0w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, n0_postag);
		cweight->m_mapSTwN0pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, st_postag);
		cweight->m_mapSTptN0w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, n0_postag);
		cweight->m_mapSTptN0pt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, n0_word, st_postag);
		cweight->m_mapSTwptN0w.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, n0_word, n0_postag);
		cweight->m_mapSTwN0wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, poses_feature);
		cweight->m_mapSTwptN0pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, poses_feature);
		cweight->m_mapSTptN0wpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, n0_word, poses_feature);
		cweight->m_mapSTwptN0wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + dis
		dis = encodeLinkDistance(st_index, n0_index == -1 ? m_nSentenceLength : n0_index);
		tri_features.refer(st_word, n0_word, dis);
		cweight->m_mapSTwN0wD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, n0_postag, dis);
		cweight->m_mapSTwN0ptD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, st_postag, dis);
		cweight->m_mapSTptN0wD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(poses_feature, dis);
		cweight->m_mapSTptN0ptD.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + st left/right head/pred
		tri_features.refer(st_word, n0_word, strp_arity);
		cweight->m_mapSTwN0wSTRPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, n0_word, n0lp_arity);
		cweight->m_mapSTwN0wN0LPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + n0
		bi_features.refer(st2_word, n0_word);
		cweight->m_mapST2wN0w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, n0_postag);
		cweight->m_mapST2wN0pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, st2_postag);
		cweight->m_mapST2ptN0w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(st2_postag, n0_postag);
		cweight->m_mapST2ptN0pt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, n0_word, st2_postag);
		cweight->m_mapST2wptN0w.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, n0_word, n0_postag);
		cweight->m_mapST2wN0wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, poses_feature);
		cweight->m_mapST2wptN0pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, poses_feature);
		cweight->m_mapST2ptN0wpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, n0_word, poses_feature);
		cweight->m_mapST2wptN0wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + dis
		dis = encodeLinkDistance(st2_index, n0_index == -1 ? m_nSentenceLength : n0_index);
		tri_features.refer(st2_word, n0_word, dis);
		cweight->m_mapST2wN0wD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, n0_postag, dis);
		cweight->m_mapST2wN0ptD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, st2_postag, dis);
		cweight->m_mapST2ptN0wD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(poses_feature, dis);
		cweight->m_mapST2ptN0ptD.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + st2 left/right head/pred
		tri_features.refer(st2_word, n0_word, st2rp_arity);
		cweight->m_mapST2wN0wST2RPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, n0_word, n0lp_arity);
		cweight->m_mapST2wN0wN0LPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + n0
		bi_features.refer(sst_word, n0_word);
		cweight->m_mapSSTwN0w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, n0_postag);
		cweight->m_mapSSTwN0pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, sst_postag);
		cweight->m_mapSSTptN0w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_2(sst_postag, n0_postag);
		cweight->m_mapSSTptN0pt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, n0_word, sst_postag);
		cweight->m_mapSSTwptN0w.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, n0_word, n0_postag);
		cweight->m_mapSSTwN0wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, poses_feature);
		cweight->m_mapSSTwptN0pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, poses_feature);
		cweight->m_mapSSTptN0wpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, n0_word, poses_feature);
		cweight->m_mapSSTwptN0wpt.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + dis
		dis = encodeLinkDistance(sst_index, n0_index == -1 ? m_nSentenceLength : n0_index);
		tri_features.refer(sst_word, n0_word, dis);
		cweight->m_mapSSTwN0wD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, n0_postag, dis);
		cweight->m_mapSSTwN0ptD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, sst_postag, dis);
		cweight->m_mapSSTptN0wD.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(poses_feature, dis);
		cweight->m_mapSSTptN0ptD.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + sst left/right head/pred
		tri_features.refer(sst_word, n0_word, sstrp_arity);
		cweight->m_mapSSTwN0wSSTRPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, n0_word, n0lp_arity);
		cweight->m_mapSSTwN0wN0LPi.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

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

		// tri-gram
		// st + n0 + st2
		bi_features.refer(st_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag));
		cweight->m_mapSTwN0ptST2pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, st2_postag));
		cweight->m_mapSTptN0wST2pt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag));
		cweight->m_mapSTptN0ptST2w.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_3(st_postag, st2_postag, n0_postag);
		cweight->m_mapSTptN0ptST2pt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + sst
		bi_features.refer(st_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag));
		cweight->m_mapSTwN0ptSSTpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, sst_postag));
		cweight->m_mapSTptN0wSSTpt.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag));
		cweight->m_mapSTptN0ptSSTw.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		poses_feature = ENCODE_POSTAG_SET_3(st_postag, sst_postag, n0_postag);
		cweight->m_mapSTptN0ptSSTpt.getOrUpdateScore(m_lPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);

		// st + n0 + st left/right head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlh_postag, n0_postag), stlh_label);
		cweight->m_mapSTwN0ptSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, stlh_postag), stlh_label);
		cweight->m_mapSTptN0wSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stlh_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), stlh_label);
		cweight->m_mapSTptN0ptSTLHwSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlh_postag, n0_postag), stlh_label);
		cweight->m_mapSTptN0ptSTLHptSTLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(stlp_postag, n0_postag), stlp_label);
		cweight->m_mapSTwN0ptSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, stlp_postag), stlp_label);
		cweight->m_mapSTptN0wSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(stlp_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), stlp_label);
		cweight->m_mapSTptN0ptSTLPwSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlp_postag, n0_postag), stlp_label);
		cweight->m_mapSTptN0ptSTLPptSTLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strh_postag, n0_postag), strh_label);
		cweight->m_mapSTwN0ptSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, strh_postag), strh_label);
		cweight->m_mapSTptN0wSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(strh_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), strh_label);
		cweight->m_mapSTptN0ptSTRHwSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strh_postag, n0_postag), strh_label);
		cweight->m_mapSTptN0ptSTRHptSTRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(strp_postag, n0_postag), strp_label);
		cweight->m_mapSTwN0ptSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, strp_postag), strp_label);
		cweight->m_mapSTptN0wSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(strp_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), strp_label);
		cweight->m_mapSTptN0ptSTRPwSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strp_postag, n0_postag), strp_label);
		cweight->m_mapSTptN0ptSTRPptSTRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + n0 + n0 left head/pred
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(n0_postag, n0lh_postag), n0lh_label);
		cweight->m_mapSTwN0ptN0LHptN0LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, n0lh_postag), n0lh_label);
		cweight->m_mapSTptN0wN0LHptN0LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0lh_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), n0lh_label);
		cweight->m_mapSTptN0ptN0LHwN0LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, n0_postag, n0lh_postag), n0lh_label);
		cweight->m_mapSTptN0ptN0LHptN0LHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st_word, ENCODE_POSTAG_SET_2(n0_postag, n0lp_postag), n0lp_label);
		cweight->m_mapSTwN0ptN0LPptN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st_postag, n0lp_postag), n0lp_label);
		cweight->m_mapSTptN0wN0LPptN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0lp_word, ENCODE_POSTAG_SET_2(st_postag, n0_postag), n0lp_label);
		cweight->m_mapSTptN0ptN0LPwN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, n0_postag, n0lp_postag), n0lp_label);
		cweight->m_mapSTptN0ptN0LPptN0LPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// st + n0 + st right head + st right pred
		poses_feature = encodePOSTagSet4(st_postag, strh_postag, strp_postag, n0_postag);
		tri_features.refer(poses_feature, strh_label, strp_label);
		cweight->m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + st left pred + st left pred 2
		poses_feature = encodePOSTagSet4(st_postag, stlp_postag, stlp2_postag, n0_postag);
		tri_features.refer(poses_feature, stlp_label, stlp2_label);
		cweight->m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + st right pred + st right pred 2
		poses_feature = encodePOSTagSet4(st_postag, strp_postag, strp2_postag, n0_postag);
		tri_features.refer(poses_feature, strp_label, strp2_label);
		cweight->m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + n0 left head + n0 left pred
		poses_feature = encodePOSTagSet4(st_postag, n0_postag, n0lh_postag, n0lp_postag);
		tri_features.refer(poses_feature, n0lh_label, n0lp_label);
		cweight->m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + n0 left pred + n0 left pred 2
		poses_feature = encodePOSTagSet4(st_postag, n0_postag, n0lp_postag, n0lp2_postag);
		tri_features.refer(poses_feature, n0lp_label, n0lp2_label);
		cweight->m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + n0 + st2 left/right head/pred
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2lh_postag, n0_postag), st2lh_label);
		cweight->m_mapST2wN0ptST2LHptST2LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, st2lh_postag), st2lh_label);
		cweight->m_mapST2ptN0wST2LHptST2LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2lh_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2lh_label);
		cweight->m_mapST2ptN0ptST2LHwST2LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2lh_postag, n0_postag), st2lh_label);
		cweight->m_mapST2ptN0ptST2LHptST2LHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2lp_postag, n0_postag), st2lp_label);
		cweight->m_mapST2wN0ptST2LPptST2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, st2lp_postag), st2lp_label);
		cweight->m_mapST2ptN0wST2LPptST2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2lp_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2lp_label);
		cweight->m_mapST2ptN0ptST2LPwST2LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2lp_postag, n0_postag), st2lp_label);
		cweight->m_mapST2ptN0ptST2LPptST2LPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2rh_postag, n0_postag), st2rh_label);
		cweight->m_mapST2wN0ptST2RHptST2RHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, st2rh_postag), st2rh_label);
		cweight->m_mapST2ptN0wST2RHptST2RHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2rh_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2rh_label);
		cweight->m_mapST2ptN0ptST2RHwST2RHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2rh_postag, n0_postag), st2rh_label);
		cweight->m_mapST2ptN0ptST2RHptST2RHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(st2rp_postag, n0_postag), st2rp_label);
		cweight->m_mapST2wN0ptST2RPptST2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, st2rp_postag), st2rp_label);
		cweight->m_mapST2ptN0wST2RPptST2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2rp_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2rp_label);
		cweight->m_mapST2ptN0ptST2RPwST2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2rp_postag, n0_postag), st2rp_label);
		cweight->m_mapST2ptN0ptST2RPptST2RPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + n0 + n0 left head/pred
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(n0_postag, n0lh_postag), n0lh_label);
		cweight->m_mapST2wN0ptN0LHptN0LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, n0lh_postag), n0lh_label);
		cweight->m_mapST2ptN0wN0LHptN0LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0lh_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), n0lh_label);
		cweight->m_mapST2ptN0ptN0LHwN0LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, n0_postag, n0lh_postag), n0lh_label);
		cweight->m_mapST2ptN0ptN0LHptN0LHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(st2_word, ENCODE_POSTAG_SET_2(n0_postag, n0lp_postag), n0lp_label);
		cweight->m_mapST2wN0ptN0LPptN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(st2_postag, n0lp_postag), n0lp_label);
		cweight->m_mapST2ptN0wN0LPptN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0lp_word, ENCODE_POSTAG_SET_2(st2_postag, n0_postag), n0lp_label);
		cweight->m_mapST2ptN0ptN0LPwN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, n0_postag, n0lp_postag), n0lp_label);
		cweight->m_mapST2ptN0ptN0LPptN0LPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		poses_feature = encodePOSTagSet4(st2_postag, st2rh_postag, st2rp_postag, n0_postag);
		tri_features.refer(poses_feature, st2rh_label, st2rp_label);
		cweight->m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + st2 left pred + st2 left pred 2
		poses_feature = encodePOSTagSet4(st2_postag, st2lp_postag, st2lp2_postag, n0_postag);
		tri_features.refer(poses_feature, st2lp_label, st2lp2_label);
		cweight->m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + st2 right pred + st2 right pred 2
		poses_feature = encodePOSTagSet4(st2_postag, st2rp_postag, st2rp2_postag, n0_postag);
		tri_features.refer(poses_feature, st2rp_label, st2rp2_label);
		cweight->m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + n0 left head + n0 left pred
		poses_feature = encodePOSTagSet4(st2_postag, n0_postag, n0lh_postag, n0lp_postag);
		tri_features.refer(poses_feature, n0lh_label, n0lp_label);
		cweight->m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + n0 left pred + n0 left pred 2
		poses_feature = encodePOSTagSet4(st2_postag, n0_postag, n0lp_postag, n0lp2_postag);
		tri_features.refer(poses_feature, n0lp_label, n0lp2_label);
		cweight->m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + n0 + sst left/right head/pred
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstlh_postag, n0_postag), sstlh_label);
		cweight->m_mapSSTwN0ptSSTLHptSSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, sstlh_postag), sstlh_label);
		cweight->m_mapSSTptN0wSSTLHptSSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sstlh_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sstlh_label);
		cweight->m_mapSSTptN0ptSSTLHwSSTLHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstlh_postag, n0_postag), sstlh_label);
		cweight->m_mapSSTptN0ptSSTLHptSSTLHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstlp_postag, n0_postag), sstlp_label);
		cweight->m_mapSSTwN0ptSSTLPptSSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, sstlp_postag), sstlp_label);
		cweight->m_mapSSTptN0wSSTLPptSSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sstlp_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sstlp_label);
		cweight->m_mapSSTptN0ptSSTLPwSSTLPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstlp_postag, n0_postag), sstlp_label);
		cweight->m_mapSSTptN0ptSSTLPptSSTLPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstrh_postag, n0_postag), sstrh_label);
		cweight->m_mapSSTwN0ptSSTRHptSSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, sstrh_postag), sstrh_label);
		cweight->m_mapSSTptN0wSSTRHptSSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sstrh_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sstrh_label);
		cweight->m_mapSSTptN0ptSSTRHwSSTRHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstrh_postag, n0_postag), sstrh_label);
		cweight->m_mapSSTptN0ptSSTRHptSSTRHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(sstrp_postag, n0_postag), sstrp_label);
		cweight->m_mapSSTwN0ptSSTRPptSSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, sstrp_postag), sstrp_label);
		cweight->m_mapSSTptN0wSSTRPptSSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sstrp_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sstrp_label);
		cweight->m_mapSSTptN0ptSSTRPwSSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstrp_postag, n0_postag), sstrp_label);
		cweight->m_mapSSTptN0ptSSTRPptSSTRPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + n0 + n0 left head/pred
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(n0_postag, n0lh_postag), n0lh_label);
		cweight->m_mapSSTwN0ptN0LHptN0LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, n0lh_postag), n0lh_label);
		cweight->m_mapSSTptN0wN0LHptN0LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0lh_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), n0lh_label);
		cweight->m_mapSSTptN0ptN0LHwN0LHl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, n0_postag, n0lh_postag), n0lh_label);
		cweight->m_mapSSTptN0ptN0LHptN0LHl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(sst_word, ENCODE_POSTAG_SET_2(n0_postag, n0lp_postag), n0lp_label);
		cweight->m_mapSSTwN0ptN0LPptN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0_word, ENCODE_POSTAG_SET_2(sst_postag, n0lp_postag), n0lp_label);
		cweight->m_mapSSTptN0wN0LPptN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		tri_features.refer(n0lp_word, ENCODE_POSTAG_SET_2(sst_postag, n0_postag), n0lp_label);
		cweight->m_mapSSTptN0ptN0LPwN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, n0_postag, n0lp_postag), n0lp_label);
		cweight->m_mapSSTptN0ptN0LPptN0LPl.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		poses_feature = encodePOSTagSet4(sst_postag, sstrh_postag, sstrp_postag, n0_postag);
		tri_features.refer(poses_feature, sstrh_label, sstrp_label);
		cweight->m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + sst left pred + sst left pred 2
		poses_feature = encodePOSTagSet4(sst_postag, sstlp_postag, sstlp2_postag, n0_postag);
		tri_features.refer(poses_feature, sstlp_label, sstlp2_label);
		cweight->m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + sst right pred + sst right pred 2
		poses_feature = encodePOSTagSet4(sst_postag, sstrp_postag, sstrp2_postag, n0_postag);
		tri_features.refer(poses_feature, sstrp_label, sstrp2_label);
		cweight->m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + n0 left head + n0 left pred
		poses_feature = encodePOSTagSet4(sst_postag, n0_postag, n0lh_postag, n0lp_postag);
		tri_features.refer(poses_feature, n0lh_label, n0lp_label);
		cweight->m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + n0 left pred + n0 left pred 2
		poses_feature = encodePOSTagSet4(sst_postag, n0_postag, n0lp_postag, n0lp2_postag);
		tri_features.refer(poses_feature, n0lp_label, n0lp2_label);
		cweight->m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.getOrUpdateScore(m_lPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + n0 + labelset
		uni_tagset.refer(st_word, st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTwSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st_word, st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTwSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(n0_word, n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapN0wN0ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st_postag, st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTptSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st_postag, st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTptSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(n0_postag, n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapN0ptN0ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, n0_postag), st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTptN0ptSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, n0_postag), st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTptN0ptSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st_postag, n0_postag), n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapSTptN0ptN0ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + n0 + labelset
		uni_tagset.refer(st2_word, st2_llabelset.bits(0), st2_llabelset.bits(1));
		cweight->m_mapST2wST2ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st2_word, st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		cweight->m_mapST2wST2rl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st2_postag, st2_llabelset.bits(0), st2_llabelset.bits(1));
		cweight->m_mapST2ptST2ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(st2_postag, st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		cweight->m_mapST2ptST2rl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2_llabelset.bits(0), st2_llabelset.bits(1));
		cweight->m_mapST2ptN0ptST2ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st2_postag, n0_postag), st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		cweight->m_mapST2ptN0ptST2rl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(st2_postag, n0_postag), n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapST2ptN0ptN0ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + n0 + labelset
		uni_tagset.refer(sst_word, sst_llabelset.bits(0), sst_llabelset.bits(1));
		cweight->m_mapSSTwSSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sst_word, sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		cweight->m_mapSSTwSSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sst_postag, sst_llabelset.bits(0), sst_llabelset.bits(1));
		cweight->m_mapSSTptSSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(sst_postag, sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		cweight->m_mapSSTptSSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sst_llabelset.bits(0), sst_llabelset.bits(1));
		cweight->m_mapSSTptN0ptSSTll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(sst_postag, n0_postag), sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		cweight->m_mapSSTptN0ptSSTrl.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(ENCODE_POSTAG_SET_2(sst_postag, n0_postag), n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapSSTptN0ptN0ll.getOrUpdateScore(m_lPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);

		if (m_bChar) {
			cweight->m_map1CharBeforeST.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, st_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharBeforeST.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, st_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharBeforeST.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, st_index, 3), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map1CharAfterST.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, st_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharAfterST.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, st_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharAfterST.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, st_index, 3), m_nScoreIndex, amount, m_nTrainingRound);

			cweight->m_map1CharBeforeN0.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, n0_index == -1 ? m_nSentenceLength : n0_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharBeforeN0.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, n0_index == -1 ? m_nSentenceLength : n0_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharBeforeN0.getOrUpdateScore(m_lPackedScore, nCharPrev(m_sSentence, n0_index == -1 ? m_nSentenceLength : n0_index, 3), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map1CharAfterN0.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, n0_index == -1 ? m_nSentenceLength : n0_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharAfterN0.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, n0_index == -1 ? m_nSentenceLength : n0_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharAfterN0.getOrUpdateScore(m_lPackedScore, nCharNext(m_sSentence, n0_index == -1 ? m_nSentenceLength : n0_index, 3), m_nScoreIndex, amount, m_nTrainingRound);
		}

		if (m_bPath) {

			if (st_index == -1 || n0_index == -1) {
				cweight->m_mapSTPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSTFPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapSTPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.POSPath[st_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSTFPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.FPOSPath[st_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
			}

			if (st2_index == -1 || n0_index == -1) {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.POSPath[st2_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.FPOSPath[st2_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
			}

			if (sst_index == -1 || n0_index == -1) {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_lPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapSSTPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.POSPath[sst_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSSTFPOSPath.getOrUpdateScore(m_lPackedScore, m_lcaAnalyzer.FPOSPath[sst_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
			}

		}

		if (m_bSuperTag) {
			const SuperTag & st_supertag = st_index == -1 ? 0 : item.superTag(st_index);
			const SuperTag & st2_supertag = st2_index == -1 ? 0 : item.superTag(st2_index);
			const SuperTag & sst_supertag = sst_index == -1 ? 0 : item.superTag(sst_index);
			const SuperTag & stl2_supertag = stl2_index == -1 ? 0 : item.superTag(stl2_index);
			const SuperTag & stl1_supertag = stl1_index == -1 ? 0 : item.superTag(stl1_index);
			const SuperTag & str1_supertag = str1_index == -1 || str1_index >= n0_index || str1_index >= m_nSentenceLength ? 0 : item.superTag(str1_index);
			const SuperTag & str2_supertag = str2_index == -1 || str2_index >= n0_index || str2_index >= m_nSentenceLength ? 0 : item.superTag(str2_index);
			const SuperTag & n0l2_supertag = n0l2_index == -1 ? 0 : item.superTag(n0l2_index);
			const SuperTag & n0l1_supertag = n0l1_index == -1 ? 0 : item.superTag(n0l1_index);

			cweight->m_mapSTst.getOrUpdateScore(m_lPackedScore, st_supertag, m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_mapST2st.getOrUpdateScore(m_lPackedScore, st2_supertag, m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_mapSSTst.getOrUpdateScore(m_lPackedScore, sst_supertag, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(stl2_supertag, -2);
			cweight->m_mapSTist.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(stl1_supertag, -1);
			cweight->m_mapSTist.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(str1_supertag, 1);
			cweight->m_mapSTist.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(str2_supertag, 2);
			cweight->m_mapSTist.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(n0l2_supertag, -2);
			cweight->m_mapN0ist.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
			bi_features.refer(n0l1_supertag, -1);
			cweight->m_mapN0ist.getOrUpdateScore(m_lPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		}
	}
}

#endif
