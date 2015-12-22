#include <stack>
#include <sstream>
#include <unordered_set>

#include "arcsr_macros.h"

namespace arcsr {
	ActionConstant::ActionConstant(const int & superTagCount) :
			LabelCount(0), LeftLabelCount(0), RightLabelCount(0), m_nSuperTagCount(superTagCount),
			AL_FIRST(POP_ROOT + 1), AL_END(AL_FIRST + (m_nSuperTagCount == 0 ? LeftLabelCount : (m_nSuperTagCount * LeftLabelCount))),
			AR_FIRST(AL_END), AR_END(AR_FIRST + LabelCount),
			RE_FIRST(AR_END), RE_END(RE_FIRST + (m_nSuperTagCount == 0 ? 1 : m_nSuperTagCount)),
			PP_FIRST(RE_END), PP_END(PP_FIRST + (m_nSuperTagCount == 0 ? 1 : m_nSuperTagCount)) {}

	ActionConstant::ActionConstant(const ActionConstant & actions) : ActionConstant(actions.m_nSuperTagCount) {}

	ActionConstant::~ActionConstant() = default;

	void ActionConstant::loadConstant(const DLabel & labels) {
		loadLabels(*this, labels);
		m_vecLeftLabelMap.clear();
		m_vecRightLabelMap.clear();
		m_vecLeftLabelMap.push_back(0);
		m_vecRightLabelMap.push_back(0);
		LeftLabelCount = RightLabelCount = 0;
		for (int i = labels.start(); i < labels.end(); ++i) {
			if (IS_LEFT_LABEL(labels.key(i))) {
				++LeftLabelCount;
				m_vecLeftLabelMap.push_back(0);
			}
			else if (IS_RIGHT_LABEL(labels.key(i))) {
				++RightLabelCount;
				m_vecRightLabelMap.push_back(0);
			}
		}
		for (int i = labels.start(); i < labels.end(); ++i) {
			if (IS_LEFT_LABEL(labels.key(i))) {
				m_vecLeftLabelMap[LEFT_LABEL_ID(m_vecLabelMap[i])] = i;
			}
			else if (IS_RIGHT_LABEL(labels.key(i))) {
				m_vecRightLabelMap[RIGHT_LABEL_ID(m_vecLabelMap[i])] = i;
			}
		}

		AL_FIRST = POP_ROOT + 1;	AL_END = AL_FIRST + (m_nSuperTagCount == 0 ? LeftLabelCount : (m_nSuperTagCount * LeftLabelCount));
		AR_FIRST = AL_END;			AR_END = AR_FIRST + RightLabelCount;
		RE_FIRST = AR_END;			RE_END = RE_FIRST + (m_nSuperTagCount == 0 ? 1 : m_nSuperTagCount);
		PP_FIRST = RE_END;			PP_END = PP_FIRST + (m_nSuperTagCount == 0 ? 1 : m_nSuperTagCount);

		print();
	}

	bool ActionConstant::extractOracle(StateItem * item, const DependencyGraph & graph) const {
		int rightNodeSeeks[MAX_SENTENCE_SIZE];
		std::vector<int> vecHeads(graph.size(), -1);
		for (int i = 0; i < graph.size(); ++i) {
			for (const auto & arc : graph[i].m_vecRightArcs) {
				if (IS_LEFT_LABEL(arc.second)) {
					vecHeads[i] = arc.first;
				}
				else if (IS_RIGHT_LABEL(arc.second)) {
					vecHeads[arc.first] = i;
				}
			}
		}
		memset(rightNodeSeeks, 0, sizeof(rightNodeSeeks));
		while (followOneAction(item, rightNodeSeeks, graph, vecHeads)) {
//			print(item->action(item->actionBack()));
		}
			;
		return *item == graph;
	}

