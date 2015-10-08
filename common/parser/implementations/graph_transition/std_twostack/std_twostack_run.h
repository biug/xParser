#ifndef _STD_TWOSTACK_RUN_H
#define _STD_TWOSTACK_RUN_H

#include "std_twostack_depparser.h"
#include "common/parser/implementations/graph_transition/graph_transition_run_base.h"

namespace std_twostack {
	class Run : public GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem> {
	protected:
		void initAction() const override;
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
		~Run();
	};
}

#endif
