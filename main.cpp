#include <memory>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "test.h"
#include "common/parser/fittings/pseudo_tree.h"


int main(int argc, char * argv[]) {

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	std::cout << std::fixed << std::setprecision(4);

	if (argc == 1) {
		return 0;
	}
	else if ((strcmp(argv[1], "train") == 0 || strcmp(argv[1], "parse") == 0 || strcmp(argv[1], "goldtest") == 0)) {
		runner(argc, argv);
	}
	else {
		if (strcmp(argv[1], "pseudotree") == 0) {
			extractPseudoTree(argc, argv);
		}
		else if (strcmp(argv[1], "restoregraph") == 0) {
			restoreGraph(argc, argv);
		}
	}
}
