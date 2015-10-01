#ifndef _STD_NIVRE_RUN_H
#define _STD_NIVRE_RUN_H

#include "std_nivre_depparser.h"
#include "common/parser/implementations/graphbased/graphrun_base.h"

namespace std_nivre {
	class Run : public GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem> {
	protected:
		void initConstant(const std::string & sInputFile = "") const override;
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
		~Run();
	};
}

#endif
