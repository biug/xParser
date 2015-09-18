#ifndef _NIRVE_STATE_H
#define _NIRVE_STATE_H

#include "nirve_macros.h"
#include "common/token/deplabel.h"

namespace nirve {

	extern int AL_SW_FIRST;
	extern int AR_SW_FIRST;
	extern int AL_SH_FIRST;
	extern int AR_SH_FIRST;
	extern int AL_RE_FIRST;
	extern int AR_RE_FIRST;

	extern int AL_SW_END;
	extern int AR_SW_END;
	extern int AL_SH_END;
	extern int AR_SH_END;
	extern int AL_RE_END;
	extern int AR_RE_END;

	class StateItem {
	private:
		int m_nStackBack;
		int m_nShiftBufferBack;
		int m_nNextWord;
		int m_nActionBack;
		tscore m_nScore;
		int m_lActionList[MAX_SENTENCE_SIZE << MAX_SENTENCE_BITS];

		bool  m_bCanSwap;

		int m_lStack[MAX_SENTENCE_SIZE];
		int m_lShiftBuffer[MAX_SENTENCE_SIZE];
		int m_lHeadL[MAX_SENTENCE_SIZE];	//heads for every node
		int m_lSubHeadL[MAX_SENTENCE_SIZE];
		int m_lHeadLabelL[MAX_SENTENCE_SIZE];	//label for every node
		int m_lSubHeadLabelL[MAX_SENTENCE_SIZE];
		int m_lHeadLNum[MAX_SENTENCE_SIZE];
		int m_lHeadR[MAX_SENTENCE_SIZE];
		int m_lSubHeadR[MAX_SENTENCE_SIZE];
		int m_lHeadLabelR[MAX_SENTENCE_SIZE];
		int m_lSubHeadLabelR[MAX_SENTENCE_SIZE];
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

		std::vector<RightNodeWithLabel> m_lRightNodes[MAX_SENTENCE_SIZE];

	public:
		StateItem();
		~StateItem();

		void arcLeft(const int & l);
		void arcRight(const int & l);
		void shift();
		void reduce();
		void swap();
		void shiftReduce();
		void arcLeftSwap(const int & l);
		void arcRightSwap(const int & l);
		void arcLeftReduce(const int & l);
		void arcRightReduce(const int & l);
		void arcLeftShift(const int & l);
		void arcRightShift(const int & l);

		const tscore & getScore() const;
		void setScore(const tscore & s);

		const int & action(const int & index) const;
		const int & stack(const int & index) const;
		const int & leftHead(const int & index) const;
		const int & rightHead(const int & index) const;
		const int & leftSubHead(const int & index) const;
		const int & rightSubHead(const int & index) const;
		const int & leftHeadLabel(const int & index) const;
		const int & rightHeadLabel(const int & index) const;
		const int & leftSubHeadLabel(const int & index) const;
		const int & rightSubHeadLabel(const int & index) const;
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

		const int & size() const;
		const int & stackBack() const;
		const int & stackTop() const;
		const int & stackSubTop() const;
		const int & bufferTop() const;
		const int & actionBack() const;

		void print() const;
		void check() const;
		bool extractOneStandard(int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph, const int & direction = 0, const int & label = 0);
		bool extractOracle(const DependencyGraph & graph);

		bool canSwap() const;
		bool canArc() const;
		bool canShiftReduce() const;

		void clear();
		void clearNext();
		void move(const int & action);
		void generateGraph(const DependencyGraph & sent, DependencyGraph & tree) const;

		bool operator<(const StateItem & i) const;
		bool operator<=(const StateItem & i) const;
		bool operator>(const StateItem & i) const;

		bool operator==(const StateItem & i) const;
		bool operator!=(const StateItem & i) const;
		bool operator==(const DependencyGraph & graph) const;
		StateItem & operator=(const StateItem & i);
	};

