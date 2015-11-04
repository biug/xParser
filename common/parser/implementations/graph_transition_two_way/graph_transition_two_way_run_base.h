#ifndef _GRAPH_TRANSITION_TWO_WAY_RUN_H
#define _GRAPH_TRANSITION_TWO_WAY_RUN_H

#include <unordered_set>

#include "common/parser/implementations/graph_macros.h"
#include "graph_transition_two_way_depparser_base.h"
#include "common/parser/run_base.h"

namespace graph_transition_two_way {
	template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
	class GraphRunBase : public RunBase {
	protected:
		bool m_bCharFeature;
		bool m_bPathFeature;
		bool m_bSuperTagFeature;

		virtual void initAction() const = 0;
		void initConstant(const std::string & sInputFile = "", const std::string & sInputReverseFile = "") const;
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
	void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::initConstant(const std::string & sInputFile, const std::string & sInputReverseFile) const {
		initTags(sInputFile, sInputReverseFile);
		initAction();

		std::cout << "constant load complete." << std::endl;
		std::cout << "label count is " << g_nGraphLabelCount << std::endl;

		std::cout << (m_bCharFeature ? "use char" : "without char") << std::endl;
		std::cout << (m_bPathFeature ? "use path" : "without path") << std::endl;
		std::cout << (m_bSuperTagFeature ? "use supertag" : "without supertag") << std::endl;
	}

	template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
	void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::train(const std::string & sInput, const std::string & sFeatureInput, const std::string & sFeatureOutput) const {
		int nRound = 0;
		int nReverseRound = 0;
		DependencyGraph ref_sent;
		DependencyGraph ref_reverse_sent;

		std::cout << "Training is started..." << std::endl;

		std::string sFeatureInputFile = sFeatureInput.substr(0, sFeatureInput.find("#"));
		std::string sReverseFeatureInputFile = sFeatureInput.substr(sFeatureInput.find("#") + 1);
		std::string sFeatureOutputFile = sFeatureOutput.substr(0, sFeatureOutput.find("#"));
		std::string sReverseFeatureOutputFile = sFeatureOutput.substr(sFeatureOutput.find("#") + 1);
		std::unique_ptr<GraphDepParserBase<STATE_TYPE>> parser(
				m_bSuperTagFeature ?
				(GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sFeatureInputFile, sFeatureOutputFile, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature) :
				(GraphDepParserBase<STATE_TYPE>*)new DEP_PARSER(sFeatureInputFile, sFeatureOutputFile, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));
		std::unique_ptr<GraphDepParserBase<STATE_TYPE>> reverse_parser(
				m_bSuperTagFeature ?
				(GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sReverseFeatureInputFile, sReverseFeatureOutputFile, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature) :
				(GraphDepParserBase<STATE_TYPE>*)new DEP_PARSER(sReverseFeatureInputFile, sReverseFeatureOutputFile, ParserState::TRAIN, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

		std::string sInputFile = sInput.substr(0, sInput.find("#"));
		std::string sInputReverseFile = sInput.substr(sInput.find("#") + 1);
		initConstant(sInputFile, sInputReverseFile);

		std::ifstream input(sInputFile);
		std::ifstream inputReverse(sInputReverseFile);
		if (input) {
			while (input >> ref_sent) {
				inputReverse >> ref_reverse_sent;
				if (!m_bSuperTagFeature) {
					clearGraphSuperTag(ref_sent);
					clearGraphSuperTag(ref_reverse_sent);
				}
				int last_error = parser->totalError();
				int last_reverse_error = reverse_parser->totalError();
				parser->train(ref_sent, ++nRound);
				reverse_parser->train(ref_reverse_sent, ++nReverseRound);
				if (nRound > 1) {
					nBackSpace("normal error rate 0.0000 ( " + std::to_string(last_error) + " / " + std::to_string(nRound - 1) + " ) ");
					nBackSpace("reverse error rate 0.0000 ( " + std::to_string(last_reverse_error) + " / " + std::to_string(nReverseRound - 1) + " ) ");
				}
				std::cout << "normal error rate " << ((double)parser->totalError() / (double)nRound);
				std::cout << " ( " << parser->totalError() << " / " << nRound << " ) ";
				std::cout << "reverse error rate " << ((double)reverse_parser->totalError() / (double)nReverseRound);
				std::cout << " ( " << reverse_parser->totalError() << " / " << nReverseRound << " ) " << std::flush;
			}
			std::cout << std::endl;
			parser->finishtraining();
			reverse_parser->finishtraining();
		}
		input.close();
		inputReverse.close();

		std::cout << "Done." << std::endl;
	}

	template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
	void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::parse(const std::string & sInput, const std::string & sOutputFile, const std::string & sFeatureInput) const {

		int nRound = 0;
		DependencyGraph sentence;
		DependencyGraph reverseSentence;
		DependencyGraph graph;
		DependencyGraph reverseGraph;
		DependencyGraph combined;

		std::cout << "Parsing is started..." << std::endl;

		std::string sFeatureInputFile = sFeatureInput.substr(0, sFeatureInput.find("#"));
		std::string sReverseFeatureInputFile = sFeatureInput.substr(sFeatureInput.find("#") + 1);
		std::unique_ptr<GraphDepParserBase<STATE_TYPE>> parser(
				m_bSuperTagFeature ?
				(GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sFeatureInputFile, sFeatureInputFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature) :
				(GraphDepParserBase<STATE_TYPE>*)new DEP_PARSER(sFeatureInputFile, sFeatureInputFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));
		std::unique_ptr<GraphDepParserBase<STATE_TYPE>> reverse_parser(
				m_bSuperTagFeature ?
				(GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sReverseFeatureInputFile, sReverseFeatureInputFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature) :
				(GraphDepParserBase<STATE_TYPE>*)new DEP_PARSER(sReverseFeatureInputFile, sReverseFeatureInputFile, ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

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
						clearGraphSuperTag(reverseSentence);
					}
					parser->parse(sentence, &graph);
					reverse_parser->parse(reverseSentence, &reverseGraph);
					combineGraph(graph, reverseGraph, combined);
					// should combine graph and reverseGraph
					output << combined;
					graph.clear();
					reverseGraph.clear();
					if (nRound > 0) {
						nBackSpace("parsing sentence " + std::to_string(nRound));
					}
					std::cout << "parsing sentence " << ++nRound << std::flush;
				}
			}
		}
		std::cout << std::endl;
		input.close();
		output.close();
	}

