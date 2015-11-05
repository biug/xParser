#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "graph_macros.h"
#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/supertag.h"
#include "common/token/deplabel.h"

int g_nGraphLabelCount;
std::vector<int> g_vecGraphLabelMap;
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

	g_nGraphLabelCount = TDepLabel::count();

	int count = 0;
	g_vecGraphLabelMap.clear();
	g_vecGraphLabelMap.push_back(0);
	std::unordered_map<std::string, int> labelMap;
	for (int i = TDepLabel::START; i <= g_nGraphLabelCount; ++i) {
		const ttoken & label = TDepLabel::key(i);
		if (IS_LEFT_LABEL(label)) {
			if (labelMap.find(DECODE_LEFT_LABEL(label)) == labelMap.end()) {
				labelMap[DECODE_LEFT_LABEL(label)] = ++count;
			}
			g_vecGraphLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_LEFT_LABEL(label)], 0));
		}
		else if (IS_RIGHT_LABEL(label)) {
			if (labelMap.find(DECODE_RIGHT_LABEL(label)) == labelMap.end()) {
				labelMap[DECODE_RIGHT_LABEL(label)] = ++count;
			}
			g_vecGraphLabelMap.push_back(ENCODE_LABEL_ID(0, labelMap[DECODE_RIGHT_LABEL(label)]));
		}
		else if (IS_TWOWAY_LABEL(label)) {
			if (labelMap.find(DECODE_TWOWAY_LEFT_LABEL(label)) == labelMap.end()) {
				labelMap[DECODE_TWOWAY_LEFT_LABEL(label)] = ++count;
			}
			if (labelMap.find(DECODE_TWOWAY_RIGHT_LABEL(label)) == labelMap.end()) {
				labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)] = ++count;
			}
			g_vecGraphLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_TWOWAY_LEFT_LABEL(label)], labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)]));
		}
	}
}

void initTags(const std::string & sInputFile, const std::string & sInputReverseFile) {
	DependencyGraph graph;
	DependencyGraph graphReverse;
	std::ifstream input(sInputFile);
	std::ifstream inputReverse(sInputReverseFile);

	std::unordered_map<int, std::unordered_set<int>> wordsCandidates;
	std::unordered_map<int, std::unordered_set<int>> postagsCandidates;
	if (input) {
		while (input >> graph) {
			inputReverse >> graphReverse;
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
	inputReverse.close();

	g_nGraphLabelCount = TDepLabel::count();

	int count = 0;
	g_vecGraphLabelMap.clear();
	g_vecGraphLabelMap.push_back(0);
	std::unordered_map<std::string, int> labelMap;
	for (int i = TDepLabel::START; i <= g_nGraphLabelCount; ++i) {
		const ttoken & label = TDepLabel::key(i);
		if (IS_LEFT_LABEL(label)) {
			if (labelMap.find(DECODE_LEFT_LABEL(label)) == labelMap.end()) {
				labelMap[DECODE_LEFT_LABEL(label)] = ++count;
			}
			g_vecGraphLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_LEFT_LABEL(label)], 0));
		}
		else if (IS_RIGHT_LABEL(label)) {
			if (labelMap.find(DECODE_RIGHT_LABEL(label)) == labelMap.end()) {
				labelMap[DECODE_RIGHT_LABEL(label)] = ++count;
			}
			g_vecGraphLabelMap.push_back(ENCODE_LABEL_ID(0, labelMap[DECODE_RIGHT_LABEL(label)]));
		}
		else if (IS_TWOWAY_LABEL(label)) {
			if (labelMap.find(DECODE_TWOWAY_LEFT_LABEL(label)) == labelMap.end()) {
				labelMap[DECODE_TWOWAY_LEFT_LABEL(label)] = ++count;
			}
			if (labelMap.find(DECODE_TWOWAY_RIGHT_LABEL(label)) == labelMap.end()) {
				labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)] = ++count;
			}
			g_vecGraphLabelMap.push_back(ENCODE_LABEL_ID(labelMap[DECODE_TWOWAY_LEFT_LABEL(label)], labelMap[DECODE_TWOWAY_RIGHT_LABEL(label)]));
		}
	}
}

