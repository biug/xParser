#include <cstring>

#include "std_titov_state.h"
#include "common/token/deplabel.h"

namespace std_titov {

	extern std::vector<int> g_vecLabelMap;

	StateItem::StateItem() {
		clear();
	}

	StateItem::~StateItem() = default;

	void StateItem::arc(const int & l) {
		const int & left = m_lStack[m_nStackBack];
		// detect arc label
		const int & labelId = g_vecLabelMap[l];
		int leftLabel = LEFT_LABEL_ID(labelId);
		int rightLabel = RIGHT_LABEL_ID(labelId);
		// arc left
		if (leftLabel != 0) {
			//add new head for left and add label
			m_lHeadR[left] = m_nNextWord;
			m_lHeadLabelR[left] = leftLabel;
			++m_lHeadRNum[left];
			int & buffer_left_pred = m_lPredL[m_nNextWord];
			if (buffer_left_pred == -1) {
				buffer_left_pred = left;
				m_lPredLabelL[m_nNextWord] = leftLabel;
			}
			else if (left < buffer_left_pred) {
				m_lSubPredL[m_nNextWord] = buffer_left_pred;
				m_lSubPredLabelL[m_nNextWord] = m_lPredLabelL[m_nNextWord];
				buffer_left_pred = left;
				m_lPredLabelL[m_nNextWord] = leftLabel;
			}
			else {
				int & sub_buffer_left_pred = m_lSubPredL[m_nNextWord];
				if (sub_buffer_left_pred == -1 || left < sub_buffer_left_pred) {
					sub_buffer_left_pred = left;
					m_lSubPredLabelL[m_nNextWord] = leftLabel;
				}
			}
			++m_lPredLNum[m_nNextWord];
			m_lPredLabelSetL[m_nNextWord].add(leftLabel);
		}
		// arc right
		if (rightLabel != 0) {
			//sibling is the previous child of buffer seek
			int & buffer_left_head = m_lHeadL[m_nNextWord];
			if (buffer_left_head == -1 || left < buffer_left_head) {
				buffer_left_head = left;
				m_lHeadLabelL[m_nNextWord] = rightLabel;
			}
			++m_lHeadLNum[m_nNextWord];
			m_lSubPredR[left] = m_lPredR[left];
			m_lPredR[left] = m_nNextWord;
			m_lSubPredLabelR[left] = m_lPredLabelR[left];
			m_lPredLabelR[left] = rightLabel;
			++m_lPredRNum[left];
			m_lPredLabelSetR[left].add(rightLabel);
		}
		//add right arcs for stack seek
		m_lRightNodes[left].push_back(RightNodeWithLabel(m_nNextWord, l));
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
			if (m_lRightNodes[i].size() > 0) {
				for (const auto & rn : m_lRightNodes[i]) {
					std::cout << i << " -- " << RIGHTNODE_POS(rn) << "(" << TDepLabel::key(RIGHTNODE_LABEL(rn)) << ")" << std::endl;
				}
			}
		}
		std::cout << "last action: ";
		printAction(m_lActionList[m_nActionBack]);
		std::cout << "score : " << m_nScore << std::endl;
		std::cout << std::endl;
	}

	void StateItem::check() const {
		StateItem item;
		for (int i = 0; i <= m_nActionBack; ++i) {
			item.move(m_lActionList[i]);
		}
		if (item != *this || m_nStackBack != -1) {
			std::cout << "origin" << std::endl;
			this->print();
			std::cout << "correct" << std::endl;
			item.print();
		}
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
		m_lHeadLabelL[m_nNextWord] = 0;
		m_lHeadLNum[m_nNextWord] = 0;
		m_lHeadR[m_nNextWord] = -1;
		m_lHeadLabelR[m_nNextWord] = 0;
		m_lHeadRNum[m_nNextWord] = 0;
		m_lPredL[m_nNextWord] = -1;
		m_lSubPredL[m_nNextWord] = -1;
		m_lPredLabelL[m_nNextWord] = 0;
		m_lSubPredLabelL[m_nNextWord] = 0;
		m_lPredLNum[m_nNextWord] = 0;
		m_lPredR[m_nNextWord] = -1;
		m_lSubPredR[m_nNextWord] = -1;
		m_lPredLabelR[m_nNextWord] = 0;
		m_lSubPredLabelR[m_nNextWord] = 0;
		m_lPredRNum[m_nNextWord] = 0;
		m_lSuperTag[m_nNextWord] = 0;
		m_lPredLabelSetL[m_nNextWord].clear();
		m_lPredLabelSetR[m_nNextWord].clear();
		m_lRightNodes[m_nNextWord].clear();
	}

	void StateItem::move(const int & action) {
		switch (decodeAction(action)) {
		case NO_ACTION:
			return;
		case REDUCE:
			reduce();
			return;
		case SWAP:
			swap();
			return;
		case ARC:
			arc(action - A_FIRST + 1);
			return;
		case SHIFT:
			shift(action - SH_FIRST);
			return;
		}
	}

	/*
		for the next word
		we draw the rightmost arc first
		then we draw arc from leftmost to right
		for example
		| 0 | 1 | 2 | 3 | 4 |					| 5 | . . .
		if we have arc 0 - 5, 2 - 5, 3 - 5, 4 - 5, '1' doesn't have arc right node righter than 5
		first we use 4 swap and get state
		| 4 |			| 0 | 1 | 2 | 3 |		| 5 | . . .
		we draw 4 - 5 first
		then we shift | 0 | and arc, get 0 - 5
		then we shift | 1 | and reduce
		then we shift | 2 | and arc, get 2 - 5
		then we shift | 3 | and arc, get 3 - 5
		then we shift | 4 | and arc, get 4 - 5
	*/
	bool StateItem::extractOneStandard(int(&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph, const int & label) {
		// remove shift-reduce
		// reduce or draw arc
		if (m_nStackBack >= 0) {
			int & seek = seeks[m_lStack[m_nStackBack]];
			const DependencyGraphNode & node = graph[m_lStack[m_nStackBack]];
			int size = GRAPHNODE_RIGHTNODES(node).size();
			while (seek < size && GRAPHNODE_RIGHTNODEPOS(node, seek) < m_nNextWord) {
				++seek;
			}
			if (seek >= size) {
				reduce();
				return true;
			}
			const RightNodeWithLabel & rnwl = GRAPHNODE_RIGHTNODE(node, seek);
			if (RIGHTNODE_POS(rnwl) == m_nNextWord) {
				++seek;
				arc(RIGHTNODE_LABEL(rnwl));
				return true;
			}
			if (m_nStackBack > 0) {
				int & seek2 = seeks[m_lStack[m_nStackBack - 1]];
				const DependencyGraphNode & node2 = graph[m_lStack[m_nStackBack - 1]];
				int size2 = GRAPHNODE_RIGHTNODES(node2).size();
				if (seek < size && seek2 < size2) {
					if (RIGHTNODE_POS(GRAPHNODE_RIGHTNODE(node, seek)) > RIGHTNODE_POS(GRAPHNODE_RIGHTNODE(node2, seek2))) {
						swap();
						return true;	
					}
				}
				else if (seek < size) {
					swap();
					return true;
				}
			}
		}
		// shfit after swap
		if (m_nNextWord < graph.size()) {
			shift(TSuperTag::code(GRAPHNODE_SUPERTAG(graph[m_nNextWord])));
			return true;
		}
		return false;
	}

	bool StateItem::extractOracle(const DependencyGraph & graph) {
		int rightNodeSeeks[MAX_SENTENCE_SIZE];
		memset(rightNodeSeeks, 0, sizeof(rightNodeSeeks));
		while (extractOneStandard(rightNodeSeeks, graph))
		{
			;
		}
		return *this == graph;
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
			if (TSuperTag::key(m_lSuperTag[i]) != GRAPHNODE_SUPERTAG(graph[i])) {
				return false;
			}
			if (m_lRightNodes[i] != GRAPHNODE_RIGHTNODES(graph[i])) {
				return false;
			}
		}
		return true;
	}

	StateItem & StateItem::operator=(const StateItem & item) {
		m_nStackBack = item.m_nStackBack;
		m_nActionBack = item.m_nActionBack;
		m_nNextWord = item.m_nNextWord;
		m_nScore = item.m_nScore;

		size_t len = sizeof(int) * (m_nNextWord + 1);
		if (m_nStackBack >= 0) {
			memcpy(m_lStack, item.m_lStack, sizeof(int) * (m_nStackBack + 1));
		}
		memcpy(m_lActionList, item.m_lActionList, sizeof(int) * (m_nActionBack + 1));

		memcpy(m_lHeadL, item.m_lHeadL, len);
		memcpy(m_lHeadLabelL, item.m_lHeadLabelL, len);
		memcpy(m_lHeadLNum, item.m_lHeadLNum, len);
		memcpy(m_lHeadR, item.m_lHeadR, len);
		memcpy(m_lHeadLabelR, item.m_lHeadLabelR, len);
		memcpy(m_lHeadRNum, item.m_lHeadRNum, len);
		memcpy(m_lPredL, item.m_lPredL, len);
		memcpy(m_lSubPredL, item.m_lSubPredL, len);
		memcpy(m_lPredLabelL, item.m_lPredLabelL, len);
		memcpy(m_lSubPredLabelL, item.m_lSubPredLabelL, len);
		memcpy(m_lPredLNum, item.m_lPredLNum, len);
		memcpy(m_lPredR, item.m_lPredR, len);
		memcpy(m_lSubPredR, item.m_lSubPredR, len);
		memcpy(m_lPredLabelR, item.m_lPredLabelR, len);
		memcpy(m_lSubPredLabelR, item.m_lSubPredLabelR, len);
		memcpy(m_lPredRNum, item.m_lPredRNum, len);
		memcpy(m_lSuperTag, item.m_lSuperTag, len);

		for (int index = 0; index <= m_nNextWord; ++index) {
			m_lRightNodes[index].clear();
			m_lRightNodes[index].insert(m_lRightNodes[index].end(), item.m_lRightNodes[index].begin(), item.m_lRightNodes[index].end());
			m_lPredLabelSetL[index] = item.m_lPredLabelSetL[index];
			m_lPredLabelSetR[index] = item.m_lPredLabelSetR[index];
		}

		return *this;
	}
}
