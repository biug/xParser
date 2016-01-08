#ifndef _ULGRAPH_TRANSITION_STATE_BASE_H
#define _ULGRAPH_TRANSITION_STATE_BASE_H

#include "graph_transition_macros.h"

#define ULA_LEFT	0
#define ULA_RIGHT	1
#define ULA_TWOWAY	2
#define ULA_MAX		3

class ULGraphTransitionStateBase {
protected:
	tscore m_nScore;

	int m_nNextWord;
	int m_nStackBack;

	int m_nActionBack;
	int m_lActionList[MAX_SENTENCE_SIZE << MAX_SENTENCE_BITS];

	int m_lStack[MAX_SENTENCE_SIZE];
	int m_lHeadL[MAX_SENTENCE_SIZE];	//heads for every node
	int m_lHeadLNum[MAX_SENTENCE_SIZE];
	int m_lHeadR[MAX_SENTENCE_SIZE];
	int m_lHeadRNum[MAX_SENTENCE_SIZE];
	int m_lPredL[MAX_SENTENCE_SIZE];		//left dependency children
	int m_lSubPredL[MAX_SENTENCE_SIZE];
	int m_lPredLNum[MAX_SENTENCE_SIZE];
	int m_lPredR[MAX_SENTENCE_SIZE];		//right dependency children
	int m_lSubPredR[MAX_SENTENCE_SIZE];
	int m_lPredRNum[MAX_SENTENCE_SIZE];

	std::vector<DepRightArc> m_vecRightArcs[MAX_SENTENCE_SIZE];

public:
	ULGraphTransitionStateBase() : m_nScore(0), m_nNextWord(0), m_nStackBack(-1), m_nActionBack(-1) {}
	virtual ~ULGraphTransitionStateBase() {};

	const tscore & getScore() const;
	void setScore(const tscore & s);

	void referAction(const int & index, const int & action);
	void referActionBack(const int & actionBack);
	const int & action(const int & index) const;
	const int & stack(const int & index) const;
	const int & leftHead(const int & index) const;
	const int & rightHead(const int & index) const;
	const int & leftPred(const int & index) const;
	const int & rightPred(const int & index) const;
	const int & leftSubPred(const int & index) const;
	const int & rightSubPred(const int & index) const;
	const int & leftHeadArity(const int & index) const;
	const int & leftPredArity(const int & index) const;
	const int & rightHeadArity(const int & index) const;
	const int & rightPredArity(const int & index) const;

	const int & size() const;
	const int & stackBack() const;
	const int & stackTop() const;
	const int & stackSubTop() const;
	const int & stackSecondSubTop() const;
	const int & actionBack() const;
	bool stackEmpty() const;

	void arc(const int & label);
	void arcLeft();
	void arcRight();
	void generateGraph(const DependencyGraph & sent, DependencyGraph & graph) const;

	virtual void print() const = 0;
};

inline const tscore & ULGraphTransitionStateBase::getScore() const {
	return m_nScore;
}

inline void ULGraphTransitionStateBase::setScore(const tscore & s) {
	m_nScore = s;
}

inline void ULGraphTransitionStateBase::referAction(const int & index, const int & action) {
	m_lActionList[index] = action;
}

inline void ULGraphTransitionStateBase::referActionBack(const int & actionBack) {
	m_nActionBack = actionBack;
}

inline const int & ULGraphTransitionStateBase::action(const int & index) const {
	return m_lActionList[index];
}

inline const int & ULGraphTransitionStateBase::stack(const int & index) const {
	return m_lStack[index];
}

inline const int & ULGraphTransitionStateBase::leftHead(const int & index) const {
	return m_lHeadL[index];
}

inline const int & ULGraphTransitionStateBase::rightHead(const int & index) const {
	return m_lHeadR[index];
}

inline const int & ULGraphTransitionStateBase::leftPred(const int & index) const {
	return m_lPredL[index];
}

inline const int & ULGraphTransitionStateBase::rightPred(const int & index) const {
	return m_lPredR[index];
}

inline const int & ULGraphTransitionStateBase::leftSubPred(const int & index) const {
	return m_lSubPredL[index];
}

inline const int & ULGraphTransitionStateBase::rightSubPred(const int & index) const {
	return m_lSubPredR[index];
}

inline const int & ULGraphTransitionStateBase::leftHeadArity(const int & index) const {
	return m_lHeadLNum[index];
}

inline const int & ULGraphTransitionStateBase::leftPredArity(const int & index) const {
	return m_lPredLNum[index];
}

inline const int & ULGraphTransitionStateBase::rightHeadArity(const int & index) const {
	return m_lHeadRNum[index];
}

inline const int & ULGraphTransitionStateBase::rightPredArity(const int & index) const {
	return m_lPredRNum[index];
}

inline const int & ULGraphTransitionStateBase::size() const {
	return m_nNextWord;
}

inline const int & ULGraphTransitionStateBase::stackBack() const {
	return m_nStackBack;
}

inline const int & ULGraphTransitionStateBase::stackSubTop() const {
	return m_lStack[m_nStackBack - 1];
}

inline const int & ULGraphTransitionStateBase::stackSecondSubTop() const {
	return m_lStack[m_nStackBack - 2];
}

inline const int & ULGraphTransitionStateBase::stackTop() const {
	return m_lStack[m_nStackBack];
}

inline const int & ULGraphTransitionStateBase::actionBack() const {
	return m_nActionBack;
}

inline bool ULGraphTransitionStateBase::stackEmpty() const {
	return m_nStackBack == -1;
}

inline void ULGraphTransitionStateBase::generateGraph(const DependencyGraph & sent, DependencyGraph & graph) const {
	for (int i = 0, n = sent.size(); i < n; ++i) {
		graph.add(sent[i]);
		graph.back().m_vecRightArcs.clear();
		for (const auto & arc : m_vecRightArcs[i]) {
			ttoken label;
			switch (arc.label) {
			case ULA_LEFT:
				label = "left1";
				break;
			case ULA_RIGHT:
				label = "right1";
				break;
			default:
				label = "twoway1";
				break;
			}
			graph.back().m_vecRightArcs.push_back(std::pair<int, ttoken>(arc.head, label));
		}
	}
}

#endif
