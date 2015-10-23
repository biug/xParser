#include <ctime>
#include <memory>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

#include "std_twostack_run.h"
#include "std_twostack_depparser.h"
#include "common/token/deplabel.h"

namespace std_twostack {

	extern int A_FIRST;
	extern int SH_FIRST;

	extern int A_END;
	extern int SH_END;

	Run::Run(const bool & bChar, const bool & bPath, const bool & bSuperTag) :
		GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem>(bChar, bPath, bSuperTag) {}

	Run::~Run() = default;

	void Run::initAction() const {

		A_FIRST = SHIFT + 1;
		A_END = SH_FIRST = A_FIRST + g_nGraphLabelCount;
		SH_END = m_bSuperTagFeature ? SH_FIRST + TSuperTag::count() + 1 : SH_FIRST + 1;

	}

}
