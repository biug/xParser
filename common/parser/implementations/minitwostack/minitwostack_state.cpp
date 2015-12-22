#include <cstring>

#include "minitwostack_state.h"
#include "common/token/deplabel.h"

namespace minitwostack {

	extern int A_MM_FIRST;
	extern int A_RC_FIRST;
	extern int A_SH_FIRST;
	extern int A_RE_FIRST;

	extern int A_MM_END;
	extern int A_RC_END;
	extern int A_SH_END;
	extern int A_RE_END;

	extern std::vector<int> LABEL_MAP;

	StateItem::StateItem() {
		clear();
	}

	StateItem::~StateItem() = default;

	void StateItem::arc(const int & label) {
		const int & left = m_lStack[m_nStackBack];
		// detect arc label
		int leftLabel = LABEL_MAP[label] >> 8;
		int rightLabel = LABEL_MAP[label] & 0xff;
		// arc left
		if (leftLabel != 0) {
			arcLeft(leftLabel);
		}
		// arc right
		if (rightLabel != 0) {
			arcRight(rightLabel);
		}
		//add right arcs for stack seek
		m_vecRightArcs[left].push_back(DepRightArc(m_nNextWord, label));
	}

	void StateItem::arcLeft(const int & label) {
		const int & left = m_lStack[m_nStackBack];
		//add new head for left and add label
		m_lHeadR[left] = m_nNextWord;
		m_lHeadLabelR[left] = label;
		++m_lHeadRNum[left];
		int & buffer_left_pred = m_lPredL[m_nNextWord];
		if (buffer_left_pred == -1) {
			buffer_left_pred = left;
			m_lPredLabelL[m_nNextWord] = label;
		}
		else if (left < buffer_left_pred) {
			m_lSubPredL[m_nNextWord] = buffer_left_pred;
			m_lSubPredLabelL[m_nNextWord] = m_lPredLabelL[m_nNextWord];
			buffer_left_pred = left;
			m_lPredLabelL[m_nNextWord] = label;
		}
		else {
			int & sub_buffer_left_pred = m_lSubPredL[m_nNextWord];
			if (sub_buffer_left_pred == -1 || left < sub_buffer_left_pred) {
				sub_buffer_left_pred = left;
				m_lSubPredLabelL[m_nNextWord] = label;
			}
		}
		++m_lPredLNum[m_nNextWord];
		m_lPredLabelSetL[m_nNextWord].add(label);
	}

	void StateItem::arcRight(const int & label) {
		const int & left = m_lStack[m_nStackBack];
		//sibling is the previous child of buffer seek
		int & buffer_left_head = m_lHeadL[m_nNextWord];
		if (buffer_left_head == -1 || left < buffer_left_head) {
			buffer_left_head = left;
			m_lHeadLabelL[m_nNextWord] = label;
		}
		++m_lHeadLNum[m_nNextWord];
		m_lSubPredR[left] = m_lPredR[left];
		m_lPredR[left] = m_nNextWord;
		m_lSubPredLabelR[left] = m_lPredLabelR[left];
		m_lPredLabelR[left] = label;
		++m_lPredRNum[left];
		m_lPredLabelSetR[left].add(label);
	}

	void StateItem::mem() {
		m_lSecondStack[++m_nSecondStackBack] = m_lStack[m_nStackBack--];
		m_lActionList[++m_nActionBack] = MEM;
	}

	void StateItem::recall() {
		m_bCanMem = false;
		m_lStack[++m_nStackBack] = m_lSecondStack[m_nSecondStackBack--];
		m_lActionList[++m_nActionBack] = RECALL;
	}

	void StateItem::shift() {
		m_bCanMem = true;
		m_lStack[++m_nStackBack] = m_nNextWord++;
		m_lActionList[++m_nActionBack] = SHIFT;
		clearNext();
	}

	void StateItem::reduce() {
		--m_nStackBack;
		m_lActionList[++m_nActionBack] = REDUCE;
	}

	void StateItem::arcMem(const int & label) {
		arc(label);
		mem();
		m_lActionList[m_nActionBack] = A_MM_FIRST + label - 1;
	}

	void StateItem::arcRecall(const int & label) {
		arc(label);
		recall();
		m_lActionList[m_nActionBack] = A_RC_FIRST + label - 1;
	}

	void StateItem::arcShift(const int & label) {
		arc(label);
		shift();
		m_lActionList[m_nActionBack] = A_SH_FIRST + label - 1;
	}

	void StateItem::arcReduce(const int & label) {
		arc(label);
		reduce();
		m_lActionList[m_nActionBack] = A_RE_FIRST + label - 1;
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
		m_lPredLabelSetL[m_nNextWord].clear();
		m_lPredLabelSetR[m_nNextWord].clear();
		m_vecRightArcs[m_nNextWord].clear();
	}

	void StateItem::print() const {
		std::cout << "complete" << std::endl;
		std::cout << "stack(" << m_nStackBack + 1 << ") :";
		for (int i = 0; i <= m_nStackBack; ++i) {
			std::cout << " " << m_lStack[i];
		}
		std::cout << std::endl;
		std::cout << "second stack(" << m_nSecondStackBack + 1 << ") :";
		for (int i = 0; i <= m_nSecondStackBack; ++i) {
			std::cout << " " << m_lSecondStack[i];
		}
		std::cout << std::endl;
		std::cout << "second stack top : " << secondStackTop() << std::endl;
		std::cout << "arcs :" << std::endl;
		for (int i = 0; i < m_nNextWord; ++i) {
			if (m_vecRightArcs[i].size() > 0) {
				for (const auto & arc : m_vecRightArcs[i]) {
					std::cout << i << " -- " << arc.head << "(" << TDepLabel::key(arc.label) << ")" << std::endl;
				}
			}
		}
		std::cout << "last action: ";
		printAction(m_lActionList[m_nActionBack]);
		std::cout << "score : " << m_nScore << std::endl;
		std::cout << std::endl;
	}

