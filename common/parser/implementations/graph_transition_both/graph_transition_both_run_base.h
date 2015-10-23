#ifndef _GRAPH_TRANSITION_BOTH_RUN_H
#define _GRAPH_TRANSITION_BOTH_RUN_H

#include "common/parser/implementations/graph_macros.h"
#include "graph_transition_both_depparser_base.h"
#include "common/parser/run_base.h"

namespace graph_transition_both {
	template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
	class GraphRunBase : public RunBase {
	protected:
		bool m_bCharFeature;
		bool m_bPathFeature;
		bool m_bSuperTagFeature;

		virtual void initAction() const = 0;
		void initConstant(const std::string & sInputFile = "") const;
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
	void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::initConstant(const std::string & sInputFile) const {
		initTags(sInputFile);
		initAction();

		std::cout << "constant load complete." << std::endl;

		std::cout << (m_bCharFeature ? "use char" : "without char") << std::endl;
		std::cout << (m_bPathFeature ? "use path" : "without path") << std::endl;
		std::cout << (m_bSuperTagFeature ? "use supertag" : "without supertag") << std::endl;
	}

	template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
	void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::train(const std::string & sInput, const std::string & sFeatureInput, const std::string & sFeatureOutput) const {
		int nRound = 0;
		DependencyGraph ref_sent;
		DependencyGraph ref_reverse_sent;

		std::cout << "Training is started..." << std::endl;

		std::unique_ptr<GraphDepParserBase<STATE_TYPE>> parser(
				m_bSuperTagFeature ?
				(GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sFeatureInput, sFeatureOutput, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature) :
				(GraphDepParserBase<STATE_TYPE>*)new DEP_PARSER(sFeatureInput, sFeatureOutput, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

		std::string sInputFile = sInput.substr(0, sInput.find("#"));
		std::string sInputReverseFile = sInput.substr(sInput.find("#") + 1);
		initConstant(sInputFile);

		std::ifstream input(sInputFile);
		std::ifstream inputReverse(sInputReverseFile);
		if (input) {
			while (input >> ref_sent) {
				inputReverse >> ref_reverse_sent;
				if (!m_bSuperTagFeature) {
					clearGraphSuperTag(ref_sent);
				}
				parser->train(ref_sent, ref_reverse_sent, ++nRound);
			}
			std::cout << std::endl;
			parser->finishtraining();
		}
		input.close();
		inputReverse.close();

		std::cout << "Done." << std::endl;
	}

	template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
	void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::parse(const std::string & sInput, const std::string & sOutputFile, const std::string & sFeatureFile) const {

		DependencyGraph sentence;
		DependencyGraph reverseSentence;
		DependencyGraph graph;

		std::cout << "Parsing is started..." << std::endl;

		std::unique_ptr<GraphDepParserBase<STATE_TYPE>> parser(
				m_bSuperTagFeature ?
				(GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sFeatureFile, sFeatureFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature) :
				(GraphDepParserBase<STATE_TYPE>*)new DEP_PARSER(sFeatureFile, sFeatureFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));
		std::string sInputFile = sInput.substr(0, sInput.find("#"));
		std::string sInputReverseFile = sInput.substr(sInput.find("#") + 1);
		std::ifstream input(sInputFile);
		std::ifstream inputReverse(sInputReverseFile);
		std::ofstream output(sOutputFile);

		initConstant();

		if (input) {
			while (input >> sentence) {
				inputReverse >> reverseSentence;
				if (sentence.size() < MAX_SENTENCE_SIZE) {
					if (!m_bSuperTagFeature) {
						clearGraphSuperTag(sentence);
					}
					parser->parse(sentence, reverseSentence, &graph);
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
}

#endif
