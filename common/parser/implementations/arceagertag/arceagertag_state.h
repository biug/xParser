#ifndef _ARCEAGERTAG_STATE_H
#define _ARCEAGERTAG_STATE_H

#include "arceagertag_macros.h"
#include "common/token/deplabel.h"

namespace arceagertag {

	extern int RE_FIRST;
	extern int PP_FIRST;
	extern int RE_END;

	class StateItem {
	private:
		int m_nStackBack;
		int m_nHeadStackBack;
		int m_nNextWord;
		int m_nActionBack;
		tscore m_nScore;

		int m_lLabels[MAX_SENTENCE_SIZE];
		int m_lStack[MAX_SENTENCE_SIZE];
		int m_lHeadStack[MAX_SENTENCE_SIZE];
		int m_lHeads[MAX_SENTENCE_SIZE];
		int m_lDepsL[MAX_SENTENCE_SIZE];
		int m_lDepsR[MAX_SENTENCE_SIZE];
		int m_lDepsNumL[MAX_SENTENCE_SIZE];
		int m_lDepsNumR[MAX_SENTENCE_SIZE];
		int m_lSibling[MAX_SENTENCE_SIZE];
		int m_lSuperTag[MAX_SENTENCE_SIZE];
		tscore m_lHeadScore[MAX_SENTENCE_SIZE];

		SetOfDepLabels m_lDepTagL[MAX_SENTENCE_SIZE];
		SetOfDepLabels m_lDepTagR[MAX_SENTENCE_SIZE];

		int m_lActions[MAX_SENTENCE_SIZE << 1];

	public:
		StateItem();
		~StateItem();

		void arcLeft(const int & t, const int & l, const tscore & hscore);
		void arcRight(const int & l, const tscore & hscore);
		void shift();
		void reduce(const int & t);
		void popRoot(const int & t);

		const tscore & getScore() const;
		void setScore(const tscore & s);

		const int & stackTop() const;
		const int & head(const int & index) const;
		const int & supertag(const int & index) const;
		const int & leftDep(const int & index) const;
		const int & rightDep(const int & index) const;
		const int & sibling(const int & index) const;
		const int & label(const int & index) const;
		const int & leftArity(const int & index) const;
		const int & rightArity(const int & index) const;
		const tscore & headScore(const int & index) const;
		const SetOfDepLabels & leftSetOfLabels(const int & index) const;
		const SetOfDepLabels & rightSetOfLabels(const int & index) const;

		const int & size() const;
		const int & stackBack() const;
		const int & headStackBack() const;
		const int & lastAction() const;

		bool afterReduce() const;
		bool stackEmpty() const;

		void print() const;

		void clear();
		void clearNext();
		void move(const int & action, const tscore & hscore = 0);
		void generateTree(const DependencyTaggedTree & sent, DependencyTaggedTree & tree) const;
		bool standardMove(const DependencyTaggedTree & tree);
		int followMove(const StateItem & item);

		bool operator<(const StateItem & i) const;
		bool operator<=(const StateItem & i) const;
		bool operator>(const StateItem & i) const;

		bool operator==(const StateItem & i) const;
		bool operator!=(const StateItem & i) const;
		StateItem & operator=(const StateItem & i);
	};

	inline void StateItem::shift() {
		m_lStack[++m_nStackBack] = m_nNextWord;
		m_lHeadStack[++m_nHeadStackBack] = m_nNextWord++;
		clearNext();
		m_lActions[++m_nActionBack] = SHIFT;
	}

	inline void StateItem::reduce(const int & t) {
		m_lSuperTag[m_lStack[m_nStackBack--]] = t;
		m_lActions[++m_nActionBack] = RE_FIRST + t - 1;
	}

	inline void StateItem::popRoot(const int & t) {
		m_lSuperTag[m_lStack[m_nStackBack]] = t;
		m_lLabels[m_lStack[m_nStackBack--]] = TDepLabel::ROOT;
		m_lActions[++m_nActionBack] = PP_FIRST + t - 1;
	}

	inline const tscore & StateItem::getScore() const {
		return m_nScore;
	}

	inline void StateItem::setScore(const tscore & s) {
		m_nScore = s;
	}

	inline const int & StateItem::stackTop() const {
		return m_lStack[m_nStackBack];
	}

	inline const int & StateItem::head(const int & index) const {
		return m_lHeads[index];
	}

	inline const int & StateItem::supertag(const int & index) const {
		return m_lSuperTag[index];
	}

	inline const int & StateItem::leftDep(const int & index) const {
		return m_lDepsL[index];
	}

	inline const int & StateItem::rightDep(const int & index) const {
		return m_lDepsR[index];
	}

	inline const int & StateItem::sibling(const int & index) const {
		return m_lSibling[index];
	}

	inline const int & StateItem::label(const int & index) const {
		return m_lLabels[index];
	}

	inline const int & StateItem::leftArity(const int & index) const {
		return m_lDepsNumL[index];
	}

	inline const int & StateItem::rightArity(const int & index) const {
		return m_lDepsNumR[index];
	}

	inline const tscore & StateItem::headScore(const int & index) const {
		return m_lHeadScore[index];
	}

	inline const SetOfDepLabels & StateItem::leftSetOfLabels(const int & index) const {
		return m_lDepTagL[index];
	}

	inline const SetOfDepLabels & StateItem::rightSetOfLabels(const int & index) const {
		return m_lDepTagR[index];
	}

	inline const int & StateItem::size() const {
		return m_nNextWord;
	}

	inline const int & StateItem::stackBack() const {
		return m_nStackBack;
	}

	inline const int & StateItem::headStackBack() const {
		return m_nHeadStackBack;
	}

	inline const int & StateItem::lastAction() const {
		return m_lActions[m_nActionBack];
	}

	inline bool StateItem::afterReduce() const {
		int action = m_lActions[m_nActionBack];
		return action >= RE_FIRST && action < RE_END;
	}

	inline bool StateItem::stackEmpty() const {
		return m_nStackBack == -1;
	}

	inline bool StateItem::operator<(const StateItem & item) const {
		return m_nScore < item.m_nScore;
	}

	inline bool StateItem::operator<=(const StateItem & item) const {
		return m_nScore <= item.m_nScore;
	}

	inline bool StateItem::operator>(const StateItem & item) const {
		return m_nScore > item.m_nScore;
	}

	inline bool StateItem::operator!=(const StateItem & item) const {
		return !(*this == item);
	}
}

#endif
