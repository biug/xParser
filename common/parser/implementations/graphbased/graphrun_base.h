#ifndef _GRAPHRUN_H
#define _GRAPHRUN_H

#include "graphmacros_base.h"
#include "graphdepparser_base.h"
#include "common/parser/run_base.h"

template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
class GraphRunBase : public RunBase {
protected:
	bool m_bCharFeature;
	bool m_bPathFeature;
	bool m_bSuperTagFeature;

	virtual void initConstant(const std::string & sInputFile = "") const = 0;
public:
	GraphRunBase(const bool & bChar = false, const bool & bPath = false, const bool & bSTag = false);
	virtual ~GraphRunBase() {};

	void train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) const override;
	void parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) const override;
	void goldtest(const std::string & sInputFile, const std::string & sFeatureInput) const override;
};

template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::GraphRunBase
(const bool & bChar, const bool & bPath, const bool & bSuperTag) :
m_bCharFeature(bChar), m_bPathFeature(bPath), m_bSuperTagFeature(bSuperTag) {}

template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::train(const std::string & sInputFile, const std::string & sFeatureInput, const std::string & sFeatureOutput) const {
	int nRound = 0;
	DependencyGraph ref_sent;

	std::cout << "Training is started..." << std::endl;

	std::unique_ptr<GraphDepParserBase<STATE_TYPE>> parser(
			m_bSuperTagFeature ?
			(GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sFeatureInput, sFeatureOutput, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature) :
			(GraphDepParserBase<STATE_TYPE>*)new DEP_PARSER(sFeatureInput, sFeatureOutput, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

	initConstant(sInputFile);

	std::ifstream input(sInputFile);
	if (input) {
		while (input >> ref_sent) {
			if (!m_bSuperTagFeature) {
				clearGraphSuperTag(ref_sent);
			}
			parser->train(ref_sent, ++nRound);
		}
		std::cout << std::endl;
		parser->finishtraining();
	}
	input.close();

	std::cout << "Done." << std::endl;
}

template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::parse(const std::string & sInputFile, const std::string & sOutputFile, const std::string & sFeatureFile) const {

	DependencyGraph sentence;
	DependencyGraph graph;

	std::cout << "Parsing is started..." << std::endl;

	std::unique_ptr<GraphDepParserBase<STATE_TYPE>> parser(
			m_bSuperTagFeature ?
			(GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sFeatureFile, sFeatureFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature) :
			(GraphDepParserBase<STATE_TYPE>*)new DEP_PARSER(sFeatureFile, sFeatureFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));
	std::ifstream input(sInputFile);
	std::ofstream output(sOutputFile);

	initConstant();

	if (input) {
		while (input >> sentence) {
			if (sentence.size() < MAX_SENTENCE_SIZE) {
				if (!m_bSuperTagFeature) {
					clearGraphSuperTag(sentence);
				}
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

template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::goldtest(const std::string & sInputFile, const std::string & sFeatureInput) const {
	int nRound = 0;
	DependencyGraph ref_sent;

	std::cout << "GoldTesting is started..." << std::endl;

	std::unique_ptr<GraphDepParserBase<STATE_TYPE>> parser((GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sFeatureInput, "", ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

	initConstant(sInputFile);

	std::ifstream input(sInputFile);
	if (input) {
		while (input >> ref_sent) {
			++nRound;
			parser->goldCheck(ref_sent);
		}
	}
	input.close();

	std::cout << std::endl << "total " << nRound << " round" << std::endl;

	std::cout << "Done." << std::endl;

}

#endif
