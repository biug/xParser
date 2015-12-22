#ifndef _ARCSR_STATE_H
#define _ARCSR_STATE_H

#include "common/parser/implementations/graph_transition/graph_transition_state.h"

namespace arcsr {
	class StateItem : public GraphTransitionStateBase {
	protected:
		int m_nHeadStackBack;
		int m_lHeads[MAX_SENTENCE_SIZE];
		int m_lLabels[MAX_SENTENCE_SIZE];

	public:
		StateItem();
		~StateItem();

		void arcLeft(const int & label, const int & leftLabel);
		void arcRight(const int & label, const int & rightLabel);

		void reduce(const int & tag, const int & action);
		void shift(const int & action);
		void arcLeft(const int & label, const int & leftLabel, const int & tag, const int & action);
		void arcRight(const int & label, const int & rightLabel, const int & action);
		void popRoot(const int & tag, const int & action);

		void clear();
		void clearNext();
		void print(const DLabel & labels, const DSuperTag & supertags) const override;

		const int & headStackBack() { return m_nHeadStackBack; }
		const int & head(const int & index) const { return m_lHeads[index]; }
		const int & label(const int & index) const { return m_lLabels[index]; };

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
