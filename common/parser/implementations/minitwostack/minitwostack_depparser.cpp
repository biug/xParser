#include <cmath>
#include <stack>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <unordered_set>

#include "minitwostack_depparser.h"
#include "common/token/word.h"
#include "common/token/pos.h"
#include "common/token/deplabel.h"

namespace minitwostack {

WordPOSTag DepParser::empty_taggedword = WordPOSTag();
WordPOSTag DepParser::start_taggedword = WordPOSTag();
WordPOSTag DepParser::end_taggedword = WordPOSTag();
WordPOSTag DepParser::middle_taggedword = WordPOSTag();
TagSet DepParser::empty_tagset = TagSet();

	DepParser::DepParser(const std::string & sFeatureInput, const std::string & sFeatureOutput, int nState, bool bChar, bool bPath) :
		DepParserBase(nState), m_bChar(bChar), m_bPath(bPath) {

		m_Weight = new Weight(sFeatureInput, sFeatureOutput);

		DepParser::empty_taggedword.refer(TWord::code(EMPTY_WORD), TPOSTag::code(EMPTY_POSTAG));
		DepParser::start_taggedword.refer(TWord::code(START_WORD), TPOSTag::code(START_POSTAG));
		DepParser::end_taggedword.refer(TWord::code(END_WORD), TPOSTag::code(END_POSTAG));
		DepParser::middle_taggedword.refer(TWord::code(MIDDLE_WORD), TPOSTag::code(MIDDLE_POSTAG));
	}

	DepParser::~DepParser() {
		delete m_Weight;
	}

	void DepParser::train(const DependencyGraph & correct, const int & round) {
		// initialize
		int idx = 0;
		m_nSentenceLength = 0;
		m_nTrueSentenceLength = correct.size();
		m_dtSyntaxTree.clear();
		for (const auto & node : correct) {
			m_dtSyntaxTree.push_back(DependencyTreeNode(POSTaggedWord(node.m_sWord, node.m_sPOSTag), node.m_nTreeHead, "_"));
			m_lSentence[idx].refer(TWord::code(node.m_sWord), TPOSTag::code(node.m_sPOSTag));
			if (correct[idx].m_sSuperTag == "K") {
				m_lKeepSentence[m_nSentenceLength++] = idx;
			}
			++idx;
		}
		if (m_bPath) {
			m_lcaAnalyzer.loadPath(m_dtSyntaxTree);
		}
		// train
		int lastTotalErrors = m_nTotalErrors;
		int lastTrainingRound = m_nTrainingRound;
		if (m_nSentenceLength > 0) {
			work(nullptr, correct);
		}
		if (lastTrainingRound > 0) {
			nBackSpace("error rate 0.0000 ( " + std::to_string(lastTotalErrors) + " / " + std::to_string(lastTrainingRound) + " ) ");
		}
		if (m_nTrainingRound > 0) {
			std::cout << "error rate " << ((double)m_nTotalErrors / (double)m_nTrainingRound);
			std::cout << " ( " << m_nTotalErrors << " / " << m_nTrainingRound << " ) " << std::flush;
		}
	}

	void DepParser::parse(const DependencyGraph & sentence, DependencyGraph * retval) {
		int idx = 0;
		m_nSentenceLength = 0;
		m_nTrueSentenceLength = sentence.size();
		m_dtSyntaxTree.clear();
		for (const auto & token : sentence) {
			m_dtSyntaxTree.push_back(DependencyTreeNode(POSTaggedWord(token.m_sWord, token.m_sPOSTag), token.m_nTreeHead, "_"));
			m_lSentence[idx].refer(TWord::code(token.m_sWord), TPOSTag::code(token.m_sPOSTag));
			if (sentence[idx].m_sSuperTag == "K") {
				m_lKeepSentence[m_nSentenceLength++] = idx;
			}
			++idx;
		}
		if (m_bPath) {
			m_lcaAnalyzer.loadPath(m_dtSyntaxTree);
		}
		work(retval, sentence);
		if (m_nTrainingRound > 1) {
			nBackSpace("parsing sentence " + std::to_string(m_nTrainingRound - 1));
		}
		std::cout << "parsing sentence " << m_nTrainingRound << std::flush;
	}

	void DepParser::goldCheck(const DependencyGraph & correct) {
		StateItem output;
		if (!output.extractOracle(correct)) {
			++m_nTotalErrors;
			if (m_nTotalErrors > 1) {
				nBackSpace("error No. " + std::to_string(m_nTotalErrors - 1));
			}
			std::cout << "error No." << m_nTotalErrors << std::flush;
		}
	}

