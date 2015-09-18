#include <fstream>

#include "twostack_weight.h"
#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace twostack {
	Weight::Weight(const std::string & sRead, const std::string & sRecord) :
		WeightBase(sRead, sRecord),
		m_mapSTw("m_mapSTw"),
		m_mapSTpt("m_mapSTpt"),
		m_mapST2w("m_mapST2w"),
		m_mapST2pt("m_mapST2pt"),
		m_map2STw("m_map2STw"),
		m_map2STpt("m_map2STpt"),
		m_mapN0w("m_mapN0w"),
		m_mapN0pt("m_mapN0pt"),

		m_mapSTwN0w("m_mapSTwN0w"),
		m_mapST2wN0w("m_mapST2wN0w"),
		m_map2STwN0w("m_map2STwN0w"),
		m_mapSTwN0pt("m_mapSTwN0pt"),
		m_mapSTptN0w("m_mapSTptN0w"),
		m_mapST2wN0pt("m_mapST2wN0pt"),
		m_mapST2ptN0w("m_mapST2ptN0w"),
		m_map2STwN0pt("m_map2STwN0pt"),
		m_map2STptN0w("m_map2STptN0w"),
		m_mapSTptN0pt("m_mapSTptN0pt"),
		m_mapST2ptN0pt("m_mapST2ptN0pt"),
		m_map2STptN0pt("m_map2STptN0pt"),

		m_mapSTwSTRHwN0w("m_mapSTwSTRHwN0w"),
		m_mapSTwSTRDwN0w("m_mapSTwSTRDwN0w"),
		m_mapSTwN0wN0LHw("m_mapSTwN0wN0LHw"),
		m_mapSTwN0wN0LDw("m_mapSTwN0wN0LDw"),

		m_mapST2wST2RHwN0w("m_mapST2wST2RHwN0w"),
		m_mapST2wST2RDwN0w("m_mapST2wST2RDwN0w"),
		m_mapST2wN0wN0LHw("m_mapST2wN0wN0LHw"),
		m_mapST2wN0wN0LDw("m_mapST2wN0wN0LDw"),

		m_map2STw2STRHwN0w("m_map2STw2STRHwN0w"),
		m_map2STw2STRDwN0w("m_map2STw2STRDwN0w"),
		m_map2STwN0wN0LHw("m_map2STwN0wN0LHw"),
		m_map2STwN0wN0LDw("m_map2STwN0wN0LDw"),

		m_mapSTwSTRHwN0pt("m_mapSTwSTRHwN0pt"),
		m_mapSTwSTRHptN0w("m_mapSTwSTRHptN0w"),
		m_mapSTptSTRHwN0w("m_mapSTptSTRHwN0w"),

		m_mapST2wST2RHwN0pt("m_mapST2wST2RHwN0pt"),
		m_mapST2wST2RHptN0w("m_mapST2wST2RHptN0w"),
		m_mapST2ptST2RHwN0w("m_mapST2ptST2RHwN0w"),

		m_map2STw2STRHwN0pt("m_map2STw2STRHwN0pt"),
		m_map2STw2STRHptN0w("m_map2STw2STRHptN0w"),
		m_map2STpt2STRHwN0w("m_map2STpt2STRHwN0w"),

		m_mapSTwSTRDwN0pt("m_mapSTwSTRDwN0pt"),
		m_mapSTwSTRDptN0w("m_mapSTwSTRDptN0w"),
		m_mapSTptSTRDwN0w("m_mapSTptSTRDwN0w"),

		m_mapST2wST2RDwN0pt("m_mapST2wST2RDwN0pt"),
		m_mapST2wST2RDptN0w("m_mapST2wST2RDptN0w"),
		m_mapST2ptST2RDwN0w("m_mapST2ptST2RDwN0w"),

		m_map2STw2STRDwN0pt("m_map2STw2STRDwN0pt"),
		m_map2STw2STRDptN0w("m_map2STw2STRDptN0w"),
		m_map2STpt2STRDwN0w("m_map2STpt2STRDwN0w"),

		m_mapSTwN0wN0LHpt("m_mapSTwN0wN0LHpt"),
		m_mapSTwN0ptN0LHw("m_mapSTwN0ptN0LHw"),
		m_mapSTptN0wN0LHw("m_mapSTptN0wN0LHw"),

		m_mapST2wN0wN0LHpt("m_mapST2wN0wN0LHpt"),
		m_mapST2wN0ptN0LHw("m_mapST2wN0ptN0LHw"),
		m_mapST2ptN0wN0LHw("m_mapST2ptN0wN0LHw"),

		m_map2STwN0wN0LHpt("m_map2STwN0wN0LHpt"),
		m_map2STwN0ptN0LHw("m_map2STwN0ptN0LHw"),
		m_map2STptN0wN0LHw("m_map2STptN0wN0LHw"),

		m_mapSTwN0wN0LDpt("m_mapSTwN0wN0LDpt"),
		m_mapSTwN0ptN0LDw("m_mapSTwN0ptN0LDw"),
		m_mapSTptN0wN0LDw("m_mapSTptN0wN0LDw"),

		m_mapST2wN0wN0LDpt("m_mapST2wN0wN0LDpt"),
		m_mapST2wN0ptN0LDw("m_mapST2wN0ptN0LDw"),
		m_mapST2ptN0wN0LDw("m_mapST2ptN0wN0LDw"),

		m_map2STwN0wN0LDpt("m_map2STwN0wN0LDpt"),
		m_map2STwN0ptN0LDw("m_map2STwN0ptN0LDw"),
		m_map2STptN0wN0LDw("m_map2STptN0wN0LDw"),

		m_mapSTwSTRHptN0pt("m_mapSTwSTRHptN0pt"),
		m_mapSTptSTRHptN0w("m_mapSTptSTRHptN0w"),
		m_mapSTptSTRHwN0pt("m_mapSTptSTRHwN0pt"),

		m_mapST2wST2RHptN0pt("m_mapST2wST2RHptN0pt"),
		m_mapST2ptST2RHptN0w("m_mapST2ptST2RHptN0w"),
		m_mapST2ptST2RHwN0pt("m_mapST2ptST2RHwN0pt"),

		m_map2STw2STRHptN0pt("m_map2STw2STRHptN0pt"),
		m_map2STpt2STRHptN0w("m_map2STpt2STRHptN0w"),
		m_map2STpt2STRHwN0pt("m_map2STpt2STRHwN0pt"),

		m_mapSTwSTRDptN0pt("m_mapSTwSTRDptN0pt"),
		m_mapSTptSTRDptN0w("m_mapSTptSTRDptN0w"),
		m_mapSTptSTRDwN0pt("m_mapSTptSTRDwN0pt"),

		m_mapST2wST2RDptN0pt("m_mapST2wST2RDptN0pt"),
		m_mapST2ptST2RDptN0w("m_mapST2ptST2RDptN0w"),
		m_mapST2ptST2RDwN0pt("m_mapST2ptST2RDwN0pt"),

		m_map2STw2STRDptN0pt("m_map2STw2STRDptN0pt"),
		m_map2STpt2STRDptN0w("m_map2STpt2STRDptN0w"),
		m_map2STpt2STRDwN0pt("m_map2STpt2STRDwN0pt"),

		m_mapSTwN0ptN0LHpt("m_mapSTwN0ptN0LHpt"),
		m_mapSTptN0ptN0LHw("m_mapSTptN0ptN0LHw"),
		m_mapSTptN0wN0LHpt("m_mapSTptN0wN0LHpt"),

		m_mapST2wN0ptN0LHpt("m_mapST2wN0ptN0LHpt"),
		m_mapST2ptN0ptN0LHw("m_mapST2ptN0ptN0LHw"),
		m_mapST2ptN0wN0LHpt("m_mapST2ptN0wN0LHpt"),

		m_map2STwN0ptN0LHpt("m_map2STwN0ptN0LHpt"),
		m_map2STptN0ptN0LHw("m_map2STptN0ptN0LHw"),
		m_map2STptN0wN0LHpt("m_map2STptN0wN0LHpt"),

		m_mapSTwN0ptN0LDpt("m_mapSTwN0ptN0LDpt"),
		m_mapSTptN0ptN0LDw("m_mapSTptN0ptN0LDw"),
		m_mapSTptN0wN0LDpt("m_mapSTptN0wN0LDpt"),

		m_mapST2wN0ptN0LDpt("m_mapST2wN0ptN0LDpt"),
		m_mapST2ptN0ptN0LDw("m_mapST2ptN0ptN0LDw"),
		m_mapST2ptN0wN0LDpt("m_mapST2ptN0wN0LDpt"),

		m_map2STwN0ptN0LDpt("m_map2STwN0ptN0LDpt"),
		m_map2STptN0ptN0LDw("m_map2STptN0ptN0LDw"),
		m_map2STptN0wN0LDpt("m_map2STptN0wN0LDpt"),

		m_mapSTptSTRHptN0pt("m_mapSTptSTRHptN0pt"),
		m_mapSTptSTRDptN0pt("m_mapSTptSTRDptN0pt"),
		m_mapSTptN0ptN0LHpt("m_mapSTptN0ptN0LHpt"),
		m_mapSTptN0ptN0LDpt("m_mapSTptN0ptN0LDpt"),

		m_mapST2ptST2RHptN0pt("m_mapST2ptST2RHptN0pt"),
		m_mapST2ptST2RDptN0pt("m_mapST2ptST2RDptN0pt"),
		m_mapST2ptN0ptN0LHpt("m_mapST2ptN0ptN0LHpt"),
		m_mapST2ptN0ptN0LDpt("m_mapST2ptN0ptN0LDpt"),

		m_map2STpt2STRHptN0pt("m_map2STpt2STRHptN0pt"),
		m_map2STpt2STRDptN0pt("m_map2STpt2STRDptN0pt"),
		m_map2STptN0ptN0LHpt("m_map2STptN0ptN0LHpt"),
		m_map2STptN0ptN0LDpt("m_map2STptN0ptN0LDpt"),

		m_mapSTPOSPath("m_mapSTPOSPath"),
		m_mapST2POSPath("m_mapST2POSPath"),
		m_mapSTFPOSPath("m_mapSTFPOSPath"),
		m_mapST2FPOSPath("m_mapST2FPOSPath"),

		m_mapN_2w("m_mapN_2w"),
		m_mapN_2pt("m_mapN_2pt"),
		m_mapN_1w("m_mapN_1w"),
		m_mapN_1pt("m_mapN_1pt"),
		m_mapN1w("m_mapN1w"),
		m_mapN1pt("m_mapN1pt"),
		m_mapN2w("m_mapN2w"),
		m_mapN2pt("m_mapN2pt"),
		m_mapN_2wN_1w("m_mapN_2wN_1w"),
		m_mapN_1wN0w("m_mapN_1wN0w"),
		m_mapN0wN1w("m_mapN0wN1w"),
		m_mapN1wN2w("m_mapN1wN2w"),
		m_mapN_2ptN_1pt("m_mapN_2ptN_1pt"),
		m_mapN_1ptN0pt("m_mapN_1ptN0pt"),
		m_mapN0ptN1pt("m_mapN0ptN1pt"),
		m_mapN1ptN2pt("m_mapN1ptN2pt"),

		m_map1CharBefore("m_map1CharBefore"),
		m_map2CharBefore("m_map2CharBefore"),
		m_map3CharBefore("m_map3CharBefore"),
		m_map1CharAfter("m_map1CharAfter"),
		m_map2CharAfter("m_map2CharAfter"),
		m_map3CharAfter("m_map3CharAfter")
	{
		loadScores();
		std::cout << "load complete." << std::endl;
	}

	Weight::~Weight() = default;

	void Weight::loadScores() {

		if (m_sReadPath.empty()) {
			std::cout << "empty path" << std::endl;
			return;
		}
		std::ifstream input(m_sReadPath);
		if (!input) {
			std::cout << m_sReadPath << std::endl;
			std::cout << "bad path" << std::endl;
			return;
		}

		input >> TWord::getTokenizer();

		input >> TPOSTag::getTokenizer();

		input >> TDepLabel::getTokenizer();

		input >> m_mapSTw;
		input >> m_mapSTpt;
		input >> m_mapST2w;
		input >> m_mapST2pt;
		input >> m_map2STw;
		input >> m_map2STpt;
		input >> m_mapN0w;
		input >> m_mapN0pt;

		input >> m_mapSTwN0w;
		input >> m_mapST2wN0w;
		input >> m_map2STwN0w;
		input >> m_mapSTwN0pt;
		input >> m_mapSTptN0w;
		input >> m_mapST2wN0pt;
		input >> m_mapST2ptN0w;
		input >> m_map2STwN0pt;
		input >> m_map2STptN0w;
		input >> m_mapSTptN0pt;
		input >> m_mapST2ptN0pt;
		input >> m_map2STptN0pt;

		input >> m_mapSTwSTRHwN0w;
		input >> m_mapSTwSTRDwN0w;
		input >> m_mapSTwN0wN0LHw;
		input >> m_mapSTwN0wN0LDw;

		input >> m_mapST2wST2RHwN0w;
		input >> m_mapST2wST2RDwN0w;
		input >> m_mapST2wN0wN0LHw;
		input >> m_mapST2wN0wN0LDw;

		input >> m_map2STw2STRHwN0w;
		input >> m_map2STw2STRDwN0w;
		input >> m_map2STwN0wN0LHw;
		input >> m_map2STwN0wN0LDw;

		input >> m_mapSTwSTRHwN0pt;
		input >> m_mapSTwSTRHptN0w;
		input >> m_mapSTptSTRHwN0w;

		input >> m_mapST2wST2RHwN0pt;
		input >> m_mapST2wST2RHptN0w;
		input >> m_mapST2ptST2RHwN0w;

		input >> m_map2STw2STRHwN0pt;
		input >> m_map2STw2STRHptN0w;
		input >> m_map2STpt2STRHwN0w;

		input >> m_mapSTwSTRDwN0pt;
		input >> m_mapSTwSTRDptN0w;
		input >> m_mapSTptSTRDwN0w;

		input >> m_mapST2wST2RDwN0pt;
		input >> m_mapST2wST2RDptN0w;
		input >> m_mapST2ptST2RDwN0w;

		input >> m_map2STw2STRDwN0pt;
		input >> m_map2STw2STRDptN0w;
		input >> m_map2STpt2STRDwN0w;

		input >> m_mapSTwN0wN0LHpt;
		input >> m_mapSTwN0ptN0LHw;
		input >> m_mapSTptN0wN0LHw;

		input >> m_mapST2wN0wN0LHpt;
		input >> m_mapST2wN0ptN0LHw;
		input >> m_mapST2ptN0wN0LHw;

		input >> m_map2STwN0wN0LHpt;
		input >> m_map2STwN0ptN0LHw;
		input >> m_map2STptN0wN0LHw;

		input >> m_mapSTwN0wN0LDpt;
		input >> m_mapSTwN0ptN0LDw;
		input >> m_mapSTptN0wN0LDw;

		input >> m_mapST2wN0wN0LDpt;
		input >> m_mapST2wN0ptN0LDw;
		input >> m_mapST2ptN0wN0LDw;

		input >> m_map2STwN0wN0LDpt;
		input >> m_map2STwN0ptN0LDw;
		input >> m_map2STptN0wN0LDw;

		input >> m_mapSTwSTRHptN0pt;
		input >> m_mapSTptSTRHptN0w;
		input >> m_mapSTptSTRHwN0pt;

		input >> m_mapST2wST2RHptN0pt;
		input >> m_mapST2ptST2RHptN0w;
		input >> m_mapST2ptST2RHwN0pt;

		input >> m_map2STw2STRHptN0pt;
		input >> m_map2STpt2STRHptN0w;
		input >> m_map2STpt2STRHwN0pt;

		input >> m_mapSTwSTRDptN0pt;
		input >> m_mapSTptSTRDptN0w;
		input >> m_mapSTptSTRDwN0pt;

		input >> m_mapST2wST2RDptN0pt;
		input >> m_mapST2ptST2RDptN0w;
		input >> m_mapST2ptST2RDwN0pt;

		input >> m_map2STw2STRDptN0pt;
		input >> m_map2STpt2STRDptN0w;
		input >> m_map2STpt2STRDwN0pt;

		input >> m_mapSTwN0ptN0LHpt;
		input >> m_mapSTptN0ptN0LHw;
		input >> m_mapSTptN0wN0LHpt;

		input >> m_mapST2wN0ptN0LHpt;
		input >> m_mapST2ptN0ptN0LHw;
		input >> m_mapST2ptN0wN0LHpt;

		input >> m_map2STwN0ptN0LHpt;
		input >> m_map2STptN0ptN0LHw;
		input >> m_map2STptN0wN0LHpt;

		input >> m_mapSTwN0ptN0LDpt;
		input >> m_mapSTptN0ptN0LDw;
		input >> m_mapSTptN0wN0LDpt;

		input >> m_mapST2wN0ptN0LDpt;
		input >> m_mapST2ptN0ptN0LDw;
		input >> m_mapST2ptN0wN0LDpt;

		input >> m_map2STwN0ptN0LDpt;
		input >> m_map2STptN0ptN0LDw;
		input >> m_map2STptN0wN0LDpt;

		input >> m_mapSTptSTRHptN0pt;
		input >> m_mapSTptSTRDptN0pt;
		input >> m_mapSTptN0ptN0LHpt;
		input >> m_mapSTptN0ptN0LDpt;

		input >> m_mapST2ptST2RHptN0pt;
		input >> m_mapST2ptST2RDptN0pt;
		input >> m_mapST2ptN0ptN0LHpt;
		input >> m_mapST2ptN0ptN0LDpt;

		input >> m_map2STpt2STRHptN0pt;
		input >> m_map2STpt2STRDptN0pt;
		input >> m_map2STptN0ptN0LHpt;
		input >> m_map2STptN0ptN0LDpt;

		input >> m_mapSTPOSPath;
		input >> m_mapST2POSPath;
		input >> m_mapSTFPOSPath;
		input >> m_mapST2FPOSPath;

		input >> m_mapN_2w;
		input >> m_mapN_2pt;
		input >> m_mapN_1w;
		input >> m_mapN_1pt;
		input >> m_mapN1w;
		input >> m_mapN1pt;
		input >> m_mapN2w;
		input >> m_mapN2pt;
		input >> m_mapN_2wN_1w;
		input >> m_mapN_1wN0w;
		input >> m_mapN0wN1w;
		input >> m_mapN1wN2w;
		input >> m_mapN_2ptN_1pt;
		input >> m_mapN_1ptN0pt;
		input >> m_mapN0ptN1pt;
		input >> m_mapN1ptN2pt;

		input >> m_map1CharBefore;
		input >> m_map2CharBefore;
		input >> m_map3CharBefore;
		input >> m_map1CharAfter;
		input >> m_map2CharAfter;
		input >> m_map3CharAfter;

		input.close();
	}

	void Weight::saveScores() const {

		if (m_sRecordPath.empty()) {
			return;
		}
		std::ofstream output(m_sRecordPath);
		if (!output) {
			return;
		}

		output << TWord::getTokenizer();

		output << TPOSTag::getTokenizer();

		output << TDepLabel::getTokenizer();

		output << m_mapSTw;
		output << m_mapSTpt;
		output << m_mapST2w;
		output << m_mapST2pt;
		output << m_map2STw;
		output << m_map2STpt;
		output << m_mapN0w;
		output << m_mapN0pt;

		output << m_mapSTwN0w;
		output << m_mapST2wN0w;
		output << m_map2STwN0w;
		output << m_mapSTwN0pt;
		output << m_mapSTptN0w;
		output << m_mapST2wN0pt;
		output << m_mapST2ptN0w;
		output << m_map2STwN0pt;
		output << m_map2STptN0w;
		output << m_mapSTptN0pt;
		output << m_mapST2ptN0pt;
		output << m_map2STptN0pt;

		output << m_mapSTwSTRHwN0w;
		output << m_mapSTwSTRDwN0w;
		output << m_mapSTwN0wN0LHw;
		output << m_mapSTwN0wN0LDw;

		output << m_mapST2wST2RHwN0w;
		output << m_mapST2wST2RDwN0w;
		output << m_mapST2wN0wN0LHw;
		output << m_mapST2wN0wN0LDw;

		output << m_map2STw2STRHwN0w;
		output << m_map2STw2STRDwN0w;
		output << m_map2STwN0wN0LHw;
		output << m_map2STwN0wN0LDw;

		output << m_mapSTwSTRHwN0pt;
		output << m_mapSTwSTRHptN0w;
		output << m_mapSTptSTRHwN0w;

		output << m_mapST2wST2RHwN0pt;
		output << m_mapST2wST2RHptN0w;
		output << m_mapST2ptST2RHwN0w;

		output << m_map2STw2STRHwN0pt;
		output << m_map2STw2STRHptN0w;
		output << m_map2STpt2STRHwN0w;

		output << m_mapSTwSTRDwN0pt;
		output << m_mapSTwSTRDptN0w;
		output << m_mapSTptSTRDwN0w;

		output << m_mapST2wST2RDwN0pt;
		output << m_mapST2wST2RDptN0w;
		output << m_mapST2ptST2RDwN0w;

		output << m_map2STw2STRDwN0pt;
		output << m_map2STw2STRDptN0w;
		output << m_map2STpt2STRDwN0w;

		output << m_mapSTwN0wN0LHpt;
		output << m_mapSTwN0ptN0LHw;
		output << m_mapSTptN0wN0LHw;

		output << m_mapST2wN0wN0LHpt;
		output << m_mapST2wN0ptN0LHw;
		output << m_mapST2ptN0wN0LHw;

		output << m_map2STwN0wN0LHpt;
		output << m_map2STwN0ptN0LHw;
		output << m_map2STptN0wN0LHw;

		output << m_mapSTwN0wN0LDpt;
		output << m_mapSTwN0ptN0LDw;
		output << m_mapSTptN0wN0LDw;

		output << m_mapST2wN0wN0LDpt;
		output << m_mapST2wN0ptN0LDw;
		output << m_mapST2ptN0wN0LDw;

		output << m_map2STwN0wN0LDpt;
		output << m_map2STwN0ptN0LDw;
		output << m_map2STptN0wN0LDw;

		output << m_mapSTwSTRHptN0pt;
		output << m_mapSTptSTRHptN0w;
		output << m_mapSTptSTRHwN0pt;

		output << m_mapST2wST2RHptN0pt;
		output << m_mapST2ptST2RHptN0w;
		output << m_mapST2ptST2RHwN0pt;

		output << m_map2STw2STRHptN0pt;
		output << m_map2STpt2STRHptN0w;
		output << m_map2STpt2STRHwN0pt;

		output << m_mapSTwSTRDptN0pt;
		output << m_mapSTptSTRDptN0w;
		output << m_mapSTptSTRDwN0pt;

		output << m_mapST2wST2RDptN0pt;
		output << m_mapST2ptST2RDptN0w;
		output << m_mapST2ptST2RDwN0pt;

		output << m_map2STw2STRDptN0pt;
		output << m_map2STpt2STRDptN0w;
		output << m_map2STpt2STRDwN0pt;

		output << m_mapSTwN0ptN0LHpt;
		output << m_mapSTptN0ptN0LHw;
		output << m_mapSTptN0wN0LHpt;

		output << m_mapST2wN0ptN0LHpt;
		output << m_mapST2ptN0ptN0LHw;
		output << m_mapST2ptN0wN0LHpt;

		output << m_map2STwN0ptN0LHpt;
		output << m_map2STptN0ptN0LHw;
		output << m_map2STptN0wN0LHpt;

		output << m_mapSTwN0ptN0LDpt;
		output << m_mapSTptN0ptN0LDw;
		output << m_mapSTptN0wN0LDpt;

		output << m_mapST2wN0ptN0LDpt;
		output << m_mapST2ptN0ptN0LDw;
		output << m_mapST2ptN0wN0LDpt;

		output << m_map2STwN0ptN0LDpt;
		output << m_map2STptN0ptN0LDw;
		output << m_map2STptN0wN0LDpt;

		output << m_mapSTptSTRHptN0pt;
		output << m_mapSTptSTRDptN0pt;
		output << m_mapSTptN0ptN0LHpt;
		output << m_mapSTptN0ptN0LDpt;

		output << m_mapST2ptST2RHptN0pt;
		output << m_mapST2ptST2RDptN0pt;
		output << m_mapST2ptN0ptN0LHpt;
		output << m_mapST2ptN0ptN0LDpt;

		output << m_map2STpt2STRHptN0pt;
		output << m_map2STpt2STRDptN0pt;
		output << m_map2STptN0ptN0LHpt;
		output << m_map2STptN0ptN0LDpt;

		output << m_mapSTPOSPath;
		output << m_mapST2POSPath;
		output << m_mapSTFPOSPath;
		output << m_mapST2FPOSPath;

		output << m_mapN_2w;
		output << m_mapN_2pt;
		output << m_mapN_1w;
		output << m_mapN_1pt;
		output << m_mapN1w;
		output << m_mapN1pt;
		output << m_mapN2w;
		output << m_mapN2pt;
		output << m_mapN_2wN_1w;
		output << m_mapN_1wN0w;
		output << m_mapN0wN1w;
		output << m_mapN1wN2w;
		output << m_mapN_2ptN_1pt;
		output << m_mapN_1ptN0pt;
		output << m_mapN0ptN1pt;
		output << m_mapN1ptN2pt;

		output << m_map1CharBefore;
		output << m_map2CharBefore;
		output << m_map3CharBefore;
		output << m_map1CharAfter;
		output << m_map2CharAfter;
		output << m_map3CharAfter;

		output.close();
	}

	void Weight::computeAverageFeatureWeights(const int & round) {
		m_mapSTw.computeAverage(round);
		m_mapSTpt.computeAverage(round);
		m_mapST2w.computeAverage(round);
		m_mapST2pt.computeAverage(round);
		m_map2STw.computeAverage(round);
		m_map2STpt.computeAverage(round);
		m_mapN0w.computeAverage(round);
		m_mapN0pt.computeAverage(round);

		m_mapSTwN0w.computeAverage(round);
		m_mapST2wN0w.computeAverage(round);
		m_map2STwN0w.computeAverage(round);
		m_mapSTwN0pt.computeAverage(round);
		m_mapSTptN0w.computeAverage(round);
		m_mapST2wN0pt.computeAverage(round);
		m_mapST2ptN0w.computeAverage(round);
		m_map2STwN0pt.computeAverage(round);
		m_map2STptN0w.computeAverage(round);
		m_mapSTptN0pt.computeAverage(round);
		m_mapST2ptN0pt.computeAverage(round);
		m_map2STptN0pt.computeAverage(round);

		m_mapSTwSTRHwN0w.computeAverage(round);
		m_mapSTwSTRDwN0w.computeAverage(round);
		m_mapSTwN0wN0LHw.computeAverage(round);
		m_mapSTwN0wN0LDw.computeAverage(round);

		m_mapST2wST2RHwN0w.computeAverage(round);
		m_mapST2wST2RDwN0w.computeAverage(round);
		m_mapST2wN0wN0LHw.computeAverage(round);
		m_mapST2wN0wN0LDw.computeAverage(round);

		m_map2STw2STRHwN0w.computeAverage(round);
		m_map2STw2STRDwN0w.computeAverage(round);
		m_map2STwN0wN0LHw.computeAverage(round);
		m_map2STwN0wN0LDw.computeAverage(round);

		m_mapSTwSTRHwN0pt.computeAverage(round);
		m_mapSTwSTRHptN0w.computeAverage(round);
		m_mapSTptSTRHwN0w.computeAverage(round);

		m_mapST2wST2RHwN0pt.computeAverage(round);
		m_mapST2wST2RHptN0w.computeAverage(round);
		m_mapST2ptST2RHwN0w.computeAverage(round);

		m_map2STw2STRHwN0pt.computeAverage(round);
		m_map2STw2STRHptN0w.computeAverage(round);
		m_map2STpt2STRHwN0w.computeAverage(round);

		m_mapSTwSTRDwN0pt.computeAverage(round);
		m_mapSTwSTRDptN0w.computeAverage(round);
		m_mapSTptSTRDwN0w.computeAverage(round);

		m_mapST2wST2RDwN0pt.computeAverage(round);
		m_mapST2wST2RDptN0w.computeAverage(round);
		m_mapST2ptST2RDwN0w.computeAverage(round);

		m_map2STw2STRDwN0pt.computeAverage(round);
		m_map2STw2STRDptN0w.computeAverage(round);
		m_map2STpt2STRDwN0w.computeAverage(round);

		m_mapSTwN0wN0LHpt.computeAverage(round);
		m_mapSTwN0ptN0LHw.computeAverage(round);
		m_mapSTptN0wN0LHw.computeAverage(round);

		m_mapST2wN0wN0LHpt.computeAverage(round);
		m_mapST2wN0ptN0LHw.computeAverage(round);
		m_mapST2ptN0wN0LHw.computeAverage(round);

		m_map2STwN0wN0LHpt.computeAverage(round);
		m_map2STwN0ptN0LHw.computeAverage(round);
		m_map2STptN0wN0LHw.computeAverage(round);

		m_mapSTwN0wN0LDpt.computeAverage(round);
		m_mapSTwN0ptN0LDw.computeAverage(round);
		m_mapSTptN0wN0LDw.computeAverage(round);

		m_mapST2wN0wN0LDpt.computeAverage(round);
		m_mapST2wN0ptN0LDw.computeAverage(round);
		m_mapST2ptN0wN0LDw.computeAverage(round);

		m_map2STwN0wN0LDpt.computeAverage(round);
		m_map2STwN0ptN0LDw.computeAverage(round);
		m_map2STptN0wN0LDw.computeAverage(round);

		m_mapSTwSTRHptN0pt.computeAverage(round);
		m_mapSTptSTRHptN0w.computeAverage(round);
		m_mapSTptSTRHwN0pt.computeAverage(round);

		m_mapST2wST2RHptN0pt.computeAverage(round);
		m_mapST2ptST2RHptN0w.computeAverage(round);
		m_mapST2ptST2RHwN0pt.computeAverage(round);

		m_map2STw2STRHptN0pt.computeAverage(round);
		m_map2STpt2STRHptN0w.computeAverage(round);
		m_map2STpt2STRHwN0pt.computeAverage(round);

		m_mapSTwSTRDptN0pt.computeAverage(round);
		m_mapSTptSTRDptN0w.computeAverage(round);
		m_mapSTptSTRDwN0pt.computeAverage(round);

		m_mapST2wST2RDptN0pt.computeAverage(round);
		m_mapST2ptST2RDptN0w.computeAverage(round);
		m_mapST2ptST2RDwN0pt.computeAverage(round);

		m_map2STw2STRDptN0pt.computeAverage(round);
		m_map2STpt2STRDptN0w.computeAverage(round);
		m_map2STpt2STRDwN0pt.computeAverage(round);

		m_mapSTwN0ptN0LHpt.computeAverage(round);
		m_mapSTptN0ptN0LHw.computeAverage(round);
		m_mapSTptN0wN0LHpt.computeAverage(round);

		m_mapST2wN0ptN0LHpt.computeAverage(round);
		m_mapST2ptN0ptN0LHw.computeAverage(round);
		m_mapST2ptN0wN0LHpt.computeAverage(round);

		m_map2STwN0ptN0LHpt.computeAverage(round);
		m_map2STptN0ptN0LHw.computeAverage(round);
		m_map2STptN0wN0LHpt.computeAverage(round);

		m_mapSTwN0ptN0LDpt.computeAverage(round);
		m_mapSTptN0ptN0LDw.computeAverage(round);
		m_mapSTptN0wN0LDpt.computeAverage(round);

		m_mapST2wN0ptN0LDpt.computeAverage(round);
		m_mapST2ptN0ptN0LDw.computeAverage(round);
		m_mapST2ptN0wN0LDpt.computeAverage(round);

		m_map2STwN0ptN0LDpt.computeAverage(round);
		m_map2STptN0ptN0LDw.computeAverage(round);
		m_map2STptN0wN0LDpt.computeAverage(round);

		m_mapSTptSTRHptN0pt.computeAverage(round);
		m_mapSTptSTRDptN0pt.computeAverage(round);
		m_mapSTptN0ptN0LHpt.computeAverage(round);
		m_mapSTptN0ptN0LDpt.computeAverage(round);

		m_mapST2ptST2RHptN0pt.computeAverage(round);
		m_mapST2ptST2RDptN0pt.computeAverage(round);
		m_mapST2ptN0ptN0LHpt.computeAverage(round);
		m_mapST2ptN0ptN0LDpt.computeAverage(round);

		m_map2STpt2STRHptN0pt.computeAverage(round);
		m_map2STpt2STRDptN0pt.computeAverage(round);
		m_map2STptN0ptN0LHpt.computeAverage(round);
		m_map2STptN0ptN0LDpt.computeAverage(round);

		m_mapSTPOSPath.computeAverage(round);
		m_mapST2POSPath.computeAverage(round);
		m_mapSTFPOSPath.computeAverage(round);
		m_mapST2FPOSPath.computeAverage(round);

		m_mapN_2w.computeAverage(round);
		m_mapN_2pt.computeAverage(round);
		m_mapN_1w.computeAverage(round);
		m_mapN_1pt.computeAverage(round);
		m_mapN1w.computeAverage(round);
		m_mapN1pt.computeAverage(round);
		m_mapN2w.computeAverage(round);
		m_mapN2pt.computeAverage(round);
		m_mapN_2wN_1w.computeAverage(round);
		m_mapN_1wN0w.computeAverage(round);
		m_mapN0wN1w.computeAverage(round);
		m_mapN1wN2w.computeAverage(round);
		m_mapN_2ptN_1pt.computeAverage(round);
		m_mapN_1ptN0pt.computeAverage(round);
		m_mapN0ptN1pt.computeAverage(round);
		m_mapN1ptN2pt.computeAverage(round);

		m_map1CharBefore.computeAverage(round);
		m_map2CharBefore.computeAverage(round);
		m_map3CharBefore.computeAverage(round);
		m_map1CharAfter.computeAverage(round);
		m_map2CharAfter.computeAverage(round);
		m_map3CharAfter.computeAverage(round);
	}
}