#ifndef _TWO_WAY_SHIFT_REDUCE_MACROS_H
#define _TWO_WAY_SHIFT_REDUCE_MACROS_H

#include "common/parser/agenda.h"
#include "common/parser/implementations/graph_macros.h"

namespace two_way_shift_reduce {

	enum Action {
		NO_ACTION = ACTION_START,
		A_SH,
		A_RE,
		REDUCE,
		SHIFT,
	};

	void printAction(const int & action);
	int decodeAction(const int & action);
}

#endif