void getPlanar(const DependencyGraph & graph, DependencyGraph & partPlanar, DependencyGraph & partAdd) {
	DependencyGraph subGraph = graph;
	std::unordered_map<BiGram<int>, int> arcs;
	for (int i = 0; i < graph.size(); ++i) {
		for (const auto & arc : GRAPHNODE_RIGHTNODES(graph[i])) {
			for (int j = 0; j < graph.size(); ++j) {
				for (const auto & sarc : GRAPHNODE_RIGHTNODES(graph[j])) {
					if (i < j && j < std::get<0>(arc) && std::get<0>(arc) < std::get<0>(sarc)) {
						arcs[BiGram<int>(i, std::get<0>(arc))] += 1;
					}
					else if (j < i && i < std::get<0>(sarc) && std::get<0>(sarc) < std::get<0>(arc)) {
						arcs[BiGram<int>(i, std::get<0>(arc))] += 1;
					}
				}
			}
		}
	}
	for (const auto & arc : arcs) {
		auto & rightNodes = GRAPHNODE_RIGHTNODES(subGraph[arc.first.first()]);
		for (auto itr = rightNodes.begin(); itr != rightNodes.end(); ++itr) {
			if (std::get<0>(*itr) == arc.first.second()) {
				rightNodes.erase(itr);
				break;
			}
		}
	}
	partPlanar = graph;
	std::unordered_set<BiGram<int>> removed;
	while (true) {
		arcs.clear();
		for (int i = 0; i < partPlanar.size(); ++i) {
			for (const auto & arc : GRAPHNODE_RIGHTNODES(partPlanar[i])) {
				for (int j = 0; j < partPlanar.size(); ++j) {
					for (const auto & sarc : GRAPHNODE_RIGHTNODES(partPlanar[j])) {
						if (i < j && j < std::get<0>(arc) && std::get<0>(arc) < std::get<0>(sarc)) {
							arcs[BiGram<int>(i, std::get<0>(arc))] += 1;
						}
						else if (j < i && i < std::get<0>(sarc) && std::get<0>(sarc) < std::get<0>(arc)) {
							arcs[BiGram<int>(i, std::get<0>(arc))] += 1;
						}
					}
				}
			}
		}
		if (arcs.size() == 0) {
			break;
		}
		int maxCount = arcs.begin()->second;
		BiGram<int> maxArc(arcs.begin()->first);
		for (const auto & arc : arcs) {
			if (arc.second > maxCount) {
				maxCount = arc.second;
				maxArc = arc.first;
			}
			else if (arc.second == maxCount) {
				for (auto & ra : removed) {
					if (!(ra.first() < arc.first.first() && arc.first.first() < ra.second() && ra.second() < arc.first.second()) &&
						!(arc.first.first() < ra.first() && ra.first() < arc.first.second() && arc.first.second() < ra.second())) {
						maxArc = arc.first;
					}
				}
			}
		}
		removed.insert(maxArc);
		auto & deleteNode = GRAPHNODE_RIGHTNODES(partPlanar[maxArc.first()]);
		for (auto itr = deleteNode.begin(); itr != deleteNode.end(); ++itr) {
			if (std::get<0>(*itr) == maxArc.second()) {
				deleteNode.erase(itr);
				break;
			}
		}
	}
	partAdd = partPlanar;
	eraseGraph(subGraph, partAdd);
}

void eraseGraph(const DependencyGraph & oracle, DependencyGraph & deleted) {
	for (int i = 0, n = oracle.size(); i < n; ++i) {
		for (const auto & arc : GRAPHNODE_RIGHTNODES(oracle[i])) {
			auto & rightNodes = GRAPHNODE_RIGHTNODES(deleted[i]);
			for (auto itr = rightNodes.begin(); itr != rightNodes.end(); ++itr) {
				if (std::get<0>(*itr) == std::get<0>(arc)) {
					rightNodes.erase(itr);
					break;
				}
			}
		}
	}
}

void eraseReverseGraph(const DependencyGraph & oracle, DependencyGraph & reverseGraph) {
	for (int i = 0, n = oracle.size(); i < n; ++i) {
		for (const auto & arc : GRAPHNODE_RIGHTNODES(oracle[i])) {
			auto & rightNodes = GRAPHNODE_RIGHTNODES(reverseGraph[n - std::get<0>(arc) - 1]);
			for (auto itr = rightNodes.begin(); itr != rightNodes.end(); ++itr) {
				if (std::get<0>(*itr) == n - i - 1) {
					rightNodes.erase(itr);
					break;
				}
			}
		}
	}
}

