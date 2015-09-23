#include <map>
#include <set>
#include <stack>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "macros_base.h"

int encodeLinkDistanceOrDirection(const int & hi, const int & di, bool dir) {
	int diff = hi - di;
	if (dir) {
		return diff > 0 ? 7 : -7;
	}
	if (diff < 0) {
		diff = -diff;
	}
	if (diff > 10) {
		diff = 6;
	}
	else if (diff > 5) {
		diff = 5;
	}
	if (hi < di) {
		diff = -diff;
	}
	return diff;
}

void clearGraphLabel(DependencyGraph & graph) {
	for (auto & node : graph) for (auto & rn : GRAPHNODE_RIGHTNODES(node)) RIGHTNODE_LABEL(rn) = 1;
}

void clearCONLLGraphLabel(DependencyCONLLGraph & graph) {
	for (auto & node : graph) for (auto & rn : CONLLGRAPHNODE_RIGHTNODES(node)) RIGHTNODE_LABEL(rn) = 1;
}

void addSyntaxTree(std::ifstream & input, DependencyGraph & graph) {
	int i = 0;
	std::string line;
	while (true) {
		std::getline(input, line);
		if (line.empty()) {
			break;
		}
		if (line.find("#") == 0 && line.find("\t") == std::string::npos) {
			continue;
		}
		std::string token;
		std::istringstream iss(line);
		iss >> token >> GRAPHNODE_TREEHEAD(graph[i]) >> GRAPHNODE_TREELABEL(graph[i]);
		if (GRAPHNODE_TREEHEAD(graph[i]) == 0) {
			GRAPHNODE_TREEHEAD(graph[i]) = graph.size() - 1;
		}
		else {
			--GRAPHNODE_TREEHEAD(graph[i]);
		}
#ifdef _DEBUG
		std::cout << GRAPHNODE_TREEHEAD(graph[i]) << " -> " << i << std::endl;
#endif
		++i;
	}
	GRAPHNODE_TREEHEAD(graph.back()) = -1;
	GRAPHNODE_TREELABEL(graph.back()) = ROOT_DEPLABEL;
}

std::string nCharPrev(const Sentence & sent, int index, int n) {
	std::string str = "";
	--index;
	while (index >= 0 && n > 0) {
		const std::string & word = SENT_WORD(sent[index]);
		int i = word.length() - 1;
		while (i >= 0) {
			const unsigned char & c = word[i];
			if ((c >> 6) != 2) {
				--n;
				if (n == 0) {
					break;
				}
			}
			--i;
		}
		str = word.substr(i == -1 ? 0 : i) + str;
		--index;
	}
	while (n--) {
		str = "(N)" + str;
	}
	return str;
}

std::string nCharNext(const Sentence & sent, int index, int n) {
	std::string str = "";
	++index;
	while (index < sent.size() && n > 0) {
		const std::string & word = SENT_WORD(sent[index]);
		int i = 1;
		while (i < word.length()) {
			const unsigned char & c = word[i];
			if ((c >> 6) != 2) {
				--n;
				if (n == 0) {
					break;
				}
			}
			++i;
		}
		if (i == word.length()) {
			--n;
		}
		str = str + word.substr(0, i);
		++index;
	}
	while (n--) {
		str = str + "(N)";
	}
	return str;
}

std::istream & operator>>(std::istream & input, Sentence & sentence) {
	sentence.clear();
	ttoken line, token;
	std::getline(input, line);
	std::istringstream iss(line);
	while (iss >> token) {
		int i = token.rfind(SENT_SPTOKEN);
		sentence.push_back(POSTaggedWord(token.substr(0, i), token.substr(i + 1)));
	}
	return input;
}

std::istream & operator>>(std::istream & input, DependencyTree & tree) {
	tree.clear();
	ttoken line, token;
	while (true) {
		std::getline(input, line);
		if (line.empty()) {
			break;
		}
		DependencyTreeNode node;
		std::istringstream iss(line);
		iss >> TREENODE_WORD(node) >> TREENODE_POSTAG(node) >> TREENODE_HEAD(node) >> TREENODE_LABEL(node);
		tree.push_back(node);
	}
	return input;
}

