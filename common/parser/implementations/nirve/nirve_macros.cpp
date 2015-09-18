#include <stack>
#include <sstream>
#include <unordered_set>

#include "nirve_macros.h"

std::istream & operator>>(std::istream & is, nirve::ScoreArray & s) {
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

std::ostream & operator<<(std::ostream & os, const nirve::ScoreArray & s) {
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

namespace nirve {

	int AL_SW_FIRST, AL_SH_FIRST, AL_RE_FIRST;
	int AR_SW_FIRST, AR_SH_FIRST, AR_RE_FIRST;
	int AL_SW_END, AL_SH_END, AL_RE_END;
	int AR_SW_END, AR_SH_END, AR_RE_END;

	bool ScoreArray::zero() const {
		for (const auto & score : m_mapScores) {
			if (!score.second.zero()) {
				return false;
			}
		}
		return true;
	}

	int decodeAction(const int & action) {
		if (action < AL_SW_FIRST) {
			return action;
		}
		else if (action < AR_SW_FIRST) {
			return AL_SW;
		}
		else if (action < AL_SH_FIRST) {
			return AR_SW;
		}
		else if (action < AR_SH_FIRST) {
			return AL_SH;
		}
		else if (action < AL_RE_FIRST) {
			return AR_SH;
		}
		else if (action < AR_RE_FIRST) {
			return AL_RE;
		}
		else {
			return AR_RE;
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
		case SWAP:
			std::cout << "swap" << std::endl;
			break;
		case SHIFT_REDUCE:
			std::cout << "shift & reduce" << std::endl;
			break;
		case AL_SW:
			std::cout << "arc left swap" << std::endl;
			break;
		case AR_SW:
			std::cout << "arc right swap" << std::endl;
			break;
		case AL_SH:
			std::cout << "arc left shift" << std::endl;
			break;
		case AR_SH:
			std::cout << "arc right shift" << std::endl;
			break;
		case AL_RE:
			std::cout << "arc left reduce" << std::endl;
			break;
		case AR_RE:
			std::cout << "arc right reduce" << std::endl;
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