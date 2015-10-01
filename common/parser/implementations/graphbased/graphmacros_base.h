#ifndef _GRAPHMACROS_BASE_H
#define _GRAPHMACROS_BASE_H

#include <unordered_map>

#include "common/token/tagset.h"
#include "common/parser/macros_base.h"
#include "include/learning/perceptron/score.h"
#include "include/learning/perceptron/packed_score.h"

typedef std::pair<int, const int &> AddScoreType;

// ScoreArray
template<class RET_TYPE>
class ScoreArray {
private:
	std::unordered_map<int, Score> m_mapScores;

public:
	ScoreArray();
	ScoreArray(const ScoreArray & s);
	~ScoreArray();

	void reset();
	bool zero() const;
	void updateCurrent(const AddScoreType & added, const int & round);
	void updateAverage(const int & round);
	void updateRetval(RET_TYPE & retval, const int & which);
	std::unordered_map<int, Score> & getScores();
	const std::unordered_map<int, Score> & getScores() const;

	ScoreArray<RET_TYPE> & operator=(const ScoreArray<RET_TYPE> & s) {
		m_mapScores.clear();
		m_mapScores.insert(s.m_mapScores.begin(), s.m_mapScores.end());
		return *this;
	}
};

template<class RET_TYPE>
inline ScoreArray<RET_TYPE>::ScoreArray() = default;

template<class RET_TYPE>
inline ScoreArray<RET_TYPE>::ScoreArray(const ScoreArray<RET_TYPE> & s) {
	m_mapScores.clear();
	m_mapScores.insert(s.m_mapScores.begin(), s.m_mapScores.end());
}

template<class RET_TYPE>
inline ScoreArray<RET_TYPE>::~ScoreArray() = default;

template<class RET_TYPE>
inline void ScoreArray<RET_TYPE>::reset() {
	m_mapScores.clear();
}

template<class RET_TYPE>
bool ScoreArray<RET_TYPE>::zero() const {
	for (const auto & score : m_mapScores) {
		if (!score.second.zero()) {
			return false;
		}
	}
	return true;
}

template<class RET_TYPE>
inline void ScoreArray<RET_TYPE>::updateCurrent(const AddScoreType & added, const int & round) {
	m_mapScores[added.first].updateCurrent(added.second, round);
}

template<class RET_TYPE>
inline void ScoreArray<RET_TYPE>::updateAverage(const int & round) {
	for (auto & score : m_mapScores) {
		score.second.updateAverage(round);
	}
}

template<class RET_TYPE>
void ScoreArray<RET_TYPE>::updateRetval(RET_TYPE & retval, const int & which) {
	switch (which) {
	case eNonAverage:
		for (auto & score : m_mapScores) {
			score.second.updateRetvalCurrent(retval[score.first]);
		}
		break;
	case eAverage:
		for (auto & score : m_mapScores) {
			score.second.updateRetvalTotal(retval[score.first]);
		}
		break;
	}
}

template<class RET_TYPE>
inline std::unordered_map<int, Score> & ScoreArray<RET_TYPE>::getScores() {
	return m_mapScores;
}

template<class RET_TYPE>
inline const std::unordered_map<int, Score> & ScoreArray<RET_TYPE>::getScores() const {
	return m_mapScores;
}

template<class RET_TYPE>
std::istream & operator>>(std::istream & is, ScoreArray<RET_TYPE> & s) {
	ttoken token;
	int index;
	cscore cs;
	tscore ts;
	s.reset();
	is >> token;
	while (is >> index) {
		is >> token >> cs >> token >> ts >> token;
		s.getScores()[index] = Score(cs, ts);
		if (token == "}") {
			break;
		}
	}
	return is;
}

template<class RET_TYPE>
std::ostream & operator<<(std::ostream & os, const ScoreArray<RET_TYPE> & s) {
	bool start = false;
	for (const auto & score : s.getScores()) {
		if (!score.second.zero()) {
			if (!start) {
				os << "{ ";
				start = true;
			}
			else {
				os << " , ";
			}
			os << score.first << " : " << score.second;
		}
	}
	os << " }";
	return os;
}

inline bool operator==(const AddScoreType & s, const int & i) {
	return s.second == i;
}

// agenda
#define AGENDA_SIZE		16
// action start
#define ACTION_START	0

#define ENCODE_POSTAG_SET_2(T1, T2)		(((T1) << 8) | (T2))
#define ENCODE_POSTAG_SET_3(T1, T2, T3)	(((T1) << 16) | ((T2) << 8) | (T3))
inline unsigned int encodePOSTagSet4(const unsigned int & t1, const unsigned int & t2, const unsigned int & t3, const unsigned int & t4) {
	return ((t1 << 24) | (t2 << 16) | (t3 << 8) | t4);
}

typedef TagSetN<2> TagSet;

typedef tscore PackedScoreType[300];
typedef tscore PackedSuperTagScoreType[30000];

typedef TriGram<unsigned int> IntTagSet;
typedef QuarGram<unsigned int> TwoIntsTagSet;

template<class RET_TYPE> using WordTagsetMap = PackedScoreMap<IntTagSet, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using POSTagTagsetMap = PackedScoreMap<IntTagSet, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;

template<class RET_TYPE> using TwoWordsTagsetMap = PackedScoreMap<TwoIntsTagSet, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using POSTagSet2TagsetMap = PackedScoreMap<IntTagSet, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;

template<class RET_TYPE> using IntMap = PackedScoreMap<Int, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using WordMap = PackedScoreMap<Int, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using POSTagMap = PackedScoreMap<Int, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using SuperTagMap = PackedScoreMap<Int, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using TwoWordsMap = PackedScoreMap<TwoInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using ThreeWordsMap = PackedScoreMap<ThreeInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using WordPOSTagMap = PackedScoreMap<TwoInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using WordWordPOSTagMap = PackedScoreMap<ThreeInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using WordPOSTagPOSTagMap = PackedScoreMap<TwoInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using WordPOSTagPOSTagIntMap = PackedScoreMap<ThreeInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using WordWordPOSTagPOSTagMap = PackedScoreMap<ThreeInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;

template<class RET_TYPE> using POSTagSet2Map = PackedScoreMap<UNSIGNED, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using POSTagSet3Map = PackedScoreMap<UNSIGNED, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using POSTagSet4Map = PackedScoreMap<UNSIGNED, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;

template<class RET_TYPE> using StringMap = PackedScoreMap<std::string, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using WordIntMap = PackedScoreMap<TwoInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using POSTagIntMap = PackedScoreMap<TwoInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using SuperTagIntMap = PackedScoreMap<TwoInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using TwoWordsIntMap = PackedScoreMap<ThreeInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using WordPOSTagIntMap = PackedScoreMap<ThreeInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using ThreeWordsIntMap = PackedScoreMap<FourInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using POSTagSet2IntMap = PackedScoreMap<TwoInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using POSTagSet3IntMap = PackedScoreMap<TwoInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;
template<class RET_TYPE> using POSTagSet4TwoIntsMap = PackedScoreMap<ThreeInts, ScoreArray<RET_TYPE>, RET_TYPE, AddScoreType>;

#endif
