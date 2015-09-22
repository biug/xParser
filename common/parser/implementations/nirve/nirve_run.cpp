#include <ctime>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

#include "common/token/deplabel.h"
#include "nirve_run.h"
#include "nirve_depparser.h"

namespace nirve {

	extern int AL_SW_FIRST;
	extern int AR_SW_FIRST;
	extern int AL_SH_FIRST;
	extern int AR_SH_FIRST;
	extern int AL_RE_FIRST;
	extern int AR_RE_FIRST;

	extern int AL_SW_END;
	extern int AR_SW_END;
	extern int AL_SH_END;
	extern int AR_SH_END;
	extern int AL_RE_END;
	extern int AR_RE_END;

	Run::Run(const bool & bChar, const bool & bPath, const bool & bLabel) : m_bCharFeature(bChar), m_bPathFeature(bPath), m_bLabelFeature(bLabel) {}

	Run::~Run() = default;

	void Run::train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) const {
		int nRound = 0;
		DependencyGraph ref_sent;

		std::cout << "Training iteration is started..." << std::endl;
		std::cout << (m_bCharFeature ? "use char" : "without char") << std::endl;
		std::cout << (m_bPathFeature ? "use path" : "without path") << std::endl;
		std::cout << (m_bLabelFeature ? "use label" : "without label") << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureInput, sFeatureOutput, m_bCharFeature, m_bPathFeature, m_bLabelFeature, ParserState::TRAIN));
		
		std::string sSdpFile;
		std::string sSyntaxTreeFile;
		if (m_bPathFeature) {
			int split = sInputFile.find("#");
			sSdpFile = sInputFile.substr(0, split);
			sSyntaxTreeFile = sInputFile.substr(split + 1);
		}
		else {
			sSdpFile = sInputFile;
		}

#ifdef _DEBUG
		std::cout << "sdp file is " << sSdpFile << std::endl;
		std::cout << "tree file is " << sSyntaxTreeFile << std::endl;
#endif

		std::ifstream input(sSdpFile);
		if (input) {
			while (input >> ref_sent)
				;
		}
		input.close();
		std::cout << "pre load complete." << std::endl;

#ifdef _DEBUG
		std::cout << "DepLabels Total ";
		std::cout << TDepLabel::getTokenizer();
#endif

		AL_SW_FIRST = SHIFT_REDUCE + 1;
		AL_SW_END = AR_SW_FIRST = (m_bLabelFeature ? AL_SW_FIRST + TDepLabel::count() : AL_SW_FIRST + 1);
		AR_SW_END = AL_SH_FIRST = (m_bLabelFeature ? AR_SW_FIRST + TDepLabel::count() : AR_SW_FIRST + 1);
		AL_SH_END = AR_SH_FIRST = (m_bLabelFeature ? AL_SH_FIRST + TDepLabel::count() : AL_SH_FIRST + 1);
		AR_SH_END = AL_RE_FIRST = (m_bLabelFeature ? AR_SH_FIRST + TDepLabel::count() : AR_SH_FIRST + 1);
		AL_RE_END = AR_RE_FIRST = (m_bLabelFeature ? AL_RE_FIRST + TDepLabel::count() : AL_RE_FIRST + 1);
		AR_RE_END = (m_bLabelFeature ? AR_RE_FIRST + TDepLabel::count() : AR_RE_FIRST + 1);

#ifdef _DEBUG
		std::cout << "al_sw_first = " << AL_SW_FIRST << std::endl;
		std::cout << "ar_sw_first = " << AR_SW_FIRST << std::endl;
		std::cout << "al_sh_first = " << AL_SH_FIRST << std::endl;
		std::cout << "ar_sh_first = " << AR_SH_FIRST << std::endl;
		std::cout << "al_re_first = " << AL_RE_FIRST << std::endl;
		std::cout << "ar_re_first = " << AR_RE_FIRST << std::endl;
