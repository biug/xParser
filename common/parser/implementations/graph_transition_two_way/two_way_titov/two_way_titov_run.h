#ifndef _TWO_WAY_TITOV_RUN_H
#define _TWO_WAY_TITOV_RUN_H

#include "two_way_titov_depparser.h"
#include "common/parser/implementations/graph_transition_two_way/graph_transition_two_way_run_base.h"

namespace two_way_titov {

	using graph_transition_two_way::GraphRunBase;

	class Run : public GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem> {
	protected:
		void initAction() const override;
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
		~Run();
	};
}

#endif
