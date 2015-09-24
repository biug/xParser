#include <stack>
#include <sstream>
#include <unordered_set>

#include "twostack_ring_macros.h"

std::istream & operator>>(std::istream & is, twostack_ring::ScoreArray & s) {
	ttoken token;
	int index;
	cscore cs;
	tscore ts;
	s.reset();
	is >> token;
	while (is >> index) {
		is >> token >> cs >> token >> ts >> token;
		s.getScores()[index] = Score(cs, ts);
		if (token == "}") {
			break;
		}
	}
	return is;
}

std::ostream & operator<<(std::ostream & os, const twostack_ring::ScoreArray & s) {
	bool start = false;
	for (const auto & score : s.getScores()) {
		if (!score.second.zero()) {
			if (!start) {
				os << "{ ";
				start = true;
			}
			else {
				os << " , ";
			}
			os << score.first << " : " << score.second;
		}
	}
	os << " }";
	return os;
}

namespace twostack_ring {

	int A_MM_FIRST, A_RC_FIRST, A_SH_FIRST, A_RE_FIRST;
	int A_MM_END, A_RC_END, A_SH_END, A_RE_END;

	std::vector<int> g_vecLabelMap;

	bool ScoreArray::zero() const {
		for (const auto & score : m_mapScores) {
			if (!score.second.zero()) {
				return false;
			}
		}
		return true;
	}

	int decodeAction(const int & action) {
		if (action < A_MM_FIRST) {
			return action;
		}
		else if (action < A_RC_FIRST) {
			return A_MM;
		}
		else if (action < A_SH_FIRST) {
			return A_RC;
		}
		else if (action < A_RE_FIRST) {
			return A_SH;
		}
		else {
			return A_RE;
		}
	}

	void printAction(const int & action) {
		switch (decodeAction(action)) {
		case MEM:
			std::cout << "mem" << std::endl;
			break;
		case RECALL:
			std::cout << "recall" << std::endl;
			break;
		case REDUCE:
			std::cout << "reduce" << std::endl;
			break;
		case SHIFT:
			std::cout << "shift" << std::endl;
			break;
		case A_MM:
			std::cout << "arc mem" << std::endl;
			break;
		case A_RC:
			std::cout << "arc recall" << std::endl;
			break;
		case A_SH:
			std::cout << "arc shift" << std::endl;
			break;
		case A_RE:
			std::cout << "arc reduce" << std::endl;
			break;
		case NO_ACTION:
			std::cout << "no action" << std::endl;
			break;
		default:
			std::cout << "wrong action" << std::endl;
			break;
		}
	}
}
