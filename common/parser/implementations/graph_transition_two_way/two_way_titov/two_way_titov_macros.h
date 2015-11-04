#ifndef _TWO_WAY_TITOV_MACROS_H
#define _TWO_WAY_TITOV_MACROS_H

#include "common/parser/agenda.h"
#include "common/parser/implementations/graph_macros.h"

namespace two_way_titov {

	enum Action {
		NO_ACTION = ACTION_START,
		A_SW,
		A_SH,
		A_RE,
		SWAP,
		REDUCE,
		SHIFT,
	};

	void printAction(const int & action);
	int decodeAction(const int & action);
}

#endif
