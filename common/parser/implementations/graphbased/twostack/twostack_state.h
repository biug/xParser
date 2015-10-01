#ifndef _TWOSTACK_STATE_H
#define _TWOSTACK_STATE_H

#include "twostack_macros.h"
#include "common/token/deplabel.h"
#include "common/parser/implementations/graphbased/graphstate_base.h"

namespace twostack {

	extern int LABEL_COUNT;

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
		bool m_bCanRecall;
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
		void arcMem(const int & l);
		void arcRecall(const int & l);
		void arcReduce(const int & l);
		void arcShift(const int & l, const int & t);

		const int & secondStackTop() const;
		const int & secondStackBack() const;

		bool canMem() const;
		bool canRecall() const;
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
		m_bCanRecall = true;
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
		m_lActionList[m_nActionBack] = A_SH_FIRST + tag * LABEL_COUNT + label - 1;
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
		return m_nSecondStackBack >= 0 && m_bCanRecall;
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
