#include <ctime>
#include <memory>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

#include "std_titov_run.h"
#include "std_titov_depparser.h"
#include "common/token/deplabel.h"

namespace std_titov {

	extern int A_FIRST;
	extern int SH_FIRST;

	extern int A_END;
	extern int SH_END;

	Run::Run(const bool & bChar, const bool & bPath, const bool & bSuperTag) :
		GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem>(bChar, bPath, bSuperTag) {}

	Run::~Run() = default;

	void Run::initAction() const {

		A_FIRST = SHIFT + 1;
		A_END = SH_FIRST = A_FIRST + LABEL_COUNT;
		SH_END = m_bSuperTagFeature ? SH_FIRST + TSuperTag::count() + 1 : SH_FIRST + 1;

	}

}
