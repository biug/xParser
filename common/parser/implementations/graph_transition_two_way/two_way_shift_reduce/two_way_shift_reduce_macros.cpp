#include <stack>
#include <sstream>
#include <unordered_set>

#include "two_way_shift_reduce_macros.h"

namespace two_way_shift_reduce {

	int A_RE_FIRST, A_SH_FIRST, SH_FIRST;
	int A_RE_END, A_SH_END, SH_END;

	int decodeAction(const int & action) {
		if (action < A_RE_FIRST) {
			return action;
		}
		else if (action < A_SH_FIRST) {
			return A_RE;
		}
		else if (action < SH_FIRST) {
			return A_SH;
		}
		else {
			return SHIFT;
		}
	}

	void printAction(const int & action) {
		switch (decodeAction(action)) {
		case SHIFT:
			std::cout << "shift with tag " << action - SH_FIRST;
			break;
		case REDUCE:
			std::cout << "reduce";
			break;
		case A_SH:
			std::cout << "arc shift with label " << (action - A_SH_FIRST) % g_nGraphLabelCount + 1 << " with tag " << (action - A_SH_FIRST) % g_nGraphLabelCount;
			break;
		case A_RE:
			std::cout << "arc reduce with label "<< action - A_RE_FIRST + 1;
			break;
		case NO_ACTION:
			std::cout << "no action";
			break;
		default:
			std::cout << "wrong action";
			break;
		}
		std::cout << " (" << action << ")" << std::endl;
	}
}
