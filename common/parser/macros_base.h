#ifndef _MACROS_H
#define _MACROS_H

#include <tuple>
#include <vector>

#include "include/ngram.h"
#include "common/token/token.h"
#include "common/token/deplabel.h"
#include "common/token/supertag.h"

#define MAX_SENTENCE_SIZE 256
#define MAX_SENTENCE_BITS 8

#define ROOT_WORD		"-ROOT-"
#define ROOT_POSTAG		"-ROOT-"
#define ROOT_DEPLABEL	"-ROOT-"
#define EMPTY_WORD		"-EMPTY-"
#define EMPTY_POSTAG	"-EMPTY-"
#define START_WORD		"-START-"
#define START_POSTAG	"-START-"
#define MIDDLE_WORD		"-MIDDLE-"
#define MIDDLE_POSTAG	"-MIDDLE-"
#define END_WORD		"-END-"
#define END_POSTAG		"-END-"

#define NULL_LABEL		"-NULL-"

#define IS_NULL(X)				((X) == -1)
#define IS_EMPTY(X)				((X) >= MAX_SENTENCE_SIZE)

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

#define SENT_SPTOKEN	"/"
#define SENT_WORD(X)	(std::get<0>(X))
#define SENT_POSTAG(X)	(std::get<1>(X))

#define TREENODE_WORD(X)			(std::get<0>(std::get<0>(X)))
#define TREENODE_POSTAG(X)			(std::get<1>(std::get<0>(X)))
#define TREENODE_POSTAGGEDWORD(X)	(std::get<0>(X))
#define TREENODE_HEAD(X)			(std::get<1>(X))
#define TREENODE_LABEL(X)			(std::get<2>(X))

#define RIGHTNODE_POS(X)			(std::get<0>(X))
#define RIGHTNODE_LABEL(X)			(std::get<1>(X))

#define GRAPH_LEFT	-1
#define GRAPH_RIGHT 1

#define GRAPHNODE_WORD(X)				(std::get<0>(std::get<0>(X)))
#define GRAPHNODE_POSTAG(X)				(std::get<1>(std::get<0>(X)))
#define GRAPHNODE_POSTAGGEDWORD(X)		(std::get<0>(X))
#define GRAPHNODE_SUPERTAG(X)			(std::get<1>(X))
#define GRAPHNODE_TREEHEAD(X)			(std::get<0>(std::get<2>(X)))
#define GRAPHNODE_TREELABEL(X)			(std::get<1>(std::get<2>(X)))
#define GRAPHNODE_RIGHTNODES(X)			(std::get<3>(X))
#define GRAPHNODE_RIGHTNODE(X,I)		(std::get<3>(X)[I])
#define GRAPHNODE_RIGHTNODEPOS(X,I)		(std::get<0>(std::get<3>(X)[I]))
#define GRAPHNODE_RIGHTNODELABEL(X,I)	(std::get<1>(std::get<3>(X)[I]))

typedef int gtype;

typedef unsigned int UNSIGNED;
typedef int Int;
typedef gtype Word;
typedef gtype POSTag;
typedef gtype SuperTag;

typedef BiGram<gtype> TwoInts;
typedef BiGram<gtype> TwoWords;
typedef BiGram<gtype> POSTagSet2;
typedef BiGram<gtype> WordPOSTag;
typedef BiGram<long long> WordSetOfDepLabels;
typedef BiGram<long long> POSTagSetOfDepLabels;

typedef TriGram<gtype> ThreeInts;
typedef TriGram<gtype> ThreeWords;
typedef TriGram<gtype> POSTagSet3;
typedef TriGram<gtype> WordIntInt;
typedef TriGram<gtype> POSTagIntInt;
typedef TriGram<gtype> WordWordPOSTag;
typedef TriGram<gtype> WordPOSTagPOSTag;

typedef QuarGram<gtype> FourInts;
typedef QuarGram<gtype> POSTagSet4;
typedef QuarGram<gtype> ThreeWordsInt;
typedef QuarGram<gtype> WordWordPOSTagPOSTag;
typedef QuarGram<gtype> WordWordPOSTagInt;

typedef BiGram<gtype> WordInt;
typedef BiGram<gtype> POSTagInt;

typedef TriGram<gtype> TwoWordsInt;
typedef TriGram<gtype> POSTagSet2Int;
typedef TriGram<gtype> WordPOSTagInt;

typedef QuarGram<gtype> POSTagSet3Int;
typedef QuarGram<gtype> TwoWordsIntInt;
typedef QuarGram<gtype> POSTagSet2IntInt;
typedef QuarGram<gtype> WordWordPOSTagInt;
typedef QuarGram<gtype> WordPOSTagPOSTagInt;

typedef QuinGram<gtype> POSTagSet4Int;
typedef QuinGram<gtype> WordWordPOSTagPOSTagInt;
typedef QuinGram<gtype> WordPOSTagPOSTagPOSTagInt;

typedef HexaGram<gtype> POSTagSet5Int;
typedef HexaGram<gtype> WordPOSTagPOSTagPOSTagPOSTagInt;

typedef std::tuple<ttoken, ttoken> POSTaggedWord;
typedef std::tuple<POSTaggedWord, int, ttoken> DependencyTreeNode;

typedef std::vector<POSTaggedWord> Sentence;
typedef std::vector<DependencyTreeNode> DependencyTree;

typedef std::tuple<int, ttoken> HeadWithLabel;
typedef std::tuple<int, int> RightNodeWithLabel;
typedef std::tuple<POSTaggedWord, ttoken, HeadWithLabel, std::vector<RightNodeWithLabel>> DependencyGraphNode;
typedef std::vector<DependencyGraphNode> DependencyGraph;

int encodeLinkDistance(const int & st, const int & n0);
int encodeLinkDistanceOrDirection(const int & hi, const int & di, bool dir);
std::string nCharPrev(const Sentence & sent, int index, int n);
std::string nCharNext(const Sentence & sent, int index, int n);

void clearGraphSuperTag(DependencyGraph & graph);

std::istream & operator>>(std::istream & input, Sentence & sentence);
std::istream & operator>>(std::istream & input, DependencyTree & tree);
std::istream & operator>>(std::istream & input, DependencyGraph & graph);

std::ostream & operator<<(std::ostream & output, const Sentence & sentence);
std::ostream & operator<<(std::ostream & output, const DependencyTree & tree);
std::ostream & operator<<(std::ostream & output, const DependencyGraph & graph);

#endif