std::istream & operator>>(std::istream & input, DependencyGraph & graph) {
	graph.clear();
	ttoken line, token;
	int root = -1;
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
		iss >> token >> GRAPHNODE_WORD(node) >> token >> GRAPHNODE_POSTAG(node);
		iss >> token;
		if (token == "+") {
			root = lines.size();
		}
		iss >> token;
		if (token == "+") {
			heads_inverse[lines.size()] = heads.size();
			heads.push_back(lines.size());
		}
		while (iss >> token) {
			tokens.push_back(token);
		}
		if (root == lines.size()) {
			tokens.push_back(ROOT_DEPLABEL);
		}
		else {
			tokens.push_back("_");
		}
		lines.push_back(tokens);
		graph.push_back(node);
	}
	if (lines.size() == 0) {
		return input;
	}
	// additional root node
	DependencyGraphNode node;
	GRAPHNODE_WORD(node) = ROOT_WORD;
	GRAPHNODE_POSTAG(node) = ROOT_POSTAG;
	graph.push_back(node);

	heads_inverse[lines.size()] = heads.size();
	heads.push_back(lines.size());
	for (int i = 0; i < lines.size(); ++i) {
		DependencyGraphNode & node = graph[i];
		const std::vector<std::string> & tokens = lines[i];
		for (int j = 0; j < tokens.size(); ++j) {
			if (tokens[j] != "_") {
				if (heads[j] > i) {
					GRAPHNODE_RIGHTNODES(node).push_back(RightNodeWithLabel(heads[j], TDepLabel::code(tokens[j]), GRAPH_LEFT));
				}
				else {
					GRAPHNODE_RIGHTNODES(graph[heads[j]]).push_back(RightNodeWithLabel(i, TDepLabel::code(tokens[j]), GRAPH_RIGHT));
				}
			}
		}
	}
	for (auto & node : graph) {
		std::sort(GRAPHNODE_RIGHTNODES(node).begin(), GRAPHNODE_RIGHTNODES(node).end(), [](const RightNodeWithLabel & rn1, const RightNodeWithLabel & rn2) { return RIGHTNODE_POS(rn1) < RIGHTNODE_POS(rn2); });
	}
	return input;
}

std::istream & operator>>(std::istream & input, DependencyCONLLGraph & graph) {
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
		DependencyCONLLGraphNode node;
		std::istringstream iss(line);
		std::vector<std::string> tokens;
		iss >> token >> CONLLGRAPHNODE_WORD(node) >> token >> CONLLGRAPHNODE_POSTAG(node) >> token >> token >> token;
		iss >> CONLLGRAPHNODE_TREELABEL(node) >> CONLLGRAPHNODE_TREEHEAD(node) >> token;
		iss >> token;
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
		DependencyCONLLGraphNode & node = graph[i];
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
		DependencyCONLLGraphNode & node = graph[std::get<0>(vecArcs[i])];
		if (i + 1 < vecArcs.size() && std::get<0>(vecArcs[i]) == std::get<0>(vecArcs[i + 1]) && std::get<1>(vecArcs[i]) == std::get<1>(vecArcs[i + 1])) {
			CONLLGRAPHNODE_RIGHTNODES(node).push_back(
					RightNodeWithCombineLabel(
							std::get<1>(vecArcs[i]),
							TDepLabel::code(ENCODE_TWOWAY_LABEL(std::get<3>(vecArcs[i]), std::get<3>(vecArcs[i + 1])))));
			++i;
		}
		else {
			CONLLGRAPHNODE_RIGHTNODES(node).push_back(
					RightNodeWithCombineLabel(
							std::get<1>(vecArcs[i]),
							TDepLabel::code((std::get<2>(vecArcs[i]) == GRAPH_LEFT ? ENCODE_LEFT_LABEL(std::get<3>(vecArcs[i])) : ENCODE_RIGHT_LABEL(std::get<3>(vecArcs[i]))))));
		}
	}
	return input;
}

std::ostream & operator<<(std::ostream & output, const Sentence & sentence) {
	auto itr = sentence.begin();
	while (true) {
		output << SENT_WORD(*itr) << SENT_SPTOKEN << SENT_POSTAG(*itr);
		if (++itr == sentence.end()) {
			break;
		}
		else {
			output << " ";
		}
	}
	output << std::endl;
	return output;
}

std::ostream & operator<<(std::ostream & output, const DependencyTree & tree) {
	for (auto itr = tree.begin(); itr != tree.end(); ++itr) {
		output << TREENODE_WORD(*itr) << "\t" << TREENODE_POSTAG(*itr) << "\t" << TREENODE_HEAD(*itr) << "\t" << TREENODE_LABEL(*itr) << std::endl;
	}
	output << std::endl;
	return output;
}

