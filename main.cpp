#include <memory>
#include <cstring>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "include/learning//tree/lca.h"
#include "common/parser/implementations/arceager/arceager_run.h"
#include "common/parser/implementations/titov/titov_run.h"
#include "common/parser/implementations/nirve/nirve_run.h"
#include "common/parser/implementations/twostack/twostack_run.h"
#include "common/parser/implementations/titov_ring/titov_ring_run.h"
#include "common/parser/implementations/nirve_ring/nirve_ring_run.h"

#include "common/parser/implementations/eisner/eisner_run.h"
#include "common/parser/implementations/eisnergc/eisnergc_run.h"
#include "common/parser/implementations/eisner3rd/eisner3rd_run.h"
#include "common/parser/implementations/eisnergc3rd/eisnergc3rd_run.h"
#include "common/parser/implementations/emptyeisner3rd/emptyeisner3rd_run.h"
#include "common/parser/implementations/emptyeisnergc3rd/emptyeisnergc3rd_run.h"

int main(int argc, char * argv[]) {

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

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
	else if (strcmp(argv[2], "titov") == 0 || strcmp(argv[2], "twostack") == 0 || strcmp(argv[2], "nirve") == 0
			|| strcmp(argv[2], "titov_ring") == 0 || strcmp(argv[2], "nirve_ring") == 0) {
		bool bChar = false;
		bool bPath = false;
		bool bLabel = false;
		if (strcmp(argv[1], "goldtest") != 0 && argc > 6) {
			for (int a = 6; a < argc; ++a) {
				if (strcmp(argv[a], "char") == 0) {
					bChar = true;
				}
				else if (strcmp(argv[a], "path") == 0) {
					bPath = true;
				}
				else if (strcmp(argv[a], "label") == 0) {
					bLabel = true;
				}
			}
		}
		if (strcmp(argv[2], "titov") == 0) {
			run.reset(new titov::Run(bChar, bPath, bLabel));
		}
		else if (strcmp(argv[2], "twostack") == 0) {
			run.reset(new twostack::Run(bChar, bPath, bLabel));
		}
		else if (strcmp(argv[2], "nirve") == 0) {
			run.reset(new nirve::Run(bChar, bPath, bLabel));
		}
		else if (strcmp(argv[2], "titov_ring") == 0) {
			run.reset(new titov_ring::Run(bChar, bPath, bLabel));
		}
		else if (strcmp(argv[2], "nirve_ring") == 0) {
			run.reset(new nirve_ring::Run(bChar, bPath, bLabel));
		}
	}

	if (strcmp(argv[1], "goldtest") == 0) {
		run->goldtest(argv[3], argv[4]);
		std::cout << TDepLabel::getTokenizer();
	}
	else if (strcmp(argv[1], "train") == 0) {

		int iteration = std::atoi(argv[5]);

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
	else if (strcmp(argv[1], "parse") == 0) {
		run->parse(argv[3], argv[5], argv[4]);
	}
}
