#include <stack>
#include <sstream>
#include <unordered_set>

#include "titov_macros.h"

std::istream & operator>>(std::istream & is, titov::ScoreArray & s) {
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

std::ostream & operator<<(std::ostream & os, const titov::ScoreArray & s) {
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

namespace titov {

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
		case SWAP:
			std::cout << "swap";
			break;
		case SHIFT:
			std::cout << "shift";
			break;
		case REDUCE:
			std::cout << "reduce";
			break;
		case SHIFT_REDUCE:
			std::cout << "shift & reduce";
			break;
		case AL_SW:
			std::cout << "arc left swap";
			break;
		case AR_SW:
			std::cout << "arc right swap";
			break;
		case AL_SH:
			std::cout << "arc left shift";
			break;
		case AR_SH:
			std::cout << "arc right shift";
			break;
		case AL_RE:
			std::cout << "arc left reduce";
			break;
		case AR_RE:
			std::cout << "arc right reduce";
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