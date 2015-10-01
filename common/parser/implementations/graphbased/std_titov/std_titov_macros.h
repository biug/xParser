#ifndef _STD_TITOV_MACROS_H
#define _STD_TITOV_MACROS_H

#include "common/parser/agenda.h"
#include "common/parser/implementations/graphbased/graphmacros_base.h"

namespace std_titov {

	enum Action {
		NO_ACTION = ACTION_START,
		ARC,
		SWAP,
		REDUCE,
		SHIFT,
	};

	void printAction(const int & action);
	int decodeAction(const int & action);
}

#endif