	void DepParser::work(DependencyGraph * retval, const DependencyGraph & correct) {

		m_abItems[0].clear();
		m_abItems[1].clear();
		m_abFinished.clear();
		m_iCandidate.clear();

		m_iCorrect.clear();

		if (m_nState == TRAIN && ! m_iCorrect.extractOracle(correct)) {
			return;
		}

		// training only if it has an oracle
		++m_nTrainingRound;

		m_pGenerator = &m_abItems[0];
		m_pGenerated = &m_abItems[1];

		m_pGenerator->insertItem(m_iCandidate);

		int step = 1;
		StateItem clearItem;

		while (step <= m_iCorrect.actionBack()) {
			decode();

			if (m_nState == TRAIN) {
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

		this->m_pGenerated->clear();

		for (const auto & iGenerator : *m_pGenerator) {
			m_abScores.clear();
			getActionScores(*iGenerator);

			const tscore & score = iGenerator->getScore();

			if (!iGenerator->stackEmpty()) {
				reduce(score);
			}

			if (iGenerator->size() < m_nSentenceLength) {
				if (iGenerator->canArc()) {
					arcReduce(score);
					if (iGenerator->canShift()) {
						arcShift(score);
					}
					if (iGenerator->canMem()) {
						arcMem(score);
					}
					if (iGenerator->canRecall()) {
						arcRecall(score);
					}
				}
				if (iGenerator->canShift()) {
					shift(score);
				}
			}

			if (iGenerator->canMem()) {
				mem(score);
			}

			if (iGenerator->canRecall()) {
				recall(score);
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
		}
	}

	void DepParser::update() {
		m_iStatesItem.clear();
		const StateItem & output = m_pGenerator->bestUnsortItem();

		int index = 0;
		if (output != m_iCorrect) {

			while (m_iStatesItem != output && index <= output.actionBack() && index <= m_iCorrect.actionBack()) {
				const int & action = output.action(index);
				const int & correct_action = m_iCorrect.action(index);
				if (action == correct_action) {
					m_iStatesItem.move(action);
				}
				else {
					break;
				}
				++index;
			}

			updateScoreForState(m_iStatesItem, m_iCorrect, index, 1);
			updateScoreForState(m_iStatesItem, output, index, -1);

			++m_nTotalErrors;
		}
	}

	void DepParser::generate(DependencyGraph * retval, const DependencyGraph & correct) {
		m_pGenerator->bestUnsortItem().generateGraph(correct, *retval);
	}

	void DepParser::getActionScores(const StateItem & item) {
		memset(m_mapPackedScore, 0, sizeof(tscore) * MAX_ACTION);
		getOrUpdateStackScore(item, std::make_pair<int, const int &>(NO_ACTION, 0));
	}

	void DepParser::updateScoreForState(const StateItem & from, const StateItem & output, int action_index, const int & amount) {
		m_iStateItem = from;
		std::pair<int, int> m_pairAmount(ACTION_START, amount);
		while (m_iStateItem != output && action_index <= output.actionBack()) {
			m_pairAmount.first = output.action(action_index++);
			getOrUpdateStackScore(m_iStateItem, m_pairAmount);
			m_iStateItem.move(m_pairAmount.first);
		}
	}

	void DepParser::getOrUpdateStackScore(const StateItem & item, const ActionScoreIncrement & amount) {

		int dis = -1;

		const int outIndex = -1;
		const int emptyLabel = 0;
		const int emptyArity = -1;

		Weight * cweight = (Weight*)m_Weight;

		// st
		const int & st_index = item.stackBack() == -1 ? outIndex : item.stackTop();
		const int & stlh_index = st_index == -1 ? outIndex : item.leftHead(st_index);
		const int & stlp_index = st_index == -1 ? outIndex : item.leftPred(st_index);
		const int & strh_index = st_index == -1 ? outIndex : item.rightHead(st_index);
		const int & strp_index = st_index == -1 ? outIndex : item.rightPred(st_index);
		// st
		const WordPOSTag & st_word_postag = st_index == -1 ? start_taggedword : m_lSentence[m_lKeepSentence[st_index]];
		const WordPOSTag & stlh_word_postag = stlh_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[stlh_index]];
		const WordPOSTag & stlp_word_postag = stlp_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[stlp_index]];
		const WordPOSTag & strh_word_postag = strh_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[strh_index]];
		const WordPOSTag & strp_word_postag = strp_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[strp_index]];
		// st
		const Word & st_word = st_word_postag.first();
		const POSTag & st_postag = st_word_postag.second();
		const TagSet & st_llabelset = st_index == -1 ? empty_tagset : item.leftPredLabelSet(st_index);
		const TagSet & st_rlabelset = st_index == -1 ? empty_tagset : item.rightPredLabelSet(st_index);
		const POSTag & stlh_postag = stlh_word_postag.second();
		const int & stlh_label = st_index == -1 ? emptyLabel : item.leftHeadLabel(st_index);
		const POSTag & stlp_postag = stlp_word_postag.second();
		const POSTag & stlp2_postag = (st_index == -1 || item.leftSubPred(st_index) == -1) ? empty_taggedword.second() : m_lSentence[m_lKeepSentence[item.leftSubPred(st_index)]].second();
		const int & stlp_label = st_index == -1 ? emptyLabel : item.leftPredLabel(st_index);
		const int & stlp2_label = st_index == -1 ? emptyLabel : item.leftSubPredLabel(st_index);
		const POSTag & strh_postag = strh_word_postag.second();
		const int & strh_label = st_index == -1 ? emptyLabel : item.rightHeadLabel(st_index);
		const POSTag & strp_postag = strp_word_postag.second();
		const POSTag & strp2_postag = (st_index == -1 || item.rightSubPred(st_index) == -1) ? empty_taggedword.second() : m_lSentence[m_lKeepSentence[item.rightSubPred(st_index)]].second();
		const int & strp_label = st_index == -1 ? emptyLabel : item.rightPredLabel(st_index);
		const int & strp2_label = st_index == -1 ? emptyLabel : item.rightSubPredLabel(st_index);
		const int & strp_arity = st_index == -1 ? emptyArity : item.rightPredArity(st_index);

