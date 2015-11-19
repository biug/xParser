#ifndef _STD_TITOV_RUN_H
#define _STD_TITOV_RUN_H

#include "std_titov_depparser.h"
#include "common/parser/implementations/graph_transition/graph_transition_run.h"

namespace std_titov {

	class Run : public GraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant> {
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSuperTag = false) :
			GraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant>
		(bChar, bPath, bSuperTag) {}
		~Run() = default;
	};
}

#endif
