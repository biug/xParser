#include <stack>
#include <sstream>
#include <unordered_set>

#include "twostack_macros.h"

std::istream & operator>>(std::istream & is, twostack::ScoreArray & s) {
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

std::ostream & operator<<(std::ostream & os, const twostack::ScoreArray & s) {
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

namespace twostack {

	int AL_MM_FIRST, AL_RC_FIRST, AL_SH_FIRST, AL_RE_FIRST;
	int AR_MM_FIRST, AR_RC_FIRST, AR_SH_FIRST, AR_RE_FIRST;
	int AL_MM_END, AL_RC_END, AL_SH_END, AL_RE_END;
	int AR_MM_END, AR_RC_END, AR_SH_END, AR_RE_END;

	bool ScoreArray::zero() const {
		for (const auto & score : m_mapScores) {
			if (!score.second.zero()) {
				return false;
			}
		}
		return true;
	}

	int decodeAction(const int & action) {
		if (action < AL_MM_FIRST) {
			return action;
		}
		else if (action < AR_MM_FIRST) {
			return AL_MM;
		}
		else if (action < AL_RC_FIRST) {
			return AR_MM;
		}
		else if (action < AR_RC_FIRST) {
			return AL_RC;
		}
		else if (action < AL_SH_FIRST) {
			return AR_RC;
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
		case SHIFT_REDUCE:
			std::cout << "shift & reduce" << std::endl;
			break;
		case AL_MM:
			std::cout << "arc left mem" << std::endl;
			break;
		case AR_MM:
			std::cout << "arc right mem" << std::endl;
			break;
		case AL_RC:
			std::cout << "arc left recall" << std::endl;
			break;
		case AR_RC:
			std::cout << "arc right recall" << std::endl;
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
