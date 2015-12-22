#include <cstring>

#include "arceagertag_state.h"
#include "common/token/deplabel.h"
#include "common/token/supertag.h"

namespace arceagertag {

	extern int LABEL_COUNT;
	extern int AL_FIRST;
	extern int AR_FIRST;
	extern int RE_FIRST;
	extern int PP_FIRST;

	StateItem::StateItem() {
		clear();
	}

	StateItem::~StateItem() = default;

	void StateItem::arcLeft(const int & t, const int & l, const tscore & hscore) {
		int left = m_lStack[m_nStackBack--];
		--m_nHeadStackBack;
		m_lSuperTag[left] = t;
		m_lHeads[left] = m_nNextWord;
		m_lLabels[left] = l;
		m_lDepTagL[m_nNextWord].add(l);
		m_lSibling[left] = m_lDepsL[m_nNextWord];
		m_lDepsL[m_nNextWord] = left;
		m_lHeadScore[left] = hscore;
		++m_lDepsNumL[m_nNextWord];
		m_lActions[++m_nActionBack] = AL_FIRST + (t - 1) * LABEL_COUNT + l - 1;
	}

	void StateItem::arcRight(const int & l, const tscore & hscore) {
		int left = m_lStack[m_nStackBack];
		m_lStack[++m_nStackBack] = m_nNextWord;
		m_lHeads[m_nNextWord] = left;
		m_lLabels[m_nNextWord] = l;
		m_lDepTagR[left].add(l);
		m_lSibling[m_nNextWord] = m_lDepsR[left];
		m_lHeadScore[m_nNextWord] = hscore;
		m_lDepsR[left] = m_nNextWord++;
		++m_lDepsNumR[left];
		clearNext();
		m_lActions[++m_nActionBack] = AR_FIRST + l - 1;
	}

	void StateItem::print() const {
		std::cout << "complete" << std::endl;
		for (int i = 0; i < m_nNextWord; ++i) {
			std::cout << "index : " << i << " head : " << m_lHeads[i] << " label : " << m_lLabels[i] << std::endl;
		}
		if (m_nStackBack >= 0) std::cout << "stack :";
		for (int i = 0; i <= m_nStackBack; ++i) {
			std::cout << " " << m_lStack[i];
		}
		if (m_nStackBack >= 0) std::cout << std::endl;
		std::cout << "action list is" << std::endl;;
		for (int i = 0; i <= m_nActionBack; ++i) {
			printAction(m_lActions[i], 0);
		}
		std::cout << "score is " << m_nScore << std::endl;
	}

	void StateItem::clear() {
		m_nNextWord = 0;
		m_nStackBack = -1;
		m_nHeadStackBack = -1;
		m_nScore = 0;
		m_lActions[m_nActionBack = 0] = NO_ACTION;
		clearNext();
	}

	void StateItem::clearNext() {
		m_lHeads[m_nNextWord] = -1;
		m_lDepsL[m_nNextWord] = -1;
		m_lDepsR[m_nNextWord] = -1;
		m_lDepsNumL[m_nNextWord] = 0;
		m_lDepsNumR[m_nNextWord] = 0;
		m_lDepTagL[m_nNextWord].clear();
		m_lDepTagR[m_nNextWord].clear();
		m_lSibling[m_nNextWord] = -1;
		m_lLabels[m_nNextWord] = 0;
	}

	void StateItem::move(const int & action, const tscore & hscore) {
		switch (decodeAction(action)) {
		case NO_ACTION:
			return;
		case SHIFT:
			shift();
			return;
		case REDUCE:
			reduce(action - RE_FIRST + 1);
			return;
		case ARC_LEFT:
			arcLeft((action - AL_FIRST) / LABEL_COUNT + 1, (action - AL_FIRST) % LABEL_COUNT + 1, hscore);
			return;
		case ARC_RIGHT:
			arcRight(action - AR_FIRST + 1, hscore);
			return;
		case POP_ROOT:
			popRoot(action - PP_FIRST + 1);
			return;
		}
	}

	void StateItem::generateTree(const DependencyTaggedTree & sent, DependencyTaggedTree & tree) const {
		int i = 0;
		tree.clear();
		for (const auto & token : sent) {
			tree.push_back(DependencyTaggedTreeNode(DependencyTreeNode(TREENODE_POSTAGGEDWORD(token.first), m_lHeads[i], TDepLabel::key(m_lLabels[i])), TSuperTag::key(m_lSuperTag[i]) + "#" + std::to_string(m_lHeadScore[i])));
			++i;
		}
	}

