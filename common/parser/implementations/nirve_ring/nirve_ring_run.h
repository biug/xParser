#include "common/parser/run_base.h"

namespace nirve_ring {
	class Run : public RunBase {
	private:
		bool m_bCharFeature;
		bool m_bPathFeature;
		bool m_bLabelFeature;
	public:
		Run(const bool & bChar = false, const bool & bPath = false, const bool & bLabel = false);
		~Run();

		void train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) const override;
		void parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) const override;
		void goldtest(const std::string & sInputFile, const std::string & sFeatureInput) const override;
	};
}
