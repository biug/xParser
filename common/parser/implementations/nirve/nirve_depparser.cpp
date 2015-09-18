#include <cmath>
#include <stack>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <unordered_set>

#include "nirve_depparser.h"
#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace nirve {

	WordPOSTag DepParser::empty_taggedword = WordPOSTag();
	SetOfDepLabels DepParser::empty_setoflabels = SetOfDepLabels();

	DepParser::DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, const bool & bChar, const bool & bPath, const bool & bLabel, int nState) :
		DepParserBase(sFeatureInput, nState),
		m_bChar(bChar), m_bPath(bPath), m_bLabel(bLabel) {

		m_nSentenceLength = 0;

		m_Weight = new Weight(sFeatureInput, sFeatureOut);

		DepParser::empty_taggedword.refer(TWord::code(EMPTY_WORD), TPOSTag::code(EMPTY_POSTAG));
		empty_setoflabels.add(0);

		m_tDecodeTime = 0;
		m_tGetScoreTime = 0;
		m_tUpdateScoreTime = 0;
		m_tStartTime = 0;
	}

	DepParser::~DepParser() {
		delete m_Weight;
	}

	void DepParser::train(const DependencyGraph & correct, const int & round) {
		// initialize
		int idx = 0;
		m_sSentence.clear();
		m_dtSyntaxTree.clear();
		m_nSentenceLength = correct.size();
		for (const auto & node : correct) {
			m_sSentence.push_back(GRAPHNODE_POSTAGGEDWORD(node));
			m_dtSyntaxTree.push_back(DependencyTreeNode(GRAPHNODE_POSTAGGEDWORD(node), GRAPHNODE_TREEHEAD(node), GRAPHNODE_TREELABEL(node)));
			m_lSentence[idx++].refer(TWord::code(GRAPHNODE_WORD(node)), TPOSTag::code(GRAPHNODE_POSTAG(node)));
		}
		if (m_bPath) {
#ifdef _DEBUG
			std::cout << "path load start" << std::endl;
#endif
			m_lcaAnalyzer.loadPath(m_dtSyntaxTree);
#ifdef _DEBUG
			std::cout << "path load complete" << std::endl;
#endif
		}
		// train
		work(nullptr, correct);
		if (m_nTrainingRound % OUTPUT_STEP == 0) {
			std::cout << m_nTotalErrors << " / " << m_nTrainingRound << std::endl;
			//printTime();
		}
	}

	void DepParser::parse(const DependencyGraph & sentence, DependencyGraph * retval) {
		int idx = 0;
		m_nTrainingRound = 0;
		m_sSentence.clear();
		m_dtSyntaxTree.clear();
		m_nSentenceLength = sentence.size();
		for (const auto & token : sentence) {
			m_sSentence.push_back(GRAPHNODE_POSTAGGEDWORD(token));
			m_dtSyntaxTree.push_back(DependencyTreeNode(GRAPHNODE_POSTAGGEDWORD(token), GRAPHNODE_TREEHEAD(token), GRAPHNODE_TREELABEL(token)));
			m_lSentence[idx++].refer(TWord::code(GRAPHNODE_WORD(token)), TPOSTag::code(GRAPHNODE_POSTAG(token)));
		}
		if (m_bPath) {
#ifdef _DEBUG
			std::cout << "path load start" << std::endl;
#endif
			m_lcaAnalyzer.loadPath(m_dtSyntaxTree);
#ifdef _DEBUG
			std::cout << "path load complete" << std::endl;
#endif
		}
		work(retval, sentence);
	}

	void DepParser::goldCheck(const DependencyGraph & correct) {
		m_iCorrect.clear();
		if (!m_iCorrect.extractOracle(correct)) {
			++m_nTotalErrors;
			std::cout << "errors at " << m_nTotalErrors << std::endl;
		}
		m_iCorrect.check();
	}

	void DepParser::work(DependencyGraph * retval, const DependencyGraph & correct) {

		m_abItems[0].clear();
		m_abItems[1].clear();
		m_abFinished.clear();
		m_iCandidate.clear();

		m_iCorrect.clear();
		if (m_nState == TRAIN && !m_iCorrect.extractOracle(correct)) {
			return;
		}

		// training only if titov has an oracle
		++m_nTrainingRound;

		m_pGenerator = &m_abItems[0];
		m_pGenerated = &m_abItems[1];

		m_pGenerator->insertItem(m_iCandidate);

		while (true) {

			decode();

			if (m_pGenerated->size() == 0) {
				break;
			}

			std::swap(m_pGenerated, m_pGenerator);
		}

		switch (m_nState) {
		case ParserState::TRAIN:
			update();
			break;
		case ParserState::PARSE:
			generate(retval, correct);
			break;
		default:
			break;
		}
	}

	void DepParser::decode() {

		m_pGenerated->clear();

		for (const auto & iGenerator : *m_pGenerator) {
			m_abScores.clear();
			getActionScores(*iGenerator);

			//auto time_start = GetTickCount();

			const tscore & score = iGenerator->getScore();

			if (iGenerator->canSwap()) {
				swap(score);
			}

			if (iGenerator->size() < m_nSentenceLength) {
				shift(score);
				if (iGenerator->canShiftReduce()) {
					shiftReduce(score);
				}
				if (iGenerator->canArc()) {
					arcLeftReduce(score);
					arcRightReduce(score);
					arcLeftShift(score);
					arcRightShift(score);
					if (iGenerator->canSwap()) {
						arcLeftSwap(score);
						arcRightSwap(score);
					}
				}
			}

			if (iGenerator->stackBack() >= 0) {
				reduce(score);
			}

			for (const auto & saScore : m_abScores) {
				m_iCandidate = *iGenerator;
				m_iCandidate.setScore(saScore->getScore());
				m_iCandidate.move(saScore->getAction());
				m_pGenerated->insertItem(m_iCandidate);
			}

			if (m_abScores.size() == 0) {
				m_abFinished.insertItem(*iGenerator);
			}

			//m_tDecodeTime += GetTickCount() - time_start;
		}
	}

	void DepParser::decodeArcs() {}

	void DepParser::update() {
		//auto time_start = GetTickCount();

		m_iStatesItem.clear();
		const StateItem & output = m_abFinished.bestUnsortItem();

#ifdef _DEBUG
		std::cout << "output" << std::endl;
		output.print();
		std::cout << "correct" << std::endl;
		m_iCorrect.print();
#endif

		int index = 0;
		if (output != m_iCorrect) {
#ifdef _DEBUG
			std::cout << std::endl << "common action" << std::endl;
#endif
			while (m_iStatesItem != output && index <= output.actionBack() && index <= m_iCorrect.actionBack()) {
				const int & action = output.action(index);
				const int & correct_action = m_iCorrect.action(index);
				if (action == correct_action) {
					m_iStatesItem.move(action);
#ifdef _DEBUG
					printAction(action);
#endif
				}
				else {
					break;
				}
				++index;
			}

			getActionScores(m_iStatesItem);

#ifdef _DEBUG
			std::cout << std::endl << "correct action" << std::endl << std::endl;
#endif
			updateScoreForState(m_iStatesItem, m_iCorrect, index, 1);
#ifdef _DEBUG
			std::cout << std::endl << "my best action" << std::endl << std::endl;
#endif
			updateScoreForState(m_iStatesItem, output, index, -1);
#ifdef _DEBUG
			std::cout << std::endl;
#endif
			++m_nTotalErrors;
		}

		//m_tUpdateScoreTime += GetTickCount() - time_start;
	}

	void DepParser::generate(DependencyGraph * retval, const DependencyGraph & correct) {
		m_abFinished.bestUnsortItem().generateGraph(correct, *retval);
	}

	void DepParser::getActionScores(const StateItem & item) {
		//auto time_start = GetTickCount();
		memset(m_mapPackedScore, 0, sizeof(tscore) * MAX_ACTION);
		getOrUpdateStackScore(item, std::make_pair<int, const int &>(NO_ACTION, 0));
		//m_tGetScoreTime += GetTickCount() - time_start;
	}

	void DepParser::updateScoreForState(const StateItem & from, const StateItem & output, const int & start_action_index, const int & amount) {
		m_iStateItem = from;
		int index = start_action_index;
		std::pair<int, int> m_pairAmount(NO_ACTION, amount);
		while (m_iStateItem != output && index <= output.actionBack()) {
			m_pairAmount.first = output.action(index++);
#ifdef _DEBUG
			printAction(m_pairAmount.first);
#endif
			getOrUpdateStackScore(m_iStateItem, m_pairAmount);
			m_iStateItem.move(m_pairAmount.first);
		}
	}

	void DepParser::getOrUpdateStackScore(const StateItem & item, const AddScoreType & amount) {

		Weight * cweight = (Weight*)m_Weight;

		const int & st_index = item.stackBack() == -1 ? -1 : item.stackTop();
		const int & strh_index = st_index == -1 ? -1 : item.rightHead(st_index);
		const int & strd_index = st_index == -1 ? -1 : item.rightPred(st_index);
		const int & st2_index = item.stackBack() <= 0 ? -1 : item.stackSubTop();
		const int & st2rh_index = st2_index == -1 ? -1 : item.rightHead(st2_index);
		const int & st2rd_index = st2_index == -1 ? -1 : item.rightPred(st2_index);
		const int & sst_index = item.bufferTop() == item.size() ? -1 : item.bufferTop();
		const int & sstrh_index = sst_index == -1 ? -1 : item.rightHead(sst_index);
		const int & sstrd_index = sst_index == -1 ? -1 : item.rightPred(sst_index);
		const int & n0_index = item.size() < m_nSentenceLength ? item.size() : -1;
		const int & n0ld_index = n0_index == -1 ? -1 : item.leftPred(n0_index);
		const int & n0lh_index = n0_index == -1 ? -1 : item.leftHead(n0_index);
		const int & n_2_index = n0_index >= 2 ? n0_index - 2 : -1;
		const int & n_1_index = n0_index >= 1 ? n0_index - 1 : -1;
		const int & n1_index = n0_index + 1 < m_nSentenceLength ? n0_index + 1 : -1;
		const int & n2_index = n0_index + 2 < m_nSentenceLength ? n0_index + 2 : -1;

		const WordPOSTag & st_word_postag = st_index == -1 ? empty_taggedword : m_lSentence[st_index];
		const WordPOSTag & strh_word_postag = strh_index == -1 ? empty_taggedword : m_lSentence[strh_index];
		const WordPOSTag & strd_word_postag = strd_index == -1 ? empty_taggedword : m_lSentence[strd_index];
		const WordPOSTag & st2_word_postag = st2_index == -1 ? empty_taggedword : m_lSentence[st2_index];
		const WordPOSTag & st2rh_word_postag = st2rh_index == -1 ? empty_taggedword : m_lSentence[st2rh_index];
		const WordPOSTag & st2rd_word_postag = st2rd_index == -1 ? empty_taggedword : m_lSentence[st2rd_index];
		const WordPOSTag & sst_word_postag = sst_index == -1 ? empty_taggedword : m_lSentence[sst_index];
		const WordPOSTag & sstrh_word_postag = sstrh_index == -1 ? empty_taggedword : m_lSentence[sstrh_index];
		const WordPOSTag & sstrd_word_postag = sstrd_index == -1 ? empty_taggedword : m_lSentence[sstrd_index];
		const WordPOSTag & n0_word_postag = n0_index == -1 ? empty_taggedword : m_lSentence[n0_index];
		const WordPOSTag & n0ld_word_postag = n0ld_index == -1 ? empty_taggedword : m_lSentence[n0ld_index];
		const WordPOSTag & n0lh_word_postag = n0lh_index == -1 ? empty_taggedword : m_lSentence[n0lh_index];
		const WordPOSTag & n_2_word_postag = n_2_index == -1 ? empty_taggedword : m_lSentence[n_2_index];
		const WordPOSTag & n_1_word_postag = n_1_index == -1 ? empty_taggedword : m_lSentence[n_1_index];
		const WordPOSTag & n1_word_postag = n1_index == -1 ? empty_taggedword : m_lSentence[n1_index];
		const WordPOSTag & n2_word_postag = n2_index == -1 ? empty_taggedword : m_lSentence[n2_index];

		const Word & st_word = st_word_postag.first();
		const Word & strh_word = strh_word_postag.first();
		const Word & strd_word = strd_word_postag.first();
		const Word & st2_word = st2_word_postag.first();
		const Word & st2rh_word = st2rh_word_postag.first();
		const Word & st2rd_word = st2rd_word_postag.first();
		const Word & sst_word = sst_word_postag.first();
		const Word & sstrh_word = sstrh_word_postag.first();
		const Word & sstrd_word = sstrd_word_postag.first();
		const Word & n0_word = n0_word_postag.first();
		const Word & n0ld_word = n0ld_word_postag.first();
		const Word & n0lh_word = n0lh_word_postag.first();
		const Word & n_2_word = n_2_word_postag.first();
		const Word & n_1_word = n_1_word_postag.first();
		const Word & n1_word = n1_word_postag.first();
		const Word & n2_word = n2_word_postag.first();

		const POSTag & st_tag = st_word_postag.second();
		const POSTag & strh_tag = strh_word_postag.second();
		const POSTag & strd_tag = strd_word_postag.second();
		const POSTag & st2_tag = st2_word_postag.second();
		const POSTag & st2rh_tag = st2rh_word_postag.second();
		const POSTag & st2rd_tag = st2rd_word_postag.second();
		const POSTag & sst_tag = sst_word_postag.second();
		const POSTag & sstrh_tag = sstrh_word_postag.second();
		const POSTag & sstrd_tag = sstrd_word_postag.second();
		const POSTag & n0_tag = n0_word_postag.second();
		const POSTag & n0ld_tag = n0ld_word_postag.second();
		const POSTag & n0lh_tag = n0lh_word_postag.second();
		const POSTag & n_2_tag = n_2_word_postag.second();
		const POSTag & n_1_tag = n_1_word_postag.second();
		const POSTag & n1_tag = n1_word_postag.second();
		const POSTag & n2_tag = n2_word_postag.second();

		cweight->m_mapSTw.getOrUpdateScore(m_mapPackedScore, st_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSTpt.getOrUpdateScore(m_mapPackedScore, st_tag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapST2w.getOrUpdateScore(m_mapPackedScore, st2_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapST2pt.getOrUpdateScore(m_mapPackedScore, st2_tag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_map2STw.getOrUpdateScore(m_mapPackedScore, sst_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_map2STpt.getOrUpdateScore(m_mapPackedScore, sst_tag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN0w.getOrUpdateScore(m_mapPackedScore, n0_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN0pt.getOrUpdateScore(m_mapPackedScore, n0_tag, m_nScoreIndex, amount, m_nTrainingRound);

		bi_word.refer(st_word, n0_word);
		cweight->m_mapSTwN0w.getOrUpdateScore(m_mapPackedScore, bi_word, m_nScoreIndex, amount, m_nTrainingRound);
		bi_word.refer(st2_word, n0_word);
		cweight->m_mapST2wN0w.getOrUpdateScore(m_mapPackedScore, bi_word, m_nScoreIndex, amount, m_nTrainingRound);
		bi_word.refer(sst_word, n0_word);
		cweight->m_map2STwN0w.getOrUpdateScore(m_mapPackedScore, bi_word, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag.refer(st_word, n0_tag);
		cweight->m_mapSTwN0pt.getOrUpdateScore(m_mapPackedScore, word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag.refer(n0_word, st_tag);
		cweight->m_mapSTptN0w.getOrUpdateScore(m_mapPackedScore, word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag.refer(st2_word, n0_tag);
		cweight->m_mapST2wN0pt.getOrUpdateScore(m_mapPackedScore, word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag.refer(n0_word, st2_tag);
		cweight->m_mapST2ptN0w.getOrUpdateScore(m_mapPackedScore, word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag.refer(sst_word, n0_tag);
		cweight->m_map2STwN0pt.getOrUpdateScore(m_mapPackedScore, word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag.refer(n0_word, sst_tag);
		cweight->m_map2STptN0w.getOrUpdateScore(m_mapPackedScore, word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_tags = ENCODE_POSTAG_SET_2(st_tag, n0_tag);
		cweight->m_mapSTptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_2_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_tags = ENCODE_POSTAG_SET_2(st2_tag, n0_tag);
		cweight->m_mapST2ptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_2_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_tags = ENCODE_POSTAG_SET_2(sst_tag, n0_tag);
		cweight->m_map2STptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_2_tags, m_nScoreIndex, amount, m_nTrainingRound);

		tri_word.refer(st_word, strh_word, n0_word);
		cweight->m_mapSTwSTRHwN0w.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);
		tri_word.refer(st_word, strd_word, n0_word);
		cweight->m_mapSTwSTRDwN0w.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);
		tri_word.refer(st_word, n0_word, n0lh_word);
		cweight->m_mapSTwN0wN0LHw.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);
		tri_word.refer(st_word, n0_word, n0ld_word);
		cweight->m_mapSTwN0wN0LDw.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);

		tri_word.refer(st2_word, st2rh_word, n0_word);
		cweight->m_mapST2wST2RHwN0w.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);
		tri_word.refer(st2_word, st2rd_word, n0_word);
		cweight->m_mapST2wST2RDwN0w.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);
		tri_word.refer(st2_word, n0_word, n0lh_word);
		cweight->m_mapST2wN0wN0LHw.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);
		tri_word.refer(st2_word, n0_word, n0ld_word);
		cweight->m_mapST2wN0wN0LDw.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);

		tri_word.refer(sst_word, sstrh_word, n0_word);
		cweight->m_map2STw2STRHwN0w.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);
		tri_word.refer(sst_word, sstrd_word, n0_word);
		cweight->m_map2STw2STRDwN0w.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);
		tri_word.refer(sst_word, n0_word, n0lh_word);
		cweight->m_map2STwN0wN0LHw.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);
		tri_word.refer(sst_word, n0_word, n0ld_word);
		cweight->m_map2STwN0wN0LDw.getOrUpdateScore(m_mapPackedScore, tri_word, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(st_word, strh_word, n0_tag);
		cweight->m_mapSTwSTRHwN0pt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st_word, n0_word, strh_tag);
		cweight->m_mapSTwSTRHptN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(strh_word, n0_word, st_tag);
		cweight->m_mapSTptSTRHwN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(st2_word, st2rh_word, n0_tag);
		cweight->m_mapST2wST2RHwN0pt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st2_word, n0_word, st2rh_tag);
		cweight->m_mapST2wST2RHptN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st2rh_word, n0_word, st2_tag);
		cweight->m_mapST2ptST2RHwN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(sst_word, sstrh_word, n0_tag);
		cweight->m_map2STw2STRHwN0pt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(sst_word, n0_word, sstrh_tag);
		cweight->m_map2STw2STRHptN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(sstrh_word, n0_word, sst_tag);
		cweight->m_map2STpt2STRHwN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(st_word, strd_word, n0_tag);
		cweight->m_mapSTwSTRDwN0pt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st_word, n0_word, strd_tag);
		cweight->m_mapSTwSTRDptN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(strd_word, n0_word, st_tag);
		cweight->m_mapSTptSTRDwN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(st2_word, st2rd_word, n0_tag);
		cweight->m_mapST2wST2RDwN0pt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st2_word, n0_word, st2rd_tag);
		cweight->m_mapST2wST2RDptN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st2rd_word, n0_word, st2_tag);
		cweight->m_mapST2ptST2RDwN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(sst_word, sstrd_word, n0_tag);
		cweight->m_map2STw2STRDwN0pt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(sst_word, n0_word, sstrd_tag);
		cweight->m_map2STw2STRDptN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(sstrd_word, n0_word, sst_tag);
		cweight->m_map2STpt2STRDwN0w.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(st_word, n0_word, n0lh_tag);
		cweight->m_mapSTwN0wN0LHpt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st_word, n0lh_word, n0_tag);
		cweight->m_mapSTwN0ptN0LHw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(n0_word, n0lh_word, st_tag);
		cweight->m_mapSTptN0wN0LHw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(st2_word, n0_word, n0lh_tag);
		cweight->m_mapST2wN0wN0LHpt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st2_word, n0lh_word, n0_tag);
		cweight->m_mapST2wN0ptN0LHw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(n0_word, n0lh_word, st2_tag);
		cweight->m_mapST2ptN0wN0LHw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(sst_word, n0_word, n0lh_tag);
		cweight->m_map2STwN0wN0LHpt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(sst_word, n0lh_word, n0_tag);
		cweight->m_map2STwN0ptN0LHw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(n0_word, n0lh_word, sst_tag);
		cweight->m_map2STptN0wN0LHw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(st_word, n0_word, n0ld_tag);
		cweight->m_mapSTwN0wN0LDpt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st_word, n0ld_word, n0_tag);
		cweight->m_mapSTwN0ptN0LDw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(n0_word, n0ld_word, st_tag);
		cweight->m_mapSTptN0wN0LDw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(st2_word, n0_word, n0ld_tag);
		cweight->m_mapST2wN0wN0LDpt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(st2_word, n0ld_word, n0_tag);
		cweight->m_mapST2wN0ptN0LDw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(n0_word, n0ld_word, st2_tag);
		cweight->m_mapST2ptN0wN0LDw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_word_tag.refer(sst_word, n0_word, n0ld_tag);
		cweight->m_map2STwN0wN0LDpt.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(sst_word, n0ld_word, n0_tag);
		cweight->m_map2STwN0ptN0LDw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_tag.refer(n0_word, n0ld_word, sst_tag);
		cweight->m_map2STptN0wN0LDw.getOrUpdateScore(m_mapPackedScore, word_word_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(st_word, strh_tag, n0_tag);
		cweight->m_mapSTwSTRHptN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, st_tag, strh_tag);
		cweight->m_mapSTptSTRHptN0w.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(strh_word, st_tag, n0_tag);
		cweight->m_mapSTptSTRHwN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(st2_word, st2rh_tag, n0_tag);
		cweight->m_mapST2wST2RHptN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, st2_tag, st2rh_tag);
		cweight->m_mapST2ptST2RHptN0w.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(st2rh_word, st2_tag, n0_tag);
		cweight->m_mapST2ptST2RHwN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(sst_word, sstrh_tag, n0_tag);
		cweight->m_map2STw2STRHptN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, sst_tag, sstrh_tag);
		cweight->m_map2STpt2STRHptN0w.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(sstrh_word, sst_tag, n0_tag);
		cweight->m_map2STpt2STRHwN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(st_word, strd_tag, n0_tag);
		cweight->m_mapSTwSTRDptN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, st_tag, strd_tag);
		cweight->m_mapSTptSTRDptN0w.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(strd_word, st_tag, n0_tag);
		cweight->m_mapSTptSTRDwN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(st2_word, st2rd_tag, n0_tag);
		cweight->m_mapST2wST2RDptN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, st2_tag, st2rd_tag);
		cweight->m_mapST2ptST2RDptN0w.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(st2rd_word, st2_tag, n0_tag);
		cweight->m_mapST2ptST2RDwN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(sst_word, sstrd_tag, n0_tag);
		cweight->m_map2STw2STRDptN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, sst_tag, sstrd_tag);
		cweight->m_map2STpt2STRDptN0w.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(sstrd_word, sst_tag, n0_tag);
		cweight->m_map2STpt2STRDwN0pt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(st_word, n0_tag, n0lh_tag);
		cweight->m_mapSTwN0ptN0LHpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0lh_word, st_tag, n0_tag);
		cweight->m_mapSTptN0ptN0LHw.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, st_tag, n0lh_tag);
		cweight->m_mapSTptN0wN0LHpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(st2_word, n0_tag, n0lh_tag);
		cweight->m_mapST2wN0ptN0LHpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0lh_word, st2_tag, n0_tag);
		cweight->m_mapST2ptN0ptN0LHw.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, st2_tag, n0lh_tag);
		cweight->m_mapST2ptN0wN0LHpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(sst_word, n0_tag, n0lh_tag);
		cweight->m_map2STwN0ptN0LHpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0lh_word, sst_tag, n0_tag);
		cweight->m_map2STptN0ptN0LHw.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, sst_tag, n0lh_tag);
		cweight->m_map2STptN0wN0LHpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(st_word, n0_tag, n0ld_tag);
		cweight->m_mapSTwN0ptN0LDpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0ld_word, st_tag, n0_tag);
		cweight->m_mapSTptN0ptN0LDw.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, st_tag, n0ld_tag);
		cweight->m_mapSTptN0wN0LDpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(st2_word, n0_tag, n0ld_tag);
		cweight->m_mapST2wN0ptN0LDpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0ld_word, st2_tag, n0_tag);
		cweight->m_mapST2ptN0ptN0LDw.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, st2_tag, n0ld_tag);
		cweight->m_mapST2ptN0wN0LDpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		word_tag_tag.refer(sst_word, n0_tag, n0ld_tag);
		cweight->m_map2STwN0ptN0LDpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0ld_word, sst_tag, n0_tag);
		cweight->m_map2STptN0ptN0LDw.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);
		word_tag_tag.refer(n0_word, sst_tag, n0ld_tag);
		cweight->m_map2STptN0wN0LDpt.getOrUpdateScore(m_mapPackedScore, word_tag_tag, m_nScoreIndex, amount, m_nTrainingRound);

		set_of_3_tags = ENCODE_POSTAG_SET_3(st_tag, strh_tag, n0_tag);
		cweight->m_mapSTptSTRHptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_tags = ENCODE_POSTAG_SET_3(st_tag, strd_tag, n0_tag);
		cweight->m_mapSTptSTRDptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_tags = ENCODE_POSTAG_SET_3(st_tag, n0_tag, n0lh_tag);
		cweight->m_mapSTptN0ptN0LHpt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_tags = ENCODE_POSTAG_SET_3(st_tag, n0_tag, n0ld_tag);
		cweight->m_mapSTptN0ptN0LDpt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);

		set_of_3_tags = ENCODE_POSTAG_SET_3(st2_tag, st2rh_tag, n0_tag);
		cweight->m_mapST2ptST2RHptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_tags = ENCODE_POSTAG_SET_3(st2_tag, st2rd_tag, n0_tag);
		cweight->m_mapST2ptST2RDptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_tags = ENCODE_POSTAG_SET_3(st2_tag, n0_tag, n0lh_tag);
		cweight->m_mapST2ptN0ptN0LHpt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_tags = ENCODE_POSTAG_SET_3(st2_tag, n0_tag, n0ld_tag);
		cweight->m_mapST2ptN0ptN0LDpt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);

		set_of_3_tags = ENCODE_POSTAG_SET_3(sst_tag, sstrh_tag, n0_tag);
		cweight->m_map2STpt2STRHptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_tags = ENCODE_POSTAG_SET_3(sst_tag, sstrd_tag, n0_tag);
		cweight->m_map2STpt2STRDptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_tags = ENCODE_POSTAG_SET_3(sst_tag, n0_tag, n0lh_tag);
		cweight->m_map2STptN0ptN0LHpt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_tags = ENCODE_POSTAG_SET_3(sst_tag, n0_tag, n0ld_tag);
		cweight->m_map2STptN0ptN0LDpt.getOrUpdateScore(m_mapPackedScore, set_of_3_tags, m_nScoreIndex, amount, m_nTrainingRound);

		cweight->m_mapN_2w.getOrUpdateScore(m_mapPackedScore, n_2_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN_2pt.getOrUpdateScore(m_mapPackedScore, n_2_tag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN_1w.getOrUpdateScore(m_mapPackedScore, n_1_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN_1pt.getOrUpdateScore(m_mapPackedScore, n_1_tag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN1w.getOrUpdateScore(m_mapPackedScore, n1_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN1pt.getOrUpdateScore(m_mapPackedScore, n1_tag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN2w.getOrUpdateScore(m_mapPackedScore, n2_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN2pt.getOrUpdateScore(m_mapPackedScore, n2_tag, m_nScoreIndex, amount, m_nTrainingRound);
		bi_word.refer(n_2_word, n_1_word);
		cweight->m_mapN_2wN_1w.getOrUpdateScore(m_mapPackedScore, bi_word, m_nScoreIndex, amount, m_nTrainingRound);
		bi_word.refer(n_1_word, n0_word);
		cweight->m_mapN_1wN0w.getOrUpdateScore(m_mapPackedScore, bi_word, m_nScoreIndex, amount, m_nTrainingRound);
		bi_word.refer(n0_word, n1_word);
		cweight->m_mapN0wN1w.getOrUpdateScore(m_mapPackedScore, bi_word, m_nScoreIndex, amount, m_nTrainingRound);
		bi_word.refer(n1_word, n2_word);
		cweight->m_mapN1wN2w.getOrUpdateScore(m_mapPackedScore, bi_word, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_tags = ENCODE_POSTAG_SET_2(n_2_tag, n_1_tag);
		cweight->m_mapN_2ptN_1pt.getOrUpdateScore(m_mapPackedScore, set_of_2_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_tags = ENCODE_POSTAG_SET_2(n_1_tag, n0_tag);
		cweight->m_mapN_1ptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_2_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_tags = ENCODE_POSTAG_SET_2(n0_tag, n1_tag);
		cweight->m_mapN0ptN1pt.getOrUpdateScore(m_mapPackedScore, set_of_2_tags, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_tags = ENCODE_POSTAG_SET_2(n1_tag, n2_tag);
		cweight->m_mapN1ptN2pt.getOrUpdateScore(m_mapPackedScore, set_of_2_tags, m_nScoreIndex, amount, m_nTrainingRound);

		if (m_bChar) {
			cweight->m_map1CharBefore.getOrUpdateScore(m_mapPackedScore, nCharPrev(m_sSentence, n0_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharBefore.getOrUpdateScore(m_mapPackedScore, nCharPrev(m_sSentence, n0_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharBefore.getOrUpdateScore(m_mapPackedScore, nCharPrev(m_sSentence, n0_index, 3), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map1CharAfter.getOrUpdateScore(m_mapPackedScore, nCharNext(m_sSentence, n0_index, 1), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map2CharAfter.getOrUpdateScore(m_mapPackedScore, nCharNext(m_sSentence, n0_index, 2), m_nScoreIndex, amount, m_nTrainingRound);
			cweight->m_map3CharAfter.getOrUpdateScore(m_mapPackedScore, nCharNext(m_sSentence, n0_index, 3), m_nScoreIndex, amount, m_nTrainingRound);
		}

		if (m_bPath) {

			if (st_index == -1 || n0_index == -1) {
				cweight->m_mapSTPOSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSTFPOSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapSTPOSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.POSPath[st_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSTFPOSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.FPOSPath[st_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
			}

			if (st2_index == -1 || n0_index == -1) {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.POSPath[st2_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.FPOSPath[st2_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
			}

		}

	}

}
