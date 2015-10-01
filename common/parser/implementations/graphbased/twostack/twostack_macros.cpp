#include <stack>
#include <sstream>
#include <unordered_set>

#include "twostack_macros.h"

namespace twostack {

	int LABEL_COUNT;

	int A_MM_FIRST, A_RC_FIRST, A_RE_FIRST, A_SH_FIRST, SH_FIRST;
	int A_MM_END, A_RC_END, A_RE_END, A_SH_END, SH_END;

	std::vector<int> g_vecLabelMap;
	SuperTagCandidates g_mapSuperTagCandidatesOfWords;
	SuperTagCandidates g_mapSuperTagCandidatesOfPOSTags;

	int decodeAction(const int & action) {
		if (action < A_MM_FIRST) {
			return action;
		}
		else if (action < A_RC_FIRST) {
			return A_MM;
		}
		else if (action < A_RE_FIRST) {
			return A_RC;
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
		case MEM:
			std::cout << "mem";
			break;
		case RECALL:
			std::cout << "recall";
			break;
		case SHIFT:
			std::cout << "shift with tag " << action - SH_FIRST;
			break;
		case REDUCE:
			std::cout << "reduce";
			break;
		case A_MM:
			std::cout << "arc mem with label " << action - A_MM_FIRST + 1;
			break;
		case A_RC:
			std::cout << "arc recall with label " << action - A_RC_FIRST + 1;
			break;
		case A_SH:
			std::cout << "arc shift with label " << (action - A_SH_FIRST) % LABEL_COUNT + 1 << " with tag " << (action - A_SH_FIRST) % LABEL_COUNT;
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
