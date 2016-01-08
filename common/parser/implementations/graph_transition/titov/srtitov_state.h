#ifndef _SRTITOV_STATE_H
#define _SRTITOV_STATE_H

#include "common/parser/implementations/graph_transition/ulgraph_transition_state.h"

namespace srtitov {
	class StateItem : public ULGraphTransitionStateBase {

	private:
		bool m_bCanSwap;

	public:
		StateItem();
		~StateItem();

		// atom action
		void reduce();
		void swap();
		void shift();
		void arc(const int & label);

		void swap(const int & action);
		void arc(const int & label, const int & action);

		void shift(const int & action);
		void shiftSwap(const int & action);
		void shiftArc(const int & label, const int & action);
		void shiftSwapArc(const int & label, const int & action);
		void shiftArcSwap(const int & label, const int & action);
		void shiftSwapArcSwap(const int & label, const int & action);
		void shiftArcSwapArc(const int & label1, const int & label2, const int & action);
		void shiftArcSwapArcSwap(const int & label1, const int & label2, const int & action);

		void reduce(const int & action);
		void reduceSwap(const int & action);
		void reduceArc(const int & label, const int & action);
		void reduceSwapArc(const int & label, const int & action);
		void reduceArcSwap(const int & label, const int & action);
		void reduceSwapArcSwap(const int & label, const int & action);
		void reduceArcSwapArc(const int & label1, const int & label2, const int & action);
		void reduceArcSwapArcSwap(const int & label1, const int & label2, const int & action);

		bool canSwap() const				{ return m_nStackBack > 0 && m_bCanSwap; }
		bool canArc() const					{ return m_nStackBack == -1 ? false : (m_vecRightArcs[m_lStack[m_nStackBack]].empty() ? true : (m_vecRightArcs[m_lStack[m_nStackBack]].back().head != m_nNextWord)); }
		bool canArc(int l, int r) const		{ return m_vecRightArcs[l].empty() ? true : (m_vecRightArcs[l].back().head != r); }

		void clear();
		void clearNext();
		void print() const override;

		bool operator<(const StateItem & item) const	{ return m_nScore < item.m_nScore; }
		bool operator<=(const StateItem & item) const	{ return m_nScore <= item.m_nScore; }
		bool operator>(const StateItem & item) const	{ return m_nScore > item.m_nScore; }
		bool operator!=(const StateItem & item) const	{ return !(*this == item); }

		bool operator==(const StateItem & item) const;
		bool operator==(const DependencyGraph & graph) const;

		StateItem & operator=(const StateItem & i);
	};
}

#endif
