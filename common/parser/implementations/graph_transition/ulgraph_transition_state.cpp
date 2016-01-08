#include "ulgraph_transition_state.h"

void ULGraphTransitionStateBase::arc(const int & label) {
	const int & left = m_lStack[m_nStackBack];
	// detect arc label
	// arc left
	switch (label) {
	case ULA_LEFT:
		arcLeft();
		break;
	case ULA_RIGHT:
		arcRight();
		break;
	default:
		arcLeft();
		arcRight();
		break;
	}
	//add right arcs for stack seek
	m_vecRightArcs[left].push_back(DepRightArc(m_nNextWord, label));
}

void ULGraphTransitionStateBase::arcLeft() {
	const int & left = m_lStack[m_nStackBack];
	//add new head for left and add label
	m_lHeadR[left] = m_nNextWord;
	++m_lHeadRNum[left];
	int & buffer_left_pred = m_lPredL[m_nNextWord];
	if (buffer_left_pred == -1) {
		buffer_left_pred = left;
	}
	else if (left < buffer_left_pred) {
		m_lSubPredL[m_nNextWord] = buffer_left_pred;
		buffer_left_pred = left;
	}
	else {
		int & sub_buffer_left_pred = m_lSubPredL[m_nNextWord];
		if (sub_buffer_left_pred == -1 || left < sub_buffer_left_pred) {
			sub_buffer_left_pred = left;
		}
	}
	++m_lPredLNum[m_nNextWord];
}

void ULGraphTransitionStateBase::arcRight() {
	const int & left = m_lStack[m_nStackBack];
	//sibling is the previous child of buffer seek
	int & buffer_left_head = m_lHeadL[m_nNextWord];
	if (buffer_left_head == -1 || left < buffer_left_head) {
		buffer_left_head = left;
	}
	++m_lHeadLNum[m_nNextWord];
	m_lSubPredR[left] = m_lPredR[left];
	m_lPredR[left] = m_nNextWord;
	++m_lPredRNum[left];
}
