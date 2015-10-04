#ifndef _TITOV_RUN_H
#define _TITOV_RUN_H

#include "titov_depparser.h"
#include "common/parser/implementations/graph_transition/graph_transition_run_base.h"

namespace titov {
	class Run : public GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem> {
	protected:
		void initConstant(const std::string & sInputFile = "") const override;
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
		~Run();
	};
}

#endif
