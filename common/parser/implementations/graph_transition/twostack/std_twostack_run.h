#ifndef _STD_TWOSTACK_RUN_H
#define _STD_TWOSTACK_RUN_H

#include "std_twostack_depparser.h"
#include "common/parser/implementations/graph_transition/graph_transition_run.h"

namespace std_twostack {

	class Run : public GraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant> {
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSuperTag = false) :
			GraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant>
		(bChar, bPath, bSuperTag) {}
		~Run() = default;
	};
}

#endif
