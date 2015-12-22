#include <stack>
#include <sstream>
#include <unordered_set>

#include "minitwostack_macros.h"

std::istream & operator>>(std::istream & is, minitwostack::ActionScores & s) {
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

std::ostream & operator<<(std::ostream & os, const minitwostack::ActionScores & s) {
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

namespace minitwostack {

	bool ActionScores::zero() const {
		for (const auto & score : m_mapScores) {
			if (!score.second.zero()) {
				return false;
			}
		}
		return true;
	}

	int A_MM_FIRST, A_RC_FIRST, A_SH_FIRST, A_RE_FIRST;
	int A_MM_END, A_RC_END, A_SH_END, A_RE_END;
	std::vector<int> LABEL_MAP;

	int decodeAction(const int & action) {
		if (action <= REDUCE) {
			return action;
		}
		else if (action < A_MM_END) {
			return A_MM;
		}
		else if (action < A_RC_END) {
			return A_RC;
		}
		else if (action < A_SH_END) {
			return A_SH;
		}
		else if (action < A_RE_END) {
			return A_RE;
		}
		return NO_ACTION;
	}

	void printAction(const int & action) {
		switch (decodeAction(action)) {
		case MEM:
			std::cout << "mem" << std::endl;
			break;
		case RECALL:
			std::cout << "recall" << std::endl;
			break;
		case SHIFT:
			std::cout << "shift" << std::endl;
			break;
		case REDUCE:
			std::cout << "reduce" << std::endl;
			break;
		case A_MM:
			std::cout << "arc mem " << (action - A_MM_FIRST + 1) << std::endl;
			break;
		case A_RC:
			std::cout << "arc recall " << (action - A_RC_FIRST + 1) << std::endl;
			break;
		case A_SH:
			std::cout << "arc shift " << (action - A_SH_FIRST + 1) << std::endl;
			break;
		case A_RE:
			std::cout << "arc reduce " << (action - A_RE_FIRST + 1) << std::endl;
			break;
		case NO_ACTION:
			std::cout << "no action" << std::endl;
			break;
		default:
			std::cout << "wrong action" << std::endl;
			break;
		}
	}

	void loadLabels() {
		int count = 0;
		LABEL_MAP.clear();
		LABEL_MAP.push_back(0);
		std::unordered_map<std::string, int> labelMap;

		for (int i = TDepLabel::getTokenizer().start(); i < TDepLabel::getTokenizer().end(); ++i) {
			const ttoken & label = TDepLabel::key(i);
			if (IS_LEFT_LABEL(label)) {
				if (labelMap.find(DECODE_LEFT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_LEFT_LABEL(label)] = ++count;
				}
				LABEL_MAP.push_back(ENCODE_LABEL_ID(labelMap[DECODE_LEFT_LABEL(label)], 0));
			}
			else if (IS_RIGHT_LABEL(label)) {
				if (labelMap.find(DECODE_RIGHT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_RIGHT_LABEL(label)] = ++count;
				}
				LABEL_MAP.push_back(ENCODE_LABEL_ID(0, labelMap[DECODE_RIGHT_LABEL(label)]));
			}
			else if (IS_TWOWAY_LABEL(label)) {
				if (labelMap.find(DECODE_TWOWAY_LEFT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_TWOWAY_LEFT_LABEL(label)] = ++count;
				}
				if (labelMap.find(DECODE_TWOWAY_RIGHT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)] = ++count;
				}
				LABEL_MAP.push_back(ENCODE_LABEL_ID(labelMap[DECODE_TWOWAY_LEFT_LABEL(label)], labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)]));
			}
		}
	}
}
