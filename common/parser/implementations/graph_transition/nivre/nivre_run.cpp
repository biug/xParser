#include <ctime>
#include <memory>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

#include "nivre_run.h"
#include "nivre_depparser.h"
#include "common/token/deplabel.h"

namespace nivre {

	extern int A_SW_FIRST;
	extern int A_RE_FIRST;
	extern int A_SH_FIRST;
	extern int SH_FIRST;

	extern int A_SW_END;
	extern int A_RE_END;
	extern int A_SH_END;
	extern int SH_END;

	Run::Run(const bool & bChar, const bool & bPath, const bool & bSuperTag) :
		GraphRunBase<DepParser<PackedScoreType>, DepParser<PackedSuperTagScoreType>, StateItem>(bChar, bPath, bSuperTag) {}

	Run::~Run() = default;

	void Run::initAction() const {

		A_SW_FIRST = SHIFT + 1;
		A_SW_END = A_RE_FIRST = A_SW_FIRST + LABEL_COUNT;
		A_RE_END = A_SH_FIRST = A_RE_FIRST + LABEL_COUNT;
		A_SH_END = SH_FIRST = (m_bSuperTagFeature ? A_SH_FIRST + LABEL_COUNT * (TSuperTag::count() + 1) : A_SH_FIRST + LABEL_COUNT);
		SH_END = m_bSuperTagFeature ? SH_FIRST + TSuperTag::count() + 1 : SH_FIRST + 1;

	}

}
