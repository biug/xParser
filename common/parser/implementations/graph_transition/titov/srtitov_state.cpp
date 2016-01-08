#include <cstring>

#include "srtitov_macros.h"

namespace srtitov {

	StateItem::StateItem() {
		clear();
	}

	StateItem::~StateItem() = default;

	void StateItem::shift() {
		m_lStack[++m_nStackBack] = m_nNextWord++;
		clearNext();
		m_bCanSwap = true;
	}

	void StateItem::reduce() {
		--m_nStackBack;
		m_bCanSwap = true;
	}

	void StateItem::swap() {
		std::swap(m_lStack[m_nStackBack - 1], m_lStack[m_nStackBack]);
	}

	void StateItem::arc(const int & label) {
		ULGraphTransitionStateBase::arc(label);
	}

	void StateItem::swap(const int & action) {
		std::swap(m_lStack[m_nStackBack - 1], m_lStack[m_nStackBack]);
		m_bCanSwap = false;
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::arc(const int & label, const int & action) {
		ULGraphTransitionStateBase::arc(label);
		m_bCanSwap = true;
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::shift(const int & action) {
		shift();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::shiftArc(const int & label, const int & action) {
		shift();
		arc(label);
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::shiftSwap(const int & action) {
		shift();
		swap();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::shiftArcSwap(const int & label, const int & action) {
		shift();
		arc(label);
		swap();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::shiftSwapArc(const int & label, const int & action) {
		shift();
		swap();
		arc(label);
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::shiftArcSwapArc(const int & label1, const int & label2, const int & action) {
		shift();
		arc(label1);
		swap();
		arc(label2);
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::shiftArcSwapArcSwap(const int & label1, const int & label2, const int & action) {
		shift();
		arc(label1);
		swap();
		arc(label2);
		swap();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::shiftSwapArcSwap(const int & label, const int & action) {
		shift();
		swap();
		arc(label);
		swap();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::reduce(const int & action) {
		reduce();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::reduceArc(const int & label, const int & action) {
		reduce();
		arc(label);
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::reduceSwap(const int & action) {
		reduce();
		swap();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::reduceArcSwap(const int & label, const int & action) {
		reduce();
		arc(label);
		swap();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::reduceSwapArc(const int & label, const int & action) {
		reduce();
		swap();
		arc(label);
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::reduceArcSwapArc(const int & label1, const int & label2, const int & action) {
		reduce();
		arc(label1);
		swap();
		arc(label2);
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::reduceArcSwapArcSwap(const int & label1, const int & label2, const int & action) {
		reduce();
		arc(label1);
		swap();
		arc(label2);
		swap();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::reduceSwapArcSwap(const int & label, const int & action) {
		reduce();
		swap();
		arc(label);
		swap();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::print() const {
		std::cout << "complete" << std::endl;
		std::cout << "stack(" << m_nStackBack + 1 << ") :";
		for (int i = 0; i <= m_nStackBack; ++i) {
			std::cout << " " << m_lStack[i];
		}
		std::cout << std::endl;
		std::cout << "arcs :" << std::endl;
		for (int i = 0; i < m_nNextWord; ++i) {
			if (m_vecRightArcs[i].size() > 0) {
				for (const auto & arc : m_vecRightArcs[i]) {
					ttoken label;
					switch (arc.label) {
					case ULA_LEFT:
						std::cout << arc.head << " -> " << i << std::endl;
						break;
					case ULA_RIGHT:
						std::cout << i << " -> " << arc.head << std::endl;
						break;
					default:
						std::cout << arc.head << " -> " << i << std::endl;
						std::cout << i << " -> " << arc.head << std::endl;
					}
				}
			}
		}
		std::cout << "actions: ";
		for (int i = 1; i <= m_nActionBack; ++i) {
			std::cout << " | " << ActionConstant().toStr(m_lActionList[i]);
		}
		std::cout << std::endl;
		std::cout << "score : " << m_nScore << std::endl;
		std::cout << std::endl;
	}

	void StateItem::clear() {
		//reset buffer seek
		m_nNextWord = 0;
		//reset stack seek
		m_nStackBack = -1;
		//reset score
		m_nScore = 0;
		//reset action
		m_nActionBack = 0;
		m_lActionList[m_nActionBack] = NO_ACTION;
		clearNext();
	}

	void StateItem::clearNext() {
		m_lHeadL[m_nNextWord] = -1;
		m_lHeadLNum[m_nNextWord] = 0;
		m_lHeadR[m_nNextWord] = -1;
		m_lHeadRNum[m_nNextWord] = 0;
		m_lPredL[m_nNextWord] = -1;
		m_lSubPredL[m_nNextWord] = -1;
		m_lPredLNum[m_nNextWord] = 0;
		m_lPredR[m_nNextWord] = -1;
		m_lSubPredR[m_nNextWord] = -1;
		m_lPredRNum[m_nNextWord] = 0;
		m_vecRightArcs[m_nNextWord].clear();
	}

	bool StateItem::operator==(const StateItem & item) const {
		if (m_nActionBack != item.m_nActionBack) {
			return false;
		}
		for (int i = m_nActionBack; i >= 0; --i) {
			if (m_lActionList[i] != item.m_lActionList[i]) {
				return false;
			}
		}
		return true;
	}

	bool StateItem::operator==(const DependencyGraph & graph) const {
		if (m_nNextWord != graph.size()) {
			return false;
		}
		for (int i = 0; i < m_nNextWord; ++i) {
			if (m_vecRightArcs[i].size() != graph[i].m_vecRightArcs.size()) {
				return false;
			}
			for (int j = 0; j < m_vecRightArcs[i].size(); ++j) {
				if (m_vecRightArcs[i][j].head != graph[i].m_vecRightArcs[j].first ||
					m_vecRightArcs[i][j].label != graph[i].m_vecRightLabels[j].first) {
					return false;
				}
			}
		}
		return true;
	}

	StateItem & StateItem::operator=(const StateItem & item) {
		m_nStackBack = item.m_nStackBack;
		m_nActionBack = item.m_nActionBack;
		m_nNextWord = item.m_nNextWord;
		m_nScore = item.m_nScore;
		m_bCanSwap = item.m_bCanSwap;

		size_t len = sizeof(int) * (m_nNextWord + 1);
		if (m_nStackBack >= 0) {
			memcpy(m_lStack, item.m_lStack, sizeof(int) * (m_nStackBack + 1));
		}
		memcpy(m_lActionList, item.m_lActionList, sizeof(int) * (m_nActionBack + 1));

		memcpy(m_lHeadL, item.m_lHeadL, len);
		memcpy(m_lHeadLNum, item.m_lHeadLNum, len);
		memcpy(m_lHeadR, item.m_lHeadR, len);
		memcpy(m_lHeadRNum, item.m_lHeadRNum, len);
		memcpy(m_lPredL, item.m_lPredL, len);
		memcpy(m_lSubPredL, item.m_lSubPredL, len);
		memcpy(m_lPredLNum, item.m_lPredLNum, len);
		memcpy(m_lPredR, item.m_lPredR, len);
		memcpy(m_lSubPredR, item.m_lSubPredR, len);
		memcpy(m_lPredRNum, item.m_lPredRNum, len);

		for (int index = 0; index <= m_nNextWord; ++index) {
			m_vecRightArcs[index] =  item.m_vecRightArcs[index];
		}

		return *this;
	}
}