void combineGraph(const DependencyGraph & oracle, const DependencyGraph & reverseOracle, DependencyGraph & combined) {
	combined.clear();
	for (const auto & node : oracle) {
		combined.push_back(node);
	}
	std::unordered_set<TriGram<int>> arcs;
	// add reverse arcs
	for (int i = 0; i < oracle.size(); ++i) {
		GRAPHNODE_RIGHTNODES(combined[i]).clear();
//		for (const auto & arc : GRAPHNODE_RIGHTNODES(oracle[i])) {
//			arcs.insert(TriGram<int>(i, std::get<0>(arc), std::get<1>(arc)));
//		}
		for (const auto & arc : GRAPHNODE_RIGHTNODES(reverseOracle[i])) {
			int label = g_vecGraphLabelMap[std::get<1>(arc)];
			int rightLabel = LEFT_LABEL_ID(label);
			int leftLabel = RIGHT_LABEL_ID(label);
			label = ENCODE_LABEL_ID(leftLabel, rightLabel);
			int lMap = 0;
			for (int j = 0; j < g_vecGraphLabelMap.size(); ++j) {
				if (g_vecGraphLabelMap[j] == label) {
					lMap = j;
					break;
				}
			}
			arcs.insert(TriGram<int>(oracle.size() - std::get<0>(arc) - 1, oracle.size() - i - 1, lMap));
		}
	}
	// add cross normal arcs
	for (int i = 0; i < oracle.size(); ++i) {
		GRAPHNODE_RIGHTNODES(combined[i]).clear();
		for (const auto & arc : GRAPHNODE_RIGHTNODES(oracle[i])) {
			bool findCross = false;
			int l = std::get<0>(arc), r = std::get<0>(arc);
			for (const auto & a : arcs) {
				if (a.first() < l && l < a.second() && a.second() < r) {
					findCross = true;
					break;
				}
				else if (l < a.first() && a.first() < r && r < a.second()) {
					findCross = true;
					break;
				}
			}
			if (findCross) {
				arcs.insert(TriGram<int>(i, std::get<0>(arc), std::get<1>(arc)));
			}
		}
	}
	std::vector<TriGram<int>> arcsVec;
	for (const auto & arc : arcs) {
		arcsVec.push_back(arc);
	}
	std::sort(arcsVec.begin(), arcsVec.end(), [](const TriGram<int>& t1, const TriGram<int>& t2){ return t1.second() < t2.second(); });
	for (const auto & arc : arcsVec) {
		GRAPHNODE_RIGHTNODES(combined[arc.first()]).push_back(RightNodeWithLabel(arc.second(), arc.third()));
	}
}

std::istream & operator>>(std::istream & input, DependencyGraph & graph) {
	graph.clear();
	ttoken line, token;
	std::vector<int> heads;
	std::map<int, int> heads_inverse;
	std::vector<std::vector<std::string>> lines;
	while (true) {
		std::getline(input, line);
		if (line.empty()) {
			break;
		}
		if (line[0] == '#') {
			continue;
		}
		DependencyGraphNode node;
		std::istringstream iss(line);
		std::vector<std::string> tokens;
		iss >> token >> GRAPHNODE_WORD(node) >> token >> GRAPHNODE_POSTAG(node) >> token >> token >> token;
		iss >> GRAPHNODE_SUPERTAG(node) >> GRAPHNODE_TREEHEAD(node) >> token >> token;
		if (token != "_") {
			heads_inverse[lines.size()] = heads.size();
			heads.push_back(lines.size());
		}
		while (iss >> token) {
			tokens.push_back(token);
		}
		lines.push_back(tokens);
		graph.push_back(node);
	}
	if (lines.size() == 0) {
		return input;
	}

	heads_inverse[lines.size()] = heads.size();
	heads.push_back(lines.size());
	std::vector<std::tuple<int, int, int, std::string>> vecArcs;
	for (int i = 0; i < lines.size(); ++i) {
		DependencyGraphNode & node = graph[i];
		const std::vector<std::string> & tokens = lines[i];
		for (int j = 0; j < tokens.size(); ++j) {
			if (tokens[j] != "_" && heads[j] != i) {
				vecArcs.push_back(std::tuple<int, int, int, std::string>(std::min(i, heads[j]), std::max(i, heads[j]), i < heads[j] ? GRAPH_LEFT : GRAPH_RIGHT, tokens[j]));
			}
		}
	}
	std::sort(vecArcs.begin(), vecArcs.end(),
			[](const std::tuple<int, int, int, std::string> & a1, const std::tuple<int, int, int, std::string> & a2) {
				return std::get<0>(a1) != std::get<0>(a2) ?
						std::get<0>(a1) < std::get<0>(a2) :
						(std::get<1>(a1) != std::get<1>(a2) ?
							std::get<1>(a1) < std::get<1>(a2) :
							std::get<2>(a1) < std::get<2>(a2));
			});
	for (int i = 0; i < vecArcs.size(); ++i) {
		DependencyGraphNode & node = graph[std::get<0>(vecArcs[i])];
		if (i + 1 < vecArcs.size() && std::get<0>(vecArcs[i]) == std::get<0>(vecArcs[i + 1]) && std::get<1>(vecArcs[i]) == std::get<1>(vecArcs[i + 1])) {
			GRAPHNODE_RIGHTNODES(node).push_back(
					RightNodeWithLabel(
							std::get<1>(vecArcs[i]),
							TDepLabel::code(ENCODE_TWOWAY_LABEL(std::get<3>(vecArcs[i]), std::get<3>(vecArcs[i + 1])))));
			++i;
		}
		else {
			GRAPHNODE_RIGHTNODES(node).push_back(
					RightNodeWithLabel(
							std::get<1>(vecArcs[i]),
							TDepLabel::code((std::get<2>(vecArcs[i]) == GRAPH_LEFT ? ENCODE_LEFT_LABEL(std::get<3>(vecArcs[i])) : ENCODE_RIGHT_LABEL(std::get<3>(vecArcs[i]))))));
		}
	}
	return input;
}

