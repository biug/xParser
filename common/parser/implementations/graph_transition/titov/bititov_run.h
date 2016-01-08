#ifndef _BITITOV_RUN_H
#define _BITITOV_RUN_H

#include "bititov_depparser.h"
#include "common/parser/implementations/graph_transition/bigraph_transition_run.h"

namespace bititov {

	using titov::StateItem;
	using titov::ActionConstant;

	class Run : public BiGraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant> {
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSuperTag = false) :
			BiGraphTransitionRun<DepParser, PackedScoreType, PackedSuperTagScoreType, StateItem, ActionConstant>
		(bChar, bPath, bSuperTag) {}
		~Run() = default;
	};
}

#endif
