#ifndef _ARCSR_MACROS_H
#define _ARCSR_MACROS_H

#include "arcsr_state.h"
#include "common/parser/agenda.h"

#include <unordered_map>

namespace arcsr {

	enum Action {
		NO_ACTION = 0,
		ARC_LEFT,
		ARC_RIGHT,
		SHIFT,
		REDUCE,
		POP_ROOT,
	};

	struct ActionConstant {

		int LabelCount;
		int LeftLabelCount;
		int RightLabelCount;
		int m_nSuperTagCount;
		std::vector<int> m_vecLabelMap;
		std::unordered_map<int, int> m_mapLeftLabelMap;
		std::unordered_map<int, int> m_mapRightLabelMap;
		int AL_FIRST, AL_END;
		int AR_FIRST, AR_END;
		int RE_FIRST, RE_END;

		ActionConstant(const int & superTagCount = 0);
		ActionConstant(const ActionConstant & actions);
		~ActionConstant();

		void loadConstant(const DLabel & labels);
		void doAction(StateItem * item, const int & action) const;
		bool extractOracle(StateItem * item, const DependencyGraph & graph) const;
		bool followOneAction(StateItem * item, int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph,
									const std::vector<int> & heads) const;
		void print(const int & action) const;
		void print() const;
	};

}

#endif
