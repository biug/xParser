#ifndef _STD_TWOSTACK_MACROS_H
#define _STD_TWOSTACK_MACROS_H

#include "twostack_state.h"
#include "common/parser/agenda.h"

namespace std_twostack {

	using twostack::StateItem;

	enum Action {
		NO_ACTION = ACTION_START,
		ARC,
		MEM,
		RECALL,
		REDUCE,
		SHIFT,
	};

	struct ActionConstant {

		int LabelCount;
		int m_nSuperTagCount;
		std::vector<int> m_vecLabelMap;
		int A_FIRST, A_END;
		int SH_FIRST, SH_END;

		ActionConstant(const int & superTagCount = 0);
		ActionConstant(const ActionConstant & actions);
		~ActionConstant();

		void loadConstant(const DLabel & labels);
		void doAction(StateItem * item, const int & action) const;
		bool extractOracle(StateItem * item, const DependencyGraph & graph) const;
		bool followOneAction(StateItem * item, int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph,
									const std::pair<int, std::pair<int, int>> & labels = {0, {0, 0}}) const;
		void print(const int & action) const;
		void print() const;
	};

}

#endif
