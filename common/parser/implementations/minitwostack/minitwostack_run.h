#ifndef _MINITWOSTACK_RUN_H
#define _MINITWOSTACK_RUN_H

#include "common/parser/run_base.h"

namespace minitwostack {
	class Run : public RunBase {
	public:
		bool m_bChar;
		bool m_bPath;

		Run(bool bChar = false, bool bPath = false);
		~Run();

		void train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) override;
		void parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) override;
		void goldtest(const std::string & sInputFile, const std::string & sFeatureInput) override;
	};
}

#endif
