#ifndef _BOTH_TWOSTACK_RUN_H
#define _BOTH_TWOSTACK_RUN_H

#include "both_twostack_depparser.h"
#include "common/parser/implementations/graph_transition_both/graph_transition_both_run_base.h"

namespace both_twostack {

	using graph_transition_both::GraphRunBase;

	class Run : public GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem> {
	protected:
		void initAction() const override;
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
		~Run();
	};
}

#endif