	bool StateItem::standardMove(const DependencyTaggedTree & tree) {
		int top;
		if (m_nNextWord == tree.size()) {
			if (m_nStackBack > 0) {
				reduce(TSuperTag::code(tree[m_lStack[m_nStackBack]].second));
				return true;
			}
			else {
				popRoot(TSuperTag::code(tree[m_lStack[m_nStackBack]].second));
				return false;
			}
		}
		if (m_nStackBack >= 0) {
			top = m_lStack[m_nStackBack];
			while (!(m_lHeads[top] == -1)) {
				top = m_lHeads[top];
			}
			if (TREENODE_HEAD(tree[top].first) == m_nNextWord) {
				if (top == m_lStack[m_nStackBack]) {
					arcLeft(TSuperTag::code(tree[top].second), TDepLabel::code(TREENODE_LABEL(tree[top].first)), 1);
					return true;
				}
				else {
					reduce(TSuperTag::code(tree[m_lStack[m_nStackBack]].second));
					return true;
				}
			}
		}
		if (TREENODE_HEAD(tree[m_nNextWord].first) == -1 || TREENODE_HEAD(tree[m_nNextWord].first) > m_nNextWord) {
			shift();
			return true;
		}
		else {
			top = m_lStack[m_nStackBack];
			if (TREENODE_HEAD(tree[m_nNextWord].first) == top) {
				arcRight(TDepLabel::code(TREENODE_LABEL(tree[m_nNextWord].first)), 1);
				return true;
			}
			else {
				reduce(TSuperTag::code(tree[top].second));
				return true;
			}
		}
	}

	int StateItem::followMove(const StateItem & item) {
		int top;
		int tag;
		if (m_nNextWord == item.m_nNextWord) {
			top = m_lStack[m_nStackBack];
			tag = item.m_lSuperTag[top];
			if (item.m_lHeads[top] == m_nNextWord) {
				return AL_FIRST + (tag - 1) * LABEL_COUNT + item.m_lLabels[top] - 1;
			}
			else if (item.m_lHeads[top] != -1) {
				return RE_FIRST + tag - 1;
			}
			else {
				return PP_FIRST + tag - 1;
			}
		}
		if (m_nStackBack >= 0) {
			top = m_lStack[m_nStackBack];
			while (!(m_lHeads[top] == -1)) {
				top = m_lHeads[top];
			}
			if (item.m_lHeads[top] == m_nNextWord) {
				tag = item.m_lSuperTag[m_lStack[m_nStackBack]];
				if (top == m_lStack[m_nStackBack]) {
					return AL_FIRST + (tag - 1) * LABEL_COUNT + item.m_lLabels[top] - 1;
				}
				else {
					return RE_FIRST + tag - 1;
				}
			}
		}
		if (item.m_lHeads[m_nNextWord] == -1 || item.m_lHeads[m_nNextWord] > m_nNextWord) {
			return SHIFT;
		}
		else {
			top = m_lStack[m_nStackBack];
			tag = item.m_lSuperTag[top];
			if (item.m_lHeads[m_nNextWord] == top) {
				return AR_FIRST + item.m_lLabels[m_nNextWord] - 1;
			}
			else {
				return RE_FIRST + tag - 1;
			}
		}
	}

	bool StateItem::operator==(const StateItem & item) const {
		if (m_nActionBack != item.m_nActionBack) {
			return false;
		}
		for (int i = m_nActionBack; i >= 0; --i) {
			if (m_lActions[i] != item.m_lActions[i]) {
				return false;
			}
		}
		return true;
	}

	StateItem & StateItem::operator=(const StateItem & item) {
		m_nStackBack = item.m_nStackBack;
		m_nHeadStackBack = item.m_nHeadStackBack;
		m_nNextWord = item.m_nNextWord;
		m_nActionBack = item.m_nActionBack;
		m_nScore = item.m_nScore;

		size_t len = sizeof(int) * (m_nNextWord + 1);
		memcpy(m_lStack, item.m_lStack, sizeof(int) * (m_nStackBack + 1));
		memcpy(m_lHeadStack, item.m_lHeadStack, sizeof(int) * (m_nHeadStackBack + 1));
		memcpy(m_lActions, item.m_lActions, sizeof(int) * (m_nActionBack + 1));

		memcpy(m_lHeads, item.m_lHeads, len);
		memcpy(m_lDepsL, item.m_lDepsL, len);
		memcpy(m_lDepsR, item.m_lDepsR, len);
		memcpy(m_lDepsNumL, item.m_lDepsNumL, len);
		memcpy(m_lDepsNumR, item.m_lDepsNumR, len);
		memcpy(m_lSibling, item.m_lSibling, len);
		memcpy(m_lLabels, item.m_lLabels, len);
		memcpy(m_lSuperTag, item.m_lSuperTag, len);
		memcpy(m_lHeadScore, item.m_lHeadScore, sizeof(tscore) * (m_nNextWord + 1));

		for (int index = 0; index <= m_nNextWord; ++index) {
			m_lDepTagL[index] = item.m_lDepTagL[index];
			m_lDepTagR[index] = item.m_lDepTagR[index];
		}

		return *this;
	}
}
