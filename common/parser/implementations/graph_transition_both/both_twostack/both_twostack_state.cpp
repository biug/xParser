#include <cstring>

#include "both_twostack_state.h"
#include "common/token/deplabel.h"

namespace both_twostack {

	StateItem::StateItem() {
		clear();
	}

	StateItem::~StateItem() = default;

	void StateItem::print() const {
		std::cout << "complete" << std::endl;
		std::cout << "stack(" << m_nStackBack + 1 << ") :";
		for (int i = 0; i <= m_nStackBack; ++i) {
			std::cout << " " << m_lStack[i];
		}
		std::cout << std::endl;
		std::cout << "shift buffer(" << m_nSecondStackBack + 1 << ") :";
		for (int i = 0; i <= m_nSecondStackBack; ++i) {
			std::cout << " " << m_lSecondStack[i];
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
		if (item != *this || m_nSecondStackBack != -1 || m_nStackBack != -1) {
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
		m_nSecondStackBack = -1;
		m_bCanMem = false;
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
		case MEM:
			mem();
			return;
		case RECALL:
			recall();
			return;
		case A_MM:
			arcMem(action - A_MM_FIRST + 1);
			return;
		case A_RC:
			arcRecall(action - A_RC_FIRST + 1);
			return;
		case A_RE:
			arcReduce(action - A_RE_FIRST + 1);
			return;
		case A_SH:
			arcShift((action - A_SH_FIRST) % g_nGraphLabelCount + 1, (action - A_SH_FIRST) / g_nGraphLabelCount);
			return;
		case SHIFT:
			shift(action - SH_FIRST);
			return;
		}
	}

	/*
	 	action priority is : "reduce" > "arc *" > "memory" > "recall" > "shift"
		for example
		| 0 | 1 |					| 2 | 3 | . . .
		if we have arc 0 - 2, 1 - 3
		we use memory, get state
		| 0 |						| 2 | 3 | . . .
		| 1 |
		we use arc-reduce + recall, get state
		| 1 |						| 2 | 3 | . . .		"0 - 2"
		we use shift + reduce, get state
		| 1 |							| 3 | . . .
		finally we use arc-reduce, get state
		|								| 3 | . . .		"1 - 3"
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
				switch (label) {
				case 0:
					reduce();
					return true;
				default:
					arcReduce(label);
					return true;
				}
			}
			const RightNodeWithLabel & rnwl = GRAPHNODE_RIGHTNODE(node, seek);
			if (RIGHTNODE_POS(rnwl) == m_nNextWord) {
				++seek;
				return extractOneStandard(seeks, graph, RIGHTNODE_LABEL(rnwl));
			}
		}
		// mem after reduce/arc
		for (int i = m_nStackBack - 1; i >= 0; --i) {
			const DependencyGraphNode & node = graph[m_lStack[i]];
			const int & seek = seeks[m_lStack[i]];
			if (seek < GRAPHNODE_RIGHTNODES(node).size() && GRAPHNODE_RIGHTNODEPOS(node, seek) == m_nNextWord) {
				switch (label) {
				case 0:
					mem();
					return true;
				default:
					arcMem(label);
					return true;
				}
			}
		}
		// recall after mem/arc
		for (int i = m_nSecondStackBack; i >= 0; --i) {
			switch (label) {
			case 0:
				recall();
				return true;
			default:
				arcRecall(label);
				return true;
			}
		}
		// shfit after recall
		if (m_nNextWord < graph.size()) {
			switch (label) {
			case 0:
				shift(TSuperTag::code(GRAPHNODE_SUPERTAG(graph[m_nNextWord])));
				return true;
			default:
				arcShift(label,TSuperTag::code(GRAPHNODE_SUPERTAG(graph[m_nNextWord])));
				return true;
			}
		}
		return false;
	}

	bool StateItem::extractOracle(const DependencyGraph & graph) {
		int rightNodeSeeks[MAX_SENTENCE_SIZE];
		memset(rightNodeSeeks, 0, sizeof(rightNodeSeeks));
		while (extractOneStandard(rightNodeSeeks, graph))
			;
		return *this == graph;
	}

	void StateItem::reverse(int size) {
		for (int i = m_nStackBack; i >= 0; --i) {
			m_lStack[i] = size - m_lStack[i] - 1;
		}
		for (int i = m_nSecondStackBack; i >= 0; --i) {
			m_lSecondStack[i] = size - m_lSecondStack[i] - 1;
		}
		int tval[MAX_SENTENCE_SIZE];
		TagSet ttag[MAX_SENTENCE_SIZE];
		for (int i = 0; i < m_nNextWord; ++i) {
			// head
			tval[i] = m_lHeadL[i];
			m_lHeadL[size - i - 1] = m_lHeadR[i];
			m_lHeadR[size - i - 1] = tval[i];
			// head label
			tval[i] = m_lHeadLabelL[i];
			m_lHeadLabelL[size - i - 1] = m_lHeadR[i];
			m_lHeadLabelR[size - i - 1] = tval[i];
			// head arity
			tval[i] = m_lHeadLNum[i];
			m_lHeadLNum[size - i - 1] = m_lHeadRNum[i];
			m_lHeadRNum[size - i - 1] = tval[i];
			// pred
			tval[i] = m_lPredL[i];
			m_lPredL[size - i - 1] = m_lPredR[i];
			m_lPredR[size - i - 1] = tval[i];
			// pred label
			tval[i] = m_lPredLabelL[i];
			m_lPredLabelL[size - i - 1] = m_lPredR[i];
			m_lPredLabelR[size - i - 1] = tval[i];
			// pred arity
			tval[i] = m_lPredLNum[i];
			m_lPredLNum[size - i - 1] = m_lPredRNum[i];
			m_lPredRNum[size - i - 1] = tval[i];
			// sub pred
			tval[i] = m_lSubPredL[i];
			m_lSubPredL[size - i - 1] = m_lSubPredR[i];
			m_lSubPredR[size - i - 1] = tval[i];
			// sub pred label
			tval[i] = m_lSubPredLabelL[i];
			m_lSubPredLabelL[size - i - 1] = m_lSubPredR[i];
			m_lSubPredLabelR[size - i - 1] = tval[i];
			// tagset
			ttag[i] = m_lPredLabelSetL[i];
			m_lPredLabelSetL[size - i - 1] = m_lPredLabelSetR[i];
			m_lPredLabelSetR[size - i - 1] = ttag[i];
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
		m_nSecondStackBack = item.m_nSecondStackBack;
		m_nActionBack = item.m_nActionBack;
		m_nNextWord = item.m_nNextWord;
		m_nScore = item.m_nScore;
		m_bCanMem = item.m_bCanMem;

		size_t len = sizeof(int) * (m_nNextWord + 1);
		if (m_nStackBack >= 0) {
			memcpy(m_lStack, item.m_lStack, sizeof(int) * (m_nStackBack + 1));
		}
		if (m_nSecondStackBack >= 0) {
			memcpy(m_lSecondStack, item.m_lSecondStack, sizeof(int) * (m_nSecondStackBack + 1));
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