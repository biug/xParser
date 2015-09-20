#include <cmath>
#include <stack>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <unordered_set>

#include "twostack_depparser.h"
#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace twostack {

	WordPOSTag DepParser::empty_taggedword = WordPOSTag();
	WordPOSTag DepParser::start_taggedword = WordPOSTag();
	WordPOSTag DepParser::middle_taggedword = WordPOSTag();
	WordPOSTag DepParser::end_taggedword = WordPOSTag();
	Tagset DepParser::empty_tagset = Tagset();

	DepParser::DepParser(const std::string & sFeatureInput, const std::string & sFeatureOut, const bool & bChar, const bool & bPath, const bool & bLabel, int nState) :
		DepParserBase(sFeatureInput, nState),
		m_bChar(bChar), m_bPath(bPath), m_bLabel(bLabel) {

		m_nSentenceLength = 0;

		m_Weight = new Weight(sFeatureInput, sFeatureOut);

		DepParser::empty_taggedword.refer(TWord::code(EMPTY_WORD), TPOSTag::code(EMPTY_POSTAG));
		DepParser::start_taggedword.refer(TWord::code(START_WORD), TPOSTag::code(START_POSTAG));
		DepParser::middle_taggedword.refer(TWord::code(MIDDLE_WORD), TPOSTag::code(MIDDLE_POSTAG));
		DepParser::end_taggedword.refer(TWord::code(END_WORD), TPOSTag::code(END_POSTAG));

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
		++m_nTrainingRound;
		if (!m_iCorrect.extractOracle(correct)) {
			++m_nTotalErrors;
			std::cout << "total error : " << m_nTrainingRound << std::endl;
			std::cout << "error at : " << m_nTotalErrors << std::endl;
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

		int step = 1;
		StateItem clearItem;

		while (step <= m_iCorrect.actionBack()) {

			decode();

			if (m_nState == TRAIN) {
				// early update check
				bool bCorrect = false;
				for (const auto & item : *m_pGenerator) {
					if (*item == clearItem) {
						bCorrect = true;
						break;
					}
				}
				if (!bCorrect) {
					m_iCorrect = clearItem;
					update();
					return;
				}
			}

			if (m_pGenerated->size() == 0) {
				break;
			}

#ifdef _DEBUG
			std::cout << "step : " << step << std::endl;
			m_pGenerated->bestUnsortItem().print();
#endif

			std::swap(m_pGenerated, m_pGenerator);

			clearItem.move(m_iCorrect.action(step++));
		}

		if (m_nState == PARSE && m_pGenerator->size() > 0) {
			while (true) {
				decode();
				if (m_pGenerated->size() == 0) {
					break;
				}

				std::swap(m_pGenerated, m_pGenerator);
			}
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

			if (iGenerator->canMem()) {
				mem(score);
			}

			if (iGenerator->canRecall()) {
				recall(score);
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
					arcLeftMem(score);
					arcRightMem(score);
					if (iGenerator->secondStackBack() >= 0) {
						arcLeftRecall(score);
						arcRightRecall(score);
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
		const StateItem & output = m_pGenerator->bestUnsortItem();

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
#ifdef _DEBUG
			std::cout << std::endl << "after move" << std::endl;
			m_iStateItem.print();
#endif
		}
	}

	void DepParser::getOrUpdateStackScore(const StateItem & item, const AddScoreType & amount) {

		Weight * cweight = (Weight*)m_Weight;

		const int & st_index = item.stackBack() == -1 ? -1 : item.stackTop();
		const int & stlh_index = st_index == -1 ? -1 : item.leftHead(st_index);
		const int & stlp_index = st_index == -1 ? -1 : item.leftPred(st_index);
		const int & strh_index = st_index == -1 ? -1 : item.rightHead(st_index);
		const int & strp_index = st_index == -1 ? -1 : item.rightPred(st_index);
		const int & st2_index = item.stackBack() <= 0 ? -1 : item.stackSubTop();
//		const int & st2lh_index = st2_index == -1 ? -1 : item.leftHead(st2_index);
//		const int & st2lp_index = st2_index == -1 ? -1 : item.leftPred(st2_index);
//		const int & st2rh_index = st2_index == -1 ? -1 : item.rightHead(st2_index);
//		const int & st2rp_index = st2_index == -1 ? -1 : item.rightPred(st2_index);
		const int & sst_index = item.secondStackBack() <= 0 ? -1 : item.secondStackTop();
		const int & n0_index = item.size() < m_nSentenceLength ? item.size() : -1;
		const int & n0lp_index = n0_index == -1 ? -1 : item.leftPred(n0_index);
		const int & n0lh_index = n0_index == -1 ? -1 : item.leftHead(n0_index);
		const int & stl2_index = st_index >= 2 ? st_index - 2 : -1;
		const int & stl1_index = st_index >= 1 ? st_index - 1 : -1;
		const int & str1_index = st_index + 1 < m_nSentenceLength ? st_index + 1 : -1;
		const int & str2_index = st_index + 2 < m_nSentenceLength ? st_index + 2 : -1;
		const int & n0l2_index = n0_index >= 2 ? n0_index - 2 : -1;
		const int & n0l1_index = n0_index >= 1 ? n0_index - 1 : -1;
		const int & n0r1_index = n0_index + 1 < m_nSentenceLength ? n0_index + 1 : -1;
		const int & n0r2_index = n0_index + 2 < m_nSentenceLength ? n0_index + 2 : -1;

		const WordPOSTag & st_word_postag = st_index == -1 ? start_taggedword : m_lSentence[st_index];
		const WordPOSTag & stlh_word_postag = stlh_index == -1 ? empty_taggedword : m_lSentence[stlh_index];
		const WordPOSTag & stlp_word_postag = stlp_index == -1 ? empty_taggedword : m_lSentence[stlp_index];
		const WordPOSTag & strh_word_postag = strh_index == -1 ? empty_taggedword : m_lSentence[strh_index];
		const WordPOSTag & strp_word_postag = strp_index == -1 ? empty_taggedword : m_lSentence[strp_index];
		const WordPOSTag & st2_word_postag = st2_index == -1 ? start_taggedword : m_lSentence[st2_index];
//		const WordPOSTag & st2lh_word_postag = st2lh_index == -1 ? empty_taggedword : m_lSentence[st2lh_index];
//		const WordPOSTag & st2lp_word_postag = st2lp_index == -1 ? empty_taggedword : m_lSentence[st2lp_index];
//		const WordPOSTag & st2rh_word_postag = st2rh_index == -1 ? empty_taggedword : m_lSentence[st2rh_index];
//		const WordPOSTag & st2rp_word_postag = st2rp_index == -1 ? empty_taggedword : m_lSentence[st2rp_index];
		const WordPOSTag & sst_word_postag = sst_index == -1 ? middle_taggedword : m_lSentence[sst_index];
		const WordPOSTag & n0_word_postag = n0_index == -1 ? end_taggedword : m_lSentence[n0_index];
		const WordPOSTag & n0lh_word_postag = n0lh_index == -1 ? empty_taggedword : m_lSentence[n0lh_index];
		const WordPOSTag & n0lp_word_postag = n0lp_index == -1 ? empty_taggedword : m_lSentence[n0lp_index];
		const WordPOSTag & stl2_word_postag = stl2_index == -1 ? start_taggedword : m_lSentence[stl2_index];
		const WordPOSTag & stl1_word_postag = stl1_index == -1 ? start_taggedword : m_lSentence[stl1_index];
		const WordPOSTag & str1_word_postag = str1_index == -1 ? end_taggedword : m_lSentence[str1_index];
		const WordPOSTag & str2_word_postag = str2_index == -1 ? end_taggedword : m_lSentence[str2_index];
		const WordPOSTag & n0l2_word_postag = n0l2_index == -1 ? start_taggedword : m_lSentence[n0l2_index];
		const WordPOSTag & n0l1_word_postag = n0l1_index == -1 ? start_taggedword : m_lSentence[n0l1_index];
		const WordPOSTag & n0r1_word_postag = n0r1_index == -1 ? end_taggedword : m_lSentence[n0r1_index];
		const WordPOSTag & n0r2_word_postag = n0r2_index == -1 ? end_taggedword : m_lSentence[n0r2_index];

		const Word & st_word = st_word_postag.first();
		const POSTag & st_postag = st_word_postag.second();
		const Tagset & st_llabelset = st_index == -1 ? empty_tagset : item.leftPredLabelSet(st_index);
		const Tagset & st_rlabelset = st_index == -1 ? empty_tagset : item.rightPredLabelSet(st_index);
		const Word & stlh_word = stlh_word_postag.first();
		const POSTag & stlh_postag = stlh_word_postag.second();
		const int & stlh_label = st_index == -1 ? 0 : item.leftHeadLabel(st_index);
		const int & stlh_arity = st_index == -1 ? -1 : item.leftHeadArity(st_index);
		const Word & stlp_word = stlp_word_postag.first();
		const POSTag & stlp_postag = stlp_word_postag.second();
		const POSTag & stlp2_postag = (st_index == -1 || item.leftSubPred(st_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(st_index)].second();
		const int & stlp_label = st_index == -1 ? 0 : item.leftPredLabel(st_index);
		const int & stlp_arity = st_index == -1 ? -1 : item.leftPredArity(st_index);
		const Word & strh_word = strh_word_postag.first();
		const POSTag & strh_postag = strh_word_postag.second();
		const int & strh_label = st_index == -1 ? 0 : item.rightHeadLabel(st_index);
		const int & strh_arity = st_index == -1 ? -1 : item.rightHeadArity(st_index);
		const Word & strp_word = strp_word_postag.first();
		const POSTag & strp_postag = strp_word_postag.second();
		const POSTag & strp2_postag = (st_index == -1 || item.rightSubPred(st_index) == -1) ? empty_taggedword.second() : m_lSentence[item.rightSubPred(st_index)].second();
		const int & strp_label = st_index == -1 ? 0 : item.rightPredLabel(st_index);
		const int & strp_arity = st_index == -1 ? -1 : item.rightPredArity(st_index);
		const Word & st2_word = st2_word_postag.first();
		const POSTag & st2_postag = st2_word_postag.second();
//		const Word & st2lh_word = st2lh_word_postag.first();
//		const POSTag & st2lh_postag = st2lh_word_postag.second();
//		const int & st2lh_label = st2_index == -1 ? 0 : item.leftHeadLabel(st2_index);
//		const int & st2lh_arity = st2_index == -1 ? -1 : item.leftHeadArity(st2_index);
//		const Word & st2lp_word = st2lp_word_postag.first();
//		const POSTag & st2lp_postag = st2lp_word_postag.second();
//		const POSTag & st2lp2_postag = (st2_index == -1 || item.leftSubPred(st2_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(st2_index)].second();
//		const int & st2lp_label = st2_index == -1 ? 0 : item.leftPredLabel(st2_index);
//		const int & st2lp_arity = st2_index == -1 ? -1 : item.leftPredArity(st2_index);
//		const Word & st2rh_word = st2rh_word_postag.first();
//		const POSTag & st2rh_postag = st2rh_word_postag.second();
//		const int & st2rh_label = st2_index == -1 ? 0 : item.rightHeadLabel(st2_index);
//		const int & st2rh_arity = st2_index == -1 ? -1 : item.rightHeadArity(st2_index);
//		const Word & st2rp_word = st2rp_word_postag.first();
//		const POSTag & st2rp_postag = st2rp_word_postag.second();
//		const POSTag & st2rp2_postag = (st2_index == -1 || item.rightSubPred(st2_index) == -1) ? empty_taggedword.second() : m_lSentence[item.rightSubPred(st2_index)].second();
//		const int & st2rp_label = st2_index == -1 ? 0 : item.rightPredLabel(st2_index);
//		const int & st2rp_arity = st2_index == -1 ? -1 : item.rightPredArity(st2_index);
		const Word & sst_word = sst_word_postag.first();
		const POSTag & sst_postag = sst_word_postag.second();
		const Word & n0_word = n0_word_postag.first();
		const POSTag & n0_postag = n0_word_postag.second();
		const Tagset & n0_llabelset = n0_index == -1 ? empty_tagset : item.leftPredLabelSet(n0_index);
		const Word & n0lh_word = n0lh_word_postag.first();
		const POSTag & n0lh_postag = n0lh_word_postag.second();
		const int & n0lh_label = n0_index == -1 ? 0 : item.leftHeadLabel(n0_index);
		const int & n0lh_arity = n0_index == -1 ? -1 : item.leftHeadArity(n0_index);
		const Word & n0lp_word = n0lp_word_postag.first();
		const POSTag & n0lp_postag = n0lp_word_postag.second();
		const POSTag & n0lp2_postag = (n0_index == -1 || item.leftSubPred(n0_index) == -1) ? empty_taggedword.second() : m_lSentence[item.leftSubPred(n0_index)].second();
		const int & n0lp_label = n0_index == -1 ? 0 : item.leftPredLabel(n0_index);
		const int & n0lp_arity = n0_index == -1 ? -1 : item.leftPredArity(n0_index);
		const Word & stl2_word = stl2_word_postag.first();
		const POSTag & stl2_postag = stl2_word_postag.second();
		const Word & stl1_word = stl1_word_postag.first();
		const POSTag & stl1_postag = stl1_word_postag.second();
		const Word & str1_word = str1_word_postag.first();
		const POSTag & str1_postag = str1_word_postag.second();
		const Word & str2_word = str2_word_postag.first();
		const POSTag & str2_postag = str2_word_postag.second();
		const Word & n0l2_word = n0l2_word_postag.first();
		const POSTag & n0l2_postag = n0l2_word_postag.second();
		const Word & n0l1_word = n0l1_word_postag.first();
		const POSTag & n0l1_postag = n0l1_word_postag.second();
		const Word & n0r1_word = n0r1_word_postag.first();
		const POSTag & n0r1_postag = n0r1_word_postag.second();
		const Word & n0r2_word = n0r2_word_postag.first();
		const POSTag & n0r2_postag = n0r2_word_postag.second();

		int dis = -1;

		// uni-gram
		// st, n0, st2
		cweight->m_mapSTw.getOrUpdateScore(m_mapPackedScore, st_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSTpt.getOrUpdateScore(m_mapPackedScore, st_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapST2w.getOrUpdateScore(m_mapPackedScore, st2_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapST2pt.getOrUpdateScore(m_mapPackedScore, st2_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSSTw.getOrUpdateScore(m_mapPackedScore, sst_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSSTpt.getOrUpdateScore(m_mapPackedScore, sst_postag, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN0w.getOrUpdateScore(m_mapPackedScore, n0_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN0pt.getOrUpdateScore(m_mapPackedScore, n0_postag, m_nScoreIndex, amount, m_nTrainingRound);

		// unigram context
		// st + sti, n0 + n0i
		word_int.refer(stl2_word, -2);
		cweight->m_mapSTiw.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(stl1_word, -1);
		cweight->m_mapSTiw.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(str1_word, 1);
		cweight->m_mapSTiw.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(str2_word, 2);
		cweight->m_mapSTiw.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		two_words_int.refer(stl2_word, stl1_word, -1);
		cweight->m_mapSTiwSTjw.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);
		two_words_int.refer(stl1_word, str1_word, 0);
		cweight->m_mapSTiwSTjw.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);
		two_words_int.refer(str1_word, str2_word, 1);
		cweight->m_mapSTiwSTjw.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(n0l2_word, -2);
		cweight->m_mapN0iw.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(n0l1_word, -1);
		cweight->m_mapN0iw.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(n0r1_word, 1);
		cweight->m_mapN0iw.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(n0r2_word, 2);
		cweight->m_mapN0iw.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		two_words_int.refer(n0l2_word, n0l1_word, -1);
		cweight->m_mapN0iwN0jw.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);
		two_words_int.refer(n0l1_word, n0r1_word, 0);
		cweight->m_mapN0iwN0jw.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);
		two_words_int.refer(n0r1_word, n0r2_word, 1);
		cweight->m_mapN0iwN0jw.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);

		postag_int.refer(stl2_postag, -2);
		cweight->m_mapSTipt.getOrUpdateScore(m_mapPackedScore, postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_int.refer(stl1_postag, -1);
		cweight->m_mapSTipt.getOrUpdateScore(m_mapPackedScore, postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_int.refer(str1_postag, 1);
		cweight->m_mapSTipt.getOrUpdateScore(m_mapPackedScore, postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_int.refer(str2_postag, 2);
		cweight->m_mapSTipt.getOrUpdateScore(m_mapPackedScore, postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_int.refer(stl2_postag, stl1_postag, -1);
		cweight->m_mapSTiptSTjpt.getOrUpdateScore(m_mapPackedScore, postag_set_2_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_int.refer(stl1_postag, str1_postag, 0);
		cweight->m_mapSTiptSTjpt.getOrUpdateScore(m_mapPackedScore, postag_set_2_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_int.refer(str1_postag, str2_postag, 1);
		cweight->m_mapSTiptSTjpt.getOrUpdateScore(m_mapPackedScore, postag_set_2_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_int.refer(n0l2_postag, -2);
		cweight->m_mapN0ipt.getOrUpdateScore(m_mapPackedScore, postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_int.refer(n0l1_postag, -1);
		cweight->m_mapN0ipt.getOrUpdateScore(m_mapPackedScore, postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_int.refer(n0r1_postag, 1);
		cweight->m_mapN0ipt.getOrUpdateScore(m_mapPackedScore, postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_int.refer(n0r2_postag, 2);
		cweight->m_mapN0ipt.getOrUpdateScore(m_mapPackedScore, postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_int.refer(n0l2_postag, n0l1_postag, -1);
		cweight->m_mapN0iptN0jpt.getOrUpdateScore(m_mapPackedScore, postag_set_2_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_int.refer(n0l1_postag, n0r1_postag, 0);
		cweight->m_mapN0iptN0jpt.getOrUpdateScore(m_mapPackedScore, postag_set_2_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_int.refer(n0r1_postag, n0r2_postag, 1);
		cweight->m_mapN0iptN0jpt.getOrUpdateScore(m_mapPackedScore, postag_set_2_int, m_nScoreIndex, amount, m_nTrainingRound);

		// unigram with label
		// st, n0
		if (m_bLabel) {
			word_int.refer(st_word, stlh_label);
			cweight->m_mapSTwSTLHl.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
			word_int.refer(st_word, stlp_label);
			cweight->m_mapSTwSTLPl.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
			word_int.refer(st_word, strh_label);
			cweight->m_mapSTwSTRHl.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
			word_int.refer(st_word, strp_label);
			cweight->m_mapSTwSTRPl.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
			word_int.refer(n0_word, n0lh_label);
			cweight->m_mapN0wN0LHl.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
			word_int.refer(n0_word, n0lp_label);
			cweight->m_mapN0wN0LPl.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		}

		// unigram with arity
		// st, n0
		word_int.refer(st_word, stlh_arity);
		cweight->m_mapSTwSTLHi.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(st_word, stlp_arity);
		cweight->m_mapSTwSTLPi.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(st_word, strh_arity);
		cweight->m_mapSTwSTRHi.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(st_word, strp_arity);
		cweight->m_mapSTwSTRPi.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(st_word, stlh_arity + strh_arity);
		cweight->m_mapSTwSTHi.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(st_word, stlp_arity + strp_arity);
		cweight->m_mapSTwSTPi.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(n0_word, n0lh_arity);
		cweight->m_mapN0wN0LHi.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_int.refer(n0_word, n0lp_arity);
		cweight->m_mapN0wN0LPi.getOrUpdateScore(m_mapPackedScore, word_int, m_nScoreIndex, amount, m_nTrainingRound);

		// bi-gram
		// st + n0
		two_words.refer(st_word, n0_word);
		cweight->m_mapSTwN0w.getOrUpdateScore(m_mapPackedScore, two_words, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(st_word, n0_postag);
		cweight->m_mapSTwN0pt.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(n0_word, st_postag);
		cweight->m_mapSTptN0w.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_postags = ENCODE_POSTAG_SET_2(st_postag, n0_postag);
		cweight->m_mapSTptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_2_postags, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(st_word, n0_word, st_postag);
		cweight->m_mapSTwptN0w.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(st_word, n0_word, n0_postag);
		cweight->m_mapSTwN0wpt.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(st_word, st_postag, n0_postag);
		cweight->m_mapSTwptN0pt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(n0_word, st_postag, n0_postag);
		cweight->m_mapSTptN0wpt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag_postag.refer(st_word, n0_word, st_postag, n0_postag);
		cweight->m_mapSTwptN0wpt.getOrUpdateScore(m_mapPackedScore, word_word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + dis
		dis = encodeLinkDistanceOrDirection(st_index, n0_index == -1 ? m_nSentenceLength : n0_index, false);
		two_words_int.refer(st_word, n0_word, dis);
		cweight->m_mapSTwN0wD.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_int.refer(st_word, n0_postag, dis);
		cweight->m_mapSTwN0ptD.getOrUpdateScore(m_mapPackedScore, word_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_int.refer(n0_word, st_postag, dis);
		cweight->m_mapSTptN0wD.getOrUpdateScore(m_mapPackedScore, word_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_int.refer(st_postag, n0_postag, dis);
		cweight->m_mapSTptN0ptD.getOrUpdateScore(m_mapPackedScore, postag_set_2_int, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + st left/right head/pred
		two_words_int.refer(st_word, n0_word, strp_arity);
		cweight->m_mapSTwN0wSTRPi.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);
		two_words_int.refer(st_word, n0_word, n0lp_arity);
		cweight->m_mapSTwN0wN0LPi.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + n0
		two_words.refer(st2_word, n0_word);
		cweight->m_mapST2wN0w.getOrUpdateScore(m_mapPackedScore, two_words, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(st2_word, n0_postag);
		cweight->m_mapST2wN0pt.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(n0_word, st2_postag);
		cweight->m_mapST2ptN0w.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_postags = ENCODE_POSTAG_SET_2(st2_postag, n0_postag);
		cweight->m_mapST2ptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_2_postags, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(st2_word, n0_word, st2_postag);
		cweight->m_mapST2wptN0w.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(st2_word, n0_word, n0_postag);
		cweight->m_mapST2wN0wpt.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(st2_word, st2_postag, n0_postag);
		cweight->m_mapST2wptN0pt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(n0_word, st2_postag, n0_postag);
		cweight->m_mapST2ptN0wpt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag_postag.refer(st2_word, n0_word, st2_postag, n0_postag);
		cweight->m_mapST2wptN0wpt.getOrUpdateScore(m_mapPackedScore, word_word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + dis
		dis = encodeLinkDistanceOrDirection(st2_index, n0_index == -1 ? m_nSentenceLength : n0_index, false);
		two_words_int.refer(st2_word, n0_word, dis);
		cweight->m_mapST2wN0wD.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_int.refer(st2_word, n0_postag, dis);
		cweight->m_mapST2wN0ptD.getOrUpdateScore(m_mapPackedScore, word_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_int.refer(n0_word, st2_postag, dis);
		cweight->m_mapST2ptN0wD.getOrUpdateScore(m_mapPackedScore, word_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_int.refer(st2_postag, n0_postag, dis);
		cweight->m_mapST2ptN0ptD.getOrUpdateScore(m_mapPackedScore, postag_set_2_int, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + n0
		two_words.refer(sst_word, n0_word);
		cweight->m_mapSSTwN0w.getOrUpdateScore(m_mapPackedScore, two_words, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(sst_word, n0_postag);
		cweight->m_mapSSTwN0pt.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(n0_word, sst_postag);
		cweight->m_mapSSTptN0w.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_postags = ENCODE_POSTAG_SET_2(sst_postag, n0_postag);
		cweight->m_mapSSTptN0pt.getOrUpdateScore(m_mapPackedScore, set_of_2_postags, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(sst_word, n0_word, sst_postag);
		cweight->m_mapSSTwptN0w.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(sst_word, n0_word, n0_postag);
		cweight->m_mapSSTwN0wpt.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(sst_word, sst_postag, n0_postag);
		cweight->m_mapSSTwptN0pt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(n0_word, sst_postag, n0_postag);
		cweight->m_mapSSTptN0wpt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag_postag.refer(sst_word, n0_word, sst_postag, n0_postag);
		cweight->m_mapSSTwptN0wpt.getOrUpdateScore(m_mapPackedScore, word_word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + dis
		dis = encodeLinkDistanceOrDirection(sst_index, n0_index == -1 ? m_nSentenceLength : n0_index, false);
		two_words_int.refer(sst_word, n0_word, dis);
		cweight->m_mapSSTwN0wD.getOrUpdateScore(m_mapPackedScore, two_words_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_int.refer(sst_word, n0_postag, dis);
		cweight->m_mapSSTwN0ptD.getOrUpdateScore(m_mapPackedScore, word_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_int.refer(n0_word, sst_postag, dis);
		cweight->m_mapSSTptN0wD.getOrUpdateScore(m_mapPackedScore, word_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_int.refer(sst_postag, n0_postag, dis);
		cweight->m_mapSSTptN0ptD.getOrUpdateScore(m_mapPackedScore, postag_set_2_int, m_nScoreIndex, amount, m_nTrainingRound);

		// st + st2
		two_words.refer(st_word, st2_word);
		cweight->m_mapSTwST2w.getOrUpdateScore(m_mapPackedScore, two_words, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(st_word, st2_postag);
		cweight->m_mapSTwST2pt.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(st2_word, st_postag);
		cweight->m_mapSTptST2w.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_postags = ENCODE_POSTAG_SET_2(st_postag, st2_postag);
		cweight->m_mapSTptST2pt.getOrUpdateScore(m_mapPackedScore, set_of_2_postags, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(st_word, st2_word, st_postag);
		cweight->m_mapSTwptST2w.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(st_word, st2_word, st2_postag);
		cweight->m_mapSTwST2wpt.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(st_word, st_postag, st2_postag);
		cweight->m_mapSTwptST2pt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(st2_word, st_postag, st2_postag);
		cweight->m_mapSTptST2wpt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag_postag.refer(st_word, st2_word, st_postag, st2_postag);
		cweight->m_mapSTwptST2wpt.getOrUpdateScore(m_mapPackedScore, word_word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);

		// st + sst
		two_words.refer(st_word, sst_word);
		cweight->m_mapSTwSSTw.getOrUpdateScore(m_mapPackedScore, two_words, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(st_word, sst_postag);
		cweight->m_mapSTwSSTpt.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag.refer(sst_word, st_postag);
		cweight->m_mapSTptSSTw.getOrUpdateScore(m_mapPackedScore, word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_2_postags = ENCODE_POSTAG_SET_2(st_postag, sst_postag);
		cweight->m_mapSTptSSTpt.getOrUpdateScore(m_mapPackedScore, set_of_2_postags, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(st_word, sst_word, st_postag);
		cweight->m_mapSTwptSSTw.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag.refer(st_word, sst_word, sst_postag);
		cweight->m_mapSTwSSTwpt.getOrUpdateScore(m_mapPackedScore, word_word_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(st_word, st_postag, sst_postag);
		cweight->m_mapSTwptSSTpt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(sst_word, st_postag, sst_postag);
		cweight->m_mapSTptSSTwpt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_word_postag_postag.refer(st_word, sst_word, st_postag, sst_postag);
		cweight->m_mapSTwptSSTwpt.getOrUpdateScore(m_mapPackedScore, word_word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);

		// tri-gram
		// st + n0 + st2
		word_postag_postag.refer(st_word, st2_postag, n0_postag);
		cweight->m_mapSTwN0ptST2pt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(n0_word, st_postag, st2_postag);
		cweight->m_mapSTptN0wST2pt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(st2_word, st_postag, n0_postag);
		cweight->m_mapSTptN0ptST2w.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_postags = ENCODE_POSTAG_SET_3(st_postag, st2_postag, n0_postag);
		cweight->m_mapSTptN0ptST2pt.getOrUpdateScore(m_mapPackedScore, set_of_3_postags, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + sst
		word_postag_postag.refer(st_word, sst_postag, n0_postag);
		cweight->m_mapSTwN0ptSSTpt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(n0_word, st_postag, sst_postag);
		cweight->m_mapSTptN0wSSTpt.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag.refer(sst_word, st_postag, n0_postag);
		cweight->m_mapSTptN0ptSSTw.getOrUpdateScore(m_mapPackedScore, word_postag_postag, m_nScoreIndex, amount, m_nTrainingRound);
		set_of_3_postags = ENCODE_POSTAG_SET_3(st_postag, sst_postag, n0_postag);
		cweight->m_mapSTptN0ptSSTpt.getOrUpdateScore(m_mapPackedScore, set_of_3_postags, m_nScoreIndex, amount, m_nTrainingRound);

		// st + n0 + st left/right head/pred
		word_postag_postag_int.refer(st_word, stlh_postag, n0_postag, stlh_label);
		cweight->m_mapSTwN0ptSTLHptSTLHl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(n0_word, st_postag, stlh_postag, stlh_label);
		cweight->m_mapSTptN0wSTLHptSTLHl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(stlh_word, st_postag, n0_postag, stlh_label);
		cweight->m_mapSTptN0ptSTLHwSTLHl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_3_int.refer(st_postag, stlh_postag, n0_postag, stlh_label);
		cweight->m_mapSTptN0ptSTLHptSTLHl.getOrUpdateScore(m_mapPackedScore, postag_set_3_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(st_word, stlp_postag, n0_postag, stlp_label);
		cweight->m_mapSTwN0ptSTLPptSTLPl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(n0_word, st_postag, stlp_postag, stlp_label);
		cweight->m_mapSTptN0wSTLPptSTLPl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(stlp_word, st_postag, n0_postag, stlp_label);
		cweight->m_mapSTptN0ptSTLPwSTLPl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_3_int.refer(st_postag, stlp_postag, n0_postag, stlp_label);
		cweight->m_mapSTptN0ptSTLPptSTLPl.getOrUpdateScore(m_mapPackedScore, postag_set_3_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(st_word, strh_postag, n0_postag, strh_label);
		cweight->m_mapSTwN0ptSTRHptSTRHl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(n0_word, st_postag, strh_postag, strh_label);
		cweight->m_mapSTptN0wSTRHptSTRHl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(strh_word, st_postag, n0_postag, strh_label);
		cweight->m_mapSTptN0ptSTRHwSTRHl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_3_int.refer(st_postag, strh_postag, n0_postag, strh_label);
		cweight->m_mapSTptN0ptSTRHptSTRHl.getOrUpdateScore(m_mapPackedScore, postag_set_3_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(st_word, strp_postag, n0_postag, strp_label);
		cweight->m_mapSTwN0ptSTRPptSTRPl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(n0_word, st_postag, strp_postag, strp_label);
		cweight->m_mapSTptN0wSTRPptSTRPl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(strp_word, st_postag, n0_postag, strp_label);
		cweight->m_mapSTptN0ptSTRPwSTRPl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_3_int.refer(st_postag, strp_postag, n0_postag, strp_label);
		cweight->m_mapSTptN0ptSTRPptSTRPl.getOrUpdateScore(m_mapPackedScore, postag_set_3_int, m_nScoreIndex, amount, m_nTrainingRound);

		// st + n0 + n0 left head/pred
		word_postag_postag_int.refer(st_word, n0_postag, n0lh_postag, n0lh_label);
		cweight->m_mapSTwN0ptN0LHptN0LHl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(n0_word, st_postag, n0lh_postag, n0lh_label);
		cweight->m_mapSTptN0wN0LHptN0LHl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(n0lh_word, st_postag, n0_postag, n0lh_label);
		cweight->m_mapSTptN0ptN0LHwN0LHl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_3_int.refer(st_postag, n0_postag, n0lh_postag, n0lh_label);
		cweight->m_mapSTptN0ptN0LHptN0LHl.getOrUpdateScore(m_mapPackedScore, postag_set_3_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(st_word, n0_postag, n0lp_postag, n0lp_label);
		cweight->m_mapSTwN0ptN0LPptN0LPl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(n0_word, st_postag, n0lp_postag, n0lp_label);
		cweight->m_mapSTptN0wN0LPptN0LPl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		word_postag_postag_int.refer(n0lp_word, st_postag, n0_postag, n0lp_label);
		cweight->m_mapSTptN0ptN0LPwN0LPl.getOrUpdateScore(m_mapPackedScore, word_postag_postag_int, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_3_int.refer(st_postag, n0_postag, n0lp_postag, n0lp_label);
		cweight->m_mapSTptN0ptN0LPptN0LPl.getOrUpdateScore(m_mapPackedScore, postag_set_3_int, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// st + n0 + st left head + st left pred
		// st + n0 + st right head + st right pred
		set_of_4_postags = encodePostagSet4(st_postag, strh_postag, strp_postag, n0_postag);
		cweight->m_mapSTptN0ptSTRHptSTRPpt.getOrUpdateScore(m_mapPackedScore, set_of_4_postags, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + st left pred + st left pred 2
		set_of_4_postags = encodePostagSet4(st_postag, stlp_postag, stlp2_postag, n0_postag);
		cweight->m_mapSTptN0ptSTLPptSTLP2pt.getOrUpdateScore(m_mapPackedScore, set_of_4_postags, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + st right pred + st right pred 2
		set_of_4_postags = encodePostagSet4(st_postag, strp_postag, strp2_postag, n0_postag);
		cweight->m_mapSTptN0ptSTRPptSTRP2pt.getOrUpdateScore(m_mapPackedScore, set_of_4_postags, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + n0 left pred + n0 left pred 2
		set_of_4_postags = encodePostagSet4(st_postag, n0_postag, n0lp_postag, n0lp2_postag);
		cweight->m_mapSTptN0ptN0LPptN0LP2pt.getOrUpdateScore(m_mapPackedScore, set_of_4_postags, m_nScoreIndex, amount, m_nTrainingRound);

		word_tagset.refer(st_word, st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTwSTll.getOrUpdateScore(m_mapPackedScore, word_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		word_tagset.refer(st_word, st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTwSTrl.getOrUpdateScore(m_mapPackedScore, word_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		word_tagset.refer(n0_word, n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapN0wN0ll.getOrUpdateScore(m_mapPackedScore, word_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		postag_tagset.refer(st_postag, st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTptSTll.getOrUpdateScore(m_mapPackedScore, postag_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		postag_tagset.refer(st_postag, st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTptSTrl.getOrUpdateScore(m_mapPackedScore, postag_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		postag_tagset.refer(n0_postag, n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapN0ptN0ll.getOrUpdateScore(m_mapPackedScore, postag_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_tagset.refer(st_postag, n0_postag, st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTptN0ptSTll.getOrUpdateScore(m_mapPackedScore, postag_set_2_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_tagset.refer(st_postag, n0_postag, st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTptN0ptSTrl.getOrUpdateScore(m_mapPackedScore, postag_set_2_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		postag_set_2_tagset.refer(st_postag, n0_postag, n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapSTptN0ptN0ll.getOrUpdateScore(m_mapPackedScore, postag_set_2_tagset, m_nScoreIndex, amount, m_nTrainingRound);

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

		if (m_bLabel) {

		}
	}

}