		// st2
		const int & st2_index = item.stackBack() <= 0 ? outIndex : item.stackSubTop();
		const int & st2lh_index = st2_index == -1 ? outIndex : item.leftHead(st2_index);
		const int & st2lp_index = st2_index == -1 ? outIndex : item.leftPred(st2_index);
		const int & st2rh_index = st2_index == -1 ? outIndex : item.rightHead(st2_index);
		const int & st2rp_index = st2_index == -1 ? outIndex : item.rightPred(st2_index);
		// st2
		const WordPOSTag & st2_word_postag = st2_index == -1 ? start_taggedword : m_lSentence[m_lKeepSentence[st2_index]];
		const WordPOSTag & st2lh_word_postag = st2lh_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[st2lh_index]];
		const WordPOSTag & st2lp_word_postag = st2lp_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[st2lp_index]];
		const WordPOSTag & st2rh_word_postag = st2rh_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[st2rh_index]];
		const WordPOSTag & st2rp_word_postag = st2rp_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[st2rp_index]];
		// st2
		const Word & st2_word = st2_word_postag.first();
		const POSTag & st2_postag = st2_word_postag.second();
		const TagSet & st2_llabelset = st2_index == -1 ? empty_tagset : item.leftPredLabelSet(st2_index);
		const TagSet & st2_rlabelset = st2_index == -1 ? empty_tagset : item.rightPredLabelSet(st2_index);
		const POSTag & st2lh_postag = st2lh_word_postag.second();
		const int & st2lh_label = st2_index == -1 ? emptyLabel : item.leftHeadLabel(st2_index);
		const POSTag & st2lp_postag = st2lp_word_postag.second();
		const POSTag & st2lp2_postag = (st2_index == -1 || item.leftSubPred(st2_index) == -1) ? empty_taggedword.second() : m_lSentence[m_lKeepSentence[item.leftSubPred(st2_index)]].second();
		const int & st2lp_label = st2_index == -1 ? emptyLabel : item.leftPredLabel(st2_index);
		const int & st2lp2_label = st2_index == -1 ? emptyLabel : item.leftSubPredLabel(st2_index);
		const POSTag & st2rh_postag = st2rh_word_postag.second();
		const int & st2rh_label = st2_index == -1 ? emptyLabel : item.rightHeadLabel(st2_index);
		const POSTag & st2rp_postag = st2rp_word_postag.second();
		const POSTag & st2rp2_postag = (st2_index == -1 || item.rightSubPred(st2_index) == -1) ? empty_taggedword.second() : m_lSentence[m_lKeepSentence[item.rightSubPred(st2_index)]].second();
		const int & st2rp_label = st2_index == -1 ? emptyLabel : item.rightPredLabel(st2_index);
		const int & st2rp2_label = st2_index == -1 ? emptyLabel : item.rightSubPredLabel(st2_index);
		const int & st2rp_arity = st2_index == -1 ? emptyArity : item.rightPredArity(st2_index);

		// sst
		// notice sst index
		const int & sst_index = item.secondStackBack() == -1 ? outIndex : item.secondStackTop();
		const int & sstlh_index = sst_index == -1 ? outIndex : item.leftHead(sst_index);
		const int & sstlp_index = sst_index == -1 ? outIndex : item.leftPred(sst_index);
		const int & sstrh_index = sst_index == -1 ? outIndex : item.rightHead(sst_index);
		const int & sstrp_index = sst_index == -1 ? outIndex : item.rightPred(sst_index);
		// sst
		const WordPOSTag & sst_word_postag = sst_index == -1 ? middle_taggedword : m_lSentence[m_lKeepSentence[sst_index]];
		const WordPOSTag & sstlh_word_postag = sstlh_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[sstlh_index]];
		const WordPOSTag & sstlp_word_postag = sstlp_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[sstlp_index]];
		const WordPOSTag & sstrh_word_postag = sstrh_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[sstrh_index]];
		const WordPOSTag & sstrp_word_postag = sstrp_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[sstrp_index]];
		// sst
		const Word & sst_word = sst_word_postag.first();
		const POSTag & sst_postag = sst_word_postag.second();
		const TagSet & sst_llabelset = sst_index == -1 ? empty_tagset : item.leftPredLabelSet(sst_index);
		const TagSet & sst_rlabelset = sst_index == -1 ? empty_tagset : item.rightPredLabelSet(sst_index);
		const POSTag & sstlh_postag = sstlh_word_postag.second();
		const int & sstlh_label = sst_index == -1 ? emptyLabel : item.leftHeadLabel(sst_index);
		const POSTag & sstlp_postag = sstlp_word_postag.second();
		const POSTag & sstlp2_postag = (sst_index == -1 || item.leftSubPred(sst_index) == -1) ? empty_taggedword.second() : m_lSentence[m_lKeepSentence[item.leftSubPred(sst_index)]].second();
		const int & sstlp_label = sst_index == -1 ? emptyLabel : item.leftPredLabel(sst_index);
		const int & sstlp2_label = sst_index == -1 ? emptyLabel : item.leftSubPredLabel(sst_index);
		const POSTag & sstrh_postag = sstrh_word_postag.second();
		const int & sstrh_label = sst_index == -1 ? emptyLabel : item.rightHeadLabel(sst_index);
		const POSTag & sstrp_postag = sstrp_word_postag.second();
		const POSTag & sstrp2_postag = (sst_index == -1 || item.rightSubPred(sst_index) == -1) ? empty_taggedword.second() : m_lSentence[m_lKeepSentence[item.rightSubPred(sst_index)]].second();
		const int & sstrp_label = sst_index == -1 ? emptyLabel : item.rightPredLabel(sst_index);
		const int & sstrp2_label = sst_index == -1 ? emptyLabel : item.rightSubPredLabel(sst_index);
		const int & sstrp_arity = sst_index == -1 ? emptyArity : item.rightPredArity(sst_index);

		// n0
		const int & n0_index = item.size() < m_nSentenceLength ? item.size() : outIndex;
		const int & n0lp_index = n0_index == -1 ? outIndex : item.leftPred(n0_index);
		const int & n0lh_index = n0_index == -1 ? outIndex : item.leftHead(n0_index);
		// n0
		const WordPOSTag & n0_word_postag = n0_index == -1 ? end_taggedword : m_lSentence[m_lKeepSentence[n0_index]];
		const WordPOSTag & n0lh_word_postag = n0lh_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[n0lh_index]];
		const WordPOSTag & n0lp_word_postag = n0lp_index == -1 ? empty_taggedword : m_lSentence[m_lKeepSentence[n0lp_index]];
		// n0
		const Word & n0_word = n0_word_postag.first();
		const POSTag & n0_postag = n0_word_postag.second();
		const TagSet & n0_llabelset = n0_index == -1 ? empty_tagset : item.leftPredLabelSet(n0_index);
		const POSTag & n0lh_postag = n0lh_word_postag.second();
		const int & n0lh_label = n0_index == -1 ? emptyLabel : item.leftHeadLabel(n0_index);
		const POSTag & n0lp_postag = n0lp_word_postag.second();
		const POSTag & n0lp2_postag = (n0_index == -1 || item.leftSubPred(n0_index) == -1) ? empty_taggedword.second() : m_lSentence[m_lKeepSentence[item.leftSubPred(n0_index)]].second();
		const int & n0lp_label = n0_index == -1 ? emptyLabel : item.leftPredLabel(n0_index);
		const int & n0lp2_label = n0_index == -1 ? emptyLabel : item.leftSubPredLabel(n0_index);
		const int & n0lp_arity = n0_index == -1 ? emptyArity : item.leftPredArity(n0_index);

		// n0
		cweight->m_mapN0w.getOrUpdateScore(m_mapPackedScore, n0_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapN0pt.getOrUpdateScore(m_mapPackedScore, n0_postag, m_nScoreIndex, amount, m_nTrainingRound);

		// st
		cweight->m_mapSTw.getOrUpdateScore(m_mapPackedScore, st_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSTpt.getOrUpdateScore(m_mapPackedScore, st_postag, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram st + n0
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, n0_postag);
		cweight->m_mapSTptN0pt.getOrUpdateScore(m_mapPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, n0_word);
		cweight->m_mapSTwN0w.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.referLast(n0_postag);
		cweight->m_mapSTwN0pt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, st_postag);
		cweight->m_mapSTptN0w.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram with arity
		bi_features.refer(poses_feature, strp_arity);
		cweight->m_mapSTptN0ptSTRPi.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.referLast(n0lp_arity);
		cweight->m_mapSTptN0ptN0LPi.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram with dis
		dis = encodeLinkDistance(st_index, n0_index == -1 ? m_nSentenceLength : n0_index);
		bi_features.referLast(dis);
		cweight->m_mapSTptN0ptD.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st2
		cweight->m_mapST2w.getOrUpdateScore(m_mapPackedScore, st2_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapST2pt.getOrUpdateScore(m_mapPackedScore, st2_postag, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram st2 + n0
		poses_feature = ENCODE_POSTAG_SET_2(st2_postag, n0_postag);
		cweight->m_mapST2ptN0pt.getOrUpdateScore(m_mapPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, n0_word);
		cweight->m_mapST2wN0w.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.referLast(n0_postag);
		cweight->m_mapST2wN0pt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, st2_postag);
		cweight->m_mapST2ptN0w.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram with arity
		bi_features.refer(poses_feature, st2rp_arity);
		cweight->m_mapST2ptN0ptST2RPi.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.referLast(n0lp_arity);
		cweight->m_mapST2ptN0ptN0LPi.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.referLast(poses_feature);
		// bigram with dis
		dis = encodeLinkDistance(st2_index, n0_index == -1 ? m_nSentenceLength : n0_index);
		bi_features.referLast(dis);
		cweight->m_mapST2ptN0ptD.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram st + st2
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, st2_postag);
		cweight->m_mapSTptST2pt.getOrUpdateScore(m_mapPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, st2_word);
		cweight->m_mapSTwST2w.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.referLast(st2_postag);
		cweight->m_mapSTwST2pt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st2_word, st_postag);
		cweight->m_mapSTptST2w.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// sst
		cweight->m_mapSSTw.getOrUpdateScore(m_mapPackedScore, sst_word, m_nScoreIndex, amount, m_nTrainingRound);
		cweight->m_mapSSTpt.getOrUpdateScore(m_mapPackedScore, sst_postag, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram sst + n0
		poses_feature = ENCODE_POSTAG_SET_2(sst_postag, n0_postag);
		cweight->m_mapSSTptN0pt.getOrUpdateScore(m_mapPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, n0_word);
		cweight->m_mapSSTwN0w.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.referLast(n0_postag);
		cweight->m_mapSSTwN0pt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0_word, sst_postag);
		cweight->m_mapSSTptN0w.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram with arity
		bi_features.refer(poses_feature, sstrp_arity);
		cweight->m_mapSSTptN0ptSSTRPi.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.referLast(n0lp_arity);
		cweight->m_mapSSTptN0ptN0LPi.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram with dis
		dis = encodeLinkDistance(sst_index, n0_index == -1 ? m_nSentenceLength : n0_index);
		bi_features.referLast(dis);
		cweight->m_mapSSTptN0ptD.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// bigram st + sst
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, sst_postag);
		cweight->m_mapSTptSSTpt.getOrUpdateScore(m_mapPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(st_word, sst_word);
		cweight->m_mapSTwSSTw.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.referLast(sst_postag);
		cweight->m_mapSTwSSTpt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(sst_word, st_postag);
		cweight->m_mapSTptSSTw.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// tri-gram
		// st + n0 + st2
		poses_feature = ENCODE_POSTAG_SET_3(st_postag, st2_postag, n0_postag);
		cweight->m_mapSTptN0ptST2pt.getOrUpdateScore(m_mapPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + sst
		poses_feature = ENCODE_POSTAG_SET_3(st_postag, sst_postag, n0_postag);
		cweight->m_mapSTptN0ptSSTpt.getOrUpdateScore(m_mapPackedScore, poses_feature, m_nScoreIndex, amount, m_nTrainingRound);

		// st + n0 + st left/right head/pred
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlh_postag, n0_postag), stlh_label);
		cweight->m_mapSTptN0ptSTLHptSTLHl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, stlp_postag, n0_postag), stlp_label);
		cweight->m_mapSTptN0ptSTLPptSTLPl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strh_postag, n0_postag), strh_label);
		cweight->m_mapSTptN0ptSTRHptSTRHl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, strp_postag, n0_postag), strp_label);
		cweight->m_mapSTptN0ptSTRPptSTRPl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + n0 + n0 left head/pred
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, n0_postag, n0lh_postag), n0lh_label);
		cweight->m_mapSTptN0ptN0LHptN0LHl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st_postag, n0_postag, n0lp_postag), n0lp_label);
		cweight->m_mapSTptN0ptN0LPptN0LPl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// st + n0 + st right head + st right pred
		tri_features.refer(encodePOSTagSet4(st_postag, strh_postag, strp_postag, n0_postag), strh_label, strp_label);
		cweight->m_mapSTptN0ptSTRHptSTRPptSTRHlSTRPl.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + st left pred + st left pred 2
		tri_features.refer(encodePOSTagSet4(st_postag, stlp_postag, stlp2_postag, n0_postag), stlp_label, stlp2_label);
		cweight->m_mapSTptN0ptSTLPptSTLP2ptSTLPlSTLP2l.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + st right pred + st right pred 2
		tri_features.refer(encodePOSTagSet4(st_postag, strp_postag, strp2_postag, n0_postag), strp_label, strp2_label);
		cweight->m_mapSTptN0ptSTRPptSTRP2ptSTRPlSTRP2l.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + n0 left head + n0 left pred
		tri_features.refer(encodePOSTagSet4(st_postag, n0_postag, n0lh_postag, n0lp_postag), n0lh_label, n0lp_label);
		cweight->m_mapSTptN0ptN0LHptN0LPptN0LHlN0LPl.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st + n0 + n0 left pred + n0 left pred 2
		tri_features.refer(encodePOSTagSet4(st_postag, n0_postag, n0lp_postag, n0lp2_postag), n0lp_label, n0lp2_label);
		cweight->m_mapSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + n0 + st2 left/right head/pred
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2lh_postag, n0_postag), st2lh_label);
		cweight->m_mapST2ptN0ptST2LHptST2LHl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2lp_postag, n0_postag), st2lp_label);
		cweight->m_mapST2ptN0ptST2LPptST2LPl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2rh_postag, n0_postag), st2rh_label);
		cweight->m_mapST2ptN0ptST2RHptST2RHl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, st2rp_postag, n0_postag), st2rp_label);
		cweight->m_mapST2ptN0ptST2RPptST2RPl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + n0 + n0 left head/pred
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, n0_postag, n0lh_postag), n0lh_label);
		cweight->m_mapST2ptN0ptN0LHptN0LHl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(st2_postag, n0_postag, n0lp_postag), n0lp_label);
		cweight->m_mapST2ptN0ptN0LPptN0LPl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// st2 + n0 + st2 right head + st2 right pred
		tri_features.refer(encodePOSTagSet4(st2_postag, st2rh_postag, st2rp_postag, n0_postag), st2rh_label, st2rp_label);
		cweight->m_mapST2ptN0ptST2RHptST2RPptST2RHlST2RPl.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + st2 left pred + st2 left pred 2
		tri_features.refer(encodePOSTagSet4(st2_postag, st2lp_postag, st2lp2_postag, n0_postag), st2lp_label, st2lp2_label);
		cweight->m_mapST2ptN0ptST2LPptST2LP2ptST2LPlST2LP2l.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + st2 right pred + st2 right pred 2
		tri_features.refer(encodePOSTagSet4(st2_postag, st2rp_postag, st2rp2_postag, n0_postag), st2rp_label, st2rp2_label);
		cweight->m_mapST2ptN0ptST2RPptST2RP2ptST2RPlST2RP2l.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + n0 left head + n0 left pred
		tri_features.refer(encodePOSTagSet4(st2_postag, n0_postag, n0lh_postag, n0lp_postag), n0lh_label, n0lp_label);
		cweight->m_mapST2ptN0ptN0LHptN0LPptN0LHlN0LPl.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// st2 + n0 + n0 left pred + n0 left pred 2
		tri_features.refer(encodePOSTagSet4(st2_postag, n0_postag, n0lp_postag, n0lp2_postag), n0lp_label, n0lp2_label);
		cweight->m_mapST2ptN0ptN0LPptN0LP2ptN0LPlN0LP2l.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + n0 + sst left/right head/pred
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstlh_postag, n0_postag), sstlh_label);
		cweight->m_mapSSTptN0ptSSTLHptSSTLHl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstlp_postag, n0_postag), sstlp_label);
		cweight->m_mapSSTptN0ptSSTLPptSSTLPl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstrh_postag, n0_postag), sstrh_label);
		cweight->m_mapSSTptN0ptSSTRHptSSTRHl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, sstrp_postag, n0_postag), sstrp_label);
		cweight->m_mapSSTptN0ptSSTRPptSSTRPl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + n0 + n0 left head/pred
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, n0_postag, n0lh_postag), n0lh_label);
		cweight->m_mapSSTptN0ptN0LHptN0LHl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_3(sst_postag, n0_postag, n0lp_postag), n0lp_label);
		cweight->m_mapSSTptN0ptN0LPptN0LPl.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		// quar-gram
		// sst + n0 + sst right head + sst right pred
		tri_features.refer(encodePOSTagSet4(sst_postag, sstrh_postag, sstrp_postag, n0_postag), sstrh_label, sstrp_label);
		cweight->m_mapSSTptN0ptSSTRHptSSTRPptSSTRHlSSTRPl.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + sst left pred + sst left pred 2
		tri_features.refer(encodePOSTagSet4(sst_postag, sstlp_postag, sstlp2_postag, n0_postag), sstlp_label, sstlp2_label);
		cweight->m_mapSSTptN0ptSSTLPptSSTLP2ptSSTLPlSSTLP2l.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + sst right pred + sst right pred 2
		tri_features.refer(encodePOSTagSet4(sst_postag, sstrp_postag, sstrp2_postag, n0_postag), sstrp_label, sstrp2_label);
		cweight->m_mapSSTptN0ptSSTRPptSSTRP2ptSSTRPlSSTRP2l.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + n0 left head + n0 left pred
		tri_features.refer(encodePOSTagSet4(sst_postag, n0_postag, n0lh_postag, n0lp_postag), n0lh_label, n0lp_label);
		cweight->m_mapSSTptN0ptN0LHptN0LPptN0LHlN0LPl.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
		// sst + n0 + n0 left pred + n0 left pred 2
		tri_features.refer(encodePOSTagSet4(sst_postag, n0_postag, n0lp_postag, n0lp2_postag), n0lp_label, n0lp2_label);
		cweight->m_mapSSTptN0ptN0LPptN0LP2ptN0LPlN0LP2l.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		// st + n0 + labelset
		poses_feature = ENCODE_POSTAG_SET_2(st_postag, n0_postag);
		uni_tagset.refer(poses_feature, st_llabelset.bits(0), st_llabelset.bits(1));
		cweight->m_mapSTptN0ptSTll.getOrUpdateScore(m_mapPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(poses_feature, st_rlabelset.bits(0), st_rlabelset.bits(1));
		cweight->m_mapSTptN0ptSTrl.getOrUpdateScore(m_mapPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(poses_feature, n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapSTptN0ptN0ll.getOrUpdateScore(m_mapPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);

		// st2 + n0 + labelset
		poses_feature = ENCODE_POSTAG_SET_2(st2_postag, n0_postag);
		uni_tagset.refer(poses_feature, st2_llabelset.bits(0), st2_llabelset.bits(1));
		cweight->m_mapST2ptN0ptST2ll.getOrUpdateScore(m_mapPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(poses_feature, st2_rlabelset.bits(0), st2_rlabelset.bits(1));
		cweight->m_mapST2ptN0ptST2rl.getOrUpdateScore(m_mapPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(poses_feature, n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapST2ptN0ptN0ll.getOrUpdateScore(m_mapPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);

		// sst + n0 + labelset
		poses_feature = ENCODE_POSTAG_SET_2(sst_postag, n0_postag);
		uni_tagset.refer(poses_feature, sst_llabelset.bits(0), sst_llabelset.bits(1));
		cweight->m_mapSSTptN0ptSSTll.getOrUpdateScore(m_mapPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(poses_feature, sst_rlabelset.bits(0), sst_rlabelset.bits(1));
		cweight->m_mapSSTptN0ptSSTrl.getOrUpdateScore(m_mapPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);
		uni_tagset.refer(poses_feature, n0_llabelset.bits(0), n0_llabelset.bits(1));
		cweight->m_mapSSTptN0ptN0ll.getOrUpdateScore(m_mapPackedScore, uni_tagset, m_nScoreIndex, amount, m_nTrainingRound);

		// context
		const int & st_t_index = m_lKeepSentence[st_index];
		const int & n0_t_index = m_lKeepSentence[n0_index];
		const int & stl2_index = st_t_index >= 2 ? st_t_index - 2 : outIndex;
		const int & stl1_index = st_t_index >= 1 ? st_t_index - 1 : outIndex;
		const int & str1_index = st_t_index + 1 < m_nTrueSentenceLength ? st_t_index + 1 : outIndex;
		const int & str2_index = st_t_index + 2 < m_nTrueSentenceLength ? st_t_index + 2 : outIndex;
		const int & n0l2_index = n0_t_index >= 2 ? n0_t_index - 2 : outIndex;
		const int & n0l1_index = n0_t_index >= 1 ? n0_t_index - 1 : outIndex;
		const int & n0r1_index = n0_t_index + 1 < m_nTrueSentenceLength ? n0_t_index + 1 : outIndex;
		const int & n0r2_index = n0_t_index + 2 < m_nTrueSentenceLength ? n0_t_index + 2 : outIndex;
		// context
		const WordPOSTag & stl2_word_postag = stl2_index == -1 ? start_taggedword : m_lSentence[stl2_index];
		const WordPOSTag & stl1_word_postag = stl1_index == -1 ? start_taggedword : m_lSentence[stl1_index];
		const WordPOSTag & str1_word_postag = str1_index == -1 ? end_taggedword : m_lSentence[str1_index];
		const WordPOSTag & str2_word_postag = str2_index == -1 ? end_taggedword : m_lSentence[str2_index];
		const WordPOSTag & n0l2_word_postag = n0l2_index == -1 ? start_taggedword : m_lSentence[n0l2_index];
		const WordPOSTag & n0l1_word_postag = n0l1_index == -1 ? start_taggedword : m_lSentence[n0l1_index];
		const WordPOSTag & n0r1_word_postag = n0r1_index == -1 ? end_taggedword : m_lSentence[n0r1_index];
		const WordPOSTag & n0r2_word_postag = n0r2_index == -1 ? end_taggedword : m_lSentence[n0r2_index];
		// context
		const POSTag & stl2_postag = stl2_word_postag.second();
		const POSTag & stl1_postag = stl1_word_postag.second();
		const POSTag & str1_postag = str1_word_postag.second();
		const POSTag & str2_postag = str2_word_postag.second();
		const POSTag & n0l2_postag = n0l2_word_postag.second();
		const POSTag & n0l1_postag = n0l1_word_postag.second();
		const POSTag & n0r1_postag = n0r1_word_postag.second();
		const POSTag & n0r2_postag = n0r2_word_postag.second();
		// unigram context
		// st + sti, n0 + n0i
		// st pos
		bi_features.refer(stl2_postag, -2);
		cweight->m_mapSTipt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(stl1_postag, -1);
		cweight->m_mapSTipt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(str1_postag, 1);
		cweight->m_mapSTipt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(str2_postag, 2);
		cweight->m_mapSTipt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(stl2_postag, stl1_postag), -1);
		cweight->m_mapSTiptSTjpt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(stl1_postag, str1_postag), 0);
		cweight->m_mapSTiptSTjpt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(str1_postag, str2_postag), 1);
		cweight->m_mapSTiptSTjpt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		// n0 pos
		bi_features.refer(n0l2_postag, -2);
		cweight->m_mapN0ipt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0l1_postag, -1);
		cweight->m_mapN0ipt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0r1_postag, 1);
		cweight->m_mapN0ipt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(n0r2_postag, 2);
		cweight->m_mapN0ipt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(n0l2_postag, n0l1_postag), -1);
		cweight->m_mapN0iptN0jpt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(n0l1_postag, n0r1_postag), 0);
		cweight->m_mapN0iptN0jpt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);
		bi_features.refer(ENCODE_POSTAG_SET_2(n0r1_postag, n0r2_postag), 1);
		cweight->m_mapN0iptN0jpt.getOrUpdateScore(m_mapPackedScore, bi_features, m_nScoreIndex, amount, m_nTrainingRound);

		if (m_bPath) {

			const WordPOSTag & st_syn_head_word_postag = (st_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[st_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[st_index])]);
			const WordPOSTag & n0_syn_head_word_postag = (n0_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[n0_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[n0_index])]);
			const WordPOSTag & st2_syn_head_word_postag = (st2_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[st2_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[st2_index])]);
			const WordPOSTag & sst_syn_head_word_postag = (sst_index == -1 || TREENODE_HEAD(m_dtSyntaxTree[sst_index]) == -1 ? empty_taggedword : m_lSentence[TREENODE_HEAD(m_dtSyntaxTree[sst_index])]);

			if (st_index == -1 || n0_index == -1) {
				cweight->m_mapSTPOSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSTFPOSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapSTPOSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.POSPath[st_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSTFPOSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.FPOSPath[st_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
			}
			tri_features.refer(st_word, n0_postag, st_syn_head_word_postag.second());
			cweight->m_mapSTwN0wSTsynhpt.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st_word, n0_postag, n0_syn_head_word_postag.second());
			cweight->m_mapSTwN0wN0synhpt.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

			if (st2_index == -1 || n0_index == -1) {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.POSPath[st2_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.FPOSPath[st2_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
			}
			tri_features.refer(st2_word, n0_postag, st2_syn_head_word_postag.second());
			cweight->m_mapST2wN0wST2synhpt.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(st2_word, n0_postag, n0_syn_head_word_postag.second());
			cweight->m_mapST2wN0wN0synhpt.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

			if (sst_index == -1 || n0_index == -1) {
				cweight->m_mapST2POSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapST2FPOSPath.getOrUpdateScore(m_mapPackedScore, "n#", m_nScoreIndex, amount, m_nTrainingRound);
			}
			else {
				cweight->m_mapSSTPOSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.POSPath[sst_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
				cweight->m_mapSSTFPOSPath.getOrUpdateScore(m_mapPackedScore, m_lcaAnalyzer.FPOSPath[sst_index][n0_index], m_nScoreIndex, amount, m_nTrainingRound);
			}
			tri_features.refer(sst_word, n0_postag, sst_syn_head_word_postag.second());
			cweight->m_mapSSTwN0wSSTsynhpt.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);
			tri_features.refer(sst_word, n0_postag, n0_syn_head_word_postag.second());
			cweight->m_mapSSTwN0wN0synhpt.getOrUpdateScore(m_mapPackedScore, tri_features, m_nScoreIndex, amount, m_nTrainingRound);

		}
	}

}
