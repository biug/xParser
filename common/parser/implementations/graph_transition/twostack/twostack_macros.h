#ifndef _TWOSTACK_MACROS_H
#define _TWOSTACK_MACROS_H

#include "common/parser/agenda.h"
#include "common/parser/implementations/graph_transition/graph_transition_macros_base.h"

namespace twostack {

	enum Action {
		NO_ACTION = ACTION_START,
		A_MM,
		A_RC,
		A_SH,
		A_RE,
		MEM,
		RECALL,
		REDUCE,
		SHIFT,
	};

	void printAction(const int & action);
	int decodeAction(const int & action);
}

#endif
