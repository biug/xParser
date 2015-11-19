#include <memory>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "test.h"
//
//#include "common/parser/implementations/graph_transition_both/both_twostack/both_twostack_run.h"
//#include "common/parser/implementations/graph_transition_two_way/two_way_titov/two_way_titov_run.h"
//#include "common/parser/implementations/graph_transition_two_way/two_way_shift_reduce/two_way_shift_reduce_run.h"


int main(int argc, char * argv[]) {

	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	std::cout << std::fixed << std::setprecision(4);

//	testGraph(argv[1], argv[2]);
//	if (strcmp(argv[1], "split") == 0) {
//		splitGraph(argv[2], argv[3], argv[4]);
//	}
//	else if (strcmp(argv[1], "combine") == 0) {
//		testCrossing(argv[2], argv[3], argv[4]);
//	}

	runner(argc, argv);
}
