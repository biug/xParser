#include <cstring>

#include "nirve_ring_state.h"
#include "common/token/deplabel.h"

namespace nirve_ring {

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
		m_lRightNodes[left].push_back(RightNodeWithCombineLabel(m_nNextWord, l));
	}

	void StateItem::print() const {
		std::cout << "complete" << std::endl;
		std::cout << "stack(" << m_nStackBack + 1 << ") :";
		for (int i = 0; i <= m_nStackBack; ++i) {
			std::cout << " " << m_lStack[i];
		}
		std::cout << std::endl;
		std::cout << "shift buffer(" << m_nShiftBufferBack + 1 << ") :";
		for (int i = 0; i <= m_nShiftBufferBack; ++i) {
			std::cout << " " << m_lShiftBuffer[i];
		}
		std::cout << std::endl;
		std::cout << "buffer top : " << bufferTop() << std::endl;
		std::cout << "arcs :" << std::endl;
		for (int i = 0; i < m_nNextWord; ++i) {
			if (m_lRightNodes[i].size() > 0) {
				std::cout << i;
				for (const auto & rn : m_lRightNodes[i]) {
					std::cout.flush();
				}
				std::cout << std::endl;
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
		if (item != *this || m_nShiftBufferBack != -1 || m_nStackBack != -1) {
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
		m_nShiftBufferBack = -1;
		m_bCanSwap = false;
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
		case SHIFT:
			shift();
			return;
		case A_SW:
			arcSwap(action - A_SW_FIRST + 1);
			return;
		case A_SH:
			arcShift(action - A_SH_FIRST + 1);
			return;
		case A_RE:
			arcReduce(action - A_RE_FIRST + 1);
			return;
		}
	}

	void StateItem::generateGraph(const DependencyCONLLGraph & sent, DependencyCONLLGraph & tree) const {
		int i = 0;
		tree.clear();
		for (const auto & token : sent) {
			tree.push_back(DependencyCONLLGraphNode(CONLLGRAPHNODE_POSTAGGEDWORD(token), HeadWithLabel(-1, NULL_LABEL), m_lRightNodes[i]));
			++i;
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
	bool StateItem::extractOneStandard(int(&seeks)[MAX_SENTENCE_SIZE], const DependencyCONLLGraph & graph, const int & label) {
		// remove shift-reduce
		// reduce or draw arc
		if (m_nStackBack >= 0) {
			int & seek = seeks[m_lStack[m_nStackBack]];
			const DependencyCONLLGraphNode & node = graph[m_lStack[m_nStackBack]];
			int size = CONLLGRAPHNODE_RIGHTNODES(node).size();
			while (seek < size && CONLLGRAPHNODE_RIGHTNODEPOS(node, seek) < m_nNextWord) {
				++seek;
			}
			if (seek >= size) {
				switch (label) {
				case 0:
					reduce();
					return true;
				default:
					arcReduce(label);
					return true;
				}
			}
			const RightNodeWithCombineLabel & rnwl = CONLLGRAPHNODE_RIGHTNODE(node, seek);
			if (COMBINERIGHTNODE_POS(rnwl) == m_nNextWord) {
				++seek;
				return extractOneStandard(seeks, graph, COMBINERIGHTNODE_LABEL(rnwl));
			}
		}
		// swap after reduce/arc
		for (int i = m_nStackBack - 1; i >= 0; --i) {
			const DependencyCONLLGraphNode & node = graph[m_lStack[i]];
			const int & seek = seeks[m_lStack[i]];
			if (seek < CONLLGRAPHNODE_RIGHTNODES(node).size() && CONLLGRAPHNODE_RIGHTNODEPOS(node, seek) == m_nNextWord) {
				switch (label) {
				case 0:
					swap();
					return true;
				default:
					arcSwap(label);
					return true;
				}
			}
		}
		// shfit after swap
		if (m_nShiftBufferBack >= 0 || bufferTop() < graph.size()) {
			switch (label) {
			case 0:
				shift();
				return true;
			default:
				arcShift(label);
				return true;
			}
		}
		return false;
	}

	bool StateItem::extractOracle(const DependencyCONLLGraph & graph) {
		int rightNodeSeeks[MAX_SENTENCE_SIZE];
		memset(rightNodeSeeks, 0, sizeof(rightNodeSeeks));
		while (extractOneStandard(rightNodeSeeks, graph))
		{
//			printAction(m_lActionList[m_nActionBack]);
//			print();
		}
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

	bool StateItem::operator==(const DependencyCONLLGraph & graph) const {
		if (m_nNextWord != graph.size()) {
			return false;
		}
		for (int i = 0; i < m_nNextWord; ++i) {
			if (m_lRightNodes[i] != CONLLGRAPHNODE_RIGHTNODES(graph[i])) {
				return false;
			}
		}
		return true;
	}

	StateItem & StateItem::operator=(const StateItem & item) {
		m_nStackBack = item.m_nStackBack;
		m_nShiftBufferBack = item.m_nShiftBufferBack;
		m_nActionBack = item.m_nActionBack;
		m_nNextWord = item.m_nNextWord;
		m_nScore = item.m_nScore;
		m_bCanSwap = item.m_bCanSwap;

		size_t len = sizeof(int) * (m_nNextWord + 1);
		if (m_nStackBack >= 0) {
			memcpy(m_lStack, item.m_lStack, sizeof(int) * (m_nStackBack + 1));
		}
		if (m_nShiftBufferBack >= 0) {
			memcpy(m_lShiftBuffer, item.m_lShiftBuffer, sizeof(int) * (m_nShiftBufferBack + 1));
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

		for (int index = 0; index <= m_nNextWord; ++index) {
			m_lRightNodes[index].clear();
			m_lRightNodes[index].insert(m_lRightNodes[index].end(), item.m_lRightNodes[index].begin(), item.m_lRightNodes[index].end());
			m_lPredLabelSetL[index] = item.m_lPredLabelSetL[index];
			m_lPredLabelSetR[index] = item.m_lPredLabelSetR[index];
		}

		return *this;
	}
}
