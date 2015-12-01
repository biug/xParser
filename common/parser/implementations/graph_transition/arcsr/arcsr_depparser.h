#ifndef _ARCSR_DEPPARSER_H
#define _ARCSR_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "arcsr_macros.h"
#include "common/parser/implementations/graph_transition/tree_weight.h"
#include "common/parser/implementations/graph_transition/graph_transition_depparser.h"

namespace arcsr {

	using ::DependencyGraph;
	using graph_transition::TreeWeight;

	template<class RET_TYPE>
	class DepParser : public GraphDepParserBase<RET_TYPE, StateItem, ActionConstant> {
	protected:

		void shift(const tscore & score);
		void reduce(const tscore & score);
		void popRoot(const tscore & score);
		void arcLeft(const tscore & score);
		void arcRight(const tscore & score);

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

		this->m_Weight = new TreeWeight<RET_TYPE, StateItem, ActionConstant>(sFeatureInput, sFeatureOutput,
				&this->m_tWords, &this->m_tPOSTags, &this->m_tLabels, &this->m_tSuperTags,
				&this->m_mapSuperTagCandidatesOfWords, &this->m_mapSuperTagCandidatesOfPOSTags);

		this->initConstant(sInputFile);
	}

	template<class RET_TYPE>
	DepParser<RET_TYPE>::~DepParser() {
		delete this->m_Weight;
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::shift(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(SHIFT, score + this->m_lPackedScore[SHIFT]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduce(const tscore & score) {
		for (int action = this->m_cActions.RE_FIRST; action < this->m_cActions.RE_END; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::popRoot(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(POP_ROOT, score + this->m_lPackedScore[POP_ROOT]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::arcLeft(const tscore & score) {
		for (int action = this->m_cActions.AL_FIRST; action < this->m_cActions.AL_END; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::arcRight(const tscore & score) {
		for (int action = this->m_cActions.AR_FIRST; action < this->m_cActions.AR_END; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
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

			if (iGenerator->size() == this->m_nSentenceLength) {
				if (iGenerator->stackBack() > 0) {
					reduce(score);
				}
				else if (iGenerator->stackBack() == 0) {
					popRoot(score);
				}
			}
			else {
				const int & action = iGenerator->action(iGenerator->actionBack());
				if (action < this->m_cActions.RE_FIRST || action >= this->m_cActions.RE_END) {
					if (iGenerator->size() < this->m_nSentenceLength - 1 || iGenerator->stackEmpty()) {
						shift(score);
					}
				}
				if (!iGenerator->stackEmpty()) {
					if (iGenerator->size() < this->m_nSentenceLength - 1 || iGenerator->headStackBack() == 0) {
						arcRight(score);
					}
					if (iGenerator->head(iGenerator->stackTop()) != -1) {
						reduce(score);
					}
					else {
						arcLeft(score);
					}
				}
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
		TreeWeight<RET_TYPE, StateItem, ActionConstant> * cweight = (TreeWeight<RET_TYPE, StateItem, ActionConstant>*)(this->m_Weight);
		cweight->getOrUpdateFeatureScores((GraphDepParserBase<RET_TYPE, StateItem, ActionConstant>*)this, item, amount);
	}
}

#endif
