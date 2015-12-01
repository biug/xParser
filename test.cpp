#include "test.h"
#include "include/dependency_primitive.h"

#include "include/learning/tree/lca.h"
#include "include/dependency_primitive.h"
#include "common/parser/fittings/pseudo_tree.h"
#include "common/parser/implementations/arceager/arceager_run.h"
#include "common/parser/implementations/graph_transition/nivre/nivre_run.h"
#include "common/parser/implementations/graph_transition/titov/titov_run.h"
#include "common/parser/implementations/graph_transition/twostack/twostack_run.h"
#include "common/parser/implementations/graph_transition/nivre/std_nivre_run.h"
#include "common/parser/implementations/graph_transition/titov/std_titov_run.h"
#include "common/parser/implementations/graph_transition/twostack/std_twostack_run.h"
#include "common/parser/implementations/graph_transition/sr/twoway_sr_run.h"
#include "common/parser/implementations/graph_transition/arcsr/arcsr_run.h"
#include "common/parser/implementations/graph_transition/titov/twoway_titov_run.h"

#include <memory>
#include <fstream>

void runner(int argc, char * argv[]) {

	std::unique_ptr<RunBase> run(nullptr);

	if (strcmp(argv[2], "arceager") == 0) {
		run.reset(new arceager::Run());
	}
	else if (strcmp(argv[2], "titov") == 0 || strcmp(argv[2], "twostack") == 0 || strcmp(argv[2], "nivre") == 0 ||
			strcmp(argv[2], "std_titov") == 0 || strcmp(argv[2], "std_twostack") == 0 || strcmp(argv[2], "std_nivre") == 0 ||
			strcmp(argv[2], "twoway_sr") == 0 || strcmp(argv[2], "twoway_titov") == 0 || strcmp(argv[2], "arcsr") == 0) {
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
			next_feature = next_feature.substr(0, next_feature.rfind("\\") + strlen("\\")) + argv[2] + "1.feat";

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
			next_feature = next_feature.substr(0, next_feature.rfind("\\") + strlen("\\")) + argv[2] + "1.feat";
			next_reverse_feature = next_reverse_feature.substr(0, next_reverse_feature.rfind("\\") + strlen("\\")) + argv[2] + "1.feat";

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
	if (argc == 4) {
		std::ifstream input(argv[2]);
		std::ofstream output(argv[3]);
		DependencyGraph goldGraph;
		while (input >> goldGraph) {
			std::vector<DependencyGraph> pGraphs, nGraphs;
			output << PseudoTreeFitting().extractPseudoTree(goldGraph, pGraphs, nGraphs);
		}
	}
	else {
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
			output << PseudoTreeFitting().extractPseudoTree(goldGraph, pGraphs, nGraphs);
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
