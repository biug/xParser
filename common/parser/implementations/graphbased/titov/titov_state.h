#ifndef _TITOV_STATE_H
#define _TITOV_STATE_H

#include "titov_macros.h"
#include "common/token/deplabel.h"
#include "common/parser/implementations/graphbased/graphstate_base.h"

namespace titov {

	extern int LABEL_COUNT;

	extern int A_SW_FIRST;
	extern int A_RE_FIRST;
	extern int A_SH_FIRST;
	extern int SH_FIRST;

	extern int A_SW_END;

	class StateItem : public GraphStateBase {

	public:
		StateItem();
		~StateItem();

		void shift(const int & t);
		void reduce();
		void swap();
		void arc(const int & l);
		void arcSwap(const int & l);
		void arcReduce(const int & l);
		void arcShift(const int & l, const int & t);

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

	inline void StateItem::arcSwap(const int & label) {
		arc(label);
		swap();
		m_lActionList[m_nActionBack] = A_SW_FIRST + label - 1;
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

	inline bool StateItem::canArc() const {
		return m_nStackBack == -1 ? false : (m_lRightNodes[m_lStack[m_nStackBack]].empty() ? true : (RIGHTNODE_POS(m_lRightNodes[m_lStack[m_nStackBack]].back()) != m_nNextWord));
	}

	inline bool StateItem::canSwap() const {
		const int & action = m_lActionList[m_nActionBack];
		return action != SWAP && !(action >= A_SW_FIRST && action < A_SW_END);
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
