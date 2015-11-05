#ifndef _GRAPH_MACROS_BASE_H
#define _GRAPH_MACROS_BASE_H

#include <string>
#include <cstring>
#include <unordered_map>

#include "include/score_array.h"
#include "common/token/tagset.h"
#include "common/parser/macros_base.h"

extern int g_nGraphLabelCount;
extern std::vector<int> g_vecGraphLabelMap;
extern SuperTagCandidates g_mapSuperTagCandidatesOfWords;
extern SuperTagCandidates g_mapSuperTagCandidatesOfPOSTags;

// agenda
#define AGENDA_SIZE		16
// action start
#define ACTION_START	0

#define MAX_LABEL_ID			256
#define MAX_LABEL_ID_BITS		8
#define LEFT_LABEL_ID(LI)		((LI) >> MAX_LABEL_ID_BITS)
#define RIGHT_LABEL_ID(LI)		((LI) & (MAX_LABEL_ID - 1))
#define ENCODE_LABEL_ID(LL,LR)	(((LL) << MAX_LABEL_ID_BITS) | (LR))

#define IS_LEFT_LABEL(L)	(L.find("left") == 0)
#define IS_RIGHT_LABEL(L)	(L.find("right") == 0)
#define IS_TWOWAY_LABEL(L)	(L.find("twoway") == 0)

#define ENCODE_LEFT_LABEL(L)		("left" + L)
#define ENCODE_RIGHT_LABEL(L)		("right" + L)
#define ENCODE_TWOWAY_LABEL(L1,L2)	("twoway" + L1 + "||" + L2)

#define DECODE_LEFT_LABEL(L)			(L.substr(strlen("left")))
#define DECODE_RIGHT_LABEL(L)			(L.substr(strlen("right")))
#define DECODE_TWOWAY_LEFT_LABEL(L)		(L.substr(strlen("twoway"), L.find("||") - strlen("twoway")))
#define DECODE_TWOWAY_RIGHT_LABEL(L)	(L.substr(L.find("||") + strlen("||")))

#define ENCODE_POSTAG_SET_2(T1, T2)		(((T1) << 8) | (T2))
#define ENCODE_POSTAG_SET_3(T1, T2, T3)	(((T1) << 16) | ((T2) << 8) | (T3))
inline unsigned int encodePOSTagSet4(const unsigned int & t1, const unsigned int & t2, const unsigned int & t3, const unsigned int & t4) {
	return ((t1 << 24) | (t2 << 16) | (t3 << 8) | t4);
}

typedef TagSetN<2> TagSet;

typedef tscore PackedScoreType[600];
typedef tscore PackedSuperTagScoreType[30000];

typedef TriGram<unsigned int> IntTagSet;
typedef QuarGram<unsigned int> TwoIntsTagSet;

template<class RET_TYPE> using WordTagsetMap = PackedScoreMap<IntTagSet, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagTagsetMap = PackedScoreMap<IntTagSet, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;

template<class RET_TYPE> using TwoWordsTagsetMap = PackedScoreMap<TwoIntsTagSet, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagSet2TagsetMap = PackedScoreMap<IntTagSet, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;

template<class RET_TYPE> using IntMap = PackedScoreMap<Int, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordMap = PackedScoreMap<Int, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagMap = PackedScoreMap<Int, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using SuperTagMap = PackedScoreMap<Int, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using TwoWordsMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using ThreeWordsMap = PackedScoreMap<ThreeInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordPOSTagMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordWordPOSTagMap = PackedScoreMap<ThreeInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordPOSTagPOSTagMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordPOSTagPOSTagIntMap = PackedScoreMap<ThreeInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordWordPOSTagPOSTagMap = PackedScoreMap<ThreeInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;

template<class RET_TYPE> using POSTagSet2Map = PackedScoreMap<Unsigned, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagSet3Map = PackedScoreMap<Unsigned, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagSet4Map = PackedScoreMap<Unsigned, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;

template<class RET_TYPE> using StringMap = PackedScoreMap<std::string, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordIntMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagIntMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using SuperTagIntMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using SuperTagSet2Map = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using TwoWordsIntMap = PackedScoreMap<ThreeInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordPOSTagIntMap = PackedScoreMap<ThreeInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordSuperTagMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagSuperTagMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using WordPOSTagSuperTagMap = PackedScoreMap<ThreeInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using ThreeWordsIntMap = PackedScoreMap<FourInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagSet2IntMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagSet3IntMap = PackedScoreMap<TwoInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;
template<class RET_TYPE> using POSTagSet4TwoIntsMap = PackedScoreMap<ThreeInts, ActionScores<RET_TYPE>, RET_TYPE, ActionScoreIncrement>;

void initTags(const std::string & sInput);
void initTags(const std::string & sInput, const std::string & sInputReverse);
void getPlanar(const DependencyGraph & graph, DependencyGraph & partPlanar, DependencyGraph & partAdded);
void eraseGraph(const DependencyGraph & oracle, DependencyGraph & deleted);
void eraseReverseGraph(const DependencyGraph & oracle, DependencyGraph & reverseOracle);
void combineGraph(const DependencyGraph & oracle, const DependencyGraph & reverseOracle, DependencyGraph & combined);

std::istream & operator>>(std::istream & input, DependencyGraph & graph);
std::ostream & operator<<(std::ostream & output, const DependencyGraph & graph);

std::istream & operator>>(std::istream & input, SuperTagCandidates & stc);
std::ostream & operator<<(std::ostream & output, const SuperTagCandidates & std);

#endif
