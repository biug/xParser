#include <fstream>
#include "include/dependency_primitive.h"

#include "include/learning/tree/lca.h"
#include "include/dependency_primitive.h"
#include "common/parser/implementations/arceager/arceager_run.h"
#include "common/parser/implementations/graph_transition/nivre/nivre_run.h"
#include "common/parser/implementations/graph_transition/titov/titov_run.h"
#include "common/parser/implementations/graph_transition/twostack/twostack_run.h"
#include "common/parser/implementations/graph_transition/nivre/std_nivre_run.h"
#include "common/parser/implementations/graph_transition/titov/std_titov_run.h"
#include "common/parser/implementations/graph_transition/twostack/std_twostack_run.h"
#include "common/parser/implementations/graph_transition/sr/twoway_sr_run.h"

#include "common/parser/implementations/graph_dp/eisner/eisner_run.h"
#include "common/parser/implementations/graph_dp/eisnergc/eisnergc_run.h"
#include "common/parser/implementations/graph_dp/eisner3rd/eisner3rd_run.h"
#include "common/parser/implementations/graph_dp/eisnergc3rd/eisnergc3rd_run.h"
#include "common/parser/implementations/graph_dp/emptyeisner3rd/emptyeisner3rd_run.h"
#include "common/parser/implementations/graph_dp/emptyeisnergc3rd/emptyeisnergc3rd_run.h"

void testGraph(const std::string & sInput, const std::string & sOutput) {
	std::ifstream input(sInput);
	std::ofstream output(sOutput);
	CoNLL08DepGraph graph;
	if (input) {
		while (input >> graph) {
			output << -graph;
		}
	}
}

void splitGraph(const std::string & sInput, const std::string & sOutput1, const std::string & sOutput2) {
	std::ifstream input(sInput);
	std::ofstream output1(sOutput1);
	std::ofstream output2(sOutput2);
	CoNLL08DepGraph graph;
	if (input) {
		while (input >> graph) {
			auto bigraph = graph.splitPlanar();
			output1 << bigraph.first;
			output2 << bigraph.second;
		}
	}
}

void testCrossing(const std::string & sInput1, const std::string & sInput2, const std::string & sOutput) {
	std::ifstream input1(sInput1);
	std::ifstream input2(sInput2);
	std::ofstream output(sOutput);
	CoNLL08DepGraph graph1, graph2;
	while (input1 >> graph1) {
		input2 >> graph2;
		output << (graph1 + graph2);
	}
}

void runner(int argc, char * argv[]) {

	std::unique_ptr<RunBase> run(nullptr);

	if (strcmp(argv[2], "arceager") == 0) {
		run.reset(new arceager::Run());
	}
	else if (strcmp(argv[2], "eisner") == 0) {
		run.reset(new eisner::Run());
	}
	else if (strcmp(argv[2], "eisner3rd") == 0) {
		run.reset(new eisner3rd::Run());
	}
	else if (strcmp(argv[2], "eisnergc") == 0) {
		run.reset(new eisnergc::Run());
	}
	else if (strcmp(argv[2], "eisnergc3rd") == 0) {
		run.reset(new eisnergc3rd::Run());
	}
	else if (strcmp(argv[2], "emptyeisner3rd") == 0) {
		run.reset(new emptyeisner3rd::Run());
	}
	else if (strcmp(argv[2], "emptyeisnergc3rd") == 0) {
		run.reset(new emptyeisnergc3rd::Run());
	}
	else if (strcmp(argv[2], "titov") == 0 || strcmp(argv[2], "twostack") == 0 || strcmp(argv[2], "nivre") == 0 ||
			strcmp(argv[2], "std_titov") == 0 || strcmp(argv[2], "std_twostack") == 0 || strcmp(argv[2], "std_nivre") == 0 ||
			strcmp(argv[2], "twoway_sr") == 0) {
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
//		else if (strcmp(argv[2], "both_twostack") == 0) {
//			run.reset(new both_twostack::Run(bChar, bPath, bSuperTag));
//		}
		else if (strcmp(argv[2], "twoway_sr") == 0) {
			run.reset(new twoway_sr::Run(bChar, bPath, bSuperTag));
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