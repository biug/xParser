#include <cstring>

#include "titov_state.h"
#include "common/token/deplabel.h"

namespace titov {

	StateItem::StateItem() {
		clear();
	}

	StateItem::~StateItem() = default;

	void StateItem::arcLeft(const int & l) {
		const int & left = m_lStack[m_nStackBack];
		//add new head for left and add label
		m_lSubHeadR[left] = m_lHeadR[left];
		m_lHeadR[left] = m_nNextWord;
		m_lSubHeadLabelR[left] = m_lHeadLabelR[left];
		m_lHeadLabelR[left] = l;
		++m_lHeadRNum[left];
		//sibling is the previous child of buffer seek
		int & buffer_left_pred = m_lPredL[m_nNextWord];
		if (buffer_left_pred == -1) {
			buffer_left_pred = left;
			m_lPredLabelL[m_nNextWord] = l;
		}
		else if (left < buffer_left_pred) {
			m_lSubPredL[m_nNextWord] = buffer_left_pred;
			m_lSubPredLabelL[m_nNextWord] = m_lPredLabelL[m_nNextWord];
			buffer_left_pred = left;
			m_lPredLabelL[m_nNextWord] = l;
		}
		else {
			int & sub_buffer_left_pred = m_lSubPredL[m_nNextWord];
			if (sub_buffer_left_pred == -1 || left < sub_buffer_left_pred) {
				sub_buffer_left_pred = left;
				m_lSubPredLabelL[m_nNextWord] = l;
			}
		}
		++m_lPredLNum[m_nNextWord];
		//add right arcs for stack seek
		m_lRightNodes[left].push_back(RightNodeWithLabel(m_nNextWord, l, GRAPH_LEFT));
	}

	void StateItem::arcRight(const int & l) {
		const int & left = m_lStack[m_nStackBack];
		//sibling is the previous child of buffer seek
		int & buffer_left_head = m_lHeadL[m_nNextWord];
		if (buffer_left_head == -1) {
			buffer_left_head = left;
			m_lHeadLabelL[m_nNextWord] = l;
		}
		else if (left < buffer_left_head) {
			m_lSubHeadL[m_nNextWord] = buffer_left_head;
			m_lSubHeadLabelL[m_nNextWord] = m_lHeadLabelL[m_nNextWord];
			buffer_left_head = left;
			m_lHeadLabelL[m_nNextWord] = l;
		}
		else {
			int & sub_buffer_left_head = m_lSubHeadL[m_nNextWord];
			if (sub_buffer_left_head == -1 || left < sub_buffer_left_head) {
				sub_buffer_left_head = left;
				m_lSubHeadLabelL[m_nNextWord] = l;
			}
		}
		++m_lHeadLNum[m_nNextWord];
		m_lSubPredR[left] = m_lPredR[left];
		m_lPredR[left] = m_nNextWord;
		m_lSubPredLabelR[left] = m_lPredLabelR[left];
		m_lPredLabelR[left] = l;
		++m_lPredRNum[left];
		m_lRightNodes[left].push_back(RightNodeWithLabel(m_nNextWord, l, GRAPH_RIGHT));
	}

	void StateItem::print() const {
		std::cout << "complete" << std::endl;
		std::cout << "stack(" << m_nStackBack + 1 << ") :";
		for (int i = 0; i <= m_nStackBack; ++i) {
			std::cout << " " << m_lStack[i];
		}
		std::cout << std::endl;
		std::cout << "buffer top : " << m_nNextWord << std::endl;
		std::cout << "arcs :" << std::endl;
		for (int i = 0; i < m_nNextWord; ++i) {
			if (m_lRightNodes[i].size() > 0) {
				std::cout << i;
				for (const auto & rn : m_lRightNodes[i]) {
					std::cout.flush();
					std::cout << (RIGHTNODE_DIRECTION(rn) == GRAPH_LEFT ? " <-" : " ->") << "(" << TDepLabel::key(RIGHTNODE_LABEL(rn)) << ")" << RIGHTNODE_POS(rn);
				}
				std::cout << std::endl;
			}
		}
		std::cout << "last action : ";
		printAction(m_lActionList[m_nActionBack]);
		std::cout << "action size : " << m_nActionBack << std::endl;
		std::cout << "action list : " << std::endl;
		for (int i = 0; i <= m_nActionBack; ++i) {
			printAction(m_lActionList[i]);
		}
		std::cout << "score : " << m_nScore << std::endl;
		std::cout << std::endl;
	}

	void StateItem::check() const {
		StateItem item;
		for (int i = 0; i <= m_nActionBack; ++i) {
			item.move(m_lActionList[i]);
		}
		if (item != *this) {
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
		m_lSubHeadL[m_nNextWord] = -1;
		m_lHeadLabelL[m_nNextWord] = 0;
		m_lHeadLNum[m_nNextWord] = 0;
		m_lHeadR[m_nNextWord] = -1;
		m_lSubHeadR[m_nNextWord] = -1;
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
		case SHIFT:
			shift();
			return;
		case SHIFT_REDUCE:
			shiftReduce();
			return;
		case AL_SW:
			arcLeftSwap(action - AL_SW_FIRST + 1);
			return;
		case AR_SW:
			arcRightSwap(action - AR_SW_FIRST + 1);
			return;
		case AL_SH:
			arcLeftShift(action - AL_SH_FIRST + 1);
			return;
		case AR_SH:
			arcRightShift(action - AR_SH_FIRST + 1);
			return;
		case AL_RE:
			arcLeftReduce(action - AL_RE_FIRST + 1);
			return;
		case AR_RE:
			arcRightReduce(action - AR_RE_FIRST + 1);
			return;
		}
	}