	bool StateItem::move(const int & action) {
		if (action <= REDUCE) {
			switch(action) {
			case MEM:
				mem();
				return true;
			case RECALL:
				recall();
				return true;
			case SHIFT:
				shift();
				return true;
			case REDUCE:
				reduce();
				return true;
			}
		}
		else if (action < A_MM_END) {
			arcMem(action - A_MM_FIRST + 1);
			return true;
		}
		else if (action < A_RC_END) {
			arcRecall(action - A_RC_FIRST + 1);
			return true;
		}
		else if (action < A_SH_END) {
			arcShift(action - A_SH_FIRST + 1);
			return true;
		}
		else if (action < A_RE_END) {
			arcReduce(action - A_RE_FIRST + 1);
			return true;
		}
		return false;
	}

	bool StateItem::extractOracle(const DependencyGraph & graph) {
		int nextWord = 0;
		int trueIndex[MAX_SENTENCE_SIZE];
		int rightNodeSeeks[MAX_SENTENCE_SIZE];
		memset(rightNodeSeeks, 0, sizeof(rightNodeSeeks));
		for (int i = 0, j = 0; i < graph.size(); ++i) {
			if (graph[i].m_sSuperTag == "K") {
				trueIndex[j++] = i;
			}
		}
		while (followOneAction(nextWord, trueIndex, rightNodeSeeks, graph, 0)) {
		}
			;
		DependencyGraph retval;
		generateGraph(graph, retval);
//		std::cout << std::endl << graph << std::endl;
//		std::cout << std::endl << retval << std::endl;
		return graph == retval;
	}

	bool StateItem::followOneAction(int & nNextWord, int (&tIndex)[MAX_SENTENCE_SIZE], int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph, const int & label) {
		if (!stackEmpty()) {
			int & seek = seeks[tIndex[stackTop()]];
			const DependencyGraphNode & node = graph[tIndex[stackTop()]];
			int size = node.m_vecRightArcs.size();
			while (seek < size && node.m_vecRightArcs[seek].first < nNextWord) {
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
			const auto & rightArc = node.m_vecRightArcs[seek];
			if (rightArc.first == nNextWord) {
				++seek;
				return followOneAction(nNextWord, tIndex, seeks, graph, TDepLabel::code(node.m_vecRightArcs[seek - 1].second));
			}
		}
		// swap after reduce/arc
		for (int i = m_nStackBack - 1; i >= 0; --i) {
			const DependencyGraphNode & node = graph[tIndex[m_lStack[i]]];
			const int & seek = seeks[tIndex[m_lStack[i]]];
			if (seek < node.m_vecRightArcs.size() && node.m_vecRightArcs[seek].first == nNextWord) {
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
		// shfit after swap
		while (nNextWord < graph.size() && graph[nNextWord].m_sSuperTag != "K") {
			++nNextWord;
		}
		if (nNextWord < graph.size()) {
			switch (label) {
			case 0:
				shift();
				++nNextWord;
				while (nNextWord < graph.size() && graph[nNextWord].m_sSuperTag != "K") {
					++nNextWord;
				}
				return true;
			default:
				arcShift(label);
				++nNextWord;
				while (nNextWord < graph.size() && graph[nNextWord].m_sSuperTag != "K") {
					++nNextWord;
				}
				return true;
			}
		}
		return false;
	}


	void StateItem::generateGraph(const DependencyGraph & sent, DependencyGraph & tree) const {
		std::vector<int> vecRealIndex;
		for (int i = 0; i < sent.size(); ++i) {
			if (sent[i].m_sSuperTag == "K") {
				vecRealIndex.push_back(i);
			}
		}
		for (int i = 0, j = 0, n = sent.size(); i < n; ++i) {
			tree.add(sent[i]);
			tree.back().m_vecRightArcs.clear();
			if (sent[i].m_sSuperTag == "K") {
				for (const auto & arc : m_vecRightArcs[j]) {
					tree.back().m_vecRightArcs.push_back(std::pair<int, ttoken>(vecRealIndex[arc.head], TDepLabel::key(arc.label)));
				}
				++j;
			}
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
			if (m_vecRightArcs[i].size() != graph[i].m_vecRightArcs.size()) {
				return false;
			}
			for (int j = 0; j < m_vecRightArcs[i].size(); ++j) {
				if (m_vecRightArcs[i][j].head != graph[i].m_vecRightArcs[j].first ||
					m_vecRightArcs[i][j].label != TDepLabel::code(graph[i].m_vecRightArcs[j].second)) {
					return false;
				}
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

		for (int index = 0; index <= m_nNextWord; ++index) {
			m_vecRightArcs[index] =  item.m_vecRightArcs[index];
			m_lPredLabelSetL[index] = item.m_lPredLabelSetL[index];
			m_lPredLabelSetR[index] = item.m_lPredLabelSetR[index];
		}

		return *this;
	}
}
