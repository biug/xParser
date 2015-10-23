#ifndef _STD_TITOV_STATE_H
#define _STD_TITOV_STATE_H

#include "std_titov_macros.h"
#include "common/token/deplabel.h"
#include "common/parser/implementations/graph_transition/graph_transition_state_base.h"

namespace std_titov {

	using graph_transition::GraphStateBase;

	extern int SH_FIRST;

	class StateItem : public GraphStateBase {

	public:
		StateItem();
		~StateItem();

		void shift(const int & t);
		void reduce();
		void swap();

		bool canSwap() const;
		bool canArc() const;

		void clear() override;
		void clearNext() override;
		void print() const override;
		void check() const override;
		void move(const int & action) override;

		bool extractOracle(const DependencyGraph & graph) override;
		bool extractOneStandard(int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph, const int & label = 0) override;

		bool operator<(const StateItem & i) const;
		bool operator<=(const StateItem & i) const;
		bool operator>(const StateItem & i) const;
		bool operator==(const StateItem & i) const;
		bool operator!=(const StateItem & i) const;

		bool operator==(const DependencyGraph & graph) const;

		StateItem & operator=(const StateItem & i);
	};

	inline void StateItem::shift(const int & t) {
		m_lSuperTag[m_nNextWord] = t;
		m_lStack[++m_nStackBack] = m_nNextWord++;
		m_lActionList[++m_nActionBack] = SH_FIRST + t;
		clearNext();
	}

	inline void StateItem::reduce() {
		--m_nStackBack;
		m_lActionList[++m_nActionBack] = REDUCE;
	}

	inline void StateItem::swap() {
		std::swap(m_lStack[m_nStackBack - 1], m_lStack[m_nStackBack]);
		m_lActionList[++m_nActionBack] = SWAP;
	}

	inline bool StateItem::canArc() const {
		return m_nStackBack == -1 ? false : (m_lRightNodes[m_lStack[m_nStackBack]].empty() ? true : (RIGHTNODE_POS(m_lRightNodes[m_lStack[m_nStackBack]].back()) != m_nNextWord));
	}

	inline bool StateItem::canSwap() const {
		return m_lActionList[m_nActionBack] != SWAP;
	}

	inline bool StateItem::operator<(const StateItem & item) const {
		return m_nScore < item.m_nScore;
	}

	inline bool StateItem::operator<=(const StateItem & item) const {
		return m_nScore <= item.m_nScore;
	}

	inline bool StateItem::operator>(const StateItem & item) const {
		return m_nScore > item.m_nScore;
	}

	inline bool StateItem::operator!=(const StateItem & item) const {
		return !(*this == item);
	}
}

#endif
