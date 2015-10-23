#ifndef _BOTH_TWOSTACK_STATE_H
#define _BOTH_TWOSTACK_STATE_H

#include "both_twostack_macros.h"
#include "common/token/deplabel.h"
#include "common/parser/implementations/graph_transition_both/graph_transition_both_state_base.h"


namespace both_twostack {

	using graph_transition_both::GraphStateBase;

	extern int A_MM_FIRST;
	extern int A_RC_FIRST;
	extern int A_RE_FIRST;
	extern int A_SH_FIRST;
	extern int SH_FIRST;

	extern int A_MM_END;
	extern int A_RC_END;

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
		void arcMem(const int & l);
		void arcRecall(const int & l);
		void arcReduce(const int & l);
		void arcShift(const int & l, const int & t);

		const int & secondStackTop() const;
		const int & secondStackBack() const;

		bool canMem() const;
		bool canRecall() const;
		bool canArc() const;
		bool canShift() const;

		void clear() override;
		void clearNext() override;
		void print() const override;
		void check() const override;
		void move(const int & action) override;

		void reverse(int size) override;
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

	inline void StateItem::arcMem(const int & label) {
		arc(label);
		mem();
		m_lActionList[m_nActionBack] = A_MM_FIRST + label - 1;
	}

	inline void StateItem::arcRecall(const int & label) {
		arc(label);
		recall();
		m_lActionList[m_nActionBack] = A_RC_FIRST + label - 1;
	}

	inline void StateItem::arcReduce(const int & label) {
		arc(label);
		reduce();
		m_lActionList[m_nActionBack] = A_RE_FIRST + label - 1;
	}

	inline void StateItem::arcShift(const int & label, const int & tag) {
		arc(label);
		shift(tag);
		m_lActionList[m_nActionBack] = A_SH_FIRST + tag * g_nGraphLabelCount + label - 1;
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