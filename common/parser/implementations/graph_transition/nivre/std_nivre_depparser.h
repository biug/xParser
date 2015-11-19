#ifndef _STD_NIVRE_DEPPARSER_H
#define _STD_NIVRE_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "std_nivre_macros.h"
#include "common/parser/implementations/graph_transition/two_stack_weight.h"
#include "common/parser/implementations/graph_transition/graph_transition_depparser.h"

namespace std_nivre {

	using ::DependencyGraph;
	using graph_transition::TwoStackWeight;

	template<class RET_TYPE>
	class DepParser : public GraphDepParserBase<RET_TYPE, StateItem, ActionConstant> {
	protected:

		void arc(const tscore & score);
		void swap(const tscore & score);
		void reduce(const tscore & score);
		void shift(const tscore & score);
		void shift(const tscore & score, const int & tokenId);

		void getActionScores(const StateItem & item);
		void getOrUpdateFeatureScores(const StateItem & item, const ActionScoreIncrement & amount) override;

	public:
		DepParser(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput, int nState,
					const bool & bChar, const bool & bPath, const bool & bSTag);
		~DepParser();

		void decode() override;
	};

	template<class RET_TYPE>
	DepParser<RET_TYPE>::DepParser(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput,
			int nState, const bool & bChar, const bool & bPath, const bool & bSTag) :
		GraphDepParserBase<RET_TYPE, StateItem, ActionConstant>(nState, bChar, bPath, bSTag) {

		this->m_Weight = new TwoStackWeight<RET_TYPE, StateItem, ActionConstant>(sFeatureInput, sFeatureOutput,
				&this->m_tWords, &this->m_tPOSTags, &this->m_tLabels, &this->m_tSuperTags,
				&this->m_mapSuperTagCandidatesOfWords, &this->m_mapSuperTagCandidatesOfPOSTags);

		this->initConstant(sInputFile);
	}

	template<class RET_TYPE>
	DepParser<RET_TYPE>::~DepParser() {
		delete this->m_Weight;
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::arc(const tscore & score) {
		for (int action = this->m_cActions.A_FIRST; action < this->m_cActions.A_END; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::swap(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(SWAP, score + this->m_lPackedScore[SWAP]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduce(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(REDUCE, score + this->m_lPackedScore[REDUCE]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::shift(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(this->m_cActions.SH_FIRST, score + this->m_lPackedScore[this->m_cActions.SH_FIRST]));
	}

	template<class RET_TYPE>
	void DepParser<RET_TYPE>::shift(const tscore & score, const int & tokenId) {
		if (this->m_mapSuperTagCandidatesOfWords.find(this->m_lSentence[tokenId].first()) != this->m_mapSuperTagCandidatesOfWords.end()) {
			for (const auto & tag : this->m_mapSuperTagCandidatesOfWords[this->m_lSentence[tokenId].first()]) {
				this->m_abScores.insertItem(ScoredAction(this->m_cActions.SH_FIRST + tag, score + this->m_lPackedScore[this->m_cActions.SH_FIRST + tag]));
			}
		}
		else {
			for (const auto & tag : this->m_mapSuperTagCandidatesOfPOSTags[this->m_lSentence[tokenId].second()]) {
				this->m_abScores.insertItem(ScoredAction(this->m_cActions.SH_FIRST + tag, score + this->m_lPackedScore[this->m_cActions.SH_FIRST + tag]));
			}
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::getActionScores(const StateItem & item) {
		memset(this->m_lPackedScore, 0, sizeof(this->m_lPackedScore));
		getOrUpdateFeatureScores(item, ActionScoreIncrement(NO_ACTION, 0));
	}

	template<class RET_TYPE>
	void DepParser<RET_TYPE>::decode() {

		this->m_pGenerated->clear();

		for (const auto & iGenerator : *(this->m_pGenerator)) {
			this->m_abScores.clear();
			getActionScores(*iGenerator);

			const tscore & score = iGenerator->getScore();

			if (!iGenerator->stackEmpty()) {
				reduce(score);
			}

			if (iGenerator->size() < this->m_nSentenceLength) {
				if (iGenerator->canArc()) {
					arc(score);
				}
				if (this->m_bSuperTag) shift(score, iGenerator->size()); else shift(score);
			}

			if (iGenerator->canSwap()) {
				swap(score);
			}

			for (const auto & saScore : this->m_abScores) {
				this->m_iCandidate = *iGenerator;
				this->m_iCandidate.setScore(saScore->getScore());
				this->m_cActions.doAction(&(this->m_iCandidate), saScore->getAction());
				this->m_pGenerated->insertItem(this->m_iCandidate);
			}

			if (this->m_abScores.size() == 0) {
				this->m_abFinished.insertItem(*iGenerator);
			}
		}
	}

	template<class RET_TYPE>
	void DepParser<RET_TYPE>::getOrUpdateFeatureScores(const StateItem & item, const ActionScoreIncrement & amount) {
		TwoStackWeight<RET_TYPE, StateItem, ActionConstant> * cweight = (TwoStackWeight<RET_TYPE, StateItem, ActionConstant>*)(this->m_Weight);
		cweight->getOrUpdateFeatureScores((GraphDepParserBase<RET_TYPE, StateItem, ActionConstant>*)this, item, item.shiftBufferEmpty() ? -1 : item.shiftBufferTop(), amount);
	}
}

#endif
