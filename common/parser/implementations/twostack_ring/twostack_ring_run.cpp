#include <ctime>
#include <memory>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

#include "common/token/deplabel.h"
#include "twostack_ring_run.h"
#include "twostack_ring_depparser.h"

namespace twostack_ring {

	extern int A_MM_FIRST;
	extern int A_RC_FIRST;
	extern int A_SH_FIRST;
	extern int A_RE_FIRST;

	extern int A_MM_END;
	extern int A_RC_END;
	extern int A_SH_END;
	extern int A_RE_END;

	extern std::vector<int> g_vecLabelMap;

	Run::Run(const bool & bChar, const bool & bPath, const bool & bLabel) : m_bCharFeature(bChar), m_bPathFeature(bPath), m_bLabelFeature(bLabel) {}

	Run::~Run() = default;

	void Run::train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) const {
		int nRound = 0;
		DependencyCONLLGraph ref_sent;

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

		A_MM_FIRST = REDUCE + 1;
		A_MM_END = A_RC_FIRST = (m_bLabelFeature ? A_MM_FIRST + TDepLabel::count() : A_MM_FIRST + 1);
		A_RC_END = A_SH_FIRST = (m_bLabelFeature ? A_RC_FIRST + TDepLabel::count() : A_RC_FIRST + 1);
		A_SH_END = A_RE_FIRST = (m_bLabelFeature ? A_SH_FIRST + TDepLabel::count() : A_SH_FIRST + 1);
		A_RE_END = (m_bLabelFeature ? A_RE_FIRST + TDepLabel::count() : A_RE_FIRST + 1);

		int count = 0;
		g_vecLabelMap.clear();
		g_vecLabelMap.push_back(0);
		std::unordered_map<std::string, int> labelMap;
		for (int i = TDepLabel::START; i <= TDepLabel::count(); ++i) {
			const ttoken & label = TDepLabel::key(i);
			if (IS_LEFT_LABEL(label)) {
				if (labelMap.find(DECODE_LEFT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_LEFT_LABEL(label)] = ++count;
				}
				g_vecLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_LEFT_LABEL(label)], 0));
			}
			else if (IS_RIGHT_LABEL(label)) {
				if (labelMap.find(DECODE_RIGHT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_RIGHT_LABEL(label)] = ++count;
				}
				g_vecLabelMap.push_back(ENCODE_LABEL_ID(0, labelMap[DECODE_RIGHT_LABEL(label)]));
			}
			else if (IS_TWOWAY_LABEL(label)) {
				if (labelMap.find(DECODE_TWOWAY_LEFT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_TWOWAY_LEFT_LABEL(label)] = ++count;
				}
				if (labelMap.find(DECODE_TWOWAY_RIGHT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)] = ++count;
				}
				g_vecLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_TWOWAY_LEFT_LABEL(label)], labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)]));
			}
		}

#ifdef _DEBUG
		std::cout << "arc_mm_first = " << A_MM_FIRST << std::endl;
		std::cout << "arc_rc_first = " << A_RC_FIRST << std::endl;
		std::cout << "arc_sh_first = " << A_SH_FIRST << std::endl;
		std::cout << "arc_re_first = " << A_RE_FIRST << std::endl;
#endif
		std::ifstream treeInput;
		if (m_bPathFeature) {
			treeInput.open(sSyntaxTreeFile);
		}
		input.open(sSdpFile);
		if (input) {
			while (input >> ref_sent) {
				if (!m_bLabelFeature) clearCONLLGraphLabel(ref_sent);
				parser->train(ref_sent, ++nRound);
			}
			parser->finishtraining();
		}
		input.close();

		std::cout << "Done." << std::endl;
	}

	void Run::parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) const {

		DependencyCONLLGraph sentence;
		DependencyCONLLGraph graph;

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

		A_MM_FIRST = REDUCE + 1;
		A_MM_END = A_RC_FIRST = (m_bLabelFeature ? A_MM_FIRST + TDepLabel::count() : A_MM_FIRST + 1);
		A_RC_END = A_SH_FIRST = (m_bLabelFeature ? A_RC_FIRST + TDepLabel::count() : A_RC_FIRST + 1);
		A_SH_END = A_RE_FIRST = (m_bLabelFeature ? A_SH_FIRST + TDepLabel::count() : A_SH_FIRST + 1);
		A_RE_END = (m_bLabelFeature ? A_RE_FIRST + TDepLabel::count() : A_RE_FIRST + 1);

		int count = 0;
		g_vecLabelMap.clear();
		g_vecLabelMap.push_back(0);
		std::unordered_map<std::string, int> labelMap;
		for (int i = TDepLabel::START; i <= TDepLabel::count(); ++i) {
			const ttoken & label = TDepLabel::key(i);
			if (IS_LEFT_LABEL(label)) {
				if (labelMap.find(DECODE_LEFT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_LEFT_LABEL(label)] = ++count;
				}
				g_vecLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_LEFT_LABEL(label)], 0));
			}
			else if (IS_RIGHT_LABEL(label)) {
				if (labelMap.find(DECODE_RIGHT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_RIGHT_LABEL(label)] = ++count;
				}
				g_vecLabelMap.push_back(ENCODE_LABEL_ID(0, labelMap[DECODE_RIGHT_LABEL(label)]));
			}
			else if (IS_TWOWAY_LABEL(label)) {
				if (labelMap.find(DECODE_TWOWAY_LEFT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_TWOWAY_LEFT_LABEL(label)] = ++count;
				}
				if (labelMap.find(DECODE_TWOWAY_RIGHT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)] = ++count;
				}
				g_vecLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_TWOWAY_LEFT_LABEL(label)], labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)]));
			}
		}

		if (m_bPathFeature) {
			treeInput.open(sSyntaxTreeFile);
		}
		if (input) {
			while (input >> sentence) {
				if (!m_bLabelFeature) clearCONLLGraphLabel(sentence);

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
		DependencyCONLLGraph ref_sent;

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

		A_MM_FIRST = REDUCE + 1;
		A_MM_END = A_RC_FIRST = A_MM_FIRST + TDepLabel::count();
		A_RC_END = A_SH_FIRST = A_RC_FIRST + TDepLabel::count();
		A_SH_END = A_RE_FIRST = A_SH_FIRST + TDepLabel::count();
		A_RE_END = A_RE_FIRST + TDepLabel::count();

		int count = 0;
		g_vecLabelMap.clear();
		g_vecLabelMap.push_back(0);
		std::unordered_map<std::string, int> labelMap;
		for (int i = TDepLabel::START; i <= TDepLabel::count(); ++i) {
			const ttoken & label = TDepLabel::key(i);
			if (IS_LEFT_LABEL(label)) {
				if (labelMap.find(DECODE_LEFT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_LEFT_LABEL(label)] = ++count;
				}
				g_vecLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_LEFT_LABEL(label)], 0));
			}
			else if (IS_RIGHT_LABEL(label)) {
				if (labelMap.find(DECODE_RIGHT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_RIGHT_LABEL(label)] = ++count;
				}
				g_vecLabelMap.push_back(ENCODE_LABEL_ID(0, labelMap[DECODE_RIGHT_LABEL(label)]));
			}
			else if (IS_TWOWAY_LABEL(label)) {
				if (labelMap.find(DECODE_TWOWAY_LEFT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_TWOWAY_LEFT_LABEL(label)] = ++count;
				}
				if (labelMap.find(DECODE_TWOWAY_RIGHT_LABEL(label)) == labelMap.end()) {
					labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)] = ++count;
				}
				g_vecLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_TWOWAY_LEFT_LABEL(label)], labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)]));
			}
		}

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
