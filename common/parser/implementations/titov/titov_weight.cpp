#include <fstream>

#include "titov_weight.h"
#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace titov {
	Weight::Weight(const std::string & sRead, const std::string & sRecord) :
		WeightBase(sRead, sRecord),
		// uni-gram
		// st, n0, st2
		m_mapSTw("m_mapSTw"),
		m_mapSTpt("m_mapSTpt"),
		m_mapST2w("m_mapST2w"),
		m_mapST2pt("m_mapST2pt"),
		m_mapN0w("m_mapN0w"),
		m_mapN0pt("m_mapN0pt"),

		// unigram context
		// st + sti, n0 + n0i
		m_mapSTwSTiw("m_mapSTwSTiw"),
		m_mapSTwSTiwSTjw("m_mapSTwSTiwSTjw"),
		m_mapN0wN0iw("m_mapN0wN0iw"),
		m_mapN0wN0iwN0jw("m_mapN0wN0iwN0jw"),
		m_mapSTptSTipt("m_mapSTptSTipt"),
		m_mapSTptSTiptSTjpt("m_mapSTptSTiptSTjpt"),
		m_mapN0ptN0ipt("m_mapN0ptN0ipt"),
		m_mapN0ptN0iptN0jpt("m_mapN0ptN0iptN0jpt"),

		// unigram with arity
		// st, n0, st2
		m_mapSTwSTLHi("m_mapSTwSTLHi"),
		m_mapSTwSTLPi("m_mapSTwSTLPi"),
		m_mapSTwSTRHi("m_mapSTwSTRHi"),
		m_mapSTwSTRPi("m_mapSTwSTRPi"),
		m_mapN0wN0LHi("m_mapN0wN0LHi"),
		m_mapN0wN0LPi("m_mapN0wN0LPi"),

		// bi-gram
		// st + n0
		m_mapSTwN0w("m_mapSTwN0w"),
		m_mapSTwN0pt("m_mapSTwN0pt"),
		m_mapSTptN0w("m_mapSTptN0w"),
		m_mapSTptN0pt("m_mapSTptN0pt"),
		m_mapSTwptN0w("m_mapSTwptN0w"),
		m_mapSTwN0wpt("m_mapSTwN0wpt"),
		m_mapSTwptN0pt("m_mapSTwptN0pt"),
		m_mapSTptN0wpt("m_mapSTptN0wpt"),
		m_mapSTwptN0wpt("m_mapSTwptN0wpt"),
		// st + n0 + dis
		m_mapSTwN0wD("m_mapSTwN0wD"),
		m_mapSTwN0ptD("m_mapSTwN0ptD"),
		m_mapSTptN0wD("m_mapSTptN0wD"),
		m_mapSTptN0ptD("m_mapSTptN0ptD"),
		// st + n0 + st left/right head/pred
		m_mapSTwN0wSTLHi("m_mapSTwN0wSTLHi"),
		m_mapSTwN0wSTLPi("m_mapSTwN0wSTLPi"),
		m_mapSTwN0wSTRHi("m_mapSTwN0wSTRHi"),
		m_mapSTwN0wSTRPi("m_mapSTwN0wSTRPi"),
		m_mapSTwN0wN0LHi("m_mapSTwN0wN0LHi"),
		m_mapSTwN0wN0LPi("m_mapSTwN0wN0LPi"),

		// st2 + n0
		m_mapST2wN0w("m_mapST2wN0w"),
		m_mapST2wN0pt("m_mapST2wN0pt"),
		m_mapST2ptN0w("m_mapST2ptN0w"),
		m_mapST2ptN0pt("m_mapST2ptN0pt"),
		m_mapST2wptN0w("m_mapST2wptN0w"),
		m_mapST2wN0wpt("m_mapST2wN0wpt"),
		m_mapST2wptN0pt("m_mapST2wptN0pt"),
		m_mapST2ptN0wpt("m_mapST2ptN0wpt"),
		m_mapST2wptN0wpt("m_mapST2wptN0wpt"),
		// st2 + n0 + dis
		m_mapST2wN0wD("m_mapST2wN0wD"),
		m_mapST2wN0ptD("m_mapST2wN0ptD"),
		m_mapST2ptN0wD("m_mapST2ptN0wD"),
		m_mapST2ptN0ptD("m_mapST2ptN0ptD"),
		// st2 + n0 + st2 left/right head/pred
		m_mapST2wN0wST2LHi("m_mapST2wN0wST2LHi"),
		m_mapST2wN0wST2LPi("m_mapST2wN0wST2LPi"),
		m_mapST2wN0wST2RHi("m_mapST2wN0wST2RHi"),
		m_mapST2wN0wST2RPi("m_mapST2wN0wST2RPi"),
		m_mapST2wN0wN0LHi("m_mapST2wN0wN0LHi"),
		m_mapST2wN0wN0LPi("m_mapST2wN0wN0LPi"),

		// st + st2
		m_mapSTwST2w("m_mapSTwST2w"),
		m_mapSTwST2pt("m_mapSTwST2pt"),
		m_mapSTptST2w("m_mapSTptST2w"),
		m_mapSTptST2pt("m_mapSTptST2pt"),
		m_mapSTwptST2w("m_mapSTwptST2w"),
		m_mapSTwST2wpt("m_mapSTwST2wpt"),
		m_mapSTwptST2pt("m_mapSTwptST2pt"),
		m_mapSTptST2wpt("m_mapSTptST2wpt"),
		m_mapSTwptST2wpt("m_mapSTwptST2wpt"),

		// tri-gram
		// st + n0 + st2
		m_mapSTwN0ptST2pt("m_mapSTwN0ptST2pt"),
		m_mapSTptN0wST2pt("m_mapSTptN0wST2pt"),
		m_mapSTptN0ptST2w("m_mapSTptN0ptST2w"),
		m_mapSTptN0ptST2pt("m_mapSTptN0ptST2pt"),

		// st + n0 + st left/right head/pred
		m_mapSTwN0ptSTLHpt("m_mapSTwN0ptSTLHpt"),
		m_mapSTptN0wSTLHpt("m_mapSTptN0wSTLHpt"),
		m_mapSTptN0ptSTLHw("m_mapSTptN0ptSTLHw"),
		m_mapSTptN0ptSTLHpt("m_mapSTptN0ptSTLHpt"),
		m_mapSTwN0ptSTLPpt("m_mapSTwN0ptSTLPpt"),
		m_mapSTptN0wSTLPpt("m_mapSTptN0wSTLPpt"),
		m_mapSTptN0ptSTLPw("m_mapSTptN0ptSTLPw"),
		m_mapSTptN0ptSTLPpt("m_mapSTptN0ptSTLPpt"),
		m_mapSTwN0ptSTRHpt("m_mapSTwN0ptSTRHpt"),
		m_mapSTptN0wSTRHpt("m_mapSTptN0wSTRHpt"),
		m_mapSTptN0ptSTRHw("m_mapSTptN0ptSTRHw"),
		m_mapSTptN0ptSTRHpt("m_mapSTptN0ptSTRHpt"),
		m_mapSTwN0ptSTRPpt("m_mapSTwN0ptSTRPpt"),
		m_mapSTptN0wSTRPpt("m_mapSTptN0wSTRPpt"),
		m_mapSTptN0ptSTRPw("m_mapSTptN0ptSTRPw"),
		m_mapSTptN0ptSTRPpt("m_mapSTptN0ptSTRPpt"),

		// st + n0 + n0 right head/pred
		m_mapSTwN0ptN0LHpt("m_mapSTwN0ptN0LHpt"),
		m_mapSTptN0wN0LHpt("m_mapSTptN0wN0LHpt"),
		m_mapSTptN0ptN0LHw("m_mapSTptN0ptN0LHw"),
		m_mapSTptN0ptN0LHpt("m_mapSTptN0ptN0LHpt"),
		m_mapSTwN0ptN0LPpt("m_mapSTwN0ptN0LPpt"),
		m_mapSTptN0wN0LPpt("m_mapSTptN0wN0LPpt"),
		m_mapSTptN0ptN0LPw("m_mapSTptN0ptN0LPw"),
		m_mapSTptN0ptN0LPpt("m_mapSTptN0ptN0LPpt"),

		// st2 + n0 + st2 left/right head/pred
		m_mapST2wN0ptST2LHpt("m_mapST2wN0ptST2LHpt"),
		m_mapST2ptN0wST2LHpt("m_mapST2ptN0wST2LHpt"),
		m_mapST2ptN0ptST2LHw("m_mapST2ptN0ptST2LHw"),
		m_mapST2ptN0ptST2LHpt("m_mapST2ptN0ptST2LHpt"),
		m_mapST2wN0ptST2LPpt("m_mapST2wN0ptST2LPpt"),
		m_mapST2ptN0wST2LPpt("m_mapST2ptN0wST2LPpt"),
		m_mapST2ptN0ptST2LPw("m_mapST2ptN0ptST2LPw"),
		m_mapST2ptN0ptST2LPpt("m_mapST2ptN0ptST2LPpt"),
		m_mapST2wN0ptST2RHpt("m_mapST2wN0ptST2RHpt"),
		m_mapST2ptN0wST2RHpt("m_mapST2ptN0wST2RHpt"),
		m_mapST2ptN0ptST2RHw("m_mapST2ptN0ptST2RHw"),
		m_mapST2ptN0ptST2RHpt("m_mapST2ptN0ptST2RHpt"),
		m_mapST2wN0ptST2RPpt("m_mapST2wN0ptST2RPpt"),
		m_mapST2ptN0wST2RPpt("m_mapST2ptN0wST2RPpt"),
		m_mapST2ptN0ptST2RPw("m_mapST2ptN0ptST2RPw"),
		m_mapST2ptN0ptST2RPpt("m_mapST2ptN0ptST2RPpt"),

		// st2 + n0 + n0 right head/pred
		m_mapST2wN0ptN0LHpt("m_mapST2wN0ptN0LHpt"),
		m_mapST2ptN0wN0LHpt("m_mapST2ptN0wN0LHpt"),
		m_mapST2ptN0ptN0LHw("m_mapST2ptN0ptN0LHw"),
		m_mapST2ptN0ptN0LHpt("m_mapST2ptN0ptN0LHpt"),
		m_mapST2wN0ptN0LPpt("m_mapST2wN0ptN0LPpt"),
		m_mapST2ptN0wN0LPpt("m_mapST2ptN0wN0LPpt"),
		m_mapST2ptN0ptN0LPw("m_mapST2ptN0ptN0LPw"),
		m_mapST2ptN0ptN0LPpt("m_mapST2ptN0ptN0LPpt"),

		// quar-gram
		// st + n0 + st left head + st left pred
		m_mapSTptN0ptSTLHptSTLPpt("m_mapSTptN0ptSTLHptSTLPpt"),
		// st + n0 + st right head + st right pred
		m_mapSTptN0ptSTRHptSTRPpt("m_mapSTptN0ptSTRHptSTRPpt"),
		// st + n0 + st left pred + st left pred 2
		m_mapSTptN0ptSTLPptSTLP2pt("m_mapSTptN0ptSTLPptSTLP2pt"),
		// st + n0 + st right pred + st right pred 2
		m_mapSTptN0ptSTRPptSTRP2pt("m_mapSTptN0ptSTRPptSTRP2pt"),
		// st + n0 + n0 left pred + n0 left pred 2
		m_mapSTptN0ptN0LPptN0LP2pt("m_mapSTptN0ptN0LPptN0LP2pt"),

		// char feature
		// for chinese
		m_map1CharBefore("m_map1CharBefore"),
		m_map2CharBefore("m_map2CharBefore"),
		m_map3CharBefore("m_map3CharBefore"),
		m_map1CharAfter("m_map1CharAfter"),
		m_map2CharAfter("m_map2CharAfter"),
		m_map3CharAfter("m_map3CharAfter"),

		// string feature
		m_mapSTPOSPath("m_mapSTPOSPath"),
		m_mapST2POSPath("m_mapST2POSPath"),
		m_mapSTFPOSPath("m_mapSTFPOSPath"),
		m_mapST2FPOSPath("m_mapST2FPOSPath")
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

		// uni-gram
		// st, n0, st2
		input >> m_mapSTw;
		input >> m_mapSTpt;
		input >> m_mapST2w;
		input >> m_mapST2pt;
		input >> m_mapN0w;
		input >> m_mapN0pt;

		// unigram context
		// st + sti, n0 + n0i
		input >> m_mapSTwSTiw;
		input >> m_mapSTwSTiwSTjw;
		input >> m_mapN0wN0iw;
		input >> m_mapN0wN0iwN0jw;
		input >> m_mapSTptSTipt;
		input >> m_mapSTptSTiptSTjpt;
		input >> m_mapN0ptN0ipt;
		input >> m_mapN0ptN0iptN0jpt;

		// unigram with arity
		// st, n0, st2
		input >> m_mapSTwSTLHi;
		input >> m_mapSTwSTLPi;
		input >> m_mapSTwSTRHi;
		input >> m_mapSTwSTRPi;
		input >> m_mapN0wN0LHi;
		input >> m_mapN0wN0LPi;

		// bi-gram
		// st + n0
		input >> m_mapSTwN0w;
		input >> m_mapSTwN0pt;
		input >> m_mapSTptN0w;
		input >> m_mapSTptN0pt;
		input >> m_mapSTwptN0w;
		input >> m_mapSTwN0wpt;
		input >> m_mapSTwptN0pt;
		input >> m_mapSTptN0wpt;
		input >> m_mapSTwptN0wpt;
		// st + n0 + dis
		input >> m_mapSTwN0wD;
		input >> m_mapSTwN0ptD;
		input >> m_mapSTptN0wD;
		input >> m_mapSTptN0ptD;
		// st + n0 + st left/right head/pred
		input >> m_mapSTwN0wSTLHi;
		input >> m_mapSTwN0wSTLPi;
		input >> m_mapSTwN0wSTRHi;
		input >> m_mapSTwN0wSTRPi;
		input >> m_mapSTwN0wN0LHi;
		input >> m_mapSTwN0wN0LPi;

		// st2 + n0
		input >> m_mapST2wN0w;
		input >> m_mapST2wN0pt;
		input >> m_mapST2ptN0w;
		input >> m_mapST2ptN0pt;
		input >> m_mapST2wptN0w;
		input >> m_mapST2wN0wpt;
		input >> m_mapST2wptN0pt;
		input >> m_mapST2ptN0wpt;
		input >> m_mapST2wptN0wpt;
		// st2 + n0 + dis
		input >> m_mapST2wN0wD;
		input >> m_mapST2wN0ptD;
		input >> m_mapST2ptN0wD;
		input >> m_mapST2ptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		input >> m_mapST2wN0wST2LHi;
		input >> m_mapST2wN0wST2LPi;
		input >> m_mapST2wN0wST2RHi;
		input >> m_mapST2wN0wST2RPi;
		input >> m_mapST2wN0wN0LHi;
		input >> m_mapST2wN0wN0LPi;

		// st + st2
		input >> m_mapSTwST2w;
		input >> m_mapSTwST2pt;
		input >> m_mapSTptST2w;
		input >> m_mapSTptST2pt;
		input >> m_mapSTwptST2w;
		input >> m_mapSTwST2wpt;
		input >> m_mapSTwptST2pt;
		input >> m_mapSTptST2wpt;
		input >> m_mapSTwptST2wpt;

		// tri-gram
		// st + n0 + st2
		input >> m_mapSTwN0ptST2pt;
		input >> m_mapSTptN0wST2pt;
		input >> m_mapSTptN0ptST2w;
		input >> m_mapSTptN0ptST2pt;

		// st + n0 + st left/right head/pred
		input >> m_mapSTwN0ptSTLHpt;
		input >> m_mapSTptN0wSTLHpt;
		input >> m_mapSTptN0ptSTLHw;
		input >> m_mapSTptN0ptSTLHpt;
		input >> m_mapSTwN0ptSTLPpt;
		input >> m_mapSTptN0wSTLPpt;
		input >> m_mapSTptN0ptSTLPw;
		input >> m_mapSTptN0ptSTLPpt;
		input >> m_mapSTwN0ptSTRHpt;
		input >> m_mapSTptN0wSTRHpt;
		input >> m_mapSTptN0ptSTRHw;
		input >> m_mapSTptN0ptSTRHpt;
		input >> m_mapSTwN0ptSTRPpt;
		input >> m_mapSTptN0wSTRPpt;
		input >> m_mapSTptN0ptSTRPw;
		input >> m_mapSTptN0ptSTRPpt;

		// st + n0 + n0 right head/pred
		input >> m_mapSTwN0ptN0LHpt;
		input >> m_mapSTptN0wN0LHpt;
		input >> m_mapSTptN0ptN0LHw;
		input >> m_mapSTptN0ptN0LHpt;
		input >> m_mapSTwN0ptN0LPpt;
		input >> m_mapSTptN0wN0LPpt;
		input >> m_mapSTptN0ptN0LPw;
		input >> m_mapSTptN0ptN0LPpt;

		// st2 + n0 + st2 left/right head/pred
		input >> m_mapST2wN0ptST2LHpt;
		input >> m_mapST2ptN0wST2LHpt;
		input >> m_mapST2ptN0ptST2LHw;
		input >> m_mapST2ptN0ptST2LHpt;
		input >> m_mapST2wN0ptST2LPpt;
		input >> m_mapST2ptN0wST2LPpt;
		input >> m_mapST2ptN0ptST2LPw;
		input >> m_mapST2ptN0ptST2LPpt;
		input >> m_mapST2wN0ptST2RHpt;
		input >> m_mapST2ptN0wST2RHpt;
		input >> m_mapST2ptN0ptST2RHw;
		input >> m_mapST2ptN0ptST2RHpt;
		input >> m_mapST2wN0ptST2RPpt;
		input >> m_mapST2ptN0wST2RPpt;
		input >> m_mapST2ptN0ptST2RPw;
		input >> m_mapST2ptN0ptST2RPpt;

		// st2 + n0 + n0 right head/pred
		input >> m_mapST2wN0ptN0LHpt;
		input >> m_mapST2ptN0wN0LHpt;
		input >> m_mapST2ptN0ptN0LHw;
		input >> m_mapST2ptN0ptN0LHpt;
		input >> m_mapST2wN0ptN0LPpt;
		input >> m_mapST2ptN0wN0LPpt;
		input >> m_mapST2ptN0ptN0LPw;
		input >> m_mapST2ptN0ptN0LPpt;

		// quar-gram
		// st + n0 + st left head + st left pred
		input >> m_mapSTptN0ptSTLHptSTLPpt;
		// st + n0 + st right head + st right pred
		input >> m_mapSTptN0ptSTRHptSTRPpt;
		// st + n0 + st left pred + st left pred 2
		input >> m_mapSTptN0ptSTLPptSTLP2pt;
		// st + n0 + st right pred + st right pred 2
		input >> m_mapSTptN0ptSTRPptSTRP2pt;
		// st + n0 + n0 left pred + n0 left pred 2
		input >> m_mapSTptN0ptN0LPptN0LP2pt;

		// char feature
		// for chinese
		input >> m_map1CharBefore;
		input >> m_map2CharBefore;
		input >> m_map3CharBefore;
		input >> m_map1CharAfter;
		input >> m_map2CharAfter;
		input >> m_map3CharAfter;

		// string feature
		input >> m_mapSTPOSPath;
		input >> m_mapST2POSPath;
		input >> m_mapSTFPOSPath;
		input >> m_mapST2FPOSPath;

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

		// uni-gram
		// st, n0, st2
		output << m_mapSTw;
		output << m_mapSTpt;
		output << m_mapST2w;
		output << m_mapST2pt;
		output << m_mapN0w;
		output << m_mapN0pt;

		// unigram context
		// st + sti, n0 + n0i
		output << m_mapSTwSTiw;
		output << m_mapSTwSTiwSTjw;
		output << m_mapN0wN0iw;
		output << m_mapN0wN0iwN0jw;
		output << m_mapSTptSTipt;
		output << m_mapSTptSTiptSTjpt;
		output << m_mapN0ptN0ipt;
		output << m_mapN0ptN0iptN0jpt;

		// unigram with arity
		// st, n0, st2
		output << m_mapSTwSTLHi;
		output << m_mapSTwSTLPi;
		output << m_mapSTwSTRHi;
		output << m_mapSTwSTRPi;
		output << m_mapN0wN0LHi;
		output << m_mapN0wN0LPi;

		// bi-gram
		// st + n0
		output << m_mapSTwN0w;
		output << m_mapSTwN0pt;
		output << m_mapSTptN0w;
		output << m_mapSTptN0pt;
		output << m_mapSTwptN0w;
		output << m_mapSTwN0wpt;
		output << m_mapSTwptN0pt;
		output << m_mapSTptN0wpt;
		output << m_mapSTwptN0wpt;
		// st + n0 + dis
		output << m_mapSTwN0wD;
		output << m_mapSTwN0ptD;
		output << m_mapSTptN0wD;
		output << m_mapSTptN0ptD;
		// st + n0 + st left/right head/pred
		output << m_mapSTwN0wSTLHi;
		output << m_mapSTwN0wSTLPi;
		output << m_mapSTwN0wSTRHi;
		output << m_mapSTwN0wSTRPi;
		output << m_mapSTwN0wN0LHi;
		output << m_mapSTwN0wN0LPi;

		// st2 + n0
		output << m_mapST2wN0w;
		output << m_mapST2wN0pt;
		output << m_mapST2ptN0w;
		output << m_mapST2ptN0pt;
		output << m_mapST2wptN0w;
		output << m_mapST2wN0wpt;
		output << m_mapST2wptN0pt;
		output << m_mapST2ptN0wpt;
		output << m_mapST2wptN0wpt;
		// st2 + n0 + dis
		output << m_mapST2wN0wD;
		output << m_mapST2wN0ptD;
		output << m_mapST2ptN0wD;
		output << m_mapST2ptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		output << m_mapST2wN0wST2LHi;
		output << m_mapST2wN0wST2LPi;
		output << m_mapST2wN0wST2RHi;
		output << m_mapST2wN0wST2RPi;
		output << m_mapST2wN0wN0LHi;
		output << m_mapST2wN0wN0LPi;

		// st + st2
		output << m_mapSTwST2w;
		output << m_mapSTwST2pt;
		output << m_mapSTptST2w;
		output << m_mapSTptST2pt;
		output << m_mapSTwptST2w;
		output << m_mapSTwST2wpt;
		output << m_mapSTwptST2pt;
		output << m_mapSTptST2wpt;
		output << m_mapSTwptST2wpt;

		// tri-gram
		// st + n0 + st2
		output << m_mapSTwN0ptST2pt;
		output << m_mapSTptN0wST2pt;
		output << m_mapSTptN0ptST2w;
		output << m_mapSTptN0ptST2pt;

		// st + n0 + st left/right head/pred
		output << m_mapSTwN0ptSTLHpt;
		output << m_mapSTptN0wSTLHpt;
		output << m_mapSTptN0ptSTLHw;
		output << m_mapSTptN0ptSTLHpt;
		output << m_mapSTwN0ptSTLPpt;
		output << m_mapSTptN0wSTLPpt;
		output << m_mapSTptN0ptSTLPw;
		output << m_mapSTptN0ptSTLPpt;
		output << m_mapSTwN0ptSTRHpt;
		output << m_mapSTptN0wSTRHpt;
		output << m_mapSTptN0ptSTRHw;
		output << m_mapSTptN0ptSTRHpt;
		output << m_mapSTwN0ptSTRPpt;
		output << m_mapSTptN0wSTRPpt;
		output << m_mapSTptN0ptSTRPw;
		output << m_mapSTptN0ptSTRPpt;

		// st + n0 + n0 right head/pred
		output << m_mapSTwN0ptN0LHpt;
		output << m_mapSTptN0wN0LHpt;
		output << m_mapSTptN0ptN0LHw;
		output << m_mapSTptN0ptN0LHpt;
		output << m_mapSTwN0ptN0LPpt;
		output << m_mapSTptN0wN0LPpt;
		output << m_mapSTptN0ptN0LPw;
		output << m_mapSTptN0ptN0LPpt;

		// st2 + n0 + st2 left/right head/pred
		output << m_mapST2wN0ptST2LHpt;
		output << m_mapST2ptN0wST2LHpt;
		output << m_mapST2ptN0ptST2LHw;
		output << m_mapST2ptN0ptST2LHpt;
		output << m_mapST2wN0ptST2LPpt;
		output << m_mapST2ptN0wST2LPpt;
		output << m_mapST2ptN0ptST2LPw;
		output << m_mapST2ptN0ptST2LPpt;
		output << m_mapST2wN0ptST2RHpt;
		output << m_mapST2ptN0wST2RHpt;
		output << m_mapST2ptN0ptST2RHw;
		output << m_mapST2ptN0ptST2RHpt;
		output << m_mapST2wN0ptST2RPpt;
		output << m_mapST2ptN0wST2RPpt;
		output << m_mapST2ptN0ptST2RPw;
		output << m_mapST2ptN0ptST2RPpt;

		// st2 + n0 + n0 right head/pred
		output << m_mapST2wN0ptN0LHpt;
		output << m_mapST2ptN0wN0LHpt;
		output << m_mapST2ptN0ptN0LHw;
		output << m_mapST2ptN0ptN0LHpt;
		output << m_mapST2wN0ptN0LPpt;
		output << m_mapST2ptN0wN0LPpt;
		output << m_mapST2ptN0ptN0LPw;
		output << m_mapST2ptN0ptN0LPpt;

		// quar-gram
		// st + n0 + st left head + st left pred
		output << m_mapSTptN0ptSTLHptSTLPpt;
		// st + n0 + st right head + st right pred
		output << m_mapSTptN0ptSTRHptSTRPpt;
		// st + n0 + st left pred + st left pred 2
		output << m_mapSTptN0ptSTLPptSTLP2pt;
		// st + n0 + st right pred + st right pred 2
		output << m_mapSTptN0ptSTRPptSTRP2pt;
		// st + n0 + n0 left pred + n0 left pred 2
		output << m_mapSTptN0ptN0LPptN0LP2pt;

		// char feature
		// for chinese
		output << m_map1CharBefore;
		output << m_map2CharBefore;
		output << m_map3CharBefore;
		output << m_map1CharAfter;
		output << m_map2CharAfter;
		output << m_map3CharAfter;

		// string feature
		output << m_mapSTPOSPath;
		output << m_mapST2POSPath;
		output << m_mapSTFPOSPath;
		output << m_mapST2FPOSPath;

		output.close();
	}

	void Weight::computeAverageFeatureWeights(const int & round) {
		// uni-gram
		// st, n0, st2
		m_mapSTw.computeAverage(round);
		m_mapSTpt.computeAverage(round);
		m_mapST2w.computeAverage(round);
		m_mapST2pt.computeAverage(round);
		m_mapN0w.computeAverage(round);
		m_mapN0pt.computeAverage(round);

		// unigram context
		// st + sti, n0 + n0i
		m_mapSTwSTiw.computeAverage(round);
		m_mapSTwSTiwSTjw.computeAverage(round);
		m_mapN0wN0iw.computeAverage(round);
		m_mapN0wN0iwN0jw.computeAverage(round);
		m_mapSTptSTipt.computeAverage(round);
		m_mapSTptSTiptSTjpt.computeAverage(round);
		m_mapN0ptN0ipt.computeAverage(round);
		m_mapN0ptN0iptN0jpt.computeAverage(round);

		// unigram with arity
		// st, n0, st2
		m_mapSTwSTLHi.computeAverage(round);
		m_mapSTwSTLPi.computeAverage(round);
		m_mapSTwSTRHi.computeAverage(round);
		m_mapSTwSTRPi.computeAverage(round);
		m_mapN0wN0LHi.computeAverage(round);
		m_mapN0wN0LPi.computeAverage(round);

		// bi-gram
		// st + n0
		m_mapSTwN0w.computeAverage(round);
		m_mapSTwN0pt.computeAverage(round);
		m_mapSTptN0w.computeAverage(round);
		m_mapSTptN0pt.computeAverage(round);
		m_mapSTwptN0w.computeAverage(round);
		m_mapSTwN0wpt.computeAverage(round);
		m_mapSTwptN0pt.computeAverage(round);
		m_mapSTptN0wpt.computeAverage(round);
		m_mapSTwptN0wpt.computeAverage(round);
		// st + n0 + dis
		m_mapSTwN0wD.computeAverage(round);
		m_mapSTwN0ptD.computeAverage(round);
		m_mapSTptN0wD.computeAverage(round);
		m_mapSTptN0ptD.computeAverage(round);
		// st + n0 + st left/right head/pred
		m_mapSTwN0wSTLHi.computeAverage(round);
		m_mapSTwN0wSTLPi.computeAverage(round);
		m_mapSTwN0wSTRHi.computeAverage(round);
		m_mapSTwN0wSTRPi.computeAverage(round);
		m_mapSTwN0wN0LHi.computeAverage(round);
		m_mapSTwN0wN0LPi.computeAverage(round);

		// st2 + n0
		m_mapST2wN0w.computeAverage(round);
		m_mapST2wN0pt.computeAverage(round);
		m_mapST2ptN0w.computeAverage(round);
		m_mapST2ptN0pt.computeAverage(round);
		m_mapST2wptN0w.computeAverage(round);
		m_mapST2wN0wpt.computeAverage(round);
		m_mapST2wptN0pt.computeAverage(round);
		m_mapST2ptN0wpt.computeAverage(round);
		m_mapST2wptN0wpt.computeAverage(round);
		// st2 + n0 + dis
		m_mapST2wN0wD.computeAverage(round);
		m_mapST2wN0ptD.computeAverage(round);
		m_mapST2ptN0wD.computeAverage(round);
		m_mapST2ptN0ptD.computeAverage(round);
		// st2 + n0 + st2 left/right head/pred
		m_mapST2wN0wST2LHi.computeAverage(round);
		m_mapST2wN0wST2LPi.computeAverage(round);
		m_mapST2wN0wST2RHi.computeAverage(round);
		m_mapST2wN0wST2RPi.computeAverage(round);
		m_mapST2wN0wN0LHi.computeAverage(round);
		m_mapST2wN0wN0LPi.computeAverage(round);

		// st + st2
		m_mapSTwST2w.computeAverage(round);
		m_mapSTwST2pt.computeAverage(round);
		m_mapSTptST2w.computeAverage(round);
		m_mapSTptST2pt.computeAverage(round);
		m_mapSTwptST2w.computeAverage(round);
		m_mapSTwST2wpt.computeAverage(round);
		m_mapSTwptST2pt.computeAverage(round);
		m_mapSTptST2wpt.computeAverage(round);
		m_mapSTwptST2wpt.computeAverage(round);

		// tri-gram
		// st + n0 + st2
		m_mapSTwN0ptST2pt.computeAverage(round);
		m_mapSTptN0wST2pt.computeAverage(round);
		m_mapSTptN0ptST2w.computeAverage(round);
		m_mapSTptN0ptST2pt.computeAverage(round);

		// st + n0 + st left/right head/pred
		m_mapSTwN0ptSTLHpt.computeAverage(round);
		m_mapSTptN0wSTLHpt.computeAverage(round);
		m_mapSTptN0ptSTLHw.computeAverage(round);
		m_mapSTptN0ptSTLHpt.computeAverage(round);
		m_mapSTwN0ptSTLPpt.computeAverage(round);
		m_mapSTptN0wSTLPpt.computeAverage(round);
		m_mapSTptN0ptSTLPw.computeAverage(round);
		m_mapSTptN0ptSTLPpt.computeAverage(round);
		m_mapSTwN0ptSTRHpt.computeAverage(round);
		m_mapSTptN0wSTRHpt.computeAverage(round);
		m_mapSTptN0ptSTRHw.computeAverage(round);
		m_mapSTptN0ptSTRHpt.computeAverage(round);
		m_mapSTwN0ptSTRPpt.computeAverage(round);
		m_mapSTptN0wSTRPpt.computeAverage(round);
		m_mapSTptN0ptSTRPw.computeAverage(round);
		m_mapSTptN0ptSTRPpt.computeAverage(round);

		// st + n0 + n0 right head/pred
		m_mapSTwN0ptN0LHpt.computeAverage(round);
		m_mapSTptN0wN0LHpt.computeAverage(round);
		m_mapSTptN0ptN0LHw.computeAverage(round);
		m_mapSTptN0ptN0LHpt.computeAverage(round);
		m_mapSTwN0ptN0LPpt.computeAverage(round);
		m_mapSTptN0wN0LPpt.computeAverage(round);
		m_mapSTptN0ptN0LPw.computeAverage(round);
		m_mapSTptN0ptN0LPpt.computeAverage(round);

		// st2 + n0 + st2 left/right head/pred
		m_mapST2wN0ptST2LHpt.computeAverage(round);
		m_mapST2ptN0wST2LHpt.computeAverage(round);
		m_mapST2ptN0ptST2LHw.computeAverage(round);
		m_mapST2ptN0ptST2LHpt.computeAverage(round);
		m_mapST2wN0ptST2LPpt.computeAverage(round);
		m_mapST2ptN0wST2LPpt.computeAverage(round);
		m_mapST2ptN0ptST2LPw.computeAverage(round);
		m_mapST2ptN0ptST2LPpt.computeAverage(round);
		m_mapST2wN0ptST2RHpt.computeAverage(round);
		m_mapST2ptN0wST2RHpt.computeAverage(round);
		m_mapST2ptN0ptST2RHw.computeAverage(round);
		m_mapST2ptN0ptST2RHpt.computeAverage(round);
		m_mapST2wN0ptST2RPpt.computeAverage(round);
		m_mapST2ptN0wST2RPpt.computeAverage(round);
		m_mapST2ptN0ptST2RPw.computeAverage(round);
		m_mapST2ptN0ptST2RPpt.computeAverage(round);

		// st2 + n0 + n0 right head/pred
		m_mapST2wN0ptN0LHpt.computeAverage(round);
		m_mapST2ptN0wN0LHpt.computeAverage(round);
		m_mapST2ptN0ptN0LHw.computeAverage(round);
		m_mapST2ptN0ptN0LHpt.computeAverage(round);
		m_mapST2wN0ptN0LPpt.computeAverage(round);
		m_mapST2ptN0wN0LPpt.computeAverage(round);
		m_mapST2ptN0ptN0LPw.computeAverage(round);
		m_mapST2ptN0ptN0LPpt.computeAverage(round);

		// quar-gram
		// st + n0 + st left head + st left pred
		m_mapSTptN0ptSTLHptSTLPpt.computeAverage(round);
		// st + n0 + st right head + st right pred
		m_mapSTptN0ptSTRHptSTRPpt.computeAverage(round);
		// st + n0 + st left pred + st left pred 2
		m_mapSTptN0ptSTLPptSTLP2pt.computeAverage(round);
		// st + n0 + st right pred + st right pred 2
		m_mapSTptN0ptSTRPptSTRP2pt.computeAverage(round);
		// st + n0 + n0 left pred + n0 left pred 2
		m_mapSTptN0ptN0LPptN0LP2pt.computeAverage(round);

		// char feature
		// for chinese
		m_map1CharBefore.computeAverage(round);
		m_map2CharBefore.computeAverage(round);
		m_map3CharBefore.computeAverage(round);
		m_map1CharAfter.computeAverage(round);
		m_map2CharAfter.computeAverage(round);
		m_map3CharAfter.computeAverage(round);

		// string feature
		m_mapSTPOSPath.computeAverage(round);
		m_mapST2POSPath.computeAverage(round);
		m_mapSTFPOSPath.computeAverage(round);
		m_mapST2FPOSPath.computeAverage(round);
	}
}