#endif
		std::ifstream treeInput;
		if (m_bPathFeature) {
			treeInput.open(sSyntaxTreeFile);
		}
		input.open(sSdpFile);
		if (input) {
			while (input >> ref_sent) {
				if (!m_bLabelFeature) clearGraphLabel(ref_sent);
				if (m_bPathFeature) addSyntaxTree(treeInput, ref_sent);
				parser->train(ref_sent, ++nRound);
			}
			parser->finishtraining();
		}
		input.close();

		std::cout << "Done." << std::endl;
	}

	void Run::parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) const {

		DependencyGraph sentence;
		DependencyGraph graph;

		std::cout << "Parsing started" << std::endl;

		std::string sSdpFile;
		std::string sSyntaxTreeFile;
		if (m_bPathFeature) {
			int split = sInputFile.find("#");
			sSdpFile = sInputFile.substr(0, split);
			sSyntaxTreeFile = sInputFile.substr(split + 1);
		}
		else {
			sSdpFile = sInputFile;
		}

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureFile, sFeatureFile, m_bCharFeature, m_bPathFeature, m_bLabelFeature, ParserState::PARSE));
		std::ifstream treeInput;
		std::ifstream input(sSdpFile);
		std::ofstream output(sOutputFile);

		AL_SW_FIRST = SHIFT_REDUCE + 1;
		AL_SW_END = AR_SW_FIRST = (m_bLabelFeature ? AL_SW_FIRST + TDepLabel::count() : AL_SW_FIRST + 1);
		AR_SW_END = AL_SH_FIRST = (m_bLabelFeature ? AR_SW_FIRST + TDepLabel::count() : AR_SW_FIRST + 1);
		AL_SH_END = AR_SH_FIRST = (m_bLabelFeature ? AL_SH_FIRST + TDepLabel::count() : AL_SH_FIRST + 1);
		AR_SH_END = AL_RE_FIRST = (m_bLabelFeature ? AR_SH_FIRST + TDepLabel::count() : AR_SH_FIRST + 1);
		AL_RE_END = AR_RE_FIRST = (m_bLabelFeature ? AL_RE_FIRST + TDepLabel::count() : AL_RE_FIRST + 1);
		AR_RE_END = (m_bLabelFeature ? AR_RE_FIRST + TDepLabel::count() : AR_RE_FIRST + 1);

		if (m_bPathFeature) {
			treeInput.open(sSyntaxTreeFile);
		}
		if (input) {
			while (input >> sentence) {
				if (!m_bLabelFeature) clearGraphLabel(sentence);
				if (m_bPathFeature) addSyntaxTree(treeInput, sentence);

				if (sentence.size() < MAX_SENTENCE_SIZE) {
					parser->parse(sentence, &graph);
					output << graph;
					graph.clear();
				}
			}
		}
		input.close();
		output.close();
	}

	void Run::goldtest(const std::string & sInputFile, const std::string & sFeatureInput) const {
		int nRound = 0;
		DependencyGraph ref_sent;

		std::cout << "GoldTest iteration is started..." << std::endl;

		auto time_begin = time(NULL);

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureInput, "", m_bCharFeature, m_bPathFeature, m_bLabelFeature, ParserState::GOLDTEST));

		std::ifstream input(sInputFile);
		if (input) {
			while (input >> ref_sent)
				;
		}
		input.close();
		std::cout << "pre load complete." << std::endl;
		std::cout << "DepLabels Total ";
		std::cout << TDepLabel::getTokenizer();

		AL_SW_FIRST = SHIFT_REDUCE + 1;
		AL_SW_END = AR_SW_FIRST = AL_SW_FIRST + TDepLabel::count();
		AR_SW_END = AL_SH_FIRST = AR_SW_FIRST + TDepLabel::count();
		AL_SH_END = AR_SH_FIRST = AL_SH_FIRST + TDepLabel::count();
		AR_SH_END = AL_RE_FIRST = AR_SH_FIRST + TDepLabel::count();
		AL_RE_END = AR_RE_FIRST = AL_RE_FIRST + TDepLabel::count();
		AR_RE_END = AR_RE_FIRST + TDepLabel::count();

		input.open(sInputFile);
		if (input) {
			while (input >> ref_sent) {
				++nRound;
				std::cout << nRound << std::endl;
				parser->goldCheck(ref_sent);
				//break;
			}
		}
		input.close();

		std::cout << "total " << nRound << " round" << std::endl;

		auto time_end = time(NULL);

		std::cout << "Done." << std::endl;

		std::cout << "Training has finished successfully. Total time taken is: " << difftime(time_end, time_begin) << "s" << std::endl;
	}
}
