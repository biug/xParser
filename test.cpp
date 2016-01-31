#include "test.h"
#include "include/dependency_primitive.h"

#include "include/learning/tree/lca.h"
#include "include/dependency_primitive.h"
#include "common/parser/fittings/pseudo_tree.h"
#include "common/parser/implementations/arceager/arceager_run.h"
#include "common/parser/implementations/arceagerpath/arceagerpath_run.h"
#include "common/parser/implementations/arceagertag/arceagertag_run.h"
#include "common/parser/implementations/minitwostack/minitwostack_run.h"
#include "common/parser/implementations/graph_transition/nivre/nivre_run.h"
#include "common/parser/implementations/graph_transition/titov/titov_run.h"
#include "common/parser/implementations/graph_transition/twostack/twostack_run.h"
#include "common/parser/implementations/graph_transition/titov/bititov_run.h"
#include "common/parser/implementations/graph_transition/nivre/binivre_run.h"
#include "common/parser/implementations/graph_transition/twostack/bitwostack_run.h"
#include "common/parser/implementations/graph_transition/titov/srtitov_run.h"
#include "common/parser/implementations/graph_transition/nivre/std_nivre_run.h"
#include "common/parser/implementations/graph_transition/titov/std_titov_run.h"
#include "common/parser/implementations/graph_transition/twostack/std_twostack_run.h"
#include "common/parser/implementations/graph_transition/sr/twoway_sr_run.h"
#include "common/parser/implementations/graph_transition/arcsr/arcsr_run.h"
#include "common/parser/implementations/graph_transition/titov/twoway_titov_run.h"

#include <memory>
#include <fstream>

#define	SLASH	"\\"

void runner(int argc, char * argv[]) {

	std::unique_ptr<RunBase> run(nullptr);

	if (strcmp(argv[2], "arceager") == 0) {
		run.reset(new arceager::Run());
	}
	else if (strcmp(argv[2], "arceagerpath") == 0) {
		run.reset(new arceagerpath::Run());
	}
	else if (strcmp(argv[2], "arceagertag") == 0) {
		run.reset(new arceagertag::Run());
	}
	else if (strcmp(argv[2], "minit") == 0) {
		run.reset(new minitwostack::Run());
	}
	else if (strcmp(argv[2], "titov") == 0 || strcmp(argv[2], "twostack") == 0 || strcmp(argv[2], "nivre") == 0 ||
			strcmp(argv[2], "std_titov") == 0 || strcmp(argv[2], "std_twostack") == 0 || strcmp(argv[2], "std_nivre") == 0 ||
			strcmp(argv[2], "twoway_sr") == 0 || strcmp(argv[2], "twoway_titov") == 0 || strcmp(argv[2], "arcsr") == 0 ||
			strcmp(argv[2], "bititov") == 0 || strcmp(argv[2], "bitwostack") == 0 || strcmp(argv[2], "binivre") == 0 ||
			strcmp(argv[2], "srtitov") == 0) {
		bool bChar = false;
		bool bPath = false;
		bool bSuperTag = false;
		if (argc > 6) {
			for (int a = 6; a < argc; ++a) {
				if (strcmp(argv[a], "char") == 0) {
					bChar = true;
				}
				else if (strcmp(argv[a], "path") == 0) {
					bPath = true;
				}
				else if (strcmp(argv[a], "supertag") == 0) {
					bSuperTag = true;
				}
			}
		}
		if (strcmp(argv[2], "nivre") == 0) {
			run.reset(new nivre::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "titov") == 0) {
			run.reset(new titov::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "twostack") == 0) {
			run.reset(new twostack::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "std_nivre") == 0) {
			run.reset(new std_nivre::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "std_titov") == 0) {
			run.reset(new std_titov::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "std_twostack") == 0) {
			run.reset(new std_twostack::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "twoway_sr") == 0) {
			run.reset(new twoway_sr::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "twoway_titov") == 0) {
			run.reset(new twoway_titov::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "arcsr") == 0) {
			run.reset(new arcsr::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "bititov") == 0) {
			run.reset(new bititov::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "binivre") == 0) {
			run.reset(new binivre::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "bitwostack") == 0) {
			run.reset(new bitwostack::Run(bChar, bPath, bSuperTag));
		}
		else if (strcmp(argv[2], "srtitov") == 0) {
			run.reset(new srtitov::Run(bChar, bPath, bSuperTag));
		}
	}

	if (strcmp(argv[1], "goldtest") == 0) {
		run->goldtest(argv[3], argv[4]);
	}
	else if (strcmp(argv[1], "train") == 0) {

		int iteration = std::atoi(argv[5]);

		if (strcmp(argv[2], "two_way_titov") != 0 && strcmp(argv[2], "two_way_shift_reduce") != 0) {
			std::string current_feature;
			std::string next_feature;

			current_feature = next_feature = argv[4];
			next_feature = next_feature.substr(0, next_feature.rfind(SLASH) + strlen(SLASH)) + argv[2] + "1.feat";

			for (int i = 0; i < iteration; ++i) {
				run->train(argv[3], current_feature, next_feature);
				current_feature = next_feature;
				next_feature = next_feature.substr(0, next_feature.rfind(argv[2]) + strlen(argv[2])) + std::to_string(i + 2) + ".feat";
			}
		}
		else {
			std::string current_feature;
			std::string next_feature;
			std::string current_reverse_feature;
			std::string next_reverse_feature;

			current_feature = next_feature = std::string(argv[4]).substr(0, std::string(argv[4]).find("#"));
			current_reverse_feature = next_reverse_feature = std::string(argv[4]).substr(std::string(argv[4]).find("#") + 1);
			next_feature = next_feature.substr(0, next_feature.rfind(SLASH) + strlen(SLASH)) + argv[2] + "1.feat";
			next_reverse_feature = next_reverse_feature.substr(0, next_reverse_feature.rfind(SLASH) + strlen(SLASH)) + argv[2] + "1.feat";

			for (int i = 0; i < iteration; ++i) {
				run->train(argv[3], current_feature + "#" + current_reverse_feature, next_feature + "#" + next_reverse_feature);
				current_feature = next_feature;
				current_reverse_feature = next_reverse_feature;
				next_feature = next_feature.substr(0, next_feature.rfind(argv[2]) + strlen(argv[2])) + std::to_string(i + 2) + ".feat";
				next_reverse_feature = next_reverse_feature.substr(0, next_reverse_feature.rfind(argv[2]) + strlen(argv[2])) + std::to_string(i + 2) + ".feat";
			}
		}
	}
	else if (strcmp(argv[1], "parse") == 0) {
		run->parse(argv[3], argv[5], argv[4]);
	}
}

