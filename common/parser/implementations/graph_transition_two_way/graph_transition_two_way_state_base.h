#ifndef _GRAPH_TRANSITION_TWO_WAY_STATE_BASE_H
#define _GRAPH_TRANSITION_TWO_WAY_STATE_BASE_H

#include "common/parser/implementations/graph_macros.h"
#include "common/token/supertag.h"

namespace graph_transition_two_way {
	class GraphStateBase {
	protected:
		tscore m_nScore;

		int m_nNextWord;
		int m_nStackBack;

		int m_nActionBack;
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
		int m_lSuperTag[MAX_SENTENCE_SIZE];
		TagSet m_lPredLabelSetL[MAX_SENTENCE_SIZE];
		TagSet m_lPredLabelSetR[MAX_SENTENCE_SIZE];

		std::vector<RightNodeWithLabel> m_lRightNodes[MAX_SENTENCE_SIZE];

	public:
		GraphStateBase() = default;
		virtual ~GraphStateBase() {};

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
		const int & superTag(const int & index) const;
		const TagSet & leftPredLabelSet(const int & index) const;
		const TagSet & rightPredLabelSet(const int & index) const;

		const int & size() const;
		const int & stackBack() const;
		const int & stackTop() const;
		const int & stackSubTop() const;
		const int & actionBack() const;

		void arc(const int & l);
		void generateGraph(const DependencyGraph & sent, DependencyGraph & tree) const;

		virtual void clear() = 0;
		virtual void clearNext() = 0;
		virtual void print() const = 0;
		virtual void check() const = 0;
		virtual void move(const int & action) = 0;

		virtual void reverse(int size) = 0;
		virtual bool extractOracle(const DependencyGraph & graph) = 0;
		virtual bool extractOneStandard(int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph, const int & label = 0) = 0;
	};

	inline const tscore & GraphStateBase::getScore() const {
		return m_nScore;
	}

	inline void GraphStateBase::setScore(const tscore & s) {
		m_nScore = s;
	}

	inline const int & GraphStateBase::action(const int & index) const {
		return m_lActionList[index];
	}

	inline const int & GraphStateBase::stack(const int & index) const {
		return m_lStack[index];
	}

	inline const int & GraphStateBase::leftHead(const int & index) const {
		return m_lHeadL[index];
	}

	inline const int & GraphStateBase::rightHead(const int & index) const {
		return m_lHeadR[index];
	}

	inline const int & GraphStateBase::leftHeadLabel(const int & index) const {
		return m_lHeadLabelL[index];
	}

	inline const int & GraphStateBase::rightHeadLabel(const int & index) const {
		return m_lHeadLabelR[index];
	}

	inline const int & GraphStateBase::leftPred(const int & index) const {
		return m_lPredL[index];
	}

	inline const int & GraphStateBase::rightPred(const int & index) const {
		return m_lPredR[index];
	}

	inline const int & GraphStateBase::leftSubPred(const int & index) const {
		return m_lSubPredL[index];
	}

	inline const int & GraphStateBase::rightSubPred(const int & index) const {
		return m_lSubPredR[index];
	}

	inline const int & GraphStateBase::leftPredLabel(const int & index) const {
		return m_lPredLabelL[index];
	}

	inline const int & GraphStateBase::rightPredLabel(const int & index) const {
		return m_lPredLabelR[index];
	}

	inline const int & GraphStateBase::leftSubPredLabel(const int & index) const {
		return m_lSubPredLabelL[index];
	}

	inline const int & GraphStateBase::rightSubPredLabel(const int & index) const {
		return m_lSubPredLabelR[index];
	}

	inline const int & GraphStateBase::leftHeadArity(const int & index) const {
		return m_lHeadLNum[index];
	}

	inline const int & GraphStateBase::leftPredArity(const int & index) const {
		return m_lPredLNum[index];
	}

	inline const int & GraphStateBase::rightHeadArity(const int & index) const {
		return m_lHeadRNum[index];
	}

	inline const int & GraphStateBase::rightPredArity(const int & index) const {
		return m_lPredRNum[index];
	}

	inline const int & GraphStateBase::superTag(const int & index) const {
		return m_lSuperTag[index];
	}

	inline const TagSet & GraphStateBase::leftPredLabelSet(const int & index) const {
		return m_lPredLabelSetL[index];
	}

	inline const TagSet & GraphStateBase::rightPredLabelSet(const int & index) const {
		return m_lPredLabelSetR[index];
	}

	inline const int & GraphStateBase::size() const {
		return m_nNextWord;
	}

	inline const int & GraphStateBase::stackBack() const {
		return m_nStackBack;
	}

	inline const int & GraphStateBase::stackSubTop() const {
		return m_lStack[m_nStackBack - 1];
	}

	inline const int & GraphStateBase::stackTop() const {
		return m_lStack[m_nStackBack];
	}

	inline const int & GraphStateBase::actionBack() const {
		return m_nActionBack;
	}

	inline void GraphStateBase::generateGraph(const DependencyGraph & sent, DependencyGraph & tree) const {
		int i = 0;
		tree.clear();
		for (const auto & token : sent) {
			tree.push_back(DependencyGraphNode(GRAPHNODE_POSTAGGEDWORD(token), TSuperTag::key(m_lSuperTag[i]), HeadWithLabel(-1, NULL_LABEL), m_lRightNodes[i]));
			++i;
		}
	}
}

#endif
