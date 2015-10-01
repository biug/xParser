#include <ctime>
#include <memory>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

#include "std_titov_run.h"
#include "std_titov_depparser.h"
#include "common/token/deplabel.h"

namespace std_titov {

	extern int LABEL_COUNT;

	extern int A_FIRST;
	extern int SH_FIRST;

	extern int A_END;
	extern int SH_END;

	extern std::vector<int> g_vecLabelMap;
	extern SuperTagCandidates g_mapSuperTagCandidatesOfWords;
	extern SuperTagCandidates g_mapSuperTagCandidatesOfPOSTags;

	Run::Run(const bool & bChar, const bool & bPath, const bool & bSuperTag) :
		GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem>(bChar, bPath, bSuperTag) {}

	Run::~Run() = default;

	void Run::initConstant(const std::string & sInputFile) const {
		DependencyGraph graph;
		std::ifstream input(sInputFile);

		std::unordered_map<int, std::unordered_set<int>> wordsCandidates;
		std::unordered_map<int, std::unordered_set<int>> postagsCandidates;
		if (input) {
			while (input >> graph) {
				for (const auto & node : graph) {
					int w = TWord::code(GRAPHNODE_WORD(node));
					int p = TPOSTag::code(GRAPHNODE_POSTAG(node));
					int t = GRAPHNODE_SUPERTAG(node) == "_" ? 0 : TSuperTag::code(GRAPHNODE_SUPERTAG(node));
					if (t != 0) {
						wordsCandidates[w].insert(t);
						postagsCandidates[p].insert(t);
					}
				}
			}
			g_mapSuperTagCandidatesOfWords.clear();
			for (const auto & word_set : wordsCandidates) {
				for (const auto & t : word_set.second) {
					g_mapSuperTagCandidatesOfWords[word_set.first].push_back(t);
				}
			}
			g_mapSuperTagCandidatesOfPOSTags.clear();
			for (const auto & postag_set : postagsCandidates) {
				for (const auto & t : postag_set.second) {
					g_mapSuperTagCandidatesOfPOSTags[postag_set.first].push_back(t);
				}
			}
		}
		input.close();
		std::cout << "macros load complete." << std::endl;

		LABEL_COUNT = TDepLabel::count();

		A_FIRST = REDUCE + 1;
		A_END = SH_FIRST = A_FIRST + LABEL_COUNT;
		SH_END = m_bSuperTagFeature ? SH_FIRST + TSuperTag::count() + 1 : SH_FIRST + 1;

		int count = 0;
		g_vecLabelMap.clear();
		g_vecLabelMap.push_back(0);
		std::unordered_map<std::string, int> labelMap;
		for (int i = TDepLabel::START; i <= LABEL_COUNT; ++i) {
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

		std::cout << (m_bCharFeature ? "use char" : "without char") << std::endl;
		std::cout << (m_bPathFeature ? "use path" : "without path") << std::endl;
		std::cout << (m_bSuperTagFeature ? "use supertag" : "without supertag") << std::endl;
	}

}