void extractPseudoTree(int argc, char * argv[]) {
	if (argc == 4 || argc == 5) {
		std::ifstream input(argv[2]);
		std::ofstream output(argv[3]);
		DependencyGraph goldGraph;
		while (input >> goldGraph) {
			std::vector<DependencyGraph> pGraphs, nGraphs;
			output << PseudoTreeFitting().extractPseudoTree(goldGraph, pGraphs, nGraphs, argc >= 5 && strcmp(argv[4], "keep") == 0);
		}
	}
	else if (argc == 10) {
		std::ifstream input(argv[2]);
		std::ofstream output(argv[3]);
		std::ifstream inputP1(argv[4]);
		std::ifstream inputP2(argv[5]);
		std::ifstream inputP3(argv[6]);
		std::ifstream inputN1(argv[7]);
		std::ifstream inputN2(argv[8]);
		std::ifstream inputN3(argv[9]);
		DependencyGraph goldGraph, graph;
		while (input >> goldGraph) {
			std::vector<DependencyGraph> pGraphs, nGraphs;
			inputP1 >> graph;
			pGraphs.push_back(graph);
			inputP2 >> graph;
			pGraphs.push_back(graph);
			inputP3 >> graph;
			pGraphs.push_back(graph);
			inputN1 >> graph;
			nGraphs.push_back(graph);
			inputN2 >> graph;
			nGraphs.push_back(graph);
			inputN3 >> graph;
			nGraphs.push_back(graph);
			output << PseudoTreeFitting().extractPseudoScoreTree(goldGraph, pGraphs, nGraphs);
		}
	}
	else if (argc == 11) {
		std::ifstream input(argv[2]);
		std::ofstream output(argv[3]);
		std::ifstream inputC(argv[4]);
		std::ifstream inputP1(argv[5]);
		std::ifstream inputP2(argv[6]);
		std::ifstream inputP3(argv[7]);
		std::ifstream inputN1(argv[8]);
		std::ifstream inputN2(argv[9]);
		std::ifstream inputN3(argv[10]);
		DependencyGraph goldGraph, cGraph, graph;
		while (input >> goldGraph) {
			std::vector<DependencyGraph> pGraphs, nGraphs;
			inputC >> cGraph;
			inputP1 >> graph;
			pGraphs.push_back(graph);
			inputP2 >> graph;
			pGraphs.push_back(graph);
			inputP3 >> graph;
			pGraphs.push_back(graph);
			inputN1 >> graph;
			nGraphs.push_back(graph);
			inputN2 >> graph;
			nGraphs.push_back(graph);
			inputN3 >> graph;
			nGraphs.push_back(graph);
			output << PseudoTreeFitting().extractPseudoScoreTree(goldGraph, cGraph, pGraphs, nGraphs);
		}
	}
}

