#include <ctime>
#include <memory>
#include <fstream>
#include <iostream>

#include "common/token/deplabel.h"
#include "minitwostack_run.h"
#include "minitwostack_depparser.h"

namespace minitwostack {
	extern int A_MM_FIRST;
	extern int A_RC_FIRST;
	extern int A_SH_FIRST;
	extern int A_RE_FIRST;

	extern int A_MM_END;
	extern int A_RC_END;
	extern int A_SH_END;
	extern int A_RE_END;

	Run::Run(bool bChar, bool bPath) : m_bChar(bChar), m_bPath(bPath) {};

	Run::~Run() = default;

	void Run::train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) {
		int nRound = 0;
		DependencyGraph ref_sent;

		std::cout << "Training iteration is started..." << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureInput, sFeatureOutput, ParserState::TRAIN));

		std::ifstream input(sInputFile);
		if (input) {
			while (input >> ref_sent) {
				for (const auto & token : ref_sent) {
					for (const auto & arc : token.m_vecRightArcs) {
						TDepLabel::add(arc.second);
					}
				}
			}
		}
		input.close();

		loadLabels();
		A_MM_FIRST = REDUCE + 1;
		A_RC_FIRST = A_MM_END = A_MM_FIRST + TDepLabel::count();
		A_SH_FIRST = A_RC_END = A_RC_FIRST + TDepLabel::count();
		A_RE_FIRST = A_SH_END = A_SH_FIRST + TDepLabel::count();
		A_RE_END = A_RE_FIRST + TDepLabel::count();

		input.open(sInputFile);
		if (input) {
			while (input >> ref_sent) {
				++nRound;
				parser->train(ref_sent, nRound);
			}
			parser->finishtraining();
		}
		input.close();

		std::cout << "Done." << std::endl;
	}

	void Run::parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) {

		DependencyGraph sentence;
		DependencyGraph tree;

		std::cout << "Parsing started" << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureFile, sFeatureFile, ParserState::PARSE));
		std::ifstream input(sInputFile);
		std::ofstream output(sOutputFile);

		loadLabels();
		A_MM_FIRST = REDUCE + 1;
		A_RC_FIRST = A_MM_END = A_MM_FIRST + TDepLabel::count();
		A_SH_FIRST = A_RC_END = A_RC_FIRST + TDepLabel::count();
		A_RE_FIRST = A_SH_END = A_SH_FIRST + TDepLabel::count();
		A_RE_END = A_RE_FIRST + TDepLabel::count();

		if (input) {
			while (input >> sentence) {
				if (sentence.size() < MAX_SENTENCE_SIZE) {
					parser->parse(sentence, &tree);
					output << tree;
					tree.clear();
				}
			}
		}
		input.close();
		output.close();
	}

	void Run::goldtest(const std::string & sInputFile, const std::string & sFeatureInput) {
		int nRound = 0;
		DependencyGraph ref_sent;

		std::cout << "GoldTest iteration is started..." << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureInput, "", ParserState::GOLDTEST));

		std::ifstream input(sInputFile);
		if (input) {
			while (input >> ref_sent) {
				for (const auto & token : ref_sent) {
					for (const auto & arc : token.m_vecRightArcs) {
						TDepLabel::add(arc.second);
					}
				}
			}
		}
		input.close();

		loadLabels();
		A_MM_FIRST = REDUCE + 1;
		A_RC_FIRST = A_MM_END = A_MM_FIRST + TDepLabel::count();
		A_SH_FIRST = A_RC_END = A_RC_FIRST + TDepLabel::count();
		A_RE_FIRST = A_SH_END = A_SH_FIRST + TDepLabel::count();
		A_RE_END = A_RE_FIRST + TDepLabel::count();

//		std::cout << "arc mem first is " << A_MM_FIRST << std::endl;
//		std::cout << "arc recall first is " << A_RC_FIRST << std::endl;
//		std::cout << "arc shift first is " << A_SH_FIRST << std::endl;
//		std::cout << "arc reduce first is " << A_RE_FIRST << std::endl;
//		std::cout << "label map size is " << TDepLabel::count() << std::endl;
//		std::cout << TDepLabel::getTokenizer() << std::endl;

		input.open(sInputFile);
		if (input) {
			while (input >> ref_sent) {
				++nRound;
				parser->goldCheck(ref_sent);
			}
		}
		input.close();

		std::cout << "total " << nRound << " round" << std::endl;

		std::cout << "Done." << std::endl;
	}
}
