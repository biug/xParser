#include <fstream>

#include "twostack_ring_weight.h"
#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace twostack_ring {
	Weight::Weight(const std::string & sRead, const std::string & sRecord) :
		WeightBase(sRead, sRecord),
		// uni-gram
		// st, n0, st2
		m_mapSTw("m_mapSTw"),
		m_mapSTpt("m_mapSTpt"),
		m_mapST2w("m_mapST2w"),
		m_mapST2pt("m_mapST2pt"),
		m_mapSSTw("m_mapSSTw"),
		m_mapSSTpt("m_mapSSTpt"),
		m_mapN0w("m_mapN0w"),
		m_mapN0pt("m_mapN0pt"),

		// unigram context
		// st + sti, n0 + n0i
		m_mapSTiw("m_mapSTiw"),
		m_mapSTiwSTjw("m_mapSTiwSTjw"),
		m_mapST2iw("m_mapST2iw"),
		m_mapST2iwST2jw("m_mapST2iwST2jw"),
		m_mapN0iw("m_mapN0iw"),
		m_mapN0iwN0jw("m_mapN0iwN0jw"),
		m_mapSTipt("m_mapSTipt"),
		m_mapSTiptSTjpt("m_mapSTiptSTjpt"),
		m_mapST2ipt("m_mapST2ipt"),
		m_mapST2iptST2jpt("m_mapST2iptST2jpt"),
		m_mapN0ipt("m_mapN0ipt"),
		m_mapN0iptN0jpt("m_mapN0iptN0jpt"),

		// unigram with label
		// st, n0
		m_mapSTwSTLHl("m_mapSTwSTLHl"),
		m_mapSTwSTLPl("m_mapSTwSTLPl"),
		m_mapSTwSTRHl("m_mapSTwSTRHl"),
		m_mapSTwSTRPl("m_mapSTwSTRPl"),
		m_mapN0wN0LHl("m_mapN0wN0LHl"),
		m_mapN0wN0LPl("m_mapN0wN0LPl"),

		// unigram with arity
		// st, n0, st2, sst
		// st
		m_mapSTwSTLHi("m_mapSTwSTLHi"),
		m_mapSTwSTLPi("m_mapSTwSTLPi"),
		m_mapSTwSTRHi("m_mapSTwSTRHi"),
		m_mapSTwSTRPi("m_mapSTwSTRPi"),
		m_mapSTwSTHi("m_mapSTwSTHi"),
		m_mapSTwSTPi("m_mapSTwSTPi"),
		// n0
		m_mapN0wN0LHi("m_mapN0wN0LHi"),
		m_mapN0wN0LPi("m_mapN0wN0LPi"),
		// st2
		m_mapST2wST2LHi("m_mapST2wST2LHi"),
		m_mapST2wST2LPi("m_mapST2wST2LPi"),
		m_mapST2wST2RHi("m_mapST2wST2RHi"),
		m_mapST2wST2RPi("m_mapST2wST2RPi"),
		m_mapST2wST2Hi("m_mapST2wST2Hi"),
		m_mapST2wST2Pi("m_mapST2wST2Pi"),
		// sst
		m_mapSSTwSSTLHi("m_mapSSTwSSTLHi"),
		m_mapSSTwSSTLPi("m_mapSSTwSSTLPi"),
		m_mapSSTwSSTRHi("m_mapSSTwSSTRHi"),
		m_mapSSTwSSTRPi("m_mapSSTwSSTRPi"),
		m_mapSSTwSSTHi("m_mapSSTwSSTHi"),
		m_mapSSTwSSTPi("m_mapSSTwSSTPi"),

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
		m_mapSTwN0wSTRPi("m_mapSTwN0wSTRPi"),
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
		m_mapST2wN0wST2RPi("m_mapST2wN0wST2RPi"),
		m_mapST2wN0wN0LPi("m_mapST2wN0wN0LPi"),

		// sst + n0
		m_mapSSTwN0w("m_mapSSTwN0w"),
		m_mapSSTwN0pt("m_mapSSTwN0pt"),
		m_mapSSTptN0w("m_mapSSTptN0w"),
		m_mapSSTptN0pt("m_mapSSTptN0pt"),
		m_mapSSTwptN0w("m_mapSSTwptN0w"),
		m_mapSSTwN0wpt("m_mapSSTwN0wpt"),
		m_mapSSTwptN0pt("m_mapSSTwptN0pt"),
		m_mapSSTptN0wpt("m_mapSSTptN0wpt"),
		m_mapSSTwptN0wpt("m_mapSSTwptN0wpt"),
		// sst + n0 + dis
		m_mapSSTwN0wD("m_mapSSTwN0wD"),
		m_mapSSTwN0ptD("m_mapSSTwN0ptD"),
		m_mapSSTptN0wD("m_mapSSTptN0wD"),
		m_mapSSTptN0ptD("m_mapSSTptN0ptD"),
		// st2 + n0 + st2 left/right head/pred
		m_mapSSTwN0wSSTRPi("m_mapSSTwN0wSSTRPi"),
		m_mapSSTwN0wN0LPi("m_mapSSTwN0wN0LPi"),

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

		// st + sst
		m_mapSTwSSTw("m_mapSTwSSTw"),
		m_mapSTwSSTpt("m_mapSTwSSTpt"),
		m_mapSTptSSTw("m_mapSTptSSTw"),
		m_mapSTptSSTpt("m_mapSTptSSTpt"),
		m_mapSTwptSSTw("m_mapSTwptSSTw"),
		m_mapSTwSSTwpt("m_mapSTwSSTwpt"),
		m_mapSTwptSSTpt("m_mapSTwptSSTpt"),
		m_mapSTptSSTwpt("m_mapSTptSSTwpt"),
		m_mapSTwptSSTwpt("m_mapSTwptSSTwpt"),

		// tri-gram
		// st + n0 + st2
		m_mapSTwN0ptST2pt("m_mapSTwN0ptST2pt"),
		m_mapSTptN0wST2pt("m_mapSTptN0wST2pt"),
		m_mapSTptN0ptST2w("m_mapSTptN0ptST2w"),
		m_mapSTptN0ptST2pt("m_mapSTptN0ptST2pt"),
		// st + n0 + sst
		m_mapSTwN0ptSSTpt("m_mapSTwN0ptSSTpt"),
		m_mapSTptN0wSSTpt("m_mapSTptN0wSSTpt"),
		m_mapSTptN0ptSSTw("m_mapSTptN0ptSSTw"),
		m_mapSTptN0ptSSTpt("m_mapSTptN0ptSSTpt"),

		// st + n0 + st left/right head/pred
		m_mapSTwN0ptSTLHptSTLHl("m_mapSTwN0ptSTLHptSTLHl"),
		m_mapSTptN0wSTLHptSTLHl("m_mapSTptN0wSTLHptSTLHl"),
		m_mapSTptN0ptSTLHwSTLHl("m_mapSTptN0ptSTLHwSTLHl"),
		m_mapSTptN0ptSTLHptSTLHl("m_mapSTptN0ptSTLHptSTLHl"),
		m_mapSTwN0ptSTLPptSTLPl("m_mapSTwN0ptSTLPptSTLPl"),
		m_mapSTptN0wSTLPptSTLPl("m_mapSTptN0wSTLPptSTLPl"),
		m_mapSTptN0ptSTLPwSTLPl("m_mapSTptN0ptSTLPwSTLPl"),
		m_mapSTptN0ptSTLPptSTLPl("m_mapSTptN0ptSTLPptSTLPl"),
		m_mapSTwN0ptSTRHptSTRHl("m_mapSTwN0ptSTRHptSTRHl"),
		m_mapSTptN0wSTRHptSTRHl("m_mapSTptN0wSTRHptSTRHl"),
		m_mapSTptN0ptSTRHwSTRHl("m_mapSTptN0ptSTRHwSTRHl"),
		m_mapSTptN0ptSTRHptSTRHl("m_mapSTptN0ptSTRHptSTRHl"),
		m_mapSTwN0ptSTRPptSTRPl("m_mapSTwN0ptSTRPptSTRPl"),
		m_mapSTptN0wSTRPptSTRPl("m_mapSTptN0wSTRPptSTRPl"),
		m_mapSTptN0ptSTRPwSTRPl("m_mapSTptN0ptSTRPwSTRPl"),
		m_mapSTptN0ptSTRPptSTRPl("m_mapSTptN0ptSTRPptSTRPl"),

		// st + n0 + n0 left head/pred
		m_mapSTwN0ptN0LHptN0LHl("m_mapSTwN0ptN0LHptN0LHl"),
		m_mapSTptN0wN0LHptN0LHl("m_mapSTptN0wN0LHptN0LHl"),
		m_mapSTptN0ptN0LHwN0LHl("m_mapSTptN0ptN0LHwN0LHl"),
		m_mapSTptN0ptN0LHptN0LHl("m_mapSTptN0ptN0LHptN0LHl"),
		m_mapSTwN0ptN0LPptN0LPl("m_mapSTwN0ptN0LPptN0LPl"),
		m_mapSTptN0wN0LPptN0LPl("m_mapSTptN0wN0LPptN0LPl"),
		m_mapSTptN0ptN0LPwN0LPl("m_mapSTptN0ptN0LPwN0LPl"),
		m_mapSTptN0ptN0LPptN0LPl("m_mapSTptN0ptN0LPptN0LPl"),

		// quar-gram
		// st + n0 + st right head + st right pred
		m_mapSTptN0ptSTRHptSTRPpt("m_mapSTptN0ptSTRHptSTRPpt"),
		// st + n0 + st left pred + st left pred 2
		m_mapSTptN0ptSTLPptSTLP2pt("m_mapSTptN0ptSTLPptSTLP2pt"),
		// st + n0 + st right pred + st right pred 2
		m_mapSTptN0ptSTRPptSTRP2pt("m_mapSTptN0ptSTRPptSTRP2pt"),
		// st + n0 + n0 left pred + n0 left pred 2
		m_mapSTptN0ptN0LPptN0LP2pt("m_mapSTptN0ptN0LPptN0LP2pt"),

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

		// st2 + n0 + n0 left head/pred
		m_mapST2wN0ptN0LHpt("m_mapST2wN0ptN0LHpt"),
		m_mapST2ptN0wN0LHpt("m_mapST2ptN0wN0LHpt"),
		m_mapST2ptN0ptN0LHw("m_mapST2ptN0ptN0LHw"),
		m_mapST2ptN0ptN0LHpt("m_mapST2ptN0ptN0LHpt"),
		m_mapST2wN0ptN0LPpt("m_mapST2wN0ptN0LPpt"),
		m_mapST2ptN0wN0LPpt("m_mapST2ptN0wN0LPpt"),
		m_mapST2ptN0ptN0LPw("m_mapST2ptN0ptN0LPw"),
		m_mapST2ptN0ptN0LPpt("m_mapST2ptN0ptN0LPpt"),

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		m_mapST2ptN0ptST2RHptST2RPpt("m_mapST2ptN0ptST2RHptST2RPpt"),
		// st2 + n0 + st2 left pred + st2 left pred 2
		m_mapST2ptN0ptST2LPptST2LP2pt("m_mapST2ptN0ptST2LPptST2LP2pt"),
		// st2 + n0 + st2 right pred + st2 right pred 2
		m_mapST2ptN0ptST2RPptST2RP2pt("m_mapST2ptN0ptST2RPptST2RP2pt"),
		// st2 + n0 + n0 left pred + n0 left pred 2
		m_mapST2ptN0ptN0LPptN0LP2pt("m_mapST2ptN0ptN0LPptN0LP2pt"),

		// sst + n0 + sst left/right head/pred
		m_mapSSTwN0ptSSTLHpt("m_mapSSTwN0ptSSTLHpt"),
		m_mapSSTptN0wSSTLHpt("m_mapSSTptN0wSSTLHpt"),
		m_mapSSTptN0ptSSTLHw("m_mapSSTptN0ptSSTLHw"),
		m_mapSSTptN0ptSSTLHpt("m_mapSSTptN0ptSSTLHpt"),
		m_mapSSTwN0ptSSTLPpt("m_mapSSTwN0ptSSTLPpt"),
		m_mapSSTptN0wSSTLPpt("m_mapSSTptN0wSSTLPpt"),
		m_mapSSTptN0ptSSTLPw("m_mapSSTptN0ptSSTLPw"),
		m_mapSSTptN0ptSSTLPpt("m_mapSSTptN0ptSSTLPpt"),
		m_mapSSTwN0ptSSTRHpt("m_mapSSTwN0ptSSTRHpt"),
		m_mapSSTptN0wSSTRHpt("m_mapSSTptN0wSSTRHpt"),
		m_mapSSTptN0ptSSTRHw("m_mapSSTptN0ptSSTRHw"),
		m_mapSSTptN0ptSSTRHpt("m_mapSSTptN0ptSSTRHpt"),
		m_mapSSTwN0ptSSTRPpt("m_mapSSTwN0ptSSTRPpt"),
		m_mapSSTptN0wSSTRPpt("m_mapSSTptN0wSSTRPpt"),
		m_mapSSTptN0ptSSTRPw("m_mapSSTptN0ptSSTRPw"),
		m_mapSSTptN0ptSSTRPpt("m_mapSSTptN0ptSSTRPpt"),

		// sst + n0 + n0 left head/pred
		m_mapSSTwN0ptN0LHpt("m_mapSSTwN0ptN0LHpt"),
		m_mapSSTptN0wN0LHpt("m_mapSSTptN0wN0LHpt"),
		m_mapSSTptN0ptN0LHw("m_mapSSTptN0ptN0LHw"),
		m_mapSSTptN0ptN0LHpt("m_mapSSTptN0ptN0LHpt"),
		m_mapSSTwN0ptN0LPpt("m_mapSSTwN0ptN0LPpt"),
		m_mapSSTptN0wN0LPpt("m_mapSSTptN0wN0LPpt"),
		m_mapSSTptN0ptN0LPw("m_mapSSTptN0ptN0LPw"),
		m_mapSSTptN0ptN0LPpt("m_mapSSTptN0ptN0LPpt"),

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		m_mapSSTptN0ptSSTRHptSSTRPpt("m_mapSSTptN0ptSSTRHptSSTRPpt"),
		// sst + n0 + sst left pred + sst left pred 2
		m_mapSSTptN0ptSSTLPptSSTLP2pt("m_mapSSTptN0ptSSTLPptSSTLP2pt"),
		// sst + n0 + sst right pred + sst right pred 2
		m_mapSSTptN0ptSSTRPptSSTRP2pt("m_mapSSTptN0ptSSTRPptSSTRP2pt"),
		// sst + n0 + n0 left pred + n0 left pred 2
		m_mapSSTptN0ptN0LPptN0LP2pt("m_mapSSTptN0ptN0LPptN0LP2pt"),

		m_mapSTwSTll("m_mapSTwSTll"),
		m_mapSTwSTrl("m_mapSTwSTrl"),
		m_mapN0wN0ll("m_mapN0wN0ll"),
		m_mapSTptSTll("m_mapSTptSTll"),
		m_mapSTptSTrl("m_mapSTptSTrl"),
		m_mapN0ptN0ll("m_mapN0ptN0ll"),
		m_mapSTptN0ptSTll("m_mapSTptN0ptSTll"),
		m_mapSTptN0ptSTrl("m_mapSTptN0ptSTrl"),
		m_mapSTptN0ptN0ll("m_mapSTptN0ptN0ll"),

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
		m_mapSSTPOSPath("m_mapSSTPOSPath"),
		m_mapSTFPOSPath("m_mapSTFPOSPath"),
		m_mapST2FPOSPath("m_mapST2FPOSPath"),
		m_mapSSTFPOSPath("m_mapSSTFPOSPath")
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
		input >> m_mapSSTw;
		input >> m_mapSSTpt;
		input >> m_mapN0w;
		input >> m_mapN0pt;

		// unigram context
		// st + sti, n0 + n0i
		input >> m_mapSTiw;
		input >> m_mapSTiwSTjw;
		input >> m_mapST2iw;
		input >> m_mapST2iwST2jw;
		input >> m_mapN0iw;
		input >> m_mapN0iwN0jw;
		input >> m_mapSTipt;
		input >> m_mapSTiptSTjpt;
		input >> m_mapST2ipt;
		input >> m_mapST2iptST2jpt;
		input >> m_mapN0ipt;
		input >> m_mapN0iptN0jpt;

		// unigram with label
		// st, n0
		input >> m_mapSTwSTLHl;
		input >> m_mapSTwSTLPl;
		input >> m_mapSTwSTRHl;
		input >> m_mapSTwSTRPl;
		input >> m_mapN0wN0LHl;
		input >> m_mapN0wN0LPl;

		// unigram with arity
		// st, n0, st2, sst
		// st
		input >> m_mapSTwSTLHi;
		input >> m_mapSTwSTLPi;
		input >> m_mapSTwSTRHi;
		input >> m_mapSTwSTRPi;
		input >> m_mapSTwSTHi;
		input >> m_mapSTwSTPi;
		// n0
		input >> m_mapN0wN0LHi;
		input >> m_mapN0wN0LPi;
		// st2
		input >> m_mapST2wST2LHi;
		input >> m_mapST2wST2LPi;
		input >> m_mapST2wST2RHi;
		input >> m_mapST2wST2RPi;
		input >> m_mapST2wST2Hi;
		input >> m_mapST2wST2Pi;
		// sst
		input >> m_mapSSTwSSTLHi;
		input >> m_mapSSTwSSTLPi;
		input >> m_mapSSTwSSTRHi;
		input >> m_mapSSTwSSTRPi;
		input >> m_mapSSTwSSTHi;
		input >> m_mapSSTwSSTPi;

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
		input >> m_mapSTwN0wSTRPi;
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
		input >> m_mapST2wN0wST2RPi;
		input >> m_mapST2wN0wN0LPi;

		// sst + n0
		input >> m_mapSSTwN0w;
		input >> m_mapSSTwN0pt;
		input >> m_mapSSTptN0w;
		input >> m_mapSSTptN0pt;
		input >> m_mapSSTwptN0w;
		input >> m_mapSSTwN0wpt;
		input >> m_mapSSTwptN0pt;
		input >> m_mapSSTptN0wpt;
		input >> m_mapSSTwptN0wpt;
		// sst + n0 + dis
		input >> m_mapSSTwN0wD;
		input >> m_mapSSTwN0ptD;
		input >> m_mapSSTptN0wD;
		input >> m_mapSSTptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		input >> m_mapSSTwN0wSSTRPi;
		input >> m_mapSSTwN0wN0LPi;

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

		// st + sst
		input >> m_mapSTwSSTw;
		input >> m_mapSTwSSTpt;
		input >> m_mapSTptSSTw;
		input >> m_mapSTptSSTpt;
		input >> m_mapSTwptSSTw;
		input >> m_mapSTwSSTwpt;
		input >> m_mapSTwptSSTpt;
		input >> m_mapSTptSSTwpt;
		input >> m_mapSTwptSSTwpt;

		// tri-gram
		// st + n0 + st2
		input >> m_mapSTwN0ptST2pt;
		input >> m_mapSTptN0wST2pt;
		input >> m_mapSTptN0ptST2w;
		input >> m_mapSTptN0ptST2pt;
		// st + n0 + sst
		input >> m_mapSTwN0ptSSTpt;
		input >> m_mapSTptN0wSSTpt;
		input >> m_mapSTptN0ptSSTw;
		input >> m_mapSTptN0ptSSTpt;

		// st + n0 + st left/right head/pred
		input >> m_mapSTwN0ptSTLHptSTLHl;
		input >> m_mapSTptN0wSTLHptSTLHl;
		input >> m_mapSTptN0ptSTLHwSTLHl;
		input >> m_mapSTptN0ptSTLHptSTLHl;
		input >> m_mapSTwN0ptSTLPptSTLPl;
		input >> m_mapSTptN0wSTLPptSTLPl;
		input >> m_mapSTptN0ptSTLPwSTLPl;
		input >> m_mapSTptN0ptSTLPptSTLPl;
		input >> m_mapSTwN0ptSTRHptSTRHl;
		input >> m_mapSTptN0wSTRHptSTRHl;
		input >> m_mapSTptN0ptSTRHwSTRHl;
		input >> m_mapSTptN0ptSTRHptSTRHl;
		input >> m_mapSTwN0ptSTRPptSTRPl;
		input >> m_mapSTptN0wSTRPptSTRPl;
		input >> m_mapSTptN0ptSTRPwSTRPl;
		input >> m_mapSTptN0ptSTRPptSTRPl;

		// st + n0 + n0 left head/pred
		input >> m_mapSTwN0ptN0LHptN0LHl;
		input >> m_mapSTptN0wN0LHptN0LHl;
		input >> m_mapSTptN0ptN0LHwN0LHl;
		input >> m_mapSTptN0ptN0LHptN0LHl;
		input >> m_mapSTwN0ptN0LPptN0LPl;
		input >> m_mapSTptN0wN0LPptN0LPl;
		input >> m_mapSTptN0ptN0LPwN0LPl;
		input >> m_mapSTptN0ptN0LPptN0LPl;

		// quar-gram
		// st + n0 + st right head + st right pred
		input >> m_mapSTptN0ptSTRHptSTRPpt;
		// st + n0 + st left pred + st left pred 2
		input >> m_mapSTptN0ptSTLPptSTLP2pt;
		// st + n0 + st right pred + st right pred 2
		input >> m_mapSTptN0ptSTRPptSTRP2pt;
		// st + n0 + n0 left pred + n0 left pred 2
		input >> m_mapSTptN0ptN0LPptN0LP2pt;

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

		// st2 + n0 + n0 left head/pred
		input >> m_mapST2wN0ptN0LHpt;
		input >> m_mapST2ptN0wN0LHpt;
		input >> m_mapST2ptN0ptN0LHw;
		input >> m_mapST2ptN0ptN0LHpt;
		input >> m_mapST2wN0ptN0LPpt;
		input >> m_mapST2ptN0wN0LPpt;
		input >> m_mapST2ptN0ptN0LPw;
		input >> m_mapST2ptN0ptN0LPpt;

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		input >> m_mapST2ptN0ptST2RHptST2RPpt;
		// st2 + n0 + st2 left pred + st2 left pred 2
		input >> m_mapST2ptN0ptST2LPptST2LP2pt;
		// st2 + n0 + st2 right pred + st2 right pred 2
		input >> m_mapST2ptN0ptST2RPptST2RP2pt;
		// st2 + n0 + n0 left pred + n0 left pred 2
		input >> m_mapST2ptN0ptN0LPptN0LP2pt;

		// sst + n0 + sst left/right head/pred
		input >> m_mapSSTwN0ptSSTLHpt;
		input >> m_mapSSTptN0wSSTLHpt;
		input >> m_mapSSTptN0ptSSTLHw;
		input >> m_mapSSTptN0ptSSTLHpt;
		input >> m_mapSSTwN0ptSSTLPpt;
		input >> m_mapSSTptN0wSSTLPpt;
		input >> m_mapSSTptN0ptSSTLPw;
		input >> m_mapSSTptN0ptSSTLPpt;
		input >> m_mapSSTwN0ptSSTRHpt;
		input >> m_mapSSTptN0wSSTRHpt;
		input >> m_mapSSTptN0ptSSTRHw;
		input >> m_mapSSTptN0ptSSTRHpt;
		input >> m_mapSSTwN0ptSSTRPpt;
		input >> m_mapSSTptN0wSSTRPpt;
		input >> m_mapSSTptN0ptSSTRPw;
		input >> m_mapSSTptN0ptSSTRPpt;

		// sst + n0 + n0 left head/pred
		input >> m_mapSSTwN0ptN0LHpt;
		input >> m_mapSSTptN0wN0LHpt;
		input >> m_mapSSTptN0ptN0LHw;
		input >> m_mapSSTptN0ptN0LHpt;
		input >> m_mapSSTwN0ptN0LPpt;
		input >> m_mapSSTptN0wN0LPpt;
		input >> m_mapSSTptN0ptN0LPw;
		input >> m_mapSSTptN0ptN0LPpt;

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		input >> m_mapSSTptN0ptSSTRHptSSTRPpt;
		// sst + n0 + sst left pred + sst left pred 2
		input >> m_mapSSTptN0ptSSTLPptSSTLP2pt;
		// sst + n0 + sst right pred + sst right pred 2
		input >> m_mapSSTptN0ptSSTRPptSSTRP2pt;
		// sst + n0 + n0 left pred + n0 left pred 2
		input >> m_mapSSTptN0ptN0LPptN0LP2pt;

		input >> m_mapSTwSTll;
		input >> m_mapSTwSTrl;
		input >> m_mapN0wN0ll;
		input >> m_mapSTptSTll;
		input >> m_mapSTptSTrl;
		input >> m_mapN0ptN0ll;
		input >> m_mapSTptN0ptSTll;
		input >> m_mapSTptN0ptSTrl;
		input >> m_mapSTptN0ptN0ll;

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
		input >> m_mapSSTPOSPath;
		input >> m_mapSTFPOSPath;
		input >> m_mapST2FPOSPath;
		input >> m_mapSSTFPOSPath;

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
		output << m_mapSSTw;
		output << m_mapSSTpt;
		output << m_mapN0w;
		output << m_mapN0pt;

		// unigram context
		// st + sti, n0 + n0i
		output << m_mapSTiw;
		output << m_mapSTiwSTjw;
		output << m_mapST2iw;
		output << m_mapST2iwST2jw;
		output << m_mapN0iw;
		output << m_mapN0iwN0jw;
		output << m_mapSTipt;
		output << m_mapSTiptSTjpt;
		output << m_mapST2ipt;
		output << m_mapST2iptST2jpt;
		output << m_mapN0ipt;
		output << m_mapN0iptN0jpt;

		// unigram with label
		// st, n0
		output << m_mapSTwSTLHl;
		output << m_mapSTwSTLPl;
		output << m_mapSTwSTRHl;
		output << m_mapSTwSTRPl;
		output << m_mapN0wN0LHl;
		output << m_mapN0wN0LPl;

		// unigram with arity
		// st, n0, st2, sst
		// st
		output << m_mapSTwSTLHi;
		output << m_mapSTwSTLPi;
		output << m_mapSTwSTRHi;
		output << m_mapSTwSTRPi;
		output << m_mapSTwSTHi;
		output << m_mapSTwSTPi;
		// n0
		output << m_mapN0wN0LHi;
		output << m_mapN0wN0LPi;
		// st2
		output << m_mapST2wST2LHi;
		output << m_mapST2wST2LPi;
		output << m_mapST2wST2RHi;
		output << m_mapST2wST2RPi;
		output << m_mapST2wST2Hi;
		output << m_mapST2wST2Pi;
		// sst
		output << m_mapSSTwSSTLHi;
		output << m_mapSSTwSSTLPi;
		output << m_mapSSTwSSTRHi;
		output << m_mapSSTwSSTRPi;
		output << m_mapSSTwSSTHi;
		output << m_mapSSTwSSTPi;

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
		output << m_mapSTwN0wSTRPi;
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
		output << m_mapST2wN0wST2RPi;
		output << m_mapST2wN0wN0LPi;

		// sst + n0
		output << m_mapSSTwN0w;
		output << m_mapSSTwN0pt;
		output << m_mapSSTptN0w;
		output << m_mapSSTptN0pt;
		output << m_mapSSTwptN0w;
		output << m_mapSSTwN0wpt;
		output << m_mapSSTwptN0pt;
		output << m_mapSSTptN0wpt;
		output << m_mapSSTwptN0wpt;
		// sst + n0 + dis
		output << m_mapSSTwN0wD;
		output << m_mapSSTwN0ptD;
		output << m_mapSSTptN0wD;
		output << m_mapSSTptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		output << m_mapSSTwN0wSSTRPi;
		output << m_mapSSTwN0wN0LPi;

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

		// st + sst
		output << m_mapSTwSSTw;
		output << m_mapSTwSSTpt;
		output << m_mapSTptSSTw;
		output << m_mapSTptSSTpt;
		output << m_mapSTwptSSTw;
		output << m_mapSTwSSTwpt;
		output << m_mapSTwptSSTpt;
		output << m_mapSTptSSTwpt;
		output << m_mapSTwptSSTwpt;

		// tri-gram
		// st + n0 + st2
		output << m_mapSTwN0ptST2pt;
		output << m_mapSTptN0wST2pt;
		output << m_mapSTptN0ptST2w;
		output << m_mapSTptN0ptST2pt;
		// st + n0 + sst
		output << m_mapSTwN0ptSSTpt;
		output << m_mapSTptN0wSSTpt;
		output << m_mapSTptN0ptSSTw;
		output << m_mapSTptN0ptSSTpt;

		// st + n0 + st left/right head/pred
		output << m_mapSTwN0ptSTLHptSTLHl;
		output << m_mapSTptN0wSTLHptSTLHl;
		output << m_mapSTptN0ptSTLHwSTLHl;
		output << m_mapSTptN0ptSTLHptSTLHl;
		output << m_mapSTwN0ptSTLPptSTLPl;
		output << m_mapSTptN0wSTLPptSTLPl;
		output << m_mapSTptN0ptSTLPwSTLPl;
		output << m_mapSTptN0ptSTLPptSTLPl;
		output << m_mapSTwN0ptSTRHptSTRHl;
		output << m_mapSTptN0wSTRHptSTRHl;
		output << m_mapSTptN0ptSTRHwSTRHl;
		output << m_mapSTptN0ptSTRHptSTRHl;
		output << m_mapSTwN0ptSTRPptSTRPl;
		output << m_mapSTptN0wSTRPptSTRPl;
		output << m_mapSTptN0ptSTRPwSTRPl;
		output << m_mapSTptN0ptSTRPptSTRPl;

		// st + n0 + n0 left head/pred
		output << m_mapSTwN0ptN0LHptN0LHl;
		output << m_mapSTptN0wN0LHptN0LHl;
		output << m_mapSTptN0ptN0LHwN0LHl;
		output << m_mapSTptN0ptN0LHptN0LHl;
		output << m_mapSTwN0ptN0LPptN0LPl;
		output << m_mapSTptN0wN0LPptN0LPl;
		output << m_mapSTptN0ptN0LPwN0LPl;
		output << m_mapSTptN0ptN0LPptN0LPl;

		// quar-gram
		// st + n0 + st right head + st right pred
		output << m_mapSTptN0ptSTRHptSTRPpt;
		// st + n0 + st left pred + st left pred 2
		output << m_mapSTptN0ptSTLPptSTLP2pt;
		// st + n0 + st right pred + st right pred 2
		output << m_mapSTptN0ptSTRPptSTRP2pt;
		// st + n0 + n0 left pred + n0 left pred 2
		output << m_mapSTptN0ptN0LPptN0LP2pt;

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

		// st2 + n0 + n0 left head/pred
		output << m_mapST2wN0ptN0LHpt;
		output << m_mapST2ptN0wN0LHpt;
		output << m_mapST2ptN0ptN0LHw;
		output << m_mapST2ptN0ptN0LHpt;
		output << m_mapST2wN0ptN0LPpt;
		output << m_mapST2ptN0wN0LPpt;
		output << m_mapST2ptN0ptN0LPw;
		output << m_mapST2ptN0ptN0LPpt;

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		output << m_mapST2ptN0ptST2RHptST2RPpt;
		// st2 + n0 + st2 left pred + st2 left pred 2
		output << m_mapST2ptN0ptST2LPptST2LP2pt;
		// st2 + n0 + st2 right pred + st2 right pred 2
		output << m_mapST2ptN0ptST2RPptST2RP2pt;
		// st2 + n0 + n0 left pred + n0 left pred 2
		output << m_mapST2ptN0ptN0LPptN0LP2pt;

		// sst + n0 + sst left/right head/pred
		output << m_mapSSTwN0ptSSTLHpt;
		output << m_mapSSTptN0wSSTLHpt;
		output << m_mapSSTptN0ptSSTLHw;
		output << m_mapSSTptN0ptSSTLHpt;
		output << m_mapSSTwN0ptSSTLPpt;
		output << m_mapSSTptN0wSSTLPpt;
		output << m_mapSSTptN0ptSSTLPw;
		output << m_mapSSTptN0ptSSTLPpt;
		output << m_mapSSTwN0ptSSTRHpt;
		output << m_mapSSTptN0wSSTRHpt;
		output << m_mapSSTptN0ptSSTRHw;
		output << m_mapSSTptN0ptSSTRHpt;
		output << m_mapSSTwN0ptSSTRPpt;
		output << m_mapSSTptN0wSSTRPpt;
		output << m_mapSSTptN0ptSSTRPw;
		output << m_mapSSTptN0ptSSTRPpt;

		// sst + n0 + n0 left head/pred
		output << m_mapSSTwN0ptN0LHpt;
		output << m_mapSSTptN0wN0LHpt;
		output << m_mapSSTptN0ptN0LHw;
		output << m_mapSSTptN0ptN0LHpt;
		output << m_mapSSTwN0ptN0LPpt;
		output << m_mapSSTptN0wN0LPpt;
		output << m_mapSSTptN0ptN0LPw;
		output << m_mapSSTptN0ptN0LPpt;

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		output << m_mapSSTptN0ptSSTRHptSSTRPpt;
		// sst + n0 + sst left pred + sst left pred 2
		output << m_mapSSTptN0ptSSTLPptSSTLP2pt;
		// sst + n0 + sst right pred + sst right pred 2
		output << m_mapSSTptN0ptSSTRPptSSTRP2pt;
		// sst + n0 + n0 left pred + n0 left pred 2
		output << m_mapSSTptN0ptN0LPptN0LP2pt;

		output << m_mapSTwSTll;
		output << m_mapSTwSTrl;
		output << m_mapN0wN0ll;
		output << m_mapSTptSTll;
		output << m_mapSTptSTrl;
		output << m_mapN0ptN0ll;
		output << m_mapSTptN0ptSTll;
		output << m_mapSTptN0ptSTrl;
		output << m_mapSTptN0ptN0ll;

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
		output << m_mapSSTPOSPath;
		output << m_mapSTFPOSPath;
		output << m_mapST2FPOSPath;
		output << m_mapSSTFPOSPath;

		output.close();
	}

	void Weight::computeAverageFeatureWeights(const int & round) {

		// uni-gram
		// st, n0, st2
		m_mapSTw.computeAverage(round);
		m_mapSTpt.computeAverage(round);
		m_mapST2w.computeAverage(round);
		m_mapST2pt.computeAverage(round);
		m_mapSSTw.computeAverage(round);
		m_mapSSTpt.computeAverage(round);
		m_mapN0w.computeAverage(round);
		m_mapN0pt.computeAverage(round);

		// unigram context
		// st + sti, n0 + n0i
		m_mapSTiw.computeAverage(round);
		m_mapSTiwSTjw.computeAverage(round);
		m_mapST2iw.computeAverage(round);
		m_mapST2iwST2jw.computeAverage(round);
		m_mapN0iw.computeAverage(round);
		m_mapN0iwN0jw.computeAverage(round);
		m_mapSTipt.computeAverage(round);
		m_mapSTiptSTjpt.computeAverage(round);
		m_mapST2ipt.computeAverage(round);
		m_mapST2iptST2jpt.computeAverage(round);
		m_mapN0ipt.computeAverage(round);
		m_mapN0iptN0jpt.computeAverage(round);

		// unigram with label
		// st, n0
		m_mapSTwSTLHl.computeAverage(round);
		m_mapSTwSTLPl.computeAverage(round);
		m_mapSTwSTRHl.computeAverage(round);
		m_mapSTwSTRPl.computeAverage(round);
		m_mapN0wN0LHl.computeAverage(round);
		m_mapN0wN0LPl.computeAverage(round);

		// unigram with arity
		// st, n0, st2, sst
		// st
		m_mapSTwSTLHi.computeAverage(round);
		m_mapSTwSTLPi.computeAverage(round);
		m_mapSTwSTRHi.computeAverage(round);
		m_mapSTwSTRPi.computeAverage(round);
		m_mapSTwSTHi.computeAverage(round);
		m_mapSTwSTPi.computeAverage(round);
		// n0
		m_mapN0wN0LHi.computeAverage(round);
		m_mapN0wN0LPi.computeAverage(round);
		// st2
		m_mapST2wST2LHi.computeAverage(round);
		m_mapST2wST2LPi.computeAverage(round);
		m_mapST2wST2RHi.computeAverage(round);
		m_mapST2wST2RPi.computeAverage(round);
		m_mapST2wST2Hi.computeAverage(round);
		m_mapST2wST2Pi.computeAverage(round);
		// sst
		m_mapSSTwSSTLHi.computeAverage(round);
		m_mapSSTwSSTLPi.computeAverage(round);
		m_mapSSTwSSTRHi.computeAverage(round);
		m_mapSSTwSSTRPi.computeAverage(round);
		m_mapSSTwSSTHi.computeAverage(round);
		m_mapSSTwSSTPi.computeAverage(round);

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
		m_mapSTwN0wSTRPi.computeAverage(round);
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
		m_mapST2wN0wST2RPi.computeAverage(round);
		m_mapST2wN0wN0LPi.computeAverage(round);

		// sst + n0
		m_mapSSTwN0w.computeAverage(round);
		m_mapSSTwN0pt.computeAverage(round);
		m_mapSSTptN0w.computeAverage(round);
		m_mapSSTptN0pt.computeAverage(round);
		m_mapSSTwptN0w.computeAverage(round);
		m_mapSSTwN0wpt.computeAverage(round);
		m_mapSSTwptN0pt.computeAverage(round);
		m_mapSSTptN0wpt.computeAverage(round);
		m_mapSSTwptN0wpt.computeAverage(round);
		// sst + n0 + dis
		m_mapSSTwN0wD.computeAverage(round);
		m_mapSSTwN0ptD.computeAverage(round);
		m_mapSSTptN0wD.computeAverage(round);
		m_mapSSTptN0ptD.computeAverage(round);
		// st2 + n0 + st2 left/right head/pred
		m_mapSSTwN0wSSTRPi.computeAverage(round);
		m_mapSSTwN0wN0LPi.computeAverage(round);

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

		// st + sst
		m_mapSTwSSTw.computeAverage(round);
		m_mapSTwSSTpt.computeAverage(round);
		m_mapSTptSSTw.computeAverage(round);
		m_mapSTptSSTpt.computeAverage(round);
		m_mapSTwptSSTw.computeAverage(round);
		m_mapSTwSSTwpt.computeAverage(round);
		m_mapSTwptSSTpt.computeAverage(round);
		m_mapSTptSSTwpt.computeAverage(round);
		m_mapSTwptSSTwpt.computeAverage(round);

		// tri-gram
		// st + n0 + st2
		m_mapSTwN0ptST2pt.computeAverage(round);
		m_mapSTptN0wST2pt.computeAverage(round);
		m_mapSTptN0ptST2w.computeAverage(round);
		m_mapSTptN0ptST2pt.computeAverage(round);
		// st + n0 + sst
		m_mapSTwN0ptSSTpt.computeAverage(round);
		m_mapSTptN0wSSTpt.computeAverage(round);
		m_mapSTptN0ptSSTw.computeAverage(round);
		m_mapSTptN0ptSSTpt.computeAverage(round);

		// st + n0 + st left/right head/pred
		m_mapSTwN0ptSTLHptSTLHl.computeAverage(round);
		m_mapSTptN0wSTLHptSTLHl.computeAverage(round);
		m_mapSTptN0ptSTLHwSTLHl.computeAverage(round);
		m_mapSTptN0ptSTLHptSTLHl.computeAverage(round);
		m_mapSTwN0ptSTLPptSTLPl.computeAverage(round);
		m_mapSTptN0wSTLPptSTLPl.computeAverage(round);
		m_mapSTptN0ptSTLPwSTLPl.computeAverage(round);
		m_mapSTptN0ptSTLPptSTLPl.computeAverage(round);
		m_mapSTwN0ptSTRHptSTRHl.computeAverage(round);
		m_mapSTptN0wSTRHptSTRHl.computeAverage(round);
		m_mapSTptN0ptSTRHwSTRHl.computeAverage(round);
		m_mapSTptN0ptSTRHptSTRHl.computeAverage(round);
		m_mapSTwN0ptSTRPptSTRPl.computeAverage(round);
		m_mapSTptN0wSTRPptSTRPl.computeAverage(round);
		m_mapSTptN0ptSTRPwSTRPl.computeAverage(round);
		m_mapSTptN0ptSTRPptSTRPl.computeAverage(round);

		// st + n0 + n0 left head/pred
		m_mapSTwN0ptN0LHptN0LHl.computeAverage(round);
		m_mapSTptN0wN0LHptN0LHl.computeAverage(round);
		m_mapSTptN0ptN0LHwN0LHl.computeAverage(round);
		m_mapSTptN0ptN0LHptN0LHl.computeAverage(round);
		m_mapSTwN0ptN0LPptN0LPl.computeAverage(round);
		m_mapSTptN0wN0LPptN0LPl.computeAverage(round);
		m_mapSTptN0ptN0LPwN0LPl.computeAverage(round);
		m_mapSTptN0ptN0LPptN0LPl.computeAverage(round);

		// quar-gram
		// st + n0 + st right head + st right pred
		m_mapSTptN0ptSTRHptSTRPpt.computeAverage(round);
		// st + n0 + st left pred + st left pred 2
		m_mapSTptN0ptSTLPptSTLP2pt.computeAverage(round);
		// st + n0 + st right pred + st right pred 2
		m_mapSTptN0ptSTRPptSTRP2pt.computeAverage(round);
		// st + n0 + n0 left pred + n0 left pred 2
		m_mapSTptN0ptN0LPptN0LP2pt.computeAverage(round);

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

		// st2 + n0 + n0 left head/pred
		m_mapST2wN0ptN0LHpt.computeAverage(round);
		m_mapST2ptN0wN0LHpt.computeAverage(round);
		m_mapST2ptN0ptN0LHw.computeAverage(round);
		m_mapST2ptN0ptN0LHpt.computeAverage(round);
		m_mapST2wN0ptN0LPpt.computeAverage(round);
		m_mapST2ptN0wN0LPpt.computeAverage(round);
		m_mapST2ptN0ptN0LPw.computeAverage(round);
		m_mapST2ptN0ptN0LPpt.computeAverage(round);

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		m_mapST2ptN0ptST2RHptST2RPpt.computeAverage(round);
		// st2 + n0 + st2 left pred + st2 left pred 2
		m_mapST2ptN0ptST2LPptST2LP2pt.computeAverage(round);
		// st2 + n0 + st2 right pred + st2 right pred 2
		m_mapST2ptN0ptST2RPptST2RP2pt.computeAverage(round);
		// st2 + n0 + n0 left pred + n0 left pred 2
		m_mapST2ptN0ptN0LPptN0LP2pt.computeAverage(round);

		// sst + n0 + sst left/right head/pred
		m_mapSSTwN0ptSSTLHpt.computeAverage(round);
		m_mapSSTptN0wSSTLHpt.computeAverage(round);
		m_mapSSTptN0ptSSTLHw.computeAverage(round);
		m_mapSSTptN0ptSSTLHpt.computeAverage(round);
		m_mapSSTwN0ptSSTLPpt.computeAverage(round);
		m_mapSSTptN0wSSTLPpt.computeAverage(round);
		m_mapSSTptN0ptSSTLPw.computeAverage(round);
		m_mapSSTptN0ptSSTLPpt.computeAverage(round);
		m_mapSSTwN0ptSSTRHpt.computeAverage(round);
		m_mapSSTptN0wSSTRHpt.computeAverage(round);
		m_mapSSTptN0ptSSTRHw.computeAverage(round);
		m_mapSSTptN0ptSSTRHpt.computeAverage(round);
		m_mapSSTwN0ptSSTRPpt.computeAverage(round);
		m_mapSSTptN0wSSTRPpt.computeAverage(round);
		m_mapSSTptN0ptSSTRPw.computeAverage(round);
		m_mapSSTptN0ptSSTRPpt.computeAverage(round);

		// sst + n0 + n0 left head/pred
		m_mapSSTwN0ptN0LHpt.computeAverage(round);
		m_mapSSTptN0wN0LHpt.computeAverage(round);
		m_mapSSTptN0ptN0LHw.computeAverage(round);
		m_mapSSTptN0ptN0LHpt.computeAverage(round);
		m_mapSSTwN0ptN0LPpt.computeAverage(round);
		m_mapSSTptN0wN0LPpt.computeAverage(round);
		m_mapSSTptN0ptN0LPw.computeAverage(round);
		m_mapSSTptN0ptN0LPpt.computeAverage(round);

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		m_mapSSTptN0ptSSTRHptSSTRPpt.computeAverage(round);
		// sst + n0 + sst left pred + sst left pred 2
		m_mapSSTptN0ptSSTLPptSSTLP2pt.computeAverage(round);
		// sst + n0 + sst right pred + sst right pred 2
		m_mapSSTptN0ptSSTRPptSSTRP2pt.computeAverage(round);
		// sst + n0 + n0 left pred + n0 left pred 2
		m_mapSSTptN0ptN0LPptN0LP2pt.computeAverage(round);

		m_mapSTwSTll.computeAverage(round);
		m_mapSTwSTrl.computeAverage(round);
		m_mapN0wN0ll.computeAverage(round);
		m_mapSTptSTll.computeAverage(round);
		m_mapSTptSTrl.computeAverage(round);
		m_mapN0ptN0ll.computeAverage(round);
		m_mapSTptN0ptSTll.computeAverage(round);
		m_mapSTptN0ptSTrl.computeAverage(round);
		m_mapSTptN0ptN0ll.computeAverage(round);

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
		m_mapSSTPOSPath.computeAverage(round);
		m_mapSTFPOSPath.computeAverage(round);
		m_mapST2FPOSPath.computeAverage(round);
		m_mapSSTFPOSPath.computeAverage(round);
	}
}
