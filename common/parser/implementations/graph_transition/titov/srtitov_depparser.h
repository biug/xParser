#ifndef _SRTITOV_DEPPARSER_H
#define _SRTITOV_DEPPARSER_H

#include <vector>
#include <unordered_set>

#include "srtitov_macros.h"
#include "common/parser/implementations/graph_transition/ul_one_stack_weight.h"
#include "common/parser/implementations/graph_transition/ulgraph_transition_depparser.h"

namespace srtitov {

	using ::DependencyGraph;
	using graph_transition::ULOneStackWeight;

	template<class RET_TYPE>
	class DepParser : public ULGraphDepParserBase<RET_TYPE, StateItem, ActionConstant> {
	protected:

		void shift(const tscore & score);
		void shiftSwap(const tscore & score);
		void shiftArc(const tscore & score);
		void shiftSwapArc(const tscore & score);
		void shiftArcSwap(const tscore & score);
		void shiftSwapArcSwap(const tscore & score);
		void shiftArcSwapArc(const tscore & score);
		void shiftArcSwapArcSwap(const tscore & score);

		void reduce(const tscore & score);
		void reduceSwap(const tscore & score);
		void reduceArc(const tscore & score);
		void reduceSwapArc(const tscore & score);
		void reduceArcSwap(const tscore & score);
		void reduceSwapArcSwap(const tscore & score);
		void reduceArcSwapArc(const tscore & score);
		void reduceArcSwapArcSwap(const tscore & score);

		void getActionScores(const StateItem & item);
		void getOrUpdateFeatureScores(const StateItem & item, const ActionScoreIncrement & amount) override;

	public:
		DepParser(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput, int nState,
					const bool & bChar, const bool & bPath);
		~DepParser();

		void decode() override;
	};

	template<class RET_TYPE>
	DepParser<RET_TYPE>::DepParser(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput,
			int nState, const bool & bChar, const bool & bPath) :
			ULGraphDepParserBase<RET_TYPE, StateItem, ActionConstant>(nState, bChar, bPath) {

		this->m_Weight = new ULOneStackWeight<RET_TYPE, StateItem, ActionConstant>(sFeatureInput, sFeatureOutput,
				&this->m_tWords, &this->m_tPOSTags);
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
	inline void DepParser<RET_TYPE>::shiftSwap(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(SHIFT_SWAP, score + this->m_lPackedScore[SHIFT_SWAP]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::shiftArc(const tscore & score) {
		for (int action = SHIFT_ARC; action < SHIFT_ARC + ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::shiftSwapArc(const tscore & score) {
		for (int action = SHIFT_SWAP_ARC; action < SHIFT_SWAP_ARC + ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::shiftArcSwap(const tscore & score) {
		for (int action = SHIFT_ARC_SWAP; action < SHIFT_ARC_SWAP + ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::shiftSwapArcSwap(const tscore & score) {
		for (int action = SHIFT_SWAP_ARC_SWAP; action < SHIFT_SWAP_ARC_SWAP + ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::shiftArcSwapArc(const tscore & score) {
		for (int action = SHIFT_ARC_SWAP_ARC; action < SHIFT_ARC_SWAP_ARC + ULA_MAX * ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::shiftArcSwapArcSwap(const tscore & score) {
		for (int action = SHIFT_ARC_SWAP_ARC_SWAP; action < SHIFT_ARC_SWAP_ARC_SWAP + ULA_MAX * ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduce(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(REDUCE, score + this->m_lPackedScore[REDUCE]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduceSwap(const tscore & score) {
		this->m_abScores.insertItem(ScoredAction(REDUCE_SWAP, score + this->m_lPackedScore[REDUCE_SWAP]));
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduceArc(const tscore & score) {
		for (int action = REDUCE_ARC; action < REDUCE_ARC + ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduceSwapArc(const tscore & score) {
		for (int action = REDUCE_SWAP_ARC; action < REDUCE_SWAP_ARC + ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduceArcSwap(const tscore & score) {
		for (int action = REDUCE_ARC_SWAP; action < REDUCE_ARC_SWAP + ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduceSwapArcSwap(const tscore & score) {
		for (int action = REDUCE_SWAP_ARC_SWAP; action < REDUCE_SWAP_ARC_SWAP + ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduceArcSwapArc(const tscore & score) {
		for (int action = REDUCE_ARC_SWAP_ARC; action < REDUCE_ARC_SWAP_ARC + ULA_MAX * ULA_MAX; ++action) {
			this->m_abScores.insertItem(ScoredAction(action, score + this->m_lPackedScore[action]));
		}
	}

	template<class RET_TYPE>
	inline void DepParser<RET_TYPE>::reduceArcSwapArcSwap(const tscore & score) {
		for (int action = REDUCE_ARC_SWAP_ARC_SWAP; action < REDUCE_ARC_SWAP_ARC_SWAP + ULA_MAX * ULA_MAX; ++action) {
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

			if (!iGenerator->stackEmpty()) {
				// all kinds of reduce
				reduce(score);
				if (iGenerator->stackBack() > 0 && iGenerator->canArc(iGenerator->stackSubTop(), iGenerator->size())) {
					reduceArc(score);
					if (iGenerator->stackBack() > 1) {
						reduceArcSwap(score);
						if (iGenerator->canArc(iGenerator->stackSecondSubTop(), iGenerator->size())) {
							reduceArcSwapArc(score);
							reduceArcSwapArcSwap(score);
						}
					}
				}
				if (iGenerator->stackBack() > 1) {
					reduceSwap(score);
					if (iGenerator->canArc(iGenerator->stackSecondSubTop(), iGenerator->size())) {
						reduceSwapArc(score);
						reduceSwapArcSwap(score);
					}
				}
			}

			if (iGenerator->size() < this->m_nSentenceLength) {
				// all kinds of shift
				shift(score);
				if (iGenerator->size() < this->m_nSentenceLength - 1) {
					shiftArc(score);
					if (iGenerator->stackBack() >= 0) {
						shiftArcSwap(score);
						if (iGenerator->canArc(iGenerator->stackTop(), iGenerator->size() + 1)) {
							shiftSwapArc(score);
							shiftSwapArcSwap(score);

							shiftArcSwapArc(score);
							shiftArcSwapArcSwap(score);
						}
					}
				}
				if (iGenerator->stackBack() >= 0) {
					shiftSwap(score);
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
		ULOneStackWeight<RET_TYPE, StateItem, ActionConstant> * cweight = (ULOneStackWeight<RET_TYPE, StateItem, ActionConstant>*)(this->m_Weight);
		cweight->getOrUpdateFeatureScores((GraphDepParserBase<RET_TYPE, StateItem, ActionConstant>*)this, item, amount);
	}
}

#endif