	bool ActionConstant::followOneAction(StateItem * item, int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph,
											const std::vector<int> & heads) const {
		// reduce or draw arc
		if (item->size() == graph.size()) {
			int tag = graph[item->stackTop()].m_nSuperTagCode;
			if (item->stackBack() > 0) {
				item->reduce(tag, RE_FIRST + (tag == 0 ? 0 : (tag - 1)));
				return true;
			}
			else {
				item->popRoot(tag, PP_FIRST + (tag == 0 ? 0 : (tag - 1)));
				return false;
			}
		}
		if (item->stackBack() >= 0) {
			int top = item->stackTop();
			while (item->head(top) != -1) {
				top = item->head(top);
			}
			if (heads[top] == item->size()) {
				int tag = graph[item->stackTop()].m_nSuperTagCode;
				if (top == item->stackTop()) {
					auto labels = graph[top].m_vecRightLabels[seeks[top]++];
					item->arcLeft(labels.first, labels.second.first, tag, AL_FIRST + (tag == 0 ? 0 : (tag - 1)) * LeftLabelCount + labels.second.first - 1);
				}
				else {
					item->reduce(tag, RE_FIRST + (tag == 0 ? 0 : (tag - 1)));
				}
				return true;
			}
		}
		if (heads[item->size()] == -1 || heads[item->size()] > item->size()) {
			item->shift(SHIFT);
			return true;
		}
		else {
			int top = item->stackTop();
			if (heads[item->size()] == top) {
				auto labels = graph[top].m_vecRightLabels[seeks[top]++];
				item->arcRight(labels.first, labels.second.second, AR_FIRST + labels.second.second - 1);
			}
			else {
				int tag = graph[top].m_nSuperTagCode;
				item->reduce(tag, RE_FIRST + (tag == 0 ? 0 : (tag - 1)));
			}
			return true;
		}
	}

	void ActionConstant::doAction(StateItem * item, const int & action) const {
		if (action <= POP_ROOT) {
			switch (action) {
			case SHIFT:
				item->shift(action);
				break;
			default:
				break;
			}
		}
		else if (action < AL_END) {
			if (m_nSuperTagCount > 0) {
				int label = (action - AL_FIRST) % LeftLabelCount + 1;
				item->arcLeft(m_vecLeftLabelMap[label], label, (action - AL_FIRST) / LeftLabelCount + 1, action);
			}
			else {
				int label = action - AL_FIRST + 1;
				item->arcLeft(m_vecLeftLabelMap[label], label, 0, action);
			}
		}
		else if (action < AR_END) {
			int label = action - AR_FIRST + 1;
			item->arcRight(m_vecRightLabelMap[label], label, action);
		}
		else if (action < RE_END) {
			item->reduce(action - RE_FIRST + 1, action);
		}
		else {
			item->popRoot(action - PP_FIRST + 1, action);
		}
	}

	void ActionConstant::print(const int & action) const {
		if (action <= POP_ROOT) {
			switch (action) {
			case NO_ACTION:
				std::cout << "no action";
				break;
			case SHIFT:
				std::cout << "shift";
				break;
			default:
				std::cout << "wrong action";
				break;
			}
		}
		else if (action < AL_END) {
			std::cout << "arc left with label " << (action - AL_FIRST) % LeftLabelCount + 1 << " with tag " << (action - AL_FIRST) / LeftLabelCount + 1;
		}
		else if (action < AR_END) {
			std::cout << "arc right with label "<< action - AR_FIRST + 1;
		}
		else if (action < RE_END) {
			std::cout << "reduce with tag " << action - RE_FIRST + 1;
		}
		else if (action < PP_END) {
			std::cout << "pop root with tag " << action - PP_FIRST + 1;
		}
		else {
			std::cout << "wrong action";
		}
		std::cout << " (" << action << ")" << std::endl;
	}

	void ActionConstant::print() const {
		std::cout << "label count is " << LabelCount << std::endl;
		std::cout << "super tag count is " << m_nSuperTagCount << std::endl;
		std::cout << "left label count is " << LeftLabelCount << std::endl;
		std::cout << "right label count is " << RightLabelCount << std::endl;
		std::cout << "arc left first is " << AL_FIRST << std::endl;
		std::cout << "arc left end is " << AL_END << std::endl;
		std::cout << "arc right first is " << AR_FIRST << std::endl;
		std::cout << "arc right end is " << AR_END << std::endl;
		std::cout << "reduce first is " << RE_FIRST << std::endl;
		std::cout << "reduce end is " << RE_END << std::endl;
	}
}
