#include "graph_transition_both_state_base.h"

extern std::vector<int> g_vecGraphLabelMap;

namespace graph_transition_both {

	void GraphStateBase::arc(const int & l) {
		const int & left = m_lStack[m_nStackBack];
		// detect arc label
		const int & labelId = g_vecGraphLabelMap[l];
		int leftLabel = LEFT_LABEL_ID(labelId);
		int rightLabel = RIGHT_LABEL_ID(labelId);
		// arc left
		if (leftLabel != 0) {
			//add new head for left and add label
			m_lHeadR[left] = m_nNextWord;
			m_lHeadLabelR[left] = leftLabel;
			++m_lHeadRNum[left];
			int & buffer_left_pred = m_lPredL[m_nNextWord];
			if (buffer_left_pred == -1) {
				buffer_left_pred = left;
				m_lPredLabelL[m_nNextWord] = leftLabel;
			}
			else if (left < buffer_left_pred) {
				m_lSubPredL[m_nNextWord] = buffer_left_pred;
				m_lSubPredLabelL[m_nNextWord] = m_lPredLabelL[m_nNextWord];
				buffer_left_pred = left;
				m_lPredLabelL[m_nNextWord] = leftLabel;
			}
			else {
				int & sub_buffer_left_pred = m_lSubPredL[m_nNextWord];
				if (sub_buffer_left_pred == -1 || left < sub_buffer_left_pred) {
					sub_buffer_left_pred = left;
					m_lSubPredLabelL[m_nNextWord] = leftLabel;
				}
			}
			++m_lPredLNum[m_nNextWord];
			m_lPredLabelSetL[m_nNextWord].add(leftLabel);
		}
		// arc right
		if (rightLabel != 0) {
			//sibling is the previous child of buffer seek
			int & buffer_left_head = m_lHeadL[m_nNextWord];
			if (buffer_left_head == -1 || left < buffer_left_head) {
				buffer_left_head = left;
				m_lHeadLabelL[m_nNextWord] = rightLabel;
			}
			++m_lHeadLNum[m_nNextWord];
			m_lSubPredR[left] = m_lPredR[left];
			m_lPredR[left] = m_nNextWord;
			m_lSubPredLabelR[left] = m_lPredLabelR[left];
			m_lPredLabelR[left] = rightLabel;
			++m_lPredRNum[left];
			m_lPredLabelSetR[left].add(rightLabel);
		}
		//add right arcs for stack seek
		m_lRightNodes[left].push_back(RightNodeWithLabel(m_nNextWord, l));
	}
}
