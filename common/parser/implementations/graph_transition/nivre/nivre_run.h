#ifndef _NIVRE_RUN_H
#define _NIVRE_RUN_H

#include "nivre_depparser.h"
#include "common/parser/implementations/graph_transition/graph_transition_run_base.h"

namespace nivre {

	using graph_transition::GraphRunBase;

	class Run : public GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem> {
	protected:
		void initAction() const override;
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
		~Run();
	};
}

#endif
