#include <stack>
#include <sstream>
#include <unordered_set>

#include "std_nivre_macros.h"

namespace std_nivre {

	int A_FIRST, SH_FIRST;
	int A_END, SH_END;

	int decodeAction(const int & action) {
		if (action < A_FIRST) {
			return action;
		}
		else if (action < SH_FIRST) {
			return ARC;
		}
		else {
			return SHIFT;
		}
	}

	void printAction(const int & action) {
		switch (decodeAction(action)) {
		case SWAP:
			std::cout << "swap";
			break;
		case SHIFT:
			std::cout << "shift with tag " << action - SH_FIRST;
			break;
		case REDUCE:
			std::cout << "reduce";
			break;
		case ARC:
			std::cout << "arc with label "<< action - A_FIRST + 1;
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
