#include <cstring>

#include "arcsr_macros.h"

namespace arcsr {

	StateItem::StateItem() {
		clear();
	}

	StateItem::~StateItem() = default;

	void StateItem::arcLeft(const int & label, const int & leftLabel) {
		const int & left = m_lStack[m_nStackBack];
		m_lHeads[left] = m_nNextWord;
		m_lLabels[left] = label;
		m_lPredLabelSetL[m_nNextWord].add(label);
		m_lSubPredL[left] = m_lPredL[m_nNextWord];
		m_lPredL[m_nNextWord] = left;
		++m_lPredLNum[m_nNextWord];
		m_vecRightArcs[left].push_back(DepRightArc(m_nNextWord, label));
	}

	void StateItem::arcRight(const int & label, const int & rightLabel) {
		const int & left = m_lStack[m_nStackBack];
		m_lHeads[m_nNextWord] = left;
		m_lLabels[m_nNextWord] = label;
		m_lPredLabelSetR[left].add(label);
		m_lSubPredR[m_nNextWord] = m_lPredR[left];
		m_lPredR[left] = m_nNextWord;
		++m_lPredRNum[left];
		m_vecRightArcs[left].push_back(DepRightArc(m_nNextWord, label));
	}

	void StateItem::reduce(const int & tag, const int & action) {
		m_lSuperTag[m_lStack[m_nStackBack--]] = tag;
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::shift(const int & action) {
		m_lStack[++m_nStackBack] = m_nNextWord++;
		++m_nHeadStackBack;
		clearNext();
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::arcLeft(const int & label, const int & leftLabel, const int & tag, const int & action) {
		arcLeft(label, leftLabel);
		reduce(tag, action);
		--m_nHeadStackBack;
	}

	void StateItem::arcRight(const int & label, const int & rightLabel, const int & action) {
		arcRight(label, rightLabel);
		shift(action);
		--m_nHeadStackBack;
	}

	void StateItem::popRoot(const int & tag, const int & action) {
		m_lSuperTag[m_lStack[m_nStackBack--]] = tag;
		m_lActionList[++m_nActionBack] = action;
	}

	void StateItem::print(const DLabel & labels, const DSuperTag & supertags) const {
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
					std::cout << i << " -- " << arc.head << "(" << labels.key(arc.label) << ")" << std::endl;
				}
			}
		}
		std::cout << "last action: ";
//		printAction(m_lActionList[m_nActionBack], labels->count());
		std::cout << "score : " << m_nScore << std::endl;
		std::cout << std::endl;
	}

	void StateItem::clear() {
		//reset buffer seek
		m_nNextWord = 0;
		//reset stack seek
		m_nStackBack = -1;
		m_nHeadStackBack = -1;
		//reset score
		m_nScore = 0;
		//reset action
		m_nActionBack = 0;
		m_lActionList[m_nActionBack] = NO_ACTION;
		clearNext();
	}

	void StateItem::clearNext() {
		m_lHeads[m_nNextWord] = -1;
		m_lLabels[m_nNextWord] = -1;
		m_lPredL[m_nNextWord] = -1;
		m_lSubPredL[m_nNextWord] = -1;
		m_lPredLNum[m_nNextWord] = 0;
		m_lPredR[m_nNextWord] = -1;
		m_lSubPredR[m_nNextWord] = -1;
		m_lPredRNum[m_nNextWord] = 0;
		m_lSuperTag[m_nNextWord] = 0;
		m_lPredLabelSetL[m_nNextWord].clear();
		m_lPredLabelSetR[m_nNextWord].clear();
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
			if (m_lSuperTag[i] != graph[i].m_nSuperTagCode) {
				return false;
			}
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
		m_nHeadStackBack = item.m_nHeadStackBack;
		m_nActionBack = item.m_nActionBack;
		m_nNextWord = item.m_nNextWord;
		m_nScore = item.m_nScore;

		size_t len = sizeof(int) * (m_nNextWord + 1);
		if (m_nStackBack >= 0) {
			memcpy(m_lStack, item.m_lStack, sizeof(int) * (m_nStackBack + 1));
		}
		memcpy(m_lActionList, item.m_lActionList, sizeof(int) * (m_nActionBack + 1));

		memcpy(m_lHeads, item.m_lHeads, len);
		memcpy(m_lLabels, item.m_lLabels, len);
		memcpy(m_lPredL, item.m_lPredL, len);
		memcpy(m_lSubPredL, item.m_lSubPredL, len);
		memcpy(m_lPredLNum, item.m_lPredLNum, len);
		memcpy(m_lPredR, item.m_lPredR, len);
		memcpy(m_lSubPredR, item.m_lSubPredR, len);
		memcpy(m_lPredRNum, item.m_lPredRNum, len);
		memcpy(m_lSuperTag, item.m_lSuperTag, len);

		for (int index = 0; index <= m_nNextWord; ++index) {
			m_vecRightArcs[index] =  item.m_vecRightArcs[index];
			m_lPredLabelSetL[index] = item.m_lPredLabelSetL[index];
			m_lPredLabelSetR[index] = item.m_lPredLabelSetR[index];
		}

		return *this;
	}
}
