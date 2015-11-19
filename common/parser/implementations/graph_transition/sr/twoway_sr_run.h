#ifndef _TWOWAY_SR_RUN_H
#define _TWOWAY_SR_RUN_H

#include "twoway_sr_depparser.h"
#include "common/parser/implementations/graph_transition/graph_transition_run.h"

namespace twoway_sr {

	using sr::StateItem;
	using sr::ActionConstant;

	class Run : public GraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant> {
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSuperTag = false) :
			GraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant>
		(bChar, bPath, bSuperTag) {}
		~Run() = default;
	};
}

#endif