	void StateItem::generateGraph(const DependencyGraph & sent, DependencyGraph & tree) const {
		int i = 0;
		tree.clear();
		for (const auto & token : sent) {
			tree.push_back(DependencyGraphNode(TREENODE_POSTAGGEDWORD(token), HeadWithLabel(-1, NULL_LABEL), m_lRightNodes[i]));
			++i;
		}
	}

	bool StateItem::extractOneStandard(int(&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph, const int & direction, const int & label) {
		if (m_nNextWord < graph.size()) {
			bool left_empty = true;
			for (int i = m_nNextWord - 1; i >= 0; --i) {
				for (const auto & rn : GRAPHNODE_RIGHTNODES(graph[i])) {
					if (RIGHTNODE_POS(rn) == m_nNextWord) {
						left_empty = false;
						break;
					}
				}
				if (!left_empty) {
					break;
				}
			}
			if (left_empty && GRAPHNODE_RIGHTNODES(graph[m_nNextWord]).size() == 0) {
				shiftReduce();
				return true;
			}
		}
		if (m_nStackBack >= 0) {
			int & seek = seeks[m_lStack[m_nStackBack]];
			const DependencyGraphNode & node = graph[m_lStack[m_nStackBack]];
			int size = GRAPHNODE_RIGHTNODES(node).size();
			while (seek < size && GRAPHNODE_RIGHTNODEPOS(node, seek) < m_nNextWord) {
				++seek;
			}
			if (seek >= size) {
				switch (direction) {
				case GRAPH_LEFT:
					arcLeftReduce(label);
					return true;
				case 0:
					reduce();
					return true;
				case GRAPH_RIGHT:
					arcRightReduce(label);
					return true;
				default:
					return false;
				}
			}
			const RightNodeWithLabel & rnwl = GRAPHNODE_RIGHTNODE(node, seek);
			if (RIGHTNODE_POS(rnwl) == m_nNextWord) {
				++seek;
				return extractOneStandard(seeks, graph, RIGHTNODE_DIRECTION(rnwl), RIGHTNODE_LABEL(rnwl));
			}
			else if (m_nStackBack >= 1) {
				int & seek2 = seeks[m_lStack[m_nStackBack - 1]];
				const DependencyGraphNode & node2 = graph[m_lStack[m_nStackBack - 1]];
				int size2 = GRAPHNODE_RIGHTNODES(node2).size();
				if (seek < size && seek2 < size2) {
					const RightNodeWithLabel & rnwl2 = GRAPHNODE_RIGHTNODE(node2, seek2);
					if (RIGHTNODE_POS(rnwl) > RIGHTNODE_POS(rnwl2)) {
						switch (direction) {
						case GRAPH_LEFT:
							arcLeftSwap(label);
							return true;
						case 0:
							swap();
							return true;
						case GRAPH_RIGHT:
							arcRightSwap(label);
							return true;
						default:
							return false;
						}
					}
				}
				else if (seek < size) {
					switch (direction) {
					case GRAPH_LEFT:
						arcLeftSwap(label);
						return true;
					case 0:
						swap();
						return true;
					case GRAPH_RIGHT:
						arcRightSwap(label);
						return true;
					default:
						return false;
					}
				}
			}
		}
		if (m_nNextWord < graph.size()) {
			switch (direction) {
			case GRAPH_LEFT:
				arcLeftShift(label);
				return true;
			case 0:
				shift();
				return true;
			case GRAPH_RIGHT:
				arcRightShift(label);
				return true;
			default:
				return false;
			}
		}
		return false;
	}

	bool StateItem::extractOracle(const DependencyGraph & graph) {
		int rightNodeSeeks[MAX_SENTENCE_SIZE];
		memset(rightNodeSeeks, 0, sizeof(rightNodeSeeks));
		while (extractOneStandard(rightNodeSeeks, graph))
			;
		if (*this == graph) {
			return true;
		}
		else {
			return false;
		}
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
		for (int i = 0; i < m_nNextWord; ++i) {
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
		memcpy(m_lSubHeadL, item.m_lSubHeadL, len);
		memcpy(m_lHeadLabelL, item.m_lHeadLabelL, len);
		memcpy(m_lSubHeadLabelL, item.m_lSubHeadLabelL, len);
		memcpy(m_lHeadLNum, item.m_lHeadLNum, len);
		memcpy(m_lHeadR, item.m_lHeadR, len);
		memcpy(m_lSubHeadR, item.m_lSubHeadR, len);
		memcpy(m_lHeadLabelR, item.m_lHeadLabelR, len);
		memcpy(m_lSubHeadLabelR, item.m_lSubHeadLabelR, len);
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

		for (int index = 0; index <= m_nNextWord; ++index) {
			m_lRightNodes[index].clear();
			m_lRightNodes[index].insert(m_lRightNodes[index].end(), item.m_lRightNodes[index].begin(), item.m_lRightNodes[index].end());
		}

		return *this;
	}
}
