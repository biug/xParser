#ifndef _NIRVE_WEIGHT_H
#define _NIRVE_WEIGHT_H

#include "nirve_macros.h"
#include "common/parser/weight_base.h"
#include "include/learning/perceptron/packed_score.h"

#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace nirve {
	class Weight : public WeightBase {
	public:
		WordMap m_mapSTw;
		POSTagMap m_mapSTpt;
		WordMap m_mapST2w;
		POSTagMap m_mapST2pt;
		WordMap m_map2STw;
		POSTagMap m_map2STpt;
		WordMap m_mapN0w;
		POSTagMap m_mapN0pt;

		TwoWordsMap m_mapSTwN0w;
		TwoWordsMap m_mapST2wN0w;
		TwoWordsMap m_map2STwN0w;
		WordPOSTagMap m_mapSTwN0pt;
		WordPOSTagMap m_mapSTptN0w;
		WordPOSTagMap m_mapST2wN0pt;
		WordPOSTagMap m_mapST2ptN0w;
		WordPOSTagMap m_map2STwN0pt;
		WordPOSTagMap m_map2STptN0w;
		POSTagSet2Map m_mapSTptN0pt;
		POSTagSet2Map m_mapST2ptN0pt;
		POSTagSet2Map m_map2STptN0pt;

		ThreeWordsMap m_mapSTwSTRHwN0w;
		ThreeWordsMap m_mapSTwSTRDwN0w;
		ThreeWordsMap m_mapSTwN0wN0LHw;
		ThreeWordsMap m_mapSTwN0wN0LDw;

		ThreeWordsMap m_mapST2wST2RHwN0w;
		ThreeWordsMap m_mapST2wST2RDwN0w;
		ThreeWordsMap m_mapST2wN0wN0LHw;
		ThreeWordsMap m_mapST2wN0wN0LDw;

		ThreeWordsMap m_map2STw2STRHwN0w;
		ThreeWordsMap m_map2STw2STRDwN0w;
		ThreeWordsMap m_map2STwN0wN0LHw;
		ThreeWordsMap m_map2STwN0wN0LDw;

		WordWordPOSTagMap m_mapSTwSTRHwN0pt;
		WordWordPOSTagMap m_mapSTwSTRHptN0w;
		WordWordPOSTagMap m_mapSTptSTRHwN0w;

		WordWordPOSTagMap m_mapST2wST2RHwN0pt;
		WordWordPOSTagMap m_mapST2wST2RHptN0w;
		WordWordPOSTagMap m_mapST2ptST2RHwN0w;

		WordWordPOSTagMap m_map2STw2STRHwN0pt;
		WordWordPOSTagMap m_map2STw2STRHptN0w;
		WordWordPOSTagMap m_map2STpt2STRHwN0w;

		WordWordPOSTagMap m_mapSTwSTRDwN0pt;
		WordWordPOSTagMap m_mapSTwSTRDptN0w;
		WordWordPOSTagMap m_mapSTptSTRDwN0w;

		WordWordPOSTagMap m_mapST2wST2RDwN0pt;
		WordWordPOSTagMap m_mapST2wST2RDptN0w;
		WordWordPOSTagMap m_mapST2ptST2RDwN0w;

		WordWordPOSTagMap m_map2STw2STRDwN0pt;
		WordWordPOSTagMap m_map2STw2STRDptN0w;
		WordWordPOSTagMap m_map2STpt2STRDwN0w;

		WordWordPOSTagMap m_mapSTwN0wN0LHpt;
		WordWordPOSTagMap m_mapSTwN0ptN0LHw;
		WordWordPOSTagMap m_mapSTptN0wN0LHw;

		WordWordPOSTagMap m_mapST2wN0wN0LHpt;
		WordWordPOSTagMap m_mapST2wN0ptN0LHw;
		WordWordPOSTagMap m_mapST2ptN0wN0LHw;

		WordWordPOSTagMap m_map2STwN0wN0LHpt;
		WordWordPOSTagMap m_map2STwN0ptN0LHw;
		WordWordPOSTagMap m_map2STptN0wN0LHw;

		WordWordPOSTagMap m_mapSTwN0wN0LDpt;
		WordWordPOSTagMap m_mapSTwN0ptN0LDw;
		WordWordPOSTagMap m_mapSTptN0wN0LDw;

		WordWordPOSTagMap m_mapST2wN0wN0LDpt;
		WordWordPOSTagMap m_mapST2wN0ptN0LDw;
		WordWordPOSTagMap m_mapST2ptN0wN0LDw;

		WordWordPOSTagMap m_map2STwN0wN0LDpt;
		WordWordPOSTagMap m_map2STwN0ptN0LDw;
		WordWordPOSTagMap m_map2STptN0wN0LDw;

		WordPOSTagPOSTagMap m_mapSTwSTRHptN0pt;
		WordPOSTagPOSTagMap m_mapSTptSTRHptN0w;
		WordPOSTagPOSTagMap m_mapSTptSTRHwN0pt;

		WordPOSTagPOSTagMap m_mapST2wST2RHptN0pt;
		WordPOSTagPOSTagMap m_mapST2ptST2RHptN0w;
		WordPOSTagPOSTagMap m_mapST2ptST2RHwN0pt;

		WordPOSTagPOSTagMap m_map2STw2STRHptN0pt;
		WordPOSTagPOSTagMap m_map2STpt2STRHptN0w;
		WordPOSTagPOSTagMap m_map2STpt2STRHwN0pt;

		WordPOSTagPOSTagMap m_mapSTwSTRDptN0pt;
		WordPOSTagPOSTagMap m_mapSTptSTRDptN0w;
		WordPOSTagPOSTagMap m_mapSTptSTRDwN0pt;

		WordPOSTagPOSTagMap m_mapST2wST2RDptN0pt;
		WordPOSTagPOSTagMap m_mapST2ptST2RDptN0w;
		WordPOSTagPOSTagMap m_mapST2ptST2RDwN0pt;

		WordPOSTagPOSTagMap m_map2STw2STRDptN0pt;
		WordPOSTagPOSTagMap m_map2STpt2STRDptN0w;
		WordPOSTagPOSTagMap m_map2STpt2STRDwN0pt;

		WordPOSTagPOSTagMap m_mapSTwN0ptN0LHpt;
		WordPOSTagPOSTagMap m_mapSTptN0ptN0LHw;
		WordPOSTagPOSTagMap m_mapSTptN0wN0LHpt;

		WordPOSTagPOSTagMap m_mapST2wN0ptN0LHpt;
		WordPOSTagPOSTagMap m_mapST2ptN0ptN0LHw;
		WordPOSTagPOSTagMap m_mapST2ptN0wN0LHpt;

		WordPOSTagPOSTagMap m_map2STwN0ptN0LHpt;
		WordPOSTagPOSTagMap m_map2STptN0ptN0LHw;
		WordPOSTagPOSTagMap m_map2STptN0wN0LHpt;

		WordPOSTagPOSTagMap m_mapSTwN0ptN0LDpt;
		WordPOSTagPOSTagMap m_mapSTptN0ptN0LDw;
		WordPOSTagPOSTagMap m_mapSTptN0wN0LDpt;

		WordPOSTagPOSTagMap m_mapST2wN0ptN0LDpt;
		WordPOSTagPOSTagMap m_mapST2ptN0ptN0LDw;
		WordPOSTagPOSTagMap m_mapST2ptN0wN0LDpt;

		WordPOSTagPOSTagMap m_map2STwN0ptN0LDpt;
		WordPOSTagPOSTagMap m_map2STptN0ptN0LDw;
		WordPOSTagPOSTagMap m_map2STptN0wN0LDpt;

		POSTagSet3Map m_mapSTptSTRHptN0pt;
		POSTagSet3Map m_mapSTptSTRDptN0pt;
		POSTagSet3Map m_mapSTptN0ptN0LHpt;
		POSTagSet3Map m_mapSTptN0ptN0LDpt;

		POSTagSet3Map m_mapST2ptST2RHptN0pt;
		POSTagSet3Map m_mapST2ptST2RDptN0pt;
		POSTagSet3Map m_mapST2ptN0ptN0LHpt;
		POSTagSet3Map m_mapST2ptN0ptN0LDpt;

		POSTagSet3Map m_map2STpt2STRHptN0pt;
		POSTagSet3Map m_map2STpt2STRDptN0pt;
		POSTagSet3Map m_map2STptN0ptN0LHpt;
		POSTagSet3Map m_map2STptN0ptN0LDpt;

		StringMap m_mapSTPOSPath;
		StringMap m_mapST2POSPath;
		StringMap m_mapSTFPOSPath;
		StringMap m_mapST2FPOSPath;

		WordMap m_mapN_2w;
		POSTagMap m_mapN_2pt;
		WordMap m_mapN_1w;
		POSTagMap m_mapN_1pt;
		WordMap m_mapN1w;
		POSTagMap m_mapN1pt;
		WordMap m_mapN2w;
		POSTagMap m_mapN2pt;
		TwoWordsMap m_mapN_2wN_1w;
		TwoWordsMap m_mapN_1wN0w;
		TwoWordsMap m_mapN0wN1w;
		TwoWordsMap m_mapN1wN2w;
		POSTagSet2Map m_mapN_2ptN_1pt;
		POSTagSet2Map m_mapN_1ptN0pt;
		POSTagSet2Map m_mapN0ptN1pt;
		POSTagSet2Map m_mapN1ptN2pt;

		StringMap m_map1CharBefore;
		StringMap m_map2CharBefore;
		StringMap m_map3CharBefore;
		StringMap m_map1CharAfter;
		StringMap m_map2CharAfter;
		StringMap m_map3CharAfter;

	public:
		Weight(const std::string & sRead, const std::string & sRecord);
		~Weight();

		void loadScores() override;
		void saveScores() const override;
		void computeAverageFeatureWeights(const int & round) override;
	};
}

#endif