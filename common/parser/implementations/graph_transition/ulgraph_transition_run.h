#ifndef _ULGRAPH_TRANSITION_RUN_H
#define _ULGRAPH_TRANSITION_RUN_H

#include "graph_transition_macros.h"

#include <memory>

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
class ULGraphTransitionRun : public RunBase {
protected:
	bool m_bCharFeature;
	bool m_bPathFeature;

	void clearGraph(DependencyGraph & graph);

public:
	ULGraphTransitionRun(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
	virtual ~ULGraphTransitionRun() {};

	void train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) override;
	void parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) override;
	void goldtest(const std::string & sInputFile, const std::string & sFeatureInput) override;
};

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
ULGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::ULGraphTransitionRun
(const bool & bChar, const bool & bPath, const bool & bSuperTag) : m_bCharFeature(bChar), m_bPathFeature(bPath) {}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void ULGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::clearGraph(DependencyGraph & graph) {
	graph.clearSuperTag();
	for (auto & node : graph) {
		node.m_vecRightLabels.clear();
		for (auto & arc : node.m_vecRightArcs) {
			if (IS_LEFT_LABEL(arc.second)) {
				arc.second = "left1";
				node.m_vecRightLabels.push_back(std::pair<int, std::pair<int, int>>(ULA_LEFT, {1, 0}));
			}
			else if (IS_RIGHT_LABEL(arc.second)) {
				arc.second = "right1";
				node.m_vecRightLabels.push_back(std::pair<int, std::pair<int, int>>(ULA_RIGHT, {0, 1}));
			}
			else {
				arc.second = "twoway1";
				node.m_vecRightLabels.push_back(std::pair<int, std::pair<int, int>>(ULA_TWOWAY, {1, 1}));
			}
		}
	}
}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void ULGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::train
(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) {
	int nRound = 0;
	DependencyGraph sentence;

	std::cout << "Training is started..." << std::endl;

	std::unique_ptr<DEP_PARSER<RET_TYPE1>> parser(
				(DEP_PARSER<RET_TYPE1>*)new DEP_PARSER<RET_TYPE1>
				(sInputFile, sFeatureInput, sFeatureOutput, ParserState::TRAIN, m_bCharFeature, m_bPathFeature));

	std::ifstream input(sInputFile);
	if (input) {
		while (input >> sentence) {
			clearGraph(sentence);
			parser->train(sentence, ++nRound);
		}
		std::cout << std::endl;
		parser->finishtraining();
	}
	input.close();

	std::cout << "Done." << std::endl;
}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void ULGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::parse
(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) {

	DependencyGraph sentence;
	DependencyGraph graph;

	std::cout << "Parsing is started..." << std::endl;

	std::unique_ptr<DEP_PARSER<RET_TYPE1>> parser(
				(DEP_PARSER<RET_TYPE1>*)new DEP_PARSER<RET_TYPE1>
				("", sFeatureFile, sFeatureFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature));
	std::ifstream input(sInputFile);
	std::ofstream output(sOutputFile);

	if (input) {
		while (input >> sentence) {
			if (sentence.size() < MAX_SENTENCE_SIZE) {
				clearGraph(sentence);
				parser->parse(sentence, &graph);
				output << graph;
				graph.clear();
			}
		}
	}
	std::cout << std::endl;
	input.close();
	output.close();
}

template<template<typename RET_TYPE> class DEP_PARSER, class RET_TYPE1, class RET_TYPE2, class STATE_TYPE, class ACTION_TYPE>
void ULGraphTransitionRun<DEP_PARSER, RET_TYPE1, RET_TYPE2, STATE_TYPE, ACTION_TYPE>::goldtest
(const std::string & sInputFile, const std::string & sFeatureInput) {
	int nRound = 0;
	DependencyGraph sentence;

	std::cout << "GoldTesting is started..." << std::endl;

	std::unique_ptr<DEP_PARSER<RET_TYPE1>> parser(
			(DEP_PARSER<RET_TYPE1>*)new DEP_PARSER<RET_TYPE1>
			(sInputFile, sFeatureInput, sFeatureInput, ParserState::PARSE,
			m_bCharFeature, m_bPathFeature));

	std::ifstream input(sInputFile);
	if (input) {
		while (input >> sentence) {
			++nRound;
			clearGraph(sentence);
			parser->goldCheck(sentence);
		}
	}
	input.close();

	std::cout << std::endl << "total " << nRound << " round" << std::endl;

	std::cout << "Done." << std::endl;

}

#endif