void restoreGraph(int argc, char * argv[]) {
	std::ifstream input(argv[2]);
	std::ofstream output(argv[3]);
	DependencyTree tree;
	while (input >> tree) {
		output << PseudoTreeFitting().pseudoTreeToGraph(tree, argc >= 5 && strcmp(argv[4], "keep") == 0);
	}
}

void addTag(int argc, char * argv[]) {
	std::ifstream inputG(argv[2]);
	std::ifstream inputP(argv[3]);
	std::ofstream output(argv[4]);
	DependencyGraph gold, pseudo;
	while (inputG >> gold) {
		inputP >> pseudo;
		output << PseudoTreeFitting().addPseudoTag(gold, pseudo);
	}
}

void testTag(int argc, char * argv[]) {
	std::ifstream inputG(argv[2]);
	std::ifstream inputP(argv[3]);
	DependencyGraph gold, pseudo;
	int id = 0;
	while (inputG >> gold) {
		inputP >> pseudo;
		DependencyGraph remain = gold - pseudo;
		std::unordered_map<int, std::unordered_set<int>> edges;
		int i = 0;
		for (const auto & node : remain) {
			for (const auto & arc : node.m_vecRightArcs) {
				edges[i].insert(arc.first);
				edges[arc.first].insert(i);
			}
			++i;
		}
		for (i = 0; i < remain.size(); ++i) {
			if (remain[i].m_sSuperTag == "K" && edges[i].empty()) {
				std::cout << "bad graph at " << id << std::endl;
				std::cout << "node is " << i << std::endl;
				std::cout << remain;
				return;
			}
			if (remain[i].m_sSuperTag == "S" && !edges[i].empty()) {
				std::cout << "bad graph at " << id << std::endl;
				std::cout << "node is " << i << std::endl;
				std::cout << remain;
				return;
			}
		}
		++id;
	}
}

void reverseGraph(int argc, char * argv[]) {
	std::ifstream inputTree(argv[2]);
	std::ifstream inputGGraph(argv[3]);
	std::ifstream inputPGraph(argv[4]);
	std::ofstream output(argv[5]);
	DependencyTaggedTree tree;
	CoNLL08DepGraph ggraph;
	CoNLL08DepGraph pgraph;
	while (inputTree >> tree) {
		inputGGraph >> ggraph;
		inputPGraph >> pgraph;
		CoNLL08DepGraph graph = ggraph - pgraph;
		for (int i = 0; i < tree.size(); ++i) {
			graph[i].m_sSuperTag = tree[i].second;
			graph[i].m_nTreeHead = TREENODE_HEAD(tree[i].first);
			if (graph[i].m_sSuperTag != "K") {
				graph[i].m_vecRightArcs.clear();
				for (int j = 0; j < i; ++j) {
					for (auto && itr = graph[j].m_vecRightArcs.begin(); itr != graph[j].m_vecRightArcs.end(); ++itr) {
						if (itr->first == i) {
							graph[j].m_vecRightArcs.erase(itr);
							break;
						}
					}
				}
			}
		}
		output << -graph;
	}
}

void combineGraph(int argc, char * argv[]) {
	std::ifstream input1(argv[2]);
	std::ifstream input2(argv[3]);
	std::ofstream output(argv[4]);
	CoNLL08DepGraph graph1, graph2;
	while (input1 >> graph1) {
		input2 >> graph2;
		output << (graph1 + graph2);
	}
}

void removeTree(int argc, char * argv[]) {
	std::ifstream inputT(argv[2]);
	std::ifstream inputG(argv[3]);
	std::ofstream output(argv[4]);
	DependencyTree tree;
	CoNLL08DepGraph graph;
	while (inputT >> tree) {
		inputG >> graph;
		output << graph - PseudoTreeFitting().pseudoTreeToGraph(tree);
	}
}

void stronglyComponents(int argc, char * argv[]) {
	bool bConnected = strcmp(argv[2], "connected") == 0;
	std::ifstream inputG(argv[3]);
	CoNLL08DepGraph graph;
	while (inputG >> graph) {
		auto components = graph.stronglyComponet(bConnected);
		for (const auto & component : components) {
			for (const auto & v : component) {
				std::cout << v << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		break;
	}
}

void graphPath(int argc, char * argv[]) {
	bool bConnected = strcmp(argv[2], "connected") == 0;
	std::ifstream inputG(argv[3]);
	CoNLL08DepGraph graph;
	while (inputG >> graph) {
		auto paths = graph.shortestPaths(bConnected);
		int n = graph.size();
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (i != j && !paths[i][j].empty()) {
					std::cout << i << ' ' << j << ':' << graph.labelPath(paths[i][j], "label") << std::endl;
				}
			}
		}
		break;
	}
}