std::ostream & operator<<(std::ostream & output, const DependencyGraph & graph) {
	std::set<int> heads_set;
	std::vector<std::vector<std::tuple<int, std::string>>> heads_for_nodes;
	int i = 0;
	for (const auto & node : graph) {
		if (GRAPHNODE_WORD(node) == ROOT_WORD) {
			break;
		}
		heads_for_nodes.push_back(std::vector<std::tuple<int, std::string>>());
		for (const auto & rn : GRAPHNODE_RIGHTNODES(node)) {
			heads_set.insert(RIGHTNODE_DIRECTION(rn) == GRAPH_LEFT ? RIGHTNODE_POS(rn) : i);
		}
		++i;
	}
	i = 0;
	std::map<int, int> heads_inverse;
	for (const auto & head : heads_set) {
		heads_inverse[head] = i++;
	}
	heads_set.erase(graph.size() - 1);
	i = 0;
	std::set<int> roots_set;
	for (const auto & node : graph) {
		if (GRAPHNODE_WORD(node) == ROOT_WORD) {
			break;
		}
		for (const auto & rn : GRAPHNODE_RIGHTNODES(node)) {
			if (GRAPHNODE_WORD(graph[RIGHTNODE_POS(rn)]) == ROOT_WORD) {
				roots_set.insert(i);
			}
			else if (RIGHTNODE_DIRECTION(rn) == GRAPH_LEFT) {
				heads_for_nodes[i].push_back(std::tuple<int, std::string>(heads_inverse[RIGHTNODE_POS(rn)], TDepLabel::key(RIGHTNODE_LABEL(rn))));
			}
			else {
				heads_for_nodes[RIGHTNODE_POS(rn)].push_back(std::tuple<int, std::string>(heads_inverse[i], TDepLabel::key(RIGHTNODE_LABEL(rn))));
			}
		}
		++i;
	}
	i = 0;
	for (const auto & node : graph) {
		if (GRAPHNODE_WORD(node) == ROOT_WORD) {
			break;
		}
		output << (i + 1) << "\t" << GRAPHNODE_WORD(node) << "\t" << GRAPHNODE_WORD(node) << "\t" << GRAPHNODE_POSTAG(node) << "\t";
		output << (roots_set.find(i) != roots_set.end() ? "+" : "-") << "\t" << (heads_set.find(i) != heads_set.end() ? "+" : "-");
		for (int j = 0, k = 0; j < heads_set.size(); ++j) {
			if (k < heads_for_nodes[i].size() && std::get<0>(heads_for_nodes[i][k]) == j) {
				output << "\t" << std::get<1>(heads_for_nodes[i][k++]);
			}
			else {
				output << "\t" << "_";
			}
		}
		output << std::endl;
		++i;
	}
	output << std::endl;
	return output;
}

std::ostream & operator<<(std::ostream & output, const DependencyCONLLGraph & graph) {
	std::set<int> heads_set;
	std::vector<std::vector<std::tuple<int, std::string>>> heads_for_nodes;
	int i = 0;
	for (const auto & node : graph) {
		heads_for_nodes.push_back(std::vector<std::tuple<int, std::string>>());
		for (const auto & rn : CONLLGRAPHNODE_RIGHTNODES(node)) {
			const std::string & label = TDepLabel::key(COMBINERIGHTNODE_LABEL(rn));
			if (IS_LEFT_LABEL(label)) {
				heads_set.insert(COMBINERIGHTNODE_POS(rn));
			}
			else if (IS_RIGHT_LABEL(label)) {
				heads_set.insert(i);
			}
			else if (IS_TWOWAY_LABEL(label)) {
				heads_set.insert(COMBINERIGHTNODE_POS(rn));
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
		for (const auto & rn : CONLLGRAPHNODE_RIGHTNODES(node)) {
			const std::string & label = TDepLabel::key(COMBINERIGHTNODE_LABEL(rn));
			if (IS_LEFT_LABEL(label)) {
				heads_for_nodes[i].push_back(std::tuple<int, std::string>(heads_inverse[RIGHTNODE_POS(rn)], DECODE_LEFT_LABEL(label)));
			}
			else if (IS_RIGHT_LABEL(label)) {
				heads_for_nodes[COMBINERIGHTNODE_POS(rn)].push_back(std::tuple<int, std::string>(heads_inverse[i], DECODE_RIGHT_LABEL(label)));
			}
			else if (IS_TWOWAY_LABEL(label)) {
				// left label
				heads_for_nodes[i].push_back(std::tuple<int, std::string>(heads_inverse[RIGHTNODE_POS(rn)], DECODE_TWOWAY_LEFT_LABEL(label)));
				// right label
				heads_for_nodes[COMBINERIGHTNODE_POS(rn)].push_back(std::tuple<int, std::string>(heads_inverse[i], DECODE_TWOWAY_RIGHT_LABEL(label)));
			}
		}
		++i;
	}
	i = 0;
	for (const auto & node : graph) {
		if (GRAPHNODE_WORD(node) == ROOT_WORD) {
			break;
		}
		output << (i + 1) << " " << CONLLGRAPHNODE_WORD(node) << " " << CONLLGRAPHNODE_WORD(node) << " " << CONLLGRAPHNODE_POSTAG(node) << " " << CONLLGRAPHNODE_POSTAG(node) << " _ _ _ _ _ ";
		output << (heads_set.find(i) != heads_set.end() ? CONLLGRAPHNODE_WORD(node) : "_");
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
