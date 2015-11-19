#ifndef _SR_STATE_H
#define _SR_STATE_H

#include "common/parser/implementations/graph_transition/graph_transition_state.h"

namespace sr {
	class StateItem : public GraphTransitionStateBase {

	public:
		StateItem();
		~StateItem();

		void reduce(const int & action);
		void shift(const int & tag, const int & action);
		void arcReduce(const int & label, const int & leftLabel, const int & rightLabel, const int & action);
		void arcShift(const int & label, const int & leftLabel, const int & rightLabel, const int & tag, const int & action);

		bool canArc() const					{ return m_nStackBack == -1 ? false : (m_vecRightArcs[m_lStack[m_nStackBack]].empty() ? true : (m_vecRightArcs[m_lStack[m_nStackBack]].back().head != m_nNextWord)); }

		void clear();
		void clearNext();
		void combineReverse(const StateItem & item);
		void print(const DLabel & labels, const DSuperTag & supertags) const override;

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
