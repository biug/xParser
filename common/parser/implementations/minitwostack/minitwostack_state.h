#ifndef _MINITWOSTACK_STATE_H
#define _MINITWOSTACK_STATE_H

#include "minitwostack_macros.h"
#include "common/token/deplabel.h"

namespace minitwostack {

	class StateItem {
	private:
		tscore m_nScore;

		bool m_bCanMem;

		int m_nNextWord;
		int m_nStackBack;
		int m_nSecondStackBack;

		int m_nActionBack;
		int m_lActionList[MAX_SENTENCE_SIZE << MAX_SENTENCE_BITS];

		int m_lStack[MAX_SENTENCE_SIZE];
		int m_lSecondStack[MAX_SENTENCE_SIZE];
		int m_lHeadL[MAX_SENTENCE_SIZE];	//heads for every node
		int m_lHeadLabelL[MAX_SENTENCE_SIZE];	//label for every node
		int m_lHeadLNum[MAX_SENTENCE_SIZE];
		int m_lHeadR[MAX_SENTENCE_SIZE];
		int m_lHeadLabelR[MAX_SENTENCE_SIZE];
		int m_lHeadRNum[MAX_SENTENCE_SIZE];
		int m_lPredL[MAX_SENTENCE_SIZE];		//left dependency children
		int m_lSubPredL[MAX_SENTENCE_SIZE];
		int m_lPredLabelL[MAX_SENTENCE_SIZE];
		int m_lSubPredLabelL[MAX_SENTENCE_SIZE];
		int m_lPredLNum[MAX_SENTENCE_SIZE];
		int m_lPredR[MAX_SENTENCE_SIZE];		//right dependency children
		int m_lSubPredR[MAX_SENTENCE_SIZE];
		int m_lPredLabelR[MAX_SENTENCE_SIZE];
		int m_lSubPredLabelR[MAX_SENTENCE_SIZE];
		int m_lPredRNum[MAX_SENTENCE_SIZE];
		TagSet m_lPredLabelSetL[MAX_SENTENCE_SIZE];
		TagSet m_lPredLabelSetR[MAX_SENTENCE_SIZE];

		std::vector<DepRightArc> m_vecRightArcs[MAX_SENTENCE_SIZE];

	public:
		StateItem();
		~StateItem();

		const tscore & getScore() const	{ return m_nScore; }
		void setScore(const tscore & s)	{ m_nScore = s; }

		const int & action(const int & index) const					{ return m_lActionList[index]; }
		const int & stack(const int & index) const					{ return m_lStack[index]; }
		const int & leftHead(const int & index) const				{ return m_lHeadL[index]; }
		const int & rightHead(const int & index) const				{ return m_lHeadR[index]; }
		const int & leftHeadLabel(const int & index) const			{ return m_lHeadLabelL[index]; }
		const int & rightHeadLabel(const int & index) const			{ return m_lHeadLabelR[index]; }
		const int & leftPred(const int & index) const				{ return m_lPredL[index]; }
		const int & rightPred(const int & index) const				{ return m_lPredR[index]; }
		const int & leftSubPred(const int & index) const			{ return m_lSubPredL[index]; }
		const int & rightSubPred(const int & index) const			{ return m_lSubPredR[index]; }
		const int & leftPredLabel(const int & index) const			{ return m_lPredLabelL[index]; }
		const int & rightPredLabel(const int & index) const			{ return m_lPredLabelR[index]; }
		const int & leftSubPredLabel(const int & index) const		{ return m_lSubPredLabelL[index]; }
		const int & rightSubPredLabel(const int & index) const		{ return m_lSubPredLabelR[index]; }
		const int & leftHeadArity(const int & index) const			{ return m_lHeadLNum[index]; }
		const int & leftPredArity(const int & index) const			{ return m_lPredLNum[index]; }
		const int & rightHeadArity(const int & index) const			{ return m_lHeadRNum[index]; }
		const int & rightPredArity(const int & index) const			{ return m_lPredRNum[index]; }
		const TagSet & leftPredLabelSet(const int & index) const	{ return m_lPredLabelSetL[index]; }
		const TagSet & rightPredLabelSet(const int & index) const	{ return m_lPredLabelSetR[index]; }

		const int & size() const				{ return m_nNextWord; }
		const int & stackBack() const			{ return m_nStackBack; }
		const int & secondStackBack() const		{ return m_nSecondStackBack; }
		const int & stackTop() const			{ return m_lStack[m_nStackBack]; }
		const int & secondStackTop() const		{ return m_lSecondStack[m_nSecondStackBack]; }
		const int & stackSubTop() const			{ return m_lStack[m_nStackBack - 1]; }
		const int & actionBack() const			{ return m_nActionBack; }
		bool stackEmpty() const					{ return m_nStackBack == -1; }

		bool canMem() const		{ return m_nStackBack > 0 && m_bCanMem; }
		bool canRecall() const	{ return m_nSecondStackBack >= 0; }
		bool canArc() const		{ return m_nStackBack == -1 ? false : (m_vecRightArcs[m_lStack[m_nStackBack]].empty() ? true : (m_vecRightArcs[m_lStack[m_nStackBack]].back().head != m_nNextWord)); }
		bool canShift() const	{ return m_nSecondStackBack == -1; }

		bool operator<(const StateItem & item) const	{ return m_nScore < item.m_nScore; }
		bool operator<=(const StateItem & item) const	{ return m_nScore <= item.m_nScore; }
		bool operator>(const StateItem & item) const	{ return m_nScore > item.m_nScore; }
		bool operator!=(const StateItem & item) const	{ return !(*this == item); }

		void arc(const int & label);
		void arcLeft(const int & label);
		void arcRight(const int & label);
		void mem();
		void recall();
		void shift();
		void reduce();
		void arcMem(const int & label);
		void arcRecall(const int & label);
		void arcShift(const int & label);
		void arcReduce(const int & label);

		void clear();
		void clearNext();
		void print() const;

		bool move(const int & action);

		bool extractOracle(const DependencyGraph & graph);
		bool followOneAction(int & nNextWord, int (&tIndex)[MAX_SENTENCE_SIZE], int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph, const int & label);

		bool operator==(const StateItem & item) const;
		bool operator==(const DependencyGraph & graph) const;

		StateItem & operator=(const StateItem & i);

		void generateGraph(const DependencyGraph & sent, DependencyGraph & tree) const;
	};
}

#endif
