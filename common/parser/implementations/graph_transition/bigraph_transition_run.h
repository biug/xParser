#ifndef _BIGRAPH_TRANSITION_RUN_H
#define _BIGRAPH_TRANSITION_RUN_H

#include "graph_transition_macros.h"

#include <memory>

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
class BiGraphTransitionRun : public RunBase {
protected:
	bool m_bCharFeature;
	bool m_bPathFeature;
	bool m_bSuperTagFeature;

public:
	BiGraphTransitionRun(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
	virtual ~BiGraphTransitionRun() {};

	void train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) override;
	void parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) override;
	void goldtest(const std::string & sInputFile, const std::string & sFeatureInput) override;
};

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
BiGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::BiGraphTransitionRun
(const bool & bChar, const bool & bPath, const bool & bSuperTag) : m_bCharFeature(bChar), m_bPathFeature(bPath), m_bSuperTagFeature(bSuperTag) {}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void BiGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::train
(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) {
	int nRound = 0;
	DependencyGraph sentence, rsentence;

	std::cout << "Training is started..." << std::endl;

	std::string sInput = sInputFile.substr(0, sInputFile.find('#'));
	std::string sRevInput = sInputFile.substr(sInputFile.find('#') + 1);

	if (m_bSuperTagFeature) {
		std::unique_ptr<DEP_PARSER<RET_TYPE2>> parser(
					(DEP_PARSER<RET_TYPE2>*)new DEP_PARSER<RET_TYPE2>
					(sInput, sFeatureInput, sFeatureOutput, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

		std::ifstream input(sInput);
		std::ifstream revInput(sRevInput);
		if (input) {
			while (input >> sentence) {
				revInput >> rsentence;
				rsentence = -rsentence;
				parser->train(sentence, rsentence, ++nRound);
			}
			std::cout << std::endl;
			parser->finishtraining();
		}
		input.close();
	}
	else {
		std::unique_ptr<DEP_PARSER<RET_TYPE1>> parser(
					(DEP_PARSER<RET_TYPE1>*)new DEP_PARSER<RET_TYPE1>
					(sInput, sFeatureInput, sFeatureOutput, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

		std::ifstream input(sInput);
		std::ifstream revInput(sRevInput);
		if (input) {
			while (input >> sentence) {
				revInput >> rsentence;
				rsentence = -rsentence;
				sentence.clearSuperTag();
				parser->train(sentence, rsentence, ++nRound);
			}
			std::cout << std::endl;
			parser->finishtraining();
		}
		input.close();
	}

	std::cout << "Done." << std::endl;
}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void BiGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::parse
(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) {

	DependencyGraph sentence, rsentence;
	DependencyGraph graph;

	std::cout << "Parsing is started..." << std::endl;

	std::string sInput = sInputFile.substr(0, sInputFile.find('#'));
	std::string sRevInput = sInputFile.substr(sInputFile.find('#') + 1);

	if (m_bSuperTagFeature) {
		std::unique_ptr<DEP_PARSER<RET_TYPE2>> parser(
					(DEP_PARSER<RET_TYPE2>*)new DEP_PARSER<RET_TYPE2>
					("", sFeatureFile, sFeatureFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));
		std::ifstream input(sInput);
		std::ifstream revInput(sRevInput);
		std::ofstream output(sOutputFile);

		if (input) {
			while (input >> sentence) {
				revInput >> rsentence;
				if (sentence.size() < MAX_SENTENCE_SIZE) {
					parser->parse(sentence, rsentence, &graph);
					output << graph;
					graph.clear();
				}
			}
		}
		std::cout << std::endl;
		input.close();
		output.close();
	}
	else {
		std::unique_ptr<DEP_PARSER<RET_TYPE1>> parser(
					(DEP_PARSER<RET_TYPE1>*)new DEP_PARSER<RET_TYPE1>
					("", sFeatureFile, sFeatureFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));
		std::ifstream input(sInput);
		std::ifstream revInput(sRevInput);
		std::ofstream output(sOutputFile);

		if (input) {
			while (input >> sentence) {
				revInput >> rsentence;
				if (sentence.size() < MAX_SENTENCE_SIZE) {
					sentence.clearSuperTag();
					parser->parse(sentence, rsentence, &graph);
					output << graph;
					graph.clear();
				}
			}
		}
		std::cout << std::endl;
		input.close();
		output.close();
	}
}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void BiGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::goldtest
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
