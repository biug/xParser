#ifndef _STD_TWOSTACK_STATE_H
#define _STD_TWOSTACK_STATE_H

#include "std_twostack_macros.h"
#include "common/token/deplabel.h"
#include "common/parser/implementations/graph_transition/graph_transition_state_base.h"

namespace std_twostack {

	extern int LABEL_COUNT;

	extern int SH_FIRST;

	class StateItem : public GraphStateBase {
	private:
		bool m_bCanMem;
		int m_nSecondStackBack;
		int m_lSecondStack[MAX_SENTENCE_SIZE];

	public:
		StateItem();
		~StateItem();

		void shift(const int & t);
		void reduce();
		void mem();
		void recall();
		void arc(const int & l);

		const int & secondStackTop() const;
		const int & secondStackBack() const;

		bool canMem() const;
		bool canRecall() const;
		bool canShift() const;
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
		m_bCanMem = true;
		m_lSuperTag[m_nNextWord] = t;
		m_lStack[++m_nStackBack] = m_nNextWord++;
		m_lActionList[++m_nActionBack] = SH_FIRST + t;
		clearNext();
	}

	inline void StateItem::reduce() {
		--m_nStackBack;
		m_lActionList[++m_nActionBack] = REDUCE;
	}

	inline void StateItem::mem() {
		m_lSecondStack[++m_nSecondStackBack] = m_lStack[m_nStackBack--];
		m_lActionList[++m_nActionBack] = MEM;
	}

	inline void StateItem::recall() {
		m_bCanMem = false;
		m_lStack[++m_nStackBack] = m_lSecondStack[m_nSecondStackBack--];
		m_lActionList[++m_nActionBack] = RECALL;
	}

	inline const int & StateItem::secondStackTop() const {
		return m_lSecondStack[m_nSecondStackBack];
	}

	inline const int & StateItem::secondStackBack() const {
		return m_nSecondStackBack;
	}

	inline bool StateItem::canArc() const {
		return m_nStackBack == -1 ? false : (m_lRightNodes[m_lStack[m_nStackBack]].empty() ? true : (RIGHTNODE_POS(m_lRightNodes[m_lStack[m_nStackBack]].back()) != m_nNextWord));
	}

	inline bool StateItem::canMem() const {
		return m_nStackBack > 0 && m_bCanMem;
	}

	inline bool StateItem::canRecall() const {
		return m_nSecondStackBack >= 0;
	}

	inline bool StateItem::canShift() const {
		return m_nSecondStackBack == -1;
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