	inline void StateItem::shift() {
		if (m_nShiftBufferBack == -1) {
			m_lStack[++m_nStackBack] = m_nNextWord++;
			m_bCanSwap = true;
			clearNext();
		}
		else {
			m_lStack[++m_nStackBack] = m_lShiftBuffer[m_nShiftBufferBack--];
			m_bCanSwap = false;
		}
		m_lActionList[++m_nActionBack] = SHIFT;
	}

	inline void StateItem::reduce() {
		--m_nStackBack;
		m_lActionList[++m_nActionBack] = REDUCE;
	}

	inline void StateItem::swap() {
		m_lShiftBuffer[++m_nShiftBufferBack] = m_lStack[m_nStackBack - 1];
		m_lStack[m_nStackBack - 1] = m_lStack[m_nStackBack];
		--m_nStackBack;
		m_lActionList[++m_nActionBack] = SWAP;
	}

	inline void StateItem::shiftReduce() {
		if (m_nShiftBufferBack == -1) {
			++m_nNextWord;
			clearNext();
		}
		else {
			--m_nShiftBufferBack;
		}
		m_lActionList[++m_nActionBack] = SHIFT_REDUCE;
	}

	inline void StateItem::arcLeftSwap(const int & label) {
		arcLeft(label);
		swap();
		m_lActionList[m_nActionBack] = AL_SW_FIRST + label - 1;
	}

	inline void StateItem::arcRightSwap(const int & label) {
		arcRight(label);
		swap();
		m_lActionList[m_nActionBack] = AR_SW_FIRST + label - 1;
	}

	inline void StateItem::arcLeftShift(const int & label) {
		arcLeft(label);
		shift();
		m_lActionList[m_nActionBack] = AL_SH_FIRST + label - 1;
	}

	inline void StateItem::arcRightShift(const int & label) {
		arcRight(label);
		shift();
		m_lActionList[m_nActionBack] = AR_SH_FIRST + label - 1;
	}

	inline void StateItem::arcLeftReduce(const int & label) {
		arcLeft(label);
		reduce();
		m_lActionList[m_nActionBack] = AL_RE_FIRST + label - 1;
	}

	inline void StateItem::arcRightReduce(const int & label) {
		arcRight(label);
		reduce();
		m_lActionList[m_nActionBack] = AR_RE_FIRST + label - 1;
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

	inline const int & StateItem::bufferTop() const {
		return m_nShiftBufferBack == -1 ? m_nNextWord : m_lShiftBuffer[m_nShiftBufferBack];
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

	inline const int & StateItem::leftSubHead(const int & index) const {
		return m_lSubHeadL[index];
	}

	inline const int & StateItem::rightSubHead(const int & index) const {
		return m_lSubHeadR[index];
	}

	inline const int & StateItem::leftHeadLabel(const int & index) const {
		return m_lHeadLabelL[index];
	}

	inline const int & StateItem::rightHeadLabel(const int & index) const {
		return m_lHeadLabelR[index];
	}

	inline const int & StateItem::leftSubHeadLabel(const int & index) const {
		return m_lSubHeadLabelL[index];
	}

	inline const int & StateItem::rightSubHeadLabel(const int & index) const {
		return m_lSubHeadLabelR[index];
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

	inline bool StateItem::canArc() const {
		return m_nStackBack == -1 ? false : (m_lRightNodes[m_lStack[m_nStackBack]].empty() ? true : (RIGHTNODE_POS(m_lRightNodes[m_lStack[m_nStackBack]].back()) != m_nNextWord));
	}

	inline bool StateItem::canSwap() const {
		return m_nStackBack > 0 && m_bCanSwap;
	}

	inline bool StateItem::canShiftReduce() const {
		for (int i = 0; i <= m_nStackBack; ++i) {
			for (const auto & rn : m_lRightNodes[m_lStack[i]]) {
				if (RIGHTNODE_POS(rn) == m_nNextWord) {
					return false;
				}
			}
		}
		return true;
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