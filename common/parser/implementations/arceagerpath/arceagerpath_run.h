#ifndef _ARCEAGERPATH_RUN_H
#define _ARCEAGERPATH_RUN_H

#include "common/parser/run_base.h"

namespace arceagerpath {
	class Run : public RunBase {
	public:
		Run();
		~Run();

		void train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) override;
		void parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) override;
		void goldtest(const std::string & sInputFile, const std::string & sFeatureInput) override;
	};
}

#endif
