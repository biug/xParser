#ifndef _MINITWOSTACK_MACROS_H
#define _MINITWOSTACK_MACROS_H

#include "common/token/tagset.h"
#include "common/parser/agenda.h"
#include "common/parser/macros_base.h"
#include "include/learning/perceptron/packed_score.h"
#include "include/dependency_primitive.h"

namespace minitwostack {

#define MAX_ACTION 200
	typedef tscore PackedScoreType[MAX_ACTION];
	typedef std::pair<int, const int &> ActionScoreIncrement;

	class ActionScores {

	private:
		std::unordered_map<int, Score> m_mapScores;

	public:
		ActionScores();
		ActionScores(const ActionScores & s);
		~ActionScores();

		void reset();
		bool zero() const;
		void updateCurrent(const ActionScoreIncrement & added, const int & round);
		void updateAverage(const int & round);
		void updateRetval(PackedScoreType & retval, const int & which);
		std::unordered_map<int, Score> & getScores();
		const std::unordered_map<int, Score> & getScores() const;

		ActionScores & operator=(const ActionScores & s) {
			m_mapScores.clear();
			m_mapScores.insert(s.m_mapScores.begin(), s.m_mapScores.end());
			return *this;
		}
	};

	inline ActionScores::ActionScores() = default;

	inline ActionScores::ActionScores(const ActionScores & s) {
		m_mapScores.clear();
		m_mapScores.insert(s.m_mapScores.begin(), s.m_mapScores.end());
	}

	inline ActionScores::~ActionScores() = default;

	inline void ActionScores::reset() {
		m_mapScores.clear();
	}

	inline void ActionScores::updateCurrent(const ActionScoreIncrement & added, const int & round) {
		m_mapScores[added.first].updateCurrent(added.second, round);
	}

	inline void ActionScores::updateAverage(const int & round) {
		for (auto && score : m_mapScores) {
			score.second.updateAverage(round);
		}
	}

	inline void ActionScores::updateRetval(PackedScoreType & retval, const int & which) {
		switch (which) {
		case eNonAverage:
			for (auto && score : m_mapScores) {
				score.second.updateRetvalCurrent(retval[score.first]);
			}
			break;
		case eAverage:
			for (auto && score : m_mapScores) {
				score.second.updateRetvalTotal(retval[score.first]);
			}
			break;
		}
	}

	inline std::unordered_map<int, Score> & ActionScores::getScores() {
		return m_mapScores;
	}

	inline const std::unordered_map<int, Score> & ActionScores::getScores() const {
		return m_mapScores;
	}
}

std::istream & operator>>(std::istream & is, minitwostack::ActionScores & s);
std::ostream & operator<<(std::ostream & os, const minitwostack::ActionScores & s);

inline bool operator==(const minitwostack::ActionScoreIncrement & s, const int & i) {
	return s.second == i;
}

namespace minitwostack {

#define AGENDA_SIZE 64

	enum Action {
		NO_ACTION = 0,
		A_MM,
		A_RC,
		A_SH,
		A_RE,
		MEM,
		RECALL,
		SHIFT,
		REDUCE
	};

	// action start
	#define ACTION_START	0

	#define ENCODE_POSTAG_SET_2(T1, T2)		(((T1) << 8) | (T2))
	#define ENCODE_POSTAG_SET_3(T1, T2, T3)	(((T1) << 16) | ((T2) << 8) | (T3))
	inline unsigned int encodePOSTagSet4(const unsigned int & t1, const unsigned int & t2, const unsigned int & t3, const unsigned int & t4) {
		return ((t1 << 24) | (t2 << 16) | (t3 << 8) | t4);
	}

	typedef TagSetN<2> TagSet;

	typedef CoNLL08DepNode DependencyGraphNode;
	typedef CoNLL08DepGraph DependencyGraph;

	typedef TriGram<unsigned int> IntTagSet;
	typedef QuarGram<unsigned int> TwoIntsTagSet;

	typedef PackedScoreMap<IntTagSet, ActionScores, PackedScoreType, ActionScoreIncrement> WordTagsetMap;
	typedef PackedScoreMap<IntTagSet, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagTagsetMap;

	typedef PackedScoreMap<TwoIntsTagSet, ActionScores, PackedScoreType, ActionScoreIncrement> TwoWordsTagsetMap;
	typedef PackedScoreMap<IntTagSet, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagSet2TagsetMap;

	typedef PackedScoreMap<Int, ActionScores, PackedScoreType, ActionScoreIncrement> IntMap;
	typedef PackedScoreMap<Int, ActionScores, PackedScoreType, ActionScoreIncrement> WordMap;
	typedef PackedScoreMap<Int, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagMap;
	typedef PackedScoreMap<Int, ActionScores, PackedScoreType, ActionScoreIncrement> SuperTagMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> TwoWordsMap;
	typedef PackedScoreMap<ThreeInts, ActionScores, PackedScoreType, ActionScoreIncrement> ThreeWordsMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> WordPOSTagMap;
	typedef PackedScoreMap<ThreeInts, ActionScores, PackedScoreType, ActionScoreIncrement> WordWordPOSTagMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> WordPOSTagPOSTagMap;
	typedef PackedScoreMap<ThreeInts, ActionScores, PackedScoreType, ActionScoreIncrement> WordPOSTagPOSTagIntMap;
	typedef PackedScoreMap<ThreeInts, ActionScores, PackedScoreType, ActionScoreIncrement> WordWordPOSTagPOSTagMap;

	typedef PackedScoreMap<Unsigned, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagSet2Map;
	typedef PackedScoreMap<Unsigned, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagSet3Map;
	typedef PackedScoreMap<Unsigned, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagSet4Map;

	typedef PackedScoreMap<std::string, ActionScores, PackedScoreType, ActionScoreIncrement> StringMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> WordIntMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagIntMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> SuperTagIntMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> SuperTagSet2Map;
	typedef PackedScoreMap<ThreeInts, ActionScores, PackedScoreType, ActionScoreIncrement> TwoWordsIntMap;
	typedef PackedScoreMap<ThreeInts, ActionScores, PackedScoreType, ActionScoreIncrement> WordPOSTagIntMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> WordSuperTagMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagSuperTagMap;
	typedef PackedScoreMap<ThreeInts, ActionScores, PackedScoreType, ActionScoreIncrement> WordPOSTagSuperTagMap;
	typedef PackedScoreMap<FourInts, ActionScores, PackedScoreType, ActionScoreIncrement> ThreeWordsIntMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagSet2IntMap;
	typedef PackedScoreMap<TwoInts, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagSet3IntMap;
	typedef PackedScoreMap<ThreeInts, ActionScores, PackedScoreType, ActionScoreIncrement> POSTagSet4TwoIntsMap;

	class StateItem;

	typedef AgendaBeam<StateItem, AGENDA_SIZE> StateAgenda;
	typedef AgendaBeam<ScoredAction, AGENDA_SIZE> ScoreAgenda;

	int decodeAction(const int & action);
	void printAction(const int & action);
	void loadLabels();
}

#endif