std::ostream & operator<<(std::ostream & output, const DependencyGraph & graph) {
	std::set<int> heads_set;
	std::vector<std::vector<std::tuple<int, std::string>>> heads_for_nodes;
	int i = 0;
	for (const auto & node : graph) {
		heads_for_nodes.push_back(std::vector<std::tuple<int, std::string>>());
		for (const auto & rn : GRAPHNODE_RIGHTNODES(node)) {
			const std::string & label = TDepLabel::key(RIGHTNODE_LABEL(rn));
			if (IS_LEFT_LABEL(label)) {
				heads_set.insert(RIGHTNODE_POS(rn));
			}
			else if (IS_RIGHT_LABEL(label)) {
				heads_set.insert(i);
			}
			else if (IS_TWOWAY_LABEL(label)) {
				heads_set.insert(RIGHTNODE_POS(rn));
				heads_set.insert(i);
			}
		}
		++i;
	}
	i = 0;
	std::map<int, int> heads_inverse;
	for (const auto & head : heads_set) {
		heads_inverse[head] = i++;
	}
	i = 0;
	for (const auto & node : graph) {
		for (const auto & rn : GRAPHNODE_RIGHTNODES(node)) {
			const std::string & label = TDepLabel::key(RIGHTNODE_LABEL(rn));
			if (IS_LEFT_LABEL(label)) {
				heads_for_nodes[i].push_back(std::tuple<int, std::string>(heads_inverse[RIGHTNODE_POS(rn)], DECODE_LEFT_LABEL(label)));
			}
			else if (IS_RIGHT_LABEL(label)) {
				heads_for_nodes[RIGHTNODE_POS(rn)].push_back(std::tuple<int, std::string>(heads_inverse[i], DECODE_RIGHT_LABEL(label)));
			}
			else if (IS_TWOWAY_LABEL(label)) {
				// left label
				heads_for_nodes[i].push_back(std::tuple<int, std::string>(heads_inverse[RIGHTNODE_POS(rn)], DECODE_TWOWAY_LEFT_LABEL(label)));
				// right label
				heads_for_nodes[RIGHTNODE_POS(rn)].push_back(std::tuple<int, std::string>(heads_inverse[i], DECODE_TWOWAY_RIGHT_LABEL(label)));
			}
		}
		++i;
	}
	i = 0;
	for (const auto & node : graph) {
		output << (i + 1) << " " << GRAPHNODE_WORD(node) << " " << GRAPHNODE_WORD(node) << " " << GRAPHNODE_POSTAG(node) << " " << GRAPHNODE_POSTAG(node) << " _ _ " << GRAPHNODE_SUPERTAG(node) << " _ _ ";
		output << (heads_set.find(i) != heads_set.end() ? GRAPHNODE_WORD(node) : "_");
		for (int j = 0, k = 0; j < heads_set.size(); ++j) {
			if (k < heads_for_nodes[i].size() && std::get<0>(heads_for_nodes[i][k]) == j) {
				output << " " << std::get<1>(heads_for_nodes[i][k++]);
			}
			else {
				output << " " << "_";
			}
		}
		output << std::endl;
		++i;
	}
	output << std::endl;
	return output;
}

std::istream & operator>>(std::istream & input, SuperTagCandidates & stc) {
	int size, key, candidate;
	ttoken line;
	input >> size;
	std::getline(input, line);
	while (size--) {
		std::getline(input, line);
		std::istringstream iss(line);
		iss >> key;
		while (iss >> candidate) {
			stc[key].push_back(candidate);
		}
	}
	return input;
}

std::ostream & operator<<(std::ostream & output, const SuperTagCandidates & stc) {
	output << stc.size() << std::endl;
	for (const auto & item : stc) {
		output << item.first;
		for (const auto & candi : item.second) {
			output << " " << candi;
		}
		output << std::endl;
	}
	return output;
}
