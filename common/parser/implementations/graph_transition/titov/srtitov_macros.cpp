#include <stack>
#include <sstream>
#include <unordered_set>

#include "srtitov_macros.h"

namespace srtitov {
	ActionConstant::ActionConstant() :
			LabelCount(0), m_nSuperTagCount(0) {}

	ActionConstant::ActionConstant(const ActionConstant & actions) : ActionConstant() {}

	ActionConstant::~ActionConstant() = default;

	void ActionConstant::loadConstant(const DLabel & labels) {
//		loadLabels(*this, labels);
	}

	bool ActionConstant::extractOracle(StateItem * item, const DependencyGraph & graph) const {
		int rightNodeSeeks[MAX_SENTENCE_SIZE];
		memset(rightNodeSeeks, 0, sizeof(rightNodeSeeks));
		while (followOneAction(item, rightNodeSeeks, graph))
			;
		// change oracle
		int newLen = 0;
//		std::string action1 = "";
		for (int i = 1; i <= item->actionBack();) {
			int j = i + 1;
			while (j <= item->actionBack() && item->action(j) != SHIFT && item->action(j) != REDUCE) ++j;
			if (j - i == 1) {
				item->referAction(++newLen, item->action(i));
//				action1 += (item->action(i) == SHIFT ? "shift " : "reduce ");
			}
			else if (j - i == 2) {
				if (item->action(i) == SHIFT) {
					item->referAction(++newLen, item->action(i + 1) == SWAP ?
							SHIFT_SWAP :
							(SHIFT_ARC + item->action(i + 1) - ARC));
//					action1 += "shift ";
//					action1 += (item->action(i + 1) == SWAP ? "swap " : "arc ");
				}
				else {
					item->referAction(++newLen, item->action(i + 1) == SWAP ?
							REDUCE_SWAP :
							(REDUCE_ARC + item->action(i + 1) - ARC));
//					action1 += "reduce ";
//					action1 += (item->action(i + 1) == SWAP ? "swap " : "arc ");
				}
			}
			else if (j - i == 3) {
				if (item->action(i) == SHIFT) {
					item->referAction(++newLen, item->action(i + 1) == SWAP ?
							(SHIFT_SWAP_ARC + item->action(i + 2) - ARC) :
							(SHIFT_ARC_SWAP + item->action(i + 1) - ARC));
//					action1 += "shift ";
//					action1 += (item->action(i + 1) == SWAP ? "swap arc " : "arc swap ");
				}
				else {
					item->referAction(++newLen, item->action(i + 1) == SWAP ?
							(REDUCE_SWAP_ARC + item->action(i + 2) - ARC) :
							(REDUCE_ARC_SWAP + item->action(i + 1) - ARC));
//					action1 += "reduce ";
//					action1 += (item->action(i + 1) == SWAP ? "swap arc " : "arc swap ");
				}
			}
			else if (j - i == 4) {
				if (item->action(i) == SHIFT) {
					item->referAction(++newLen, item->action(i + 1) == SWAP ?
							(SHIFT_SWAP_ARC_SWAP + item->action(i + 2) - ARC) :
							(SHIFT_ARC_SWAP_ARC + (item->action(i + 1) - ARC) * ULA_MAX + (item->action(i + 3) - ARC)));
//					action1 += "shift ";
//					action1 += (item->action(i + 1) == SWAP ? "swap arc swap " : "arc swap arc ");
				}
				else {
					item->referAction(++newLen, item->action(i + 1) == SWAP ?
							(REDUCE_SWAP_ARC_SWAP + item->action(i + 2) - ARC) :
							(REDUCE_ARC_SWAP_ARC + (item->action(i + 1) - ARC) * ULA_MAX + (item->action(i + 3) - ARC)));
//					action1 += "reduce ";
//					action1 += (item->action(i + 1) == SWAP ? "swap arc swap " : "arc swap arc ");
				}
			}
			else {
				if (item->action(i) == SHIFT) {
					item->referAction(++newLen,
							SHIFT_ARC_SWAP_ARC_SWAP + (item->action(i + 1) - ARC) * ULA_MAX + (item->action(i + 3) - ARC));
//					action1 += "shift arc swap arc swap ";
				}
				else {
					item->referAction(++newLen,
							REDUCE_ARC_SWAP_ARC_SWAP + (item->action(i + 1) - ARC) * ULA_MAX + (item->action(i + 3) - ARC));
//					action1 += "reduce arc swap arc swap ";
				}
			}
			i = j;
		}
		item->referActionBack(newLen);
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
	bool ActionConstant::followOneAction(StateItem * item, int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph) const {
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
				item->arc(labels.first, ARC + labels.first);
				return true;
			}
			if (item->stackBack() > 0) {
				int & seek2 = seeks[item->stackSubTop()];
				const DependencyGraphNode & node2 = graph[item->stackSubTop()];
				int size2 = node2.m_vecRightArcs.size();
				if (seek < size && seek2 < size2) {
					if (node.m_vecRightArcs[seek].first > node2.m_vecRightArcs[seek2].first) {
						item->swap(SWAP);
						return true;
					}
				}
				else if (seek < size) {
					item->swap(SWAP);
					return true;
				}
			}
		}
		// shfit after swap
		if (item->size() < graph.size()) {
			item->shift(SHIFT);
			return true;
		}
		return false;
	}

	void ActionConstant::doAction(StateItem * item, const int & action) const {
		switch (action) {
		case REDUCE:
			item->reduce(action);
			break;
		case SHIFT:
			item->shift(action);
			break;
		case REDUCE_SWAP:
			item->reduceSwap(action);
			break;
		case SHIFT_SWAP:
			item->shiftSwap(action);
			break;
		case REDUCE_ARC:
		case REDUCE_ARC + 1:
		case REDUCE_ARC + 2:
			item->reduceArc(action - REDUCE_ARC, action);
			break;
		case SHIFT_ARC:
		case SHIFT_ARC + 1:
		case SHIFT_ARC + 2:
			item->shiftArc(action - SHIFT_ARC, action);
			break;
		case REDUCE_SWAP_ARC:
		case REDUCE_SWAP_ARC + 1:
		case REDUCE_SWAP_ARC + 2:
			item->reduceSwapArc(action - REDUCE_SWAP_ARC, action);
			break;
		case SHIFT_SWAP_ARC:
		case SHIFT_SWAP_ARC + 1:
		case SHIFT_SWAP_ARC + 2:
			item->shiftSwapArc(action - SHIFT_SWAP_ARC, action);
			break;
		case REDUCE_ARC_SWAP:
		case REDUCE_ARC_SWAP + 1:
		case REDUCE_ARC_SWAP + 2:
			item->reduceArcSwap(action - REDUCE_ARC_SWAP, action);
			break;
		case SHIFT_ARC_SWAP:
		case SHIFT_ARC_SWAP + 1:
		case SHIFT_ARC_SWAP + 2:
			item->shiftArcSwap(action - SHIFT_ARC_SWAP, action);
			break;
		case REDUCE_SWAP_ARC_SWAP:
		case REDUCE_SWAP_ARC_SWAP + 1:
		case REDUCE_SWAP_ARC_SWAP + 2:
			item->reduceSwapArcSwap(action - REDUCE_SWAP_ARC_SWAP, action);
			break;
		case SHIFT_SWAP_ARC_SWAP:
		case SHIFT_SWAP_ARC_SWAP + 1:
		case SHIFT_SWAP_ARC_SWAP + 2:
			item->shiftSwapArcSwap(action - SHIFT_SWAP_ARC_SWAP, action);
			break;
		default:
			if (action >= REDUCE_ARC_SWAP_ARC_SWAP) {
				item->reduceArcSwapArcSwap((action - REDUCE_ARC_SWAP_ARC_SWAP) / ULA_MAX, (action - REDUCE_ARC_SWAP_ARC_SWAP) % ULA_MAX, action);
			}
			else if (action >= REDUCE_ARC_SWAP_ARC) {
				item->reduceArcSwapArc((action - REDUCE_ARC_SWAP_ARC) / ULA_MAX, (action - REDUCE_ARC_SWAP_ARC) % ULA_MAX, action);
			}
			else if (action >= SHIFT_ARC_SWAP_ARC_SWAP) {
				item->shiftArcSwapArcSwap((action - SHIFT_ARC_SWAP_ARC_SWAP) / ULA_MAX, (action - SHIFT_ARC_SWAP_ARC_SWAP) % ULA_MAX, action);
			}
			else {
				item->shiftArcSwapArc((action - SHIFT_ARC_SWAP_ARC) / ULA_MAX, (action - SHIFT_ARC_SWAP_ARC) % ULA_MAX, action);
			}
			break;
		}
	}

	std::string ActionConstant::toStr(const int & action) const {
		switch (action) {
		case REDUCE:
			return "reduce";
			break;
		case SHIFT:
			return "shift";
			break;
		case REDUCE_SWAP:
			return "reduce + swap";
			break;
		case SHIFT_SWAP:
			return "shift + swap";
			break;
		case REDUCE_ARC:
		case REDUCE_ARC + 1:
		case REDUCE_ARC + 2:
			return "reduce + arc";
			break;
		case SHIFT_ARC:
		case SHIFT_ARC + 1:
		case SHIFT_ARC + 2:
			return "shift + arc";
			break;
		case REDUCE_SWAP_ARC:
		case REDUCE_SWAP_ARC + 1:
		case REDUCE_SWAP_ARC + 2:
			return "reduce + swap + arc";
			break;
		case SHIFT_SWAP_ARC:
		case SHIFT_SWAP_ARC + 1:
		case SHIFT_SWAP_ARC + 2:
			return "shift + swap + arc";
			break;
		case REDUCE_ARC_SWAP:
		case REDUCE_ARC_SWAP + 1:
		case REDUCE_ARC_SWAP + 2:
			return "reduce + arc + swap";
			break;
		case SHIFT_ARC_SWAP:
		case SHIFT_ARC_SWAP + 1:
		case SHIFT_ARC_SWAP + 2:
			return "shift + arc + swap";
			break;
		case REDUCE_SWAP_ARC_SWAP:
		case REDUCE_SWAP_ARC_SWAP + 1:
		case REDUCE_SWAP_ARC_SWAP + 2:
			return "reduce + swap + arc + swap";
			break;
		case SHIFT_SWAP_ARC_SWAP:
		case SHIFT_SWAP_ARC_SWAP + 1:
		case SHIFT_SWAP_ARC_SWAP + 2:
			return "shift + swap + arc + swap";
			break;
		case SWAP:
			return "swap";
			break;
		case ARC:
		case ARC + 1:
		case ARC + 2:
			return "arc";
			break;
		default:
			if (action >= REDUCE_ARC_SWAP_ARC_SWAP) {
				return "reduce + arc + swap + arc + swap";
			}
			else if (action >= REDUCE_ARC_SWAP_ARC) {
				return "reduce + arc + swap + arc";
			}
			else if (action >= SHIFT_ARC_SWAP_ARC_SWAP) {
				return "shift + arc + swap + arc + swap";
			}
			else {
				return "shift + arc + swap + arc ";
			}
		}
	}
}
