#include <stack>
#include <sstream>
#include <unordered_set>

#include "nivre_macros.h"

namespace nivre {

	int LABEL_COUNT;

	int A_SW_FIRST, A_RE_FIRST, A_SH_FIRST, SH_FIRST;
	int A_SW_END, A_RE_END, A_SH_END, SH_END;

	std::vector<int> g_vecLabelMap;
	SuperTagCandidates g_mapSuperTagCandidatesOfWords;
	SuperTagCandidates g_mapSuperTagCandidatesOfPOSTags;

	int decodeAction(const int & action) {
		if (action < A_SW_FIRST) {
			return action;
		}
		else if (action < A_RE_FIRST) {
			return A_SW;
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
		case SWAP:
			std::cout << "swap";
			break;
		case SHIFT:
			std::cout << "shift";
			break;
		case REDUCE:
			std::cout << "reduce";
			break;
		case A_SW:
			std::cout << "arc swap";
			break;
		case A_SH:
			std::cout << "arc shift";
			break;
		case A_RE:
			std::cout << "arc reduce";
			break;
		case NO_ACTION:
			std::cout << "no action";
			break;
		default:
			std::cout << "wrong action";
			break;
		}
		std::cout << "(" << action << ")" << std::endl;
	}
}
