#include <fstream>

#include "minitwostack_weight.h"
#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace minitwostack {

	Weight::Weight(const std::string & sRead, const std::string & sRecord) :
		WeightBase(sRead, sRecord),
		// uni-gram
		// st, n0, st2, sst
		// st
		m_mapSTw("m_mapSTw"),
		m_mapSTpt("m_mapSTpt"),
		// n0
		m_mapN0w("m_mapN0w"),
		m_mapN0pt("m_mapN0pt"),
		// st2
		m_mapST2w("m_mapST2w"),
		m_mapST2pt("m_mapST2pt"),
		// sst
		m_mapSSTw("m_mapSSTw"),
		m_mapSSTpt("m_mapSSTpt"),

		// unigram context
		// st, n0
		// st context
		m_mapSTipt("m_mapSTipt"),
		m_mapSTiptSTjpt("m_mapSTiptSTjpt"),
		// n0 context
		m_mapN0ipt("m_mapN0ipt"),
		m_mapN0iptN0jpt("m_mapN0iptN0jpt"),

		// bi-gram
		// st + n0
		m_mapSTwN0w("m_mapSTwN0w"),
		m_mapSTwN0pt("m_mapSTwN0pt"),
		m_mapSTptN0w("m_mapSTptN0w"),
		m_mapSTptN0pt("m_mapSTptN0pt"),
		// st + n0 + dis
		m_mapSTptN0ptD("m_mapSTptN0ptD"),
		// st + n0 + st left/right head/pred
		m_mapSTptN0ptSTLPi("m_mapSTptN0ptSTLPi"),
		m_mapSTptN0ptSTRPi("m_mapSTptN0ptSTRPi"),
		m_mapSTptN0ptN0LPi("m_mapSTptN0ptN0LPi"),

		// st2 + n0
		m_mapST2wN0w("m_mapST2wN0w"),
		m_mapST2wN0pt("m_mapST2wN0pt"),
		m_mapST2ptN0w("m_mapST2ptN0w"),
		m_mapST2ptN0pt("m_mapST2ptN0pt"),
		// st2 + n0 + dis
		m_mapST2ptN0ptD("m_mapST2ptN0ptD"),
		// st2 + n0 + st2 left/right head/pred
		m_mapST2ptN0ptST2LPi("m_mapST2ptN0ptST2LPi"),
		m_mapST2ptN0ptST2RPi("m_mapST2ptN0ptST2RPi"),
		m_mapST2ptN0ptN0LPi("m_mapST2ptN0ptN0LPi"),

		// sst + n0
		m_mapSSTwN0w("m_mapSSTwN0w"),
		m_mapSSTwN0pt("m_mapSSTwN0pt"),
		m_mapSSTptN0w("m_mapSSTptN0w"),
		m_mapSSTptN0pt("m_mapSSTptN0pt"),
		// sst + n0 + dis
		m_mapSSTptN0ptD("m_mapSSTptN0ptD"),
		// st2 + n0 + st2 left/right head/pred
		m_mapSSTptN0ptSSTLPi("m_mapSSTptN0ptSSTLPi"),
		m_mapSSTptN0ptSSTRPi("m_mapSSTptN0ptSSTRPi"),
		m_mapSSTptN0ptN0LPi("m_mapSSTptN0ptN0LPi"),

		// st + st2
		m_mapSTwST2w("m_mapSTwST2w"),
		m_mapSTwST2pt("m_mapSTwST2pt"),
		m_mapSTptST2w("m_mapSTptST2w"),
		m_mapSTptST2pt("m_mapSTptST2pt"),

		// st + sst
		m_mapSTwSSTw("m_mapSTwSSTw"),
		m_mapSTwSSTpt("m_mapSTwSSTpt"),
		m_mapSTptSSTw("m_mapSTptSSTw"),
		m_mapSTptSSTpt("m_mapSTptSSTpt"),

		// tri-gram
		// st + n0 + st2
		m_mapSTptN0ptST2pt("m_mapSTptN0ptST2pt"),
		// st + n0 + sst
		m_mapSTptN0ptSSTpt("m_mapSTptN0ptSSTpt"),

		// st + n0 + st left/right head/pred
		m_mapSTptN0ptSTLHptSTLHl("m_mapSTptN0ptSTLHptSTLHl"),
		m_mapSTptN0ptSTLPptSTLPl("m_mapSTptN0ptSTLPptSTLPl"),
		m_mapSTptN0ptSTRHptSTRHl("m_mapSTptN0ptSTRHptSTRHl"),
		m_mapSTptN0ptSTRPptSTRPl("m_mapSTptN0ptSTRPptSTRPl"),

		// st + n0 + n0 left head/pred
		m_mapSTptN0ptN0LHptN0LHl("m_mapSTptN0ptN0LHptN0LHl"),
		m_mapSTptN0ptN0LPptN0LPl("m_mapSTptN0ptN0LPptN0LPl"),

		// quar-gram
		// st + n0 + st right head + st right pred
		m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl("m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl"),
		// st + n0 + st left pred + st left pred 2
		m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l("m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l"),
		// st + n0 + st right pred + st right pred 2
		m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l("m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l"),
		// st + n0 + n0 left head + n0 left pred
		m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl("m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl"),
		// st + n0 + n0 left pred + n0 left pred 2
		m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l("m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l"),

		// st2 + n0 + st2 left/right head/pred
		m_mapST2ptN0ptST2LHptST2LHl("m_mapST2ptN0ptST2LHptST2LHl"),
		m_mapST2ptN0ptST2LPptST2LPl("m_mapST2ptN0ptST2LPptST2LPl"),
		m_mapST2ptN0ptST2RHptST2RHl("m_mapST2ptN0ptST2RHptST2RHl"),
		m_mapST2ptN0ptST2RPptST2RPl("m_mapST2ptN0ptST2RPptST2RPl"),

		// st + n0 + n0 left head/pred
		m_mapST2ptN0ptN0LHptN0LHl("m_mapST2ptN0ptN0LHptN0LHl"),
		m_mapST2ptN0ptN0LPptN0LPl("m_mapST2ptN0ptN0LPptN0LPl"),

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl("m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl"),
		// st2 + n0 + st2 left pred + st2 left pred 2
		m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l("m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l"),
		// st2 + n0 + st2 right pred + st2 right pred 2
		m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l("m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l"),
		// st2 + n0 + n0 left head + n0 left pred
		m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl("m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl"),
		// st2 + n0 + n0 left pred + n0 left pred 2
		m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l("m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l"),

		// sst + n0 + sst left/right head/pred
		m_mapSSTptN0ptSSTLHptSSTLHl("m_mapSSTptN0ptSSTLHptSSTLHl"),
		m_mapSSTptN0ptSSTLPptSSTLPl("m_mapSSTptN0ptSSTLPptSSTLPl"),
		m_mapSSTptN0ptSSTRHptSSTRHl("m_mapSSTptN0ptSSTRHptSSTRHl"),
		m_mapSSTptN0ptSSTRPptSSTRPl("m_mapSSTptN0ptSSTRPptSSTRPl"),

		// st + n0 + n0 left head/pred
		m_mapSSTptN0ptN0LHptN0LHl("m_mapSSTptN0ptN0LHptN0LHl"),
		m_mapSSTptN0ptN0LPptN0LPl("m_mapSSTptN0ptN0LPptN0LPl"),

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl("m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl"),
		// sst + n0 + sst left pred + sst left pred 2
		m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l("m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l"),
		// sst + n0 + sst right pred + sst right pred 2
		m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l("m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l"),
		// sst + n0 + n0 left head + n0 left pred
		m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl("m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl"),
		// sst + n0 + n0 left pred + n0 left pred 2
		m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l("m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l"),

		// st + n0 + label set
		m_mapSTptN0ptSTll("m_mapSTptN0ptSTll"),
		m_mapSTptN0ptSTrl("m_mapSTptN0ptSTrl"),
		m_mapSTptN0ptN0ll("m_mapSTptN0ptN0ll"),
		// st2 + n0 + label set
		m_mapST2ptN0ptST2ll("m_mapST2ptN0ptST2ll"),
		m_mapST2ptN0ptST2rl("m_mapST2ptN0ptST2rl"),
		m_mapST2ptN0ptN0ll("m_mapST2ptN0ptN0ll"),
		// sst + n0 + label set
		m_mapSSTptN0ptSSTll("m_mapSSTptN0ptSSTll"),
		m_mapSSTptN0ptSSTrl("m_mapSSTptN0ptSSTrl"),
		m_mapSSTptN0ptN0ll("m_mapSSTptN0ptN0ll"),

		// char feature (for chinese)
		// st context char
		m_map1CharBeforeST("m_map1CharBeforeST"),
		m_map2CharBeforeST("m_map2CharBeforeST"),
		m_map3CharBeforeST("m_map3CharBeforeST"),
		m_map1CharAfterST("m_map1CharAfterST"),
		m_map2CharAfterST("m_map2CharAfterST"),
		m_map3CharAfterST("m_map3CharAfterST"),
		// n0 context char
		m_map1CharBeforeN0("m_map1CharBeforeN0"),
		m_map2CharBeforeN0("m_map2CharBeforeN0"),
		m_map3CharBeforeN0("m_map3CharBeforeN0"),
		m_map1CharAfterN0("m_map1CharAfterN0"),
		m_map2CharAfterN0("m_map2CharAfterN0"),
		m_map3CharAfterN0("m_map3CharAfterN0"),

		// path feature
		// st - n0
		m_mapSTPOSPath("m_mapSTPOSPath"),
		m_mapSTFPOSPath("m_mapSTFPOSPath"),
		m_mapSTwN0wSTsynhpt("m_mapSTwN0wSTsynhpt"),
		m_mapSTwN0wN0synhpt("m_mapSTwN0wN0synhpt"),
		// st2 - n0
		m_mapST2POSPath("m_mapST2POSPath"),
		m_mapST2FPOSPath("m_mapST2FPOSPath"),
		m_mapST2wN0wST2synhpt("m_mapST2wN0wST2synhpt"),
		m_mapST2wN0wN0synhpt("m_mapST2wN0wN0synhpt"),
		// sst - n0
		m_mapSSTPOSPath("m_mapSSTPOSPath"),
		m_mapSSTFPOSPath("m_mapSSTFPOSPath"),
		m_mapSSTwN0wSSTsynhpt("m_mapSSTwN0wSSTsynhpt"),
		m_mapSSTwN0wN0synhpt("m_mapSSTwN0wN0synhpt")
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
		// st, n0, st2, sst
		// st
		input >> m_mapSTw;
		input >> m_mapSTpt;
		// n0
		input >> m_mapN0w;
		input >> m_mapN0pt;
		// st2
		input >> m_mapST2w;
		input >> m_mapST2pt;
		// sst
		input >> m_mapSSTw;
		input >> m_mapSSTpt;

		// unigram context
		// st, n0
		// st context
		input >> m_mapSTipt;
		input >> m_mapSTiptSTjpt;
		// n0 context
		input >> m_mapN0ipt;
		input >> m_mapN0iptN0jpt;

		// bi-gram
		// st + n0
		input >> m_mapSTwN0w;
		input >> m_mapSTwN0pt;
		input >> m_mapSTptN0w;
		input >> m_mapSTptN0pt;
		// st + n0 + dis
		input >> m_mapSTptN0ptD;
		// st + n0 + st left/right head/pred
		input >> m_mapSTptN0ptSTLPi;
		input >> m_mapSTptN0ptSTRPi;
		input >> m_mapSTptN0ptN0LPi;

		// st2 + n0
		input >> m_mapST2wN0w;
		input >> m_mapST2wN0pt;
		input >> m_mapST2ptN0w;
		input >> m_mapST2ptN0pt;
		// st2 + n0 + dis
		input >> m_mapST2ptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		input >> m_mapST2ptN0ptST2LPi;
		input >> m_mapST2ptN0ptST2RPi;
		input >> m_mapST2ptN0ptN0LPi;

		// sst + n0
		input >> m_mapSSTwN0w;
		input >> m_mapSSTwN0pt;
		input >> m_mapSSTptN0w;
		input >> m_mapSSTptN0pt;
		// sst + n0 + dis
		input >> m_mapSSTptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		input >> m_mapSSTptN0ptSSTLPi;
		input >> m_mapSSTptN0ptSSTRPi;
		input >> m_mapSSTptN0ptN0LPi;

		// st + st2
		input >> m_mapSTwST2w;
		input >> m_mapSTwST2pt;
		input >> m_mapSTptST2w;
		input >> m_mapSTptST2pt;

		// st + sst
		input >> m_mapSTwSSTw;
		input >> m_mapSTwSSTpt;
		input >> m_mapSTptSSTw;
		input >> m_mapSTptSSTpt;

		// tri-gram
		// st + n0 + st2
		input >> m_mapSTptN0ptST2pt;
		// st + n0 + sst
		input >> m_mapSTptN0ptSSTpt;

		// st + n0 + st left/right head/pred
		input >> m_mapSTptN0ptSTLHptSTLHl;
		input >> m_mapSTptN0ptSTLPptSTLPl;
		input >> m_mapSTptN0ptSTRHptSTRHl;
		input >> m_mapSTptN0ptSTRPptSTRPl;

		// st + n0 + n0 left head/pred
		input >> m_mapSTptN0ptN0LHptN0LHl;
		input >> m_mapSTptN0ptN0LPptN0LPl;

		// quar-gram
		// st + n0 + st right head + st right pred
		input >> m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl;
		// st + n0 + st left pred + st left pred 2
		input >> m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + n0 + st right pred + st right pred 2
		input >> m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + n0 + n0 left head + n0 left pred
		input >> m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st + n0 + n0 left pred + n0 left pred 2
		input >> m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st2 + n0 + st2 left/right head/pred
		input >> m_mapST2ptN0ptST2LHptST2LHl;
		input >> m_mapST2ptN0ptST2LPptST2LPl;
		input >> m_mapST2ptN0ptST2RHptST2RHl;
		input >> m_mapST2ptN0ptST2RPptST2RPl;

		// st + n0 + n0 left head/pred
		input >> m_mapST2ptN0ptN0LHptN0LHl;
		input >> m_mapST2ptN0ptN0LPptN0LPl;

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		input >> m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl;
		// st2 + n0 + st2 left pred + st2 left pred 2
		input >> m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l;
		// st2 + n0 + st2 right pred + st2 right pred 2
		input >> m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l;
		// st2 + n0 + n0 left head + n0 left pred
		input >> m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st2 + n0 + n0 left pred + n0 left pred 2
		input >> m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// sst + n0 + sst left/right head/pred
		input >> m_mapSSTptN0ptSSTLHptSSTLHl;
		input >> m_mapSSTptN0ptSSTLPptSSTLPl;
		input >> m_mapSSTptN0ptSSTRHptSSTRHl;
		input >> m_mapSSTptN0ptSSTRPptSSTRPl;

		// st + n0 + n0 left head/pred
		input >> m_mapSSTptN0ptN0LHptN0LHl;
		input >> m_mapSSTptN0ptN0LPptN0LPl;

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		input >> m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl;
		// sst + n0 + sst left pred + sst left pred 2
		input >> m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l;
		// sst + n0 + sst right pred + sst right pred 2
		input >> m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l;
		// sst + n0 + n0 left head + n0 left pred
		input >> m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// sst + n0 + n0 left pred + n0 left pred 2
		input >> m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st + n0 + label set
		input >> m_mapSTptN0ptSTll;
		input >> m_mapSTptN0ptSTrl;
		input >> m_mapSTptN0ptN0ll;
		// st2 + n0 + label set
		input >> m_mapST2ptN0ptST2ll;
		input >> m_mapST2ptN0ptST2rl;
		input >> m_mapST2ptN0ptN0ll;
		// sst + n0 + label set
		input >> m_mapSSTptN0ptSSTll;
		input >> m_mapSSTptN0ptSSTrl;
		input >> m_mapSSTptN0ptN0ll;

		// char feature (for chinese)
		// st context char
		input >> m_map1CharBeforeST;
		input >> m_map2CharBeforeST;
		input >> m_map3CharBeforeST;
		input >> m_map1CharAfterST;
		input >> m_map2CharAfterST;
		input >> m_map3CharAfterST;
		// n0 context char
		input >> m_map1CharBeforeN0;
		input >> m_map2CharBeforeN0;
		input >> m_map3CharBeforeN0;
		input >> m_map1CharAfterN0;
		input >> m_map2CharAfterN0;
		input >> m_map3CharAfterN0;

		// path feature
		// st - n0
		input >> m_mapSTPOSPath;
		input >> m_mapSTFPOSPath;
		input >> m_mapSTwN0wSTsynhpt;
		input >> m_mapSTwN0wN0synhpt;
		// st2 - n0
		input >> m_mapST2POSPath;
		input >> m_mapST2FPOSPath;
		input >> m_mapST2wN0wST2synhpt;
		input >> m_mapST2wN0wN0synhpt;
		// sst - n0
		input >> m_mapSSTPOSPath;
		input >> m_mapSSTFPOSPath;
		input >> m_mapSSTwN0wSSTsynhpt;
		input >> m_mapSSTwN0wN0synhpt;

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
		// st, n0, st2, sst
		// st
		output << m_mapSTw;
		output << m_mapSTpt;
		// n0
		output << m_mapN0w;
		output << m_mapN0pt;
		// st2
		output << m_mapST2w;
		output << m_mapST2pt;
		// sst
		output << m_mapSSTw;
		output << m_mapSSTpt;

		// unigram context
		// st, n0
		// st context
		output << m_mapSTipt;
		output << m_mapSTiptSTjpt;
		// n0 context
		output << m_mapN0ipt;
		output << m_mapN0iptN0jpt;

		// bi-gram
		// st + n0
		output << m_mapSTwN0w;
		output << m_mapSTwN0pt;
		output << m_mapSTptN0w;
		output << m_mapSTptN0pt;
		// st + n0 + dis
		output << m_mapSTptN0ptD;
		// st + n0 + st left/right head/pred
		output << m_mapSTptN0ptSTLPi;
		output << m_mapSTptN0ptSTRPi;
		output << m_mapSTptN0ptN0LPi;

		// st2 + n0
		output << m_mapST2wN0w;
		output << m_mapST2wN0pt;
		output << m_mapST2ptN0w;
		output << m_mapST2ptN0pt;
		// st2 + n0 + dis
		output << m_mapST2ptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		output << m_mapST2ptN0ptST2LPi;
		output << m_mapST2ptN0ptST2RPi;
		output << m_mapST2ptN0ptN0LPi;

		// sst + n0
		output << m_mapSSTwN0w;
		output << m_mapSSTwN0pt;
		output << m_mapSSTptN0w;
		output << m_mapSSTptN0pt;
		// sst + n0 + dis
		output << m_mapSSTptN0ptD;
		// st2 + n0 + st2 left/right head/pred
		output << m_mapSSTptN0ptSSTLPi;
		output << m_mapSSTptN0ptSSTRPi;
		output << m_mapSSTptN0ptN0LPi;

		// st + st2
		output << m_mapSTwST2w;
		output << m_mapSTwST2pt;
		output << m_mapSTptST2w;
		output << m_mapSTptST2pt;

		// st + sst
		output << m_mapSTwSSTw;
		output << m_mapSTwSSTpt;
		output << m_mapSTptSSTw;
		output << m_mapSTptSSTpt;

		// tri-gram
		// st + n0 + st2
		output << m_mapSTptN0ptST2pt;
		// st + n0 + sst
		output << m_mapSTptN0ptSSTpt;

		// st + n0 + st left/right head/pred
		output << m_mapSTptN0ptSTLHptSTLHl;
		output << m_mapSTptN0ptSTLPptSTLPl;
		output << m_mapSTptN0ptSTRHptSTRHl;
		output << m_mapSTptN0ptSTRPptSTRPl;

		// st + n0 + n0 left head/pred
		output << m_mapSTptN0ptN0LHptN0LHl;
		output << m_mapSTptN0ptN0LPptN0LPl;

		// quar-gram
		// st + n0 + st right head + st right pred
		output << m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl;
		// st + n0 + st left pred + st left pred 2
		output << m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l;
		// st + n0 + st right pred + st right pred 2
		output << m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l;
		// st + n0 + n0 left head + n0 left pred
		output << m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st + n0 + n0 left pred + n0 left pred 2
		output << m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st2 + n0 + st2 left/right head/pred
		output << m_mapST2ptN0ptST2LHptST2LHl;
		output << m_mapST2ptN0ptST2LPptST2LPl;
		output << m_mapST2ptN0ptST2RHptST2RHl;
		output << m_mapST2ptN0ptST2RPptST2RPl;

		// st + n0 + n0 left head/pred
		output << m_mapST2ptN0ptN0LHptN0LHl;
		output << m_mapST2ptN0ptN0LPptN0LPl;

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		output << m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl;
		// st2 + n0 + st2 left pred + st2 left pred 2
		output << m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l;
		// st2 + n0 + st2 right pred + st2 right pred 2
		output << m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l;
		// st2 + n0 + n0 left head + n0 left pred
		output << m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl;
		// st2 + n0 + n0 left pred + n0 left pred 2
		output << m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// sst + n0 + sst left/right head/pred
		output << m_mapSSTptN0ptSSTLHptSSTLHl;
		output << m_mapSSTptN0ptSSTLPptSSTLPl;
		output << m_mapSSTptN0ptSSTRHptSSTRHl;
		output << m_mapSSTptN0ptSSTRPptSSTRPl;

		// st + n0 + n0 left head/pred
		output << m_mapSSTptN0ptN0LHptN0LHl;
		output << m_mapSSTptN0ptN0LPptN0LPl;

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		output << m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl;
		// sst + n0 + sst left pred + sst left pred 2
		output << m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l;
		// sst + n0 + sst right pred + sst right pred 2
		output << m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l;
		// sst + n0 + n0 left head + n0 left pred
		output << m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl;
		// sst + n0 + n0 left pred + n0 left pred 2
		output << m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l;

		// st + n0 + label set
		output << m_mapSTptN0ptSTll;
		output << m_mapSTptN0ptSTrl;
		output << m_mapSTptN0ptN0ll;
		// st2 + n0 + label set
		output << m_mapST2ptN0ptST2ll;
		output << m_mapST2ptN0ptST2rl;
		output << m_mapST2ptN0ptN0ll;
		// sst + n0 + label set
		output << m_mapSSTptN0ptSSTll;
		output << m_mapSSTptN0ptSSTrl;
		output << m_mapSSTptN0ptN0ll;

		// char feature (for chinese)
		// st context char
		output << m_map1CharBeforeST;
		output << m_map2CharBeforeST;
		output << m_map3CharBeforeST;
		output << m_map1CharAfterST;
		output << m_map2CharAfterST;
		output << m_map3CharAfterST;
		// n0 context char
		output << m_map1CharBeforeN0;
		output << m_map2CharBeforeN0;
		output << m_map3CharBeforeN0;
		output << m_map1CharAfterN0;
		output << m_map2CharAfterN0;
		output << m_map3CharAfterN0;

		// path feature
		// st - n0
		output << m_mapSTPOSPath;
		output << m_mapSTFPOSPath;
		output << m_mapSTwN0wSTsynhpt;
		output << m_mapSTwN0wN0synhpt;
		// st2 - n0
		output << m_mapST2POSPath;
		output << m_mapST2FPOSPath;
		output << m_mapST2wN0wST2synhpt;
		output << m_mapST2wN0wN0synhpt;
		// sst - n0
		output << m_mapSSTPOSPath;
		output << m_mapSSTFPOSPath;
		output << m_mapSSTwN0wSSTsynhpt;
		output << m_mapSSTwN0wN0synhpt;

		output.close();
	}

	void Weight::computeAverageFeatureWeights(const int & round) {

		// uni-gram
		// st, n0, st2, sst
		// st
		m_mapSTw.computeAverage(round);
		m_mapSTpt.computeAverage(round);
		// n0
		m_mapN0w.computeAverage(round);
		m_mapN0pt.computeAverage(round);
		// st2
		m_mapST2w.computeAverage(round);
		m_mapST2pt.computeAverage(round);
		// sst
		m_mapSSTw.computeAverage(round);
		m_mapSSTpt.computeAverage(round);

		// unigram context
		// st, n0
		// st context
		m_mapSTipt.computeAverage(round);
		m_mapSTiptSTjpt.computeAverage(round);
		// n0 context
		m_mapN0ipt.computeAverage(round);
		m_mapN0iptN0jpt.computeAverage(round);

		// bi-gram
		// st + n0
		m_mapSTwN0w.computeAverage(round);
		m_mapSTwN0pt.computeAverage(round);
		m_mapSTptN0w.computeAverage(round);
		m_mapSTptN0pt.computeAverage(round);
		// st + n0 + dis
		m_mapSTptN0ptD.computeAverage(round);
		// st + n0 + st left/right head/pred
		m_mapSTptN0ptSTLPi.computeAverage(round);
		m_mapSTptN0ptSTRPi.computeAverage(round);
		m_mapSTptN0ptN0LPi.computeAverage(round);

		// st2 + n0
		m_mapST2wN0w.computeAverage(round);
		m_mapST2wN0pt.computeAverage(round);
		m_mapST2ptN0w.computeAverage(round);
		m_mapST2ptN0pt.computeAverage(round);
		// st2 + n0 + dis
		m_mapST2ptN0ptD.computeAverage(round);
		// st2 + n0 + st2 left/right head/pred
		m_mapST2ptN0ptST2LPi.computeAverage(round);
		m_mapST2ptN0ptST2RPi.computeAverage(round);
		m_mapST2ptN0ptN0LPi.computeAverage(round);

		// sst + n0
		m_mapSSTwN0w.computeAverage(round);
		m_mapSSTwN0pt.computeAverage(round);
		m_mapSSTptN0w.computeAverage(round);
		m_mapSSTptN0pt.computeAverage(round);
		// sst + n0 + dis
		m_mapSSTptN0ptD.computeAverage(round);
		// st2 + n0 + st2 left/right head/pred
		m_mapSSTptN0ptSSTLPi.computeAverage(round);
		m_mapSSTptN0ptSSTRPi.computeAverage(round);
		m_mapSSTptN0ptN0LPi.computeAverage(round);

		// st + st2
		m_mapSTwST2w.computeAverage(round);
		m_mapSTwST2pt.computeAverage(round);
		m_mapSTptST2w.computeAverage(round);
		m_mapSTptST2pt.computeAverage(round);

		// st + sst
		m_mapSTwSSTw.computeAverage(round);
		m_mapSTwSSTpt.computeAverage(round);
		m_mapSTptSSTw.computeAverage(round);
		m_mapSTptSSTpt.computeAverage(round);

		// tri-gram
		// st + n0 + st2
		m_mapSTptN0ptST2pt.computeAverage(round);
		// st + n0 + sst
		m_mapSTptN0ptSSTpt.computeAverage(round);

		// st + n0 + st left/right head/pred
		m_mapSTptN0ptSTLHptSTLHl.computeAverage(round);
		m_mapSTptN0ptSTLPptSTLPl.computeAverage(round);
		m_mapSTptN0ptSTRHptSTRHl.computeAverage(round);
		m_mapSTptN0ptSTRPptSTRPl.computeAverage(round);

		// st + n0 + n0 left head/pred
		m_mapSTptN0ptN0LHptN0LHl.computeAverage(round);
		m_mapSTptN0ptN0LPptN0LPl.computeAverage(round);

		// quar-gram
		// st + n0 + st right head + st right pred
		m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl.computeAverage(round);
		// st + n0 + st left pred + st left pred 2
		m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l.computeAverage(round);
		// st + n0 + st right pred + st right pred 2
		m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l.computeAverage(round);
		// st + n0 + n0 left head + n0 left pred
		m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl.computeAverage(round);
		// st + n0 + n0 left pred + n0 left pred 2
		m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.computeAverage(round);

		// st2 + n0 + st2 left/right head/pred
		m_mapST2ptN0ptST2LHptST2LHl.computeAverage(round);
		m_mapST2ptN0ptST2LPptST2LPl.computeAverage(round);
		m_mapST2ptN0ptST2RHptST2RHl.computeAverage(round);
		m_mapST2ptN0ptST2RPptST2RPl.computeAverage(round);

		// st + n0 + n0 left head/pred
		m_mapST2ptN0ptN0LHptN0LHl.computeAverage(round);
		m_mapST2ptN0ptN0LPptN0LPl.computeAverage(round);

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl.computeAverage(round);
		// st2 + n0 + st2 left pred + st2 left pred 2
		m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l.computeAverage(round);
		// st2 + n0 + st2 right pred + st2 right pred 2
		m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l.computeAverage(round);
		// st2 + n0 + n0 left head + n0 left pred
		m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl.computeAverage(round);
		// st2 + n0 + n0 left pred + n0 left pred 2
		m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l.computeAverage(round);

		// sst + n0 + sst left/right head/pred
		m_mapSSTptN0ptSSTLHptSSTLHl.computeAverage(round);
		m_mapSSTptN0ptSSTLPptSSTLPl.computeAverage(round);
		m_mapSSTptN0ptSSTRHptSSTRHl.computeAverage(round);
		m_mapSSTptN0ptSSTRPptSSTRPl.computeAverage(round);

		// st + n0 + n0 left head/pred
		m_mapSSTptN0ptN0LHptN0LHl.computeAverage(round);
		m_mapSSTptN0ptN0LPptN0LPl.computeAverage(round);

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl.computeAverage(round);
		// sst + n0 + sst left pred + sst left pred 2
		m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l.computeAverage(round);
		// sst + n0 + sst right pred + sst right pred 2
		m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l.computeAverage(round);
		// sst + n0 + n0 left head + n0 left pred
		m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl.computeAverage(round);
		// sst + n0 + n0 left pred + n0 left pred 2
		m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.computeAverage(round);

		// st + n0 + label set
		m_mapSTptN0ptSTll.computeAverage(round);
		m_mapSTptN0ptSTrl.computeAverage(round);
		m_mapSTptN0ptN0ll.computeAverage(round);
		// st2 + n0 + label set
		m_mapST2ptN0ptST2ll.computeAverage(round);
		m_mapST2ptN0ptST2rl.computeAverage(round);
		m_mapST2ptN0ptN0ll.computeAverage(round);
		// sst + n0 + label set
		m_mapSSTptN0ptSSTll.computeAverage(round);
		m_mapSSTptN0ptSSTrl.computeAverage(round);
		m_mapSSTptN0ptN0ll.computeAverage(round);

		// char feature (for chinese)
		// st context char
		m_map1CharBeforeST.computeAverage(round);
		m_map2CharBeforeST.computeAverage(round);
		m_map3CharBeforeST.computeAverage(round);
		m_map1CharAfterST.computeAverage(round);
		m_map2CharAfterST.computeAverage(round);
		m_map3CharAfterST.computeAverage(round);
		// n0 context char
		m_map1CharBeforeN0.computeAverage(round);
		m_map2CharBeforeN0.computeAverage(round);
		m_map3CharBeforeN0.computeAverage(round);
		m_map1CharAfterN0.computeAverage(round);
		m_map2CharAfterN0.computeAverage(round);
		m_map3CharAfterN0.computeAverage(round);

		// path feature
		// st - n0
		m_mapSTPOSPath.computeAverage(round);
		m_mapSTFPOSPath.computeAverage(round);
		m_mapSTwN0wSTsynhpt.computeAverage(round);
		m_mapSTwN0wN0synhpt.computeAverage(round);
		// st2 - n0
		m_mapST2POSPath.computeAverage(round);
		m_mapST2FPOSPath.computeAverage(round);
		m_mapST2wN0wST2synhpt.computeAverage(round);
		m_mapST2wN0wN0synhpt.computeAverage(round);
		// sst - n0
		m_mapSSTPOSPath.computeAverage(round);
		m_mapSSTFPOSPath.computeAverage(round);
		m_mapSSTwN0wSSTsynhpt.computeAverage(round);
		m_mapSSTwN0wN0synhpt.computeAverage(round);
	}
}
