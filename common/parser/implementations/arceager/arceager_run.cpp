#include <ctime>
#include <memory>
#include <fstream>
#include <iostream>

#include "common/token/deplabel.h"
#include "arceager_run.h"
#include "arceager_depparser.h"

namespace arceager {
	extern int AL_FIRST;
	extern int AR_FIRST;

	extern int AL_END;
	extern int AR_END;

	Run::Run() = default;

	Run::~Run() = default;

	void Run::train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) {
		int nRound = 0;
		DependencyTree ref_sent;

		std::cout << "Training iteration is started..." << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureInput, sFeatureOutput, ParserState::TRAIN));
		TDepLabel::add("ROOT");

		std::ifstream input(sInputFile);
		if (input) {
			while (input >> ref_sent) {
				for (const auto & token : ref_sent) {
					TDepLabel::add(TREENODE_LABEL(token));
				}
			}
		}
		input.close();
		AL_FIRST = POP_ROOT + 1;
		AL_END = AR_FIRST = AL_FIRST + TDepLabel::count();
		AR_END = AR_FIRST + TDepLabel::count();
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

		Sentence sentence;
		DependencyTree tree;

		std::cout << "Parsing started" << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureFile, sFeatureFile, ParserState::PARSE));
		std::ifstream input(sInputFile);
		std::ofstream output(sOutputFile);

		AL_FIRST = POP_ROOT + 1;
		AL_END = AR_FIRST = AL_FIRST + TDepLabel::count();
		AR_END = AR_FIRST + TDepLabel::count();

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
		DependencyTree ref_sent;

		std::cout << "GoldTest iteration is started..." << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureInput, "", ParserState::GOLDTEST));
		TDepLabel::add("ROOT");

		std::ifstream input(sInputFile);
		if (input) {
			while (input >> ref_sent) {
				for (const auto & token : ref_sent) {
					TDepLabel::add(TREENODE_LABEL(token));
				}
			}
		}
		input.close();
		AL_FIRST = POP_ROOT + 1;
		AL_END = AR_FIRST = AL_FIRST + TDepLabel::count();
		AR_END = AR_FIRST + TDepLabel::count();
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
