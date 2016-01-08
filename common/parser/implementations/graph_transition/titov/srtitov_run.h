#ifndef _SRTITOV_RUN_H
#define _SRTITOV_RUN_H

#include "srtitov_depparser.h"
#include "common/parser/implementations/graph_transition/ulgraph_transition_run.h"

namespace srtitov {

	class Run : public ULGraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant> {
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSuperTag = false) :
			ULGraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant>
		(bChar, bPath, bSuperTag) {}
		~Run() = default;
	};
}

#endif
