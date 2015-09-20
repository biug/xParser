#include <stack>
#include <sstream>
#include <unordered_set>

#include "arceager_macros.h"

std::istream & operator>>(std::istream & is, arceager::ScoreArray & s) {
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

std::ostream & operator<<(std::ostream & os, const arceager::ScoreArray & s) {
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

namespace arceager {

	bool ScoreArray::zero() const {
		for (const auto & score : m_mapScores) {
			if (!score.second.zero()) {
				return false;
			}
		}
		return true;
	}

	int AL_FIRST, AR_FIRST;
	int AL_END, AR_END;

	int decodeAction(const int & action) {
		if (action < AL_FIRST) {
			return action;
		}
		else if (action < AR_FIRST) {
			return ARC_LEFT;
		}
		else {
			return ARC_RIGHT;
		}
	}

	void printAction(const int & action) {
		switch (decodeAction(action)) {
		case REDUCE:
			std::cout << "reduce" << std::endl;
			break;
		case SHIFT:
			std::cout << "shift" << std::endl;
			break;
		case POP_ROOT:
			std::cout << "pop root" << std::endl;
			break;
		case ARC_LEFT:
			std::cout << "arc left " << (action - AL_FIRST + 1) << std::endl;
			break;
		case ARC_RIGHT:
			std::cout << "arc right " << (action - AR_FIRST + 1) << std::endl;
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