	template<class DEP_PARSER, class SUPERTAG_DEP_PARSER, class STATE_TYPE>
	void GraphRunBase<DEP_PARSER, SUPERTAG_DEP_PARSER, STATE_TYPE>::goldtest(const std::string & sInput, const std::string & sFeatureInput) const {
		int nRound = 0;
		int nError = 0;
		DependencyGraph combined;
		DependencyGraph ref_sent, ref_output;
		DependencyGraph ref_reverse_sent, ref_reverse_output;

		std::cout << "GoldTesting is started..." << std::endl;

		std::unique_ptr<GraphDepParserBase<STATE_TYPE>> parser((GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sFeatureInput, "", ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));
		std::unique_ptr<GraphDepParserBase<STATE_TYPE>> reverse_parser((GraphDepParserBase<STATE_TYPE>*)new SUPERTAG_DEP_PARSER(sFeatureInput, "", ParserState::PARSE, m_bCharFeature, m_bPathFeature, m_bSuperTagFeature));

		std::string sInputFile = sInput.substr(0, sInput.find("#"));
		std::string sInputReverseFile = sInput.substr(sInput.find("#") + 1);
		std::cout << sInputFile << std::endl;
		std::cout << sInputReverseFile << std::endl;
		initConstant(sInputFile, sInputReverseFile);

		std::ifstream input(sInputFile);
		std::ifstream inputReverse(sInputReverseFile);
		if (input) {
			while (input >> ref_sent) {
				inputReverse >> ref_reverse_sent;
				++nRound;
				parser->goldCheck(ref_sent, ref_output);
				reverse_parser->goldCheck(ref_reverse_sent, ref_reverse_output);
				combineGraph(ref_output, ref_reverse_output, combined);
				for (int i = 0; i < ref_sent.size(); ++i) {
					if (GRAPHNODE_RIGHTNODES(ref_sent[i]) != GRAPHNODE_RIGHTNODES(combined[i])) {
						++nError;
						break;
					}
				}
			}
		}
		input.close();
		inputReverse.close();

		std::cout << std::endl << "total " << nRound << " round and " << nError << std::endl;

		std::cout << "Done." << std::endl;

	}
}

#endif
