#ifndef _TITOV_RING_STATE_H
#define _TITOV_RING_STATE_H

#include "titov_ring_macros.h"
#include "common/token/tagset.h"
#include "common/token/deplabel.h"

namespace titov_ring {

	extern int A_SW_FIRST;
	extern int A_SH_FIRST;
	extern int A_RE_FIRST;

	extern int A_SW_END;
	extern int A_SH_END;
	extern int A_RE_END;

	class StateItem {
	private:
		int m_nStackBack;
		int m_nNextWord;
		int m_nActionBack;
		tscore m_nScore;
		int m_lActionList[MAX_SENTENCE_SIZE << MAX_SENTENCE_BITS];

		int m_lStack[MAX_SENTENCE_SIZE];
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

		Tagset m_lPredLabelSetL[MAX_SENTENCE_SIZE];
		Tagset m_lPredLabelSetR[MAX_SENTENCE_SIZE];

		std::vector<RightNodeWithLabel> m_lRightNodes[MAX_SENTENCE_SIZE];

	public:
		StateItem();
		~StateItem();

		void arc(const int & l);
		void shift();
		void reduce();
		void swap();
		void arcSwap(const int & l);
		void arcReduce(const int & l);
		void arcShift(const int & l);

		const tscore & getScore() const;
		void setScore(const tscore & s);

		const int & action(const int & index) const;
		const int & stack(const int & index) const;
		const int & leftHead(const int & index) const;
		const int & rightHead(const int & index) const;
		const int & leftHeadLabel(const int & index) const;
		const int & rightHeadLabel(const int & index) const;
		const int & leftPred(const int & index) const;
		const int & rightPred(const int & index) const;
		const int & leftSubPred(const int & index) const;
		const int & rightSubPred(const int & index) const;
		const int & leftPredLabel(const int & index) const;
		const int & rightPredLabel(const int & index) const;
		const int & leftSubPredLabel(const int & index) const;
		const int & rightSubPredLabel(const int & index) const;
		const int & leftHeadArity(const int & index) const;
		const int & leftPredArity(const int & index) const;
		const int & rightHeadArity(const int & index) const;
		const int & rightPredArity(const int & index) const;
		const Tagset & leftPredLabelSet(const int & index) const;
		const Tagset & rightPredLabelSet(const int & index) const;

		const int & size() const;
		const int & stackBack() const;
		const int & stackTop() const;
		const int & stackSubTop() const;
		const int & actionBack() const;

		void print() const;
		void check() const;
		bool extractOneStandard(int (&seeks)[MAX_SENTENCE_SIZE], const DependencyCONLLGraph & graph, const int & label = 0);
		bool extractOracle(const DependencyCONLLGraph & graph);

		bool canSwap() const;
		bool canArc() const;

		void clear();
		void clearNext();
		void move(const int & action);
		void generateGraph(const DependencyCONLLGraph & sent, DependencyCONLLGraph & tree) const;

		bool operator<(const StateItem & i) const;
		bool operator<=(const StateItem & i) const;
		bool operator>(const StateItem & i) const;

		bool operator==(const StateItem & i) const;
		bool operator!=(const StateItem & i) const;
		bool operator==(const DependencyCONLLGraph & graph) const;
		StateItem & operator=(const StateItem & i);
	};

	inline void StateItem::swap() {
		std::swap(m_lStack[m_nStackBack], m_lStack[m_nStackBack - 1]);
		m_lActionList[++m_nActionBack] = SWAP;
	}

	inline void StateItem::shift() {
		m_lStack[++m_nStackBack] = m_nNextWord++;
		m_lActionList[++m_nActionBack] = SHIFT;
		clearNext();
	}

	inline void StateItem::reduce() {
		--m_nStackBack;
		m_lActionList[++m_nActionBack] = REDUCE;
	}

	inline void StateItem::arcSwap(const int & label) {
		arc(label);
		swap();
		m_lActionList[m_nActionBack] = A_SW_FIRST + label - 1;
	}

	inline void StateItem::arcShift(const int & label) {
		arc(label);
		shift();
		m_lActionList[m_nActionBack] = A_SH_FIRST + label - 1;
	}

	inline void StateItem::arcReduce(const int & label) {
		arc(label);
		reduce();
		m_lActionList[m_nActionBack] = A_RE_FIRST + label - 1;
	}

	inline const tscore & StateItem::getScore() const {
		return m_nScore;
	}

	inline void StateItem::setScore(const tscore & s) {
		m_nScore = s;
	}

	inline const int & StateItem::size() const {
		return m_nNextWord;
	}

	inline const int & StateItem::stackBack() const {
		return m_nStackBack;
	}

	inline const int & StateItem::stackTop() const {
		return m_lStack[m_nStackBack];
	}

	inline const int & StateItem::stackSubTop() const {
		return m_lStack[m_nStackBack - 1];
	}

	inline const int & StateItem::actionBack() const {
		return m_nActionBack;
	}

	inline const int & StateItem::action(const int & index) const {
		return m_lActionList[index];
	}

	inline const int & StateItem::stack(const int & index) const {
		return m_lStack[index];
	}

	inline const int & StateItem::leftHead(const int & index) const {
		return m_lHeadL[index];
	}

	inline const int & StateItem::rightHead(const int & index) const {
		return m_lHeadR[index];
	}

	inline const int & StateItem::leftHeadLabel(const int & index) const {
		return m_lHeadLabelL[index];
	}

	inline const int & StateItem::rightHeadLabel(const int & index) const {
		return m_lHeadLabelR[index];
	}

	inline const int & StateItem::leftPred(const int & index) const {
		return m_lPredL[index];
	}

	inline const int & StateItem::rightPred(const int & index) const {
		return m_lPredR[index];
	}

	inline const int & StateItem::leftSubPred(const int & index) const {
		return m_lSubPredL[index];
	}

	inline const int & StateItem::rightSubPred(const int & index) const {
		return m_lSubPredR[index];
	}

	inline const int & StateItem::leftPredLabel(const int & index) const {
		return m_lPredLabelL[index];
	}

	inline const int & StateItem::rightPredLabel(const int & index) const {
		return m_lPredLabelR[index];
	}

	inline const int & StateItem::leftSubPredLabel(const int & index) const {
		return m_lSubPredLabelL[index];
	}

	inline const int & StateItem::rightSubPredLabel(const int & index) const {
		return m_lSubPredLabelR[index];
	}

	inline const int & StateItem::leftHeadArity(const int & index) const {
		return m_lHeadLNum[index];
	}

	inline const int & StateItem::leftPredArity(const int & index) const {
		return m_lPredLNum[index];
	}

	inline const int & StateItem::rightHeadArity(const int & index) const {
		return m_lHeadRNum[index];
	}

	inline const int & StateItem::rightPredArity(const int & index) const {
		return m_lPredRNum[index];
	}

	inline const Tagset & StateItem::leftPredLabelSet(const int & index) const {
		return m_lPredLabelSetL[index];
	}

	inline const Tagset & StateItem::rightPredLabelSet(const int & index) const {
		return m_lPredLabelSetR[index];
	}

	inline bool StateItem::canArc() const {
		return m_nStackBack == -1 ? false : (m_lRightNodes[m_lStack[m_nStackBack]].empty() ? true : (RIGHTNODE_POS(m_lRightNodes[m_lStack[m_nStackBack]].back()) != m_nNextWord));
	}

	inline bool StateItem::canSwap() const {
		const int & action = m_lActionList[m_nActionBack];
		return action != SWAP && (action < A_SW_FIRST || action >= A_SW_END) && m_nStackBack > 0;
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
