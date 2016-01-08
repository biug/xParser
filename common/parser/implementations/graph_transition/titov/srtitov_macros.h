#ifndef _SRTITOV_MACROS_H
#define _SRTITOV_MACROS_H

#include "srtitov_state.h"
#include "common/parser/agenda.h"

namespace srtitov {

	enum Action {
		NO_ACTION = ACTION_START,
		SWAP,
		ARC,
		SHIFT					= ARC + ULA_MAX,
		REDUCE,

		SHIFT_SWAP,
		SHIFT_ARC,
		SHIFT_SWAP_ARC			= SHIFT_ARC + ULA_MAX,
		SHIFT_ARC_SWAP			= SHIFT_SWAP_ARC + ULA_MAX,
		SHIFT_SWAP_ARC_SWAP		= SHIFT_ARC_SWAP + ULA_MAX,
		SHIFT_ARC_SWAP_ARC		= SHIFT_SWAP_ARC_SWAP + ULA_MAX,
		SHIFT_ARC_SWAP_ARC_SWAP	= SHIFT_ARC_SWAP_ARC + ULA_MAX * ULA_MAX,

		REDUCE_SWAP				= SHIFT_ARC_SWAP_ARC_SWAP + ULA_MAX * ULA_MAX,
		REDUCE_ARC,
		REDUCE_SWAP_ARC			= REDUCE_ARC + ULA_MAX,
		REDUCE_ARC_SWAP			= REDUCE_SWAP_ARC + ULA_MAX,
		REDUCE_SWAP_ARC_SWAP	= REDUCE_ARC_SWAP + ULA_MAX,
		REDUCE_ARC_SWAP_ARC		= REDUCE_SWAP_ARC_SWAP + ULA_MAX,
		REDUCE_ARC_SWAP_ARC_SWAP= REDUCE_ARC_SWAP_ARC + ULA_MAX * ULA_MAX,

		MAX_ACTION				= REDUCE_ARC_SWAP_ARC_SWAP + ULA_MAX * ULA_MAX,
	};

	struct ActionConstant {

		int LabelCount;
		int m_nSuperTagCount;
		std::vector<int> m_vecLabelMap;

		ActionConstant();
		ActionConstant(const ActionConstant & actions);
		~ActionConstant();

		void loadConstant(const DLabel & labels);
		void doAction(StateItem * item, const int & action) const;
		bool extractOracle(StateItem * item, const DependencyGraph & graph) const;
		bool followOneAction(StateItem * item, int (&seeks)[MAX_SENTENCE_SIZE], const DependencyGraph & graph) const;
		std::string toStr(const int & action) const;
	};

}

#endif
