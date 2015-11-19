#include <stack>
#include <sstream>
#include <unordered_set>

#include "std_nivre_macros.h"

namespace std_nivre {
	ActionConstant::ActionConstant(const int & superTagCount) :
			LabelCount(0), m_nSuperTagCount(superTagCount),
			A_FIRST(SHIFT + 1), A_END(A_FIRST + LabelCount),
			SH_FIRST(A_END), SH_END(SH_FIRST + superTagCount + 1) {}

	ActionConstant::ActionConstant(const ActionConstant & actions) : ActionConstant(actions.m_nSuperTagCount) {}

	ActionConstant::~ActionConstant() = default;

	void ActionConstant::loadConstant(const DLabel & labels) {
		loadLabels(*this, labels);

		A_FIRST = SHIFT + 1;	A_END = A_FIRST + LabelCount;
		SH_FIRST = A_END;		SH_END = SH_FIRST + m_nSuperTagCount + 1;
	}

	bool ActionConstant::extractOracle(StateItem * item, const DependencyGraph & graph) const {
		int rightNodeSeeks[MAX_SENTENCE_SIZE];
		memset(rightNodeSeeks, 0, sizeof(rightNodeSeeks));
		while (followOneAction(item, rightNodeSeeks, graph))
			;
		return *item == graph;
	}

	/*
	 	action priority is : "reduce" > "arc *" > "swap" > "shift"
		for example
		| 0 | 1 |					| 2 | 3 | . . .
		if we have arc 0 - 2, 1 - 3
		we use swap, get state
		| 1 | 0 |					| 2 | 3 | . . .
		we use arc-reduce, get state
		| 1 |						| 2 | 3 | . . .		"0 - 2"
		we use shift + reduce, get state
		| 1 |							| 3 | . . .
		finally we use arc-reduce, get state
		|								| 3 | . . .		"1 - 3"
	*/
	bool ActionConstant::followOneAction(StateItem * item, int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph,
								const std::pair<int, std::pair<int, int>> & labels) const {
		// reduce or draw arc
		if (!item->stackEmpty()) {
			int & seek = seeks[item->stackTop()];
			const DependencyGraphNode & node = graph[item->stackTop()];
			int size = node.m_vecRightArcs.size();
			while (seek < size && node.m_vecRightArcs[seek].first < item->size()) {
				++seek;
			}
			if (seek >= size) {
				item->reduce(REDUCE);
				return true;
			}
			const auto & rightArc = node.m_vecRightArcs[seek];
			if (rightArc.first == item->size()) {
				const auto & labels = node.m_vecRightLabels[seek++];
				item->arc(labels.first, labels.second.first, labels.second.second, A_FIRST + labels.first - 1);
				return true;
			}
		}
		// swap after reduce/arc
		for (int i = item->stackBack() - 1; i >= 0; --i) {
			const DependencyGraphNode & node = graph[item->stack(i)];
			const int & seek = seeks[item->stack(i)];
			if (seek < node.m_vecRightArcs.size() && node.m_vecRightArcs[seek].first == item->size()) {
				item->swap(SWAP);
				return true;
			}
		}
		// shfit after swap
		if (!item->shiftBufferEmpty() || item->size() < graph.size()) {
			const int & tag = !item->shiftBufferEmpty() ? 0 : graph[item->size()].m_nSuperTagCode;
			item->shift(tag, SH_FIRST + tag);
			return true;
		}
		return false;
	}

	void ActionConstant::doAction(StateItem * item, const int & action) const {
		if (action <= SHIFT) {
			switch (action) {
			case SWAP:
				item->swap(action);
				return;
			case REDUCE:
				item->reduce(action);
				return;
			default:
				return;
			}
		}
		else if (action < A_END) {
			int label = action - A_FIRST + 1;
			int labelId = m_vecLabelMap[label];
			item->arc(label, LEFT_LABEL_ID(labelId), RIGHT_LABEL_ID(labelId), action);
		}
		else {
			item->shift(action - SH_FIRST, action);
		}
	}

	void ActionConstant::print(const int & action) const {
		if (action <= SHIFT) {
			switch (action) {
			case NO_ACTION:
				std::cout << "no action";
				break;
			case SWAP:
				std::cout << "swap";
				break;
			case REDUCE:
				std::cout << "reduce";
				break;
			default:
				std::cout << "wrong action";
				break;
			}
		}
		else if (action < A_END) {
			std::cout << "arc with label " << action - A_FIRST + 1;
		}
		else if (action < SH_END) {
			std::cout << "shift with tag " << action - SH_FIRST;
		}
		else {
			std::cout << "wrong action";
		}
		std::cout << " (" << action << ")" << std::endl;
	}

	void ActionConstant::print() const {
		std::cout << "label count is " << LabelCount << std::endl;
		std::cout << "arc first is " << A_FIRST << std::endl;
		std::cout << "arc end is " << A_END << std::endl;
		std::cout << "shift first is " << SH_FIRST << std::endl;
		std::cout << "shift end is " << SH_END << std::endl;
	}
}
