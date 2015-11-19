#ifndef _TWOWAY_SR_DEPPARSER_H
#define _TWOWAY_SR_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "sr_macros.h"
#include "common/parser/implementations/graph_transition/sim_one_stack_weight.h"
#include "common/parser/implementations/graph_transition/twoway_graph_transition_depparser.h"

namespace twoway_sr {

	using ::DependencyGraph;
	using sr::StateItem;
	using sr::ActionConstant;
	using graph_transition::SimOneStackWeight;

	template<class RET_TYPE>
	class DepParser : public TwoWayGraphDepParserBase<RET_TYPE, StateItem, ActionConstant> {
	protected:

		void reduce(const tscore & score);
		void arcReduce(const tscore & score);
		void shift(const tscore & score);
		void arcShift(const tscore & score);
		void shift(const tscore & score, const int & tokenId);
		void arcShift(const tscore & score, const int & tokenId);

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
		TwoWayGraphDepParserBase<RET_TYPE, StateItem, ActionConstant>(nState, bChar, bPath, bSTag) {

		this->m_lWeights[0] = new SimOneStackWeight<RET_TYPE, StateItem, ActionConstant>(sFeatureInput + "0", sFeatureOutput + "0",
				&this->m_tWords[0], &this->m_tPOSTags[0], &this->m_tLabels[0], &this->m_tSuperTags[0],
				&this->m_mapSuperTagCandidatesOfWords[0], &this->m_mapSuperTagCandidatesOfPOSTags[0]);

		this->m_lWeights[1] = new SimOneStackWeight<RET_TYPE, StateItem, ActionConstant>(sFeatureInput + "1", sFeatureOutput + "1",
				&this->m_tWords[1], &this->m_tPOSTags[1], &this->m_tLabels[1], &this->m_tSuperTags[1],
				&this->m_mapSuperTagCandidatesOfWords[1], &this->m_mapSuperTagCandidatesOfPOSTags[1]);

		this->initConstant(sInputFile);
	}

	template<class RET_TYPE>
	DepParser<RET_TYPE>::~DepParser() {
		delete this->m_Weight;
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduce(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(sr::REDUCE, score + this->m_lPackedScore[sr::REDUCE]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::arcReduce(const tscore & score) {
		for (int action = this->m_cActions.A_RE_FIRST; action < this->m_cActions.A_RE_END; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::shift(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(this->m_cActions.SH_FIRST, score + this->m_lPackedScore[this->m_cActions.SH_FIRST]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::arcShift(const tscore & score) {
		for (int action = this->m_cActions.A_SH_FIRST, n = this->m_cActions.A_SH_FIRST + this->m_cActions.LabelCount; action < n; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	void DepParser<RET_TYPE>::shift(const tscore & score, const int & tokenId) {
		if (this->m_mapSuperTagCandidatesOfWords[this->m_nInputIndex].find(this->m_lSentence[tokenId].first()) != this->m_mapSuperTagCandidatesOfWords[this->m_nInputIndex].end()) {
			for (const auto & tag : this->m_mapSuperTagCandidatesOfWords[this->m_nInputIndex][this->m_lSentence[tokenId].first()]) {
				this->m_abScores.insertItem(ScoredAction(this->m_cActions.SH_FIRST + tag, score + this->m_lPackedScore[this->m_cActions.SH_FIRST + tag]));
			}
		}
		else {
			for (const auto & tag : this->m_mapSuperTagCandidatesOfPOSTags[this->m_nInputIndex][this->m_lSentence[tokenId].second()]) {
				this->m_abScores.insertItem(ScoredAction(this->m_cActions.SH_FIRST + tag, score + this->m_lPackedScore[this->m_cActions.SH_FIRST + tag]));
			}
		}
	}

	template<class RET_TYPE>
	void DepParser<RET_TYPE>::arcShift(const tscore & score, const int & tokenId) {
		if (this->m_mapSuperTagCandidatesOfWords[this->m_nInputIndex].find(this->m_lSentence[tokenId].first()) != this->m_mapSuperTagCandidatesOfWords[this->m_nInputIndex].end()) {
			for (const auto & tag : this->m_mapSuperTagCandidatesOfWords[this->m_nInputIndex][this->m_lSentence[tokenId].first()]) {
				for (int action = this->m_cActions.A_SH_FIRST + tag * this->m_cActions.LabelCount, n = action + this->m_cActions.LabelCount; action < n; ++action) {
					this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
				}
			}
		}
		else {
			for (const auto & tag : this->m_mapSuperTagCandidatesOfPOSTags[this->m_nInputIndex][this->m_lSentence[tokenId].second()]) {
				for (int action = this->m_cActions.A_SH_FIRST + tag * this->m_cActions.LabelCount, n = action + this->m_cActions.LabelCount; action < n; ++action) {
					this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
				}
			}
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::getActionScores(const StateItem & item) {
		memset(this->m_lPackedScore, 0, sizeof(this->m_lPackedScore));
		getOrUpdateFeatureScores(item, ActionScoreIncrement(sr::NO_ACTION, 0));
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
					arcReduce(score);
					if (this->m_bSuperTag) arcShift(score, iGenerator->size()); else arcShift(score);
				}
				if (this->m_bSuperTag) shift(score, iGenerator->size()); else shift(score);
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
		SimOneStackWeight<RET_TYPE, StateItem, ActionConstant> * cweight = (SimOneStackWeight<RET_TYPE, StateItem, ActionConstant>*)(this->m_lWeights[this->m_nInputIndex]);
		cweight->getOrUpdateFeatureScores((GraphDepParserBase<RET_TYPE, StateItem, ActionConstant>*)this, item, amount);
	}
}

#endif
