#include <ctime>
#include <memory>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

#include "two_way_titov_run.h"
#include "two_way_titov_depparser.h"
#include "common/token/deplabel.h"

namespace two_way_titov {

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
		A_SW_END = A_RE_FIRST = A_SW_FIRST + g_nGraphLabelCount;
		A_RE_END = A_SH_FIRST = A_RE_FIRST + g_nGraphLabelCount;
		A_SH_END = SH_FIRST = (m_bSuperTagFeature ? A_SH_FIRST + g_nGraphLabelCount * (TSuperTag::count() + 1) : A_SH_FIRST + g_nGraphLabelCount);
		SH_END = m_bSuperTagFeature ? SH_FIRST + TSuperTag::count() + 1 : SH_FIRST + 1;

	}

}
