#ifndef _TWOSTACK_MACROS_H
#define _TWOSTACK_MACROS_H

#include "common/parser/agenda.h"
#include "common/parser/macros_base.h"
#include "include/learning/perceptron/packed_score.h"

namespace twostack {

#define MAX_ACTION 500
	typedef tscore PackedScoreType[MAX_ACTION];
	typedef std::pair<int, const int &> AddScoreType;

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
		void updateRetval(PackedScoreType & retval, const int & which);
		std::unordered_map<int, Score> & getScores();
		const std::unordered_map<int, Score> & getScores() const;

		ScoreArray & operator=(const ScoreArray & s) {
			m_mapScores.clear();
			m_mapScores.insert(s.m_mapScores.begin(), s.m_mapScores.end());
			return *this;
		}
	};

	inline ScoreArray::ScoreArray() = default;

	inline ScoreArray::ScoreArray(const ScoreArray & s) {
		m_mapScores.clear();
		m_mapScores.insert(s.m_mapScores.begin(), s.m_mapScores.end());
	}

	inline ScoreArray::~ScoreArray() = default;

	inline void ScoreArray::reset() {
		m_mapScores.clear();
	}

	inline void ScoreArray::updateCurrent(const AddScoreType & added, const int & round) {
		m_mapScores[added.first].updateCurrent(added.second, round);
	}

	inline void ScoreArray::updateAverage(const int & round) {
		for (auto & score : m_mapScores) {
			score.second.updateAverage(round);
		}
	}

	inline void ScoreArray::updateRetval(PackedScoreType & retval, const int & which) {
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

	inline std::unordered_map<int, Score> & ScoreArray::getScores() {
		return m_mapScores;
	}

	inline const std::unordered_map<int, Score> & ScoreArray::getScores() const {
		return m_mapScores;
	}
}

std::istream & operator>>(std::istream & is, twostack::ScoreArray & s);
std::ostream & operator<<(std::ostream & os, const twostack::ScoreArray & s);

inline bool operator==(const twostack::AddScoreType & s, const int & i) {
	return s.second == i;
}

namespace twostack {

#define OUTPUT_STEP 100

#define AGENDA_SIZE 64

	enum Action {
		NO_ACTION = 0,
		AL_MM,
		AR_MM,
		AL_RC,
		AR_RC,
		AL_SH,
		AR_SH,
		AL_RE,
		AR_RE,
		MEM,
		RECALL,
		SHIFT,
		REDUCE,
		SHIFT_REDUCE
	};

#define ENCODE_POSTAG_SET_2(T1, T2)		(((T1) << 8) | (T2))
#define ENCODE_POSTAG_SET_3(T1, T2, T3)	(((T1) << 16) | ((T2) << 8) | (T3))

	typedef SetOfLabels<int, long long> SetOfDepLabels;

	typedef PackedScoreMap<Int, ScoreArray, PackedScoreType, AddScoreType> IntMap;
	typedef PackedScoreMap<Word, ScoreArray, PackedScoreType, AddScoreType> WordMap;
	typedef PackedScoreMap<POSTag, ScoreArray, PackedScoreType, AddScoreType> POSTagMap;
	typedef PackedScoreMap<TwoWords, ScoreArray, PackedScoreType, AddScoreType> TwoWordsMap;
	typedef PackedScoreMap<ThreeWords, ScoreArray, PackedScoreType, AddScoreType> ThreeWordsMap;
	typedef PackedScoreMap<WordPOSTag, ScoreArray, PackedScoreType, AddScoreType> WordPOSTagMap;
	typedef PackedScoreMap<WordWordPOSTag, ScoreArray, PackedScoreType, AddScoreType> WordWordPOSTagMap;
	typedef PackedScoreMap<WordPOSTagPOSTag, ScoreArray, PackedScoreType, AddScoreType> WordPOSTagPOSTagMap;
	typedef PackedScoreMap<WordWordPOSTagPOSTag, ScoreArray, PackedScoreType, AddScoreType> TwoPOSTaggedWordsMap;

	typedef PackedScoreMap<Int, ScoreArray, PackedScoreType, AddScoreType> POSTagSet2Map;
	typedef PackedScoreMap<Int, ScoreArray, PackedScoreType, AddScoreType> POSTagSet3Map;

	typedef PackedScoreMap<std::string, ScoreArray, PackedScoreType, AddScoreType> StringMap;
	typedef PackedScoreMap<WordInt, ScoreArray, PackedScoreType, AddScoreType> WordIntMap;
	typedef PackedScoreMap<POSTagInt, ScoreArray, PackedScoreType, AddScoreType> POSTagIntMap;
	typedef PackedScoreMap<TwoWordsInt, ScoreArray, PackedScoreType, AddScoreType> WordWordIntMap;
	typedef PackedScoreMap<POSTagSet2Int, ScoreArray, PackedScoreType, AddScoreType> POSTagPOSTagIntMap;

	class StateItem;

	typedef AgendaBeam<StateItem, AGENDA_SIZE> StateAgenda;
	typedef AgendaBeam<ScoredAction, AGENDA_SIZE> ScoreAgenda;

	void printAction(const int & action);
	int decodeAction(const int & action);
}

#endif