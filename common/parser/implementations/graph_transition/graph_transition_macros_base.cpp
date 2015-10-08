#include <string>
#include <cstring>
#include <fstream>
#include <unordered_set>
#include <unordered_map>

#include "common/token/pos.h"
#include "common/token/word.h"
#include "graph_transition_macros_base.h"

int LABEL_COUNT;
std::vector<int> g_vecLabelMap;
SuperTagCandidates g_mapSuperTagCandidatesOfWords;
SuperTagCandidates g_mapSuperTagCandidatesOfPOSTags;

void initTags(const std::string & sInputFile) {
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

	LABEL_COUNT = TDepLabel::count();

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
}
