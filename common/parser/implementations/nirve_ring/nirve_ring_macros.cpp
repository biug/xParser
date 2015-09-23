#include <stack>
#include <sstream>
#include <unordered_set>

#include "nirve_ring_macros.h"

std::istream & operator>>(std::istream & is, nirve_ring::ScoreArray & s) {
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

std::ostream & operator<<(std::ostream & os, const nirve_ring::ScoreArray & s) {
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

namespace nirve_ring {

	int A_SW_FIRST, A_SH_FIRST, A_RE_FIRST;
	int A_SW_END, A_SH_END, A_RE_END;

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
		if (action < A_SW_FIRST) {
			return action;
		}
		else if (action < A_SH_FIRST) {
			return A_SW;
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
