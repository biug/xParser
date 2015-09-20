#include <ctime>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

#include "common/token/deplabel.h"
#include "twostack_run.h"
#include "twostack_depparser.h"

namespace twostack {

	extern int AL_MM_FIRST;
	extern int AR_MM_FIRST;
	extern int AL_RC_FIRST;
	extern int AR_RC_FIRST;
	extern int AL_SH_FIRST;
	extern int AR_SH_FIRST;
	extern int AL_RE_FIRST;
	extern int AR_RE_FIRST;

	extern int AL_MM_END;
	extern int AR_MM_END;
	extern int AL_RC_END;
	extern int AR_RC_END;
	extern int AL_SH_END;
	extern int AR_SH_END;
	extern int AL_RE_END;
	extern int AR_RE_END;

	Run::Run(const bool & bChar, const bool & bPath, const bool & bLabel) : m_bCharFeature(bChar), m_bPathFeature(bPath), m_bLabelFeature(bLabel) {}

	Run::~Run() = default;

	void clearLabel(DependencyGraph & graph) {
		for (auto & node : graph) for (auto & rn : GRAPHNODE_RIGHTNODES(node)) RIGHTNODE_LABEL(rn) = 1;
	}

	void addSyntaxTree(std::ifstream & input, DependencyGraph & graph) {
		int i = 0;
		std::string line;
		while (true) {
			std::getline(input, line);
			if (line.empty()) {
				break;
			}
			if (line.find("#") == 0) {
				continue;
			}
			std::string token;
			std::istringstream iss(line);
			iss >> token >> GRAPHNODE_TREEHEAD(graph[i]) >> GRAPHNODE_TREELABEL(graph[i]);
			if (GRAPHNODE_TREEHEAD(graph[i]) == 0) {
				GRAPHNODE_TREEHEAD(graph[i]) = graph.size() - 1;
			}
			else {
				--GRAPHNODE_TREEHEAD(graph[i]);
			}
#ifdef _DEBUG
			std::cout << GRAPHNODE_TREEHEAD(graph[i]) << " -> " << i << std::endl;
#endif
			++i;
		}
		GRAPHNODE_TREEHEAD(graph.back()) = -1;
		GRAPHNODE_TREELABEL(graph.back()) = ROOT_DEPLABEL;
	}

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

		AL_MM_FIRST = SHIFT_REDUCE + 1;
		AL_MM_END = AR_MM_FIRST = (m_bLabelFeature ? AL_MM_FIRST + TDepLabel::count() : AL_MM_FIRST + 1);
		AR_MM_END = AL_RC_FIRST = (m_bLabelFeature ? AR_MM_FIRST + TDepLabel::count() : AR_MM_FIRST + 1);
		AL_RC_END = AR_RC_FIRST = (m_bLabelFeature ? AL_RC_FIRST + TDepLabel::count() : AL_RC_FIRST + 1);
		AR_RC_END = AL_SH_FIRST = (m_bLabelFeature ? AR_RC_FIRST + TDepLabel::count() : AR_RC_FIRST + 1);
		AL_SH_END = AR_SH_FIRST = (m_bLabelFeature ? AL_SH_FIRST + TDepLabel::count() : AL_SH_FIRST + 1);
		AR_SH_END = AL_RE_FIRST = (m_bLabelFeature ? AR_SH_FIRST + TDepLabel::count() : AR_SH_FIRST + 1);
		AL_RE_END = AR_RE_FIRST = (m_bLabelFeature ? AL_RE_FIRST + TDepLabel::count() : AL_RE_FIRST + 1);
		AR_RE_END = (m_bLabelFeature ? AR_RE_FIRST + TDepLabel::count() : AR_RE_FIRST + 1);

#ifdef _DEBUG
		std::cout << "al_mm_first = " << AL_MM_FIRST << std::endl;
		std::cout << "ar_mm_first = " << AR_MM_FIRST << std::endl;
		std::cout << "al_rc_first = " << AL_RC_FIRST << std::endl;
		std::cout << "ar_rc_first = " << AR_RC_FIRST << std::endl;
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
				if (!m_bLabelFeature) clearLabel(ref_sent);
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

		AL_MM_FIRST = SHIFT_REDUCE + 1;
		AL_MM_END = AR_MM_FIRST = (m_bLabelFeature ? AL_MM_FIRST + TDepLabel::count() : AL_MM_FIRST + 1);
		AR_MM_END = AL_RC_FIRST = (m_bLabelFeature ? AR_MM_FIRST + TDepLabel::count() : AR_MM_FIRST + 1);
		AL_RC_END = AR_RC_FIRST = (m_bLabelFeature ? AL_RC_FIRST + TDepLabel::count() : AL_RC_FIRST + 1);
		AR_RC_END = AL_SH_FIRST = (m_bLabelFeature ? AR_RC_FIRST + TDepLabel::count() : AR_RC_FIRST + 1);
		AL_SH_END = AR_SH_FIRST = (m_bLabelFeature ? AL_SH_FIRST + TDepLabel::count() : AL_SH_FIRST + 1);
		AR_SH_END = AL_RE_FIRST = (m_bLabelFeature ? AR_SH_FIRST + TDepLabel::count() : AR_SH_FIRST + 1);
		AL_RE_END = AR_RE_FIRST = (m_bLabelFeature ? AL_RE_FIRST + TDepLabel::count() : AL_RE_FIRST + 1);
		AR_RE_END = (m_bLabelFeature ? AR_RE_FIRST + TDepLabel::count() : AR_RE_FIRST + 1);

		if (m_bPathFeature) {
			treeInput.open(sSyntaxTreeFile);
		}
		if (input) {
			while (input >> sentence) {
				if (!m_bLabelFeature) clearLabel(sentence);
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

		AL_MM_FIRST = SHIFT_REDUCE + 1;
		AL_MM_END = AR_MM_FIRST = AL_MM_FIRST + TDepLabel::count();
		AR_MM_END = AL_RC_FIRST = AR_MM_FIRST + TDepLabel::count();
		AL_RC_END = AR_RC_FIRST = AL_RC_FIRST + TDepLabel::count();
		AR_RC_END = AL_SH_FIRST = AR_RC_FIRST + TDepLabel::count();
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
