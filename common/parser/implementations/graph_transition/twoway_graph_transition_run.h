#ifndef _TWOWAY_GRAPH_TRANSITION_RUN_H
#define _TWOWAY_GRAPH_TRANSITION_RUN_H

#include "graph_transition_macros.h"

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
class TwoWayGraphTransitionRun : public RunBase {
protected:
	bool m_bCharFeature;
	bool m_bPathFeature;
	bool m_bSuperTagFeature;

public:
	TwoWayGraphTransitionRun(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
	virtual ~TwoWayGraphTransitionRun() {};

	void train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) override;
	void parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) override;
	void goldtest(const std::string & sInputFile, const std::string & sFeatureInput) override;
};

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
TwoWayGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::TwoWayGraphTransitionRun
(const bool & bChar, const bool & bPath, const bool & bSuperTag) : m_bCharFeature(bChar), m_bPathFeature(bPath), m_bSuperTagFeature(bSuperTag) {}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::train
(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) {
	int nRound = 0;
	DependencyGraph sentence;

	std::cout << "Training is started..." << std::endl;

	if (m_bSuperTagFeature) {
		std::unique_ptr<DEP_PARSER<RET_TYPE2>> parser(
					(DEP_PARSER<RET_TYPE2>*)new DEP_PARSER<RET_TYPE2>
					(sInputFile, sFeatureInput, sFeatureOutput, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

		std::ifstream input(sInputFile);
		if (input) {
			while (input >> sentence) {
				if (!m_bSuperTagFeature) {
					sentence.clearSuperTag();
				}
				parser->train(sentence, ++nRound);
			}
			std::cout << std::endl;
			parser->finishtraining();
		}
		input.close();
	}
	else {
		std::unique_ptr<DEP_PARSER<RET_TYPE1>> parser(
					(DEP_PARSER<RET_TYPE1>*)new DEP_PARSER<RET_TYPE1>
					(sInputFile, sFeatureInput, sFeatureOutput, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

		std::ifstream input(sInputFile);
		if (input) {
			while (input >> sentence) {
				if (!m_bSuperTagFeature) {
					sentence.clearSuperTag();
				}
				parser->train(sentence, ++nRound);
			}
			std::cout << std::endl;
			parser->finishtraining();
		}
		input.close();
	}

	std::cout << "Done." << std::endl;
}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::parse
(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) {

	DependencyGraph sentence;
	DependencyGraph graph[3];

	std::cout << "Parsing is started..." << std::endl;

	std::ifstream input(sInputFile);
	std::ofstream output(sOutputFile);
	std::ofstream output0(sOutputFile + "0");
	std::ofstream output1(sOutputFile + "1");

	if (m_bSuperTagFeature) {
		std::unique_ptr<DEP_PARSER<RET_TYPE2>> parser(
					(DEP_PARSER<RET_TYPE2>*)new DEP_PARSER<RET_TYPE2>
					("", sFeatureFile, sFeatureFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));


		if (input) {
			while (input >> sentence) {
				if (sentence.size() < MAX_SENTENCE_SIZE) {
					if (!m_bSuperTagFeature) {
						sentence.clearSuperTag();
					}
					parser->parse(sentence, graph);
					output0 << graph[0];
					output1 << graph[1];
					output << graph[2];
					graph[0].clear();
					graph[1].clear();
					graph[2].clear();
				}
			}
		}
	}
	else {
		std::unique_ptr<DEP_PARSER<RET_TYPE1>> parser(
					(DEP_PARSER<RET_TYPE1>*)new DEP_PARSER<RET_TYPE1>
					("", sFeatureFile, sFeatureFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

		if (input) {
			while (input >> sentence) {
				if (sentence.size() < MAX_SENTENCE_SIZE) {
					if (!m_bSuperTagFeature) {
						sentence.clearSuperTag();
					}
					parser->parse(sentence, graph);
					output0 << graph[0];
					output1 << graph[1];
					output << graph[2];
					graph[0].clear();
					graph[1].clear();
					graph[2].clear();
				}
			}
		}
	}
	std::cout << std::endl;
	input.close();
	output.close();
	output0.close();
	output1.close();
}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void TwoWayGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::goldtest
(const std::string & sInputFile, const std::string & sFeatureInput) {
	int nRound = 0;
	DependencyGraph sentence;

	std::cout << "GoldTesting is started..." << std::endl;

	std::unique_ptr<DEP_PARSER<RET_TYPE2>> parser(
			(DEP_PARSER<RET_TYPE2>*)new DEP_PARSER<RET_TYPE2>
			(sInputFile, sFeatureInput, sFeatureInput, ParserState::PARSE,
			m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

	std::ifstream input(sInputFile);
	if (input) {
		while (input >> sentence) {
			++nRound;
			if (!m_bSuperTagFeature) {
				sentence.clearSuperTag();
			}
			parser->goldCheck(sentence);
		}
	}
	input.close();

	std::cout << std::endl << "total " << nRound << " round" << std::endl;

	std::cout << "Done." << std::endl;

}

#endif
