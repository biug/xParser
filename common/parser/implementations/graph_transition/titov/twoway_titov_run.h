#ifndef _TWOWAY_TITOV_RUN_H
#define _TWOWAY_TITOV_RUN_H

#include "twoway_titov_depparser.h"
#include "common/parser/implementations/graph_transition/graph_transition_run.h"

namespace twoway_titov {

	using titov::StateItem;
	using titov::ActionConstant;

	class Run : public GraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant> {
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSuperTag = false) :
			GraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant>
		(bChar, bPath, bSuperTag) {}
		~Run() = default;
	};
}

#endif
