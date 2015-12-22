#include <ctime>
#include <memory>
#include <fstream>
#include <iostream>

#include "common/token/deplabel.h"
#include "common/token/supertag.h"
#include "arceagertag_run.h"
#include "arceagertag_depparser.h"

namespace arceagertag {
	extern int LABEL_COUNT;

	extern int AL_FIRST;
	extern int AR_FIRST;
	extern int RE_FIRST;
	extern int PP_FIRST;

	extern int AL_END;
	extern int AR_END;
	extern int RE_END;
	extern int PP_END;

	Run::Run() = default;

	Run::~Run() = default;

	void Run::train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) {
		int nRound = 0;
		DependencyTaggedTree ref_sent;

		std::cout << "Training iteration is started..." << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureInput, sFeatureOutput, ParserState::TRAIN));
		TDepLabel::add("ROOT");

		std::ifstream input(sInputFile);
		if (input) {
			while (input >> ref_sent) {
				for (const auto & token : ref_sent) {
					TDepLabel::add(TREENODE_LABEL(token.first));
					TSuperTag::add(token.second);
				}
			}
		}
		input.close();

		LABEL_COUNT = TDepLabel::count();
		AL_FIRST = POP_ROOT + 1;
		AL_END = AR_FIRST = AL_FIRST + TSuperTag::count() * TDepLabel::count();
		AR_END = RE_FIRST = AR_FIRST + TDepLabel::count();
		RE_END = PP_FIRST = RE_FIRST + TSuperTag::count();
		PP_END = PP_FIRST + TSuperTag::count();

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
		DependencyTaggedTree tree;

		std::cout << "Parsing started" << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureFile, sFeatureFile, ParserState::PARSE));
		std::ifstream input(sInputFile);
		std::ofstream output(sOutputFile);

		LABEL_COUNT = TDepLabel::count();
		AL_FIRST = POP_ROOT + 1;
		AL_END = AR_FIRST = AL_FIRST + TSuperTag::count() * TDepLabel::count();
		AR_END = RE_FIRST = AR_FIRST + TDepLabel::count();
		RE_END = PP_FIRST = RE_FIRST + TSuperTag::count();
		PP_END = PP_FIRST + TSuperTag::count();

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
		DependencyTaggedTree ref_sent;

		std::cout << "GoldTest iteration is started..." << std::endl;

		std::unique_ptr<DepParser> parser(new DepParser(sFeatureInput, "", ParserState::GOLDTEST));
		TDepLabel::add("ROOT");

		std::ifstream input(sInputFile);
		if (input) {
			while (input >> ref_sent) {
				for (const auto & token : ref_sent) {
					TDepLabel::add(TREENODE_LABEL(token.first));
					TSuperTag::add(token.second);
				}
			}
		}
		input.close();

		LABEL_COUNT = TDepLabel::count();
		AL_FIRST = POP_ROOT + 1;
		AL_END = AR_FIRST = AL_FIRST + TSuperTag::count() * TDepLabel::count();
		AR_END = RE_FIRST = AR_FIRST + TDepLabel::count();
		RE_END = PP_FIRST = RE_FIRST + TSuperTag::count();
		PP_END = PP_FIRST + TSuperTag::count();

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
