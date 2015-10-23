#ifndef _STD_TWOSTACK_MACROS_H
#define _STD_TWOSTACK_MACROS_H

#include "common/parser/agenda.h"
#include "common/parser/implementations/graph_macros.h"

namespace std_twostack {

	enum Action {
		NO_ACTION = ACTION_START,
		ARC,
		MEM,
		RECALL,
		REDUCE,
		SHIFT,
	};

	void printAction(const int & action);
	int decodeAction(const int & action);
}

#endif
