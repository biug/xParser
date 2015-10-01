#ifndef _STD_TITOV_RUN_H
#define _STD_TITOV_RUN_H

#include "std_titov_depparser.h"
#include "common/parser/implementations/graphbased/graphrun_base.h"

namespace std_titov {
	class Run : public GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem> {
	protected:
		void initConstant(const std::string & sInputFile = "") const override;
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
		~Run();
	};
}

#endif
