#ifndef _TOKEN_H
#define _TOKEN_H
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

typedef std::string ttoken;

class Token {
protected:
	std::unordered_map<ttoken, int> m_mapTokens;
	std::vector<ttoken> m_vecKeys;
	int m_nStartingToken;
	int m_nWaterMark;

public:
	Token(const int & nTokenStartFrom = 1);
	const int & lookup(const ttoken & key);
	int find(const ttoken & key, const int & val = 0);
	const ttoken & key(const int & token) const;
	int count() const;
	void add(const ttoken & key);

	friend std::istream & operator>>(std::istream & is, Token & token) {
		int count;
		is >> count;
		ttoken t;
		while (count--) {
			is >> t;
			token.lookup(t);
		}
		return is;
	}

	friend std::ostream & operator<<(std::ostream & os, const Token & token) {
		os << token.m_vecKeys.size() << std::endl;
		for (auto t : token.m_vecKeys) {
			os << t << std::endl;
		}
		return os;
	}
};

inline Token::Token(const int & nTokenStartFrom) {
	m_nWaterMark = m_nStartingToken = nTokenStartFrom;
}

inline const int & Token::lookup(const ttoken & key) {
	if (m_mapTokens.find(key) == m_mapTokens.end()) {
		m_mapTokens[key] = m_nWaterMark++;
		m_vecKeys.push_back(key);
	}
	return m_mapTokens[key];
}

inline int Token::find(const ttoken & key, const int & val) {
	return m_mapTokens.find(key) == m_mapTokens.end() ? val : m_mapTokens[key];
}

inline const ttoken & Token::key(const int & token) const {
	return m_vecKeys[token - m_nStartingToken];
}

inline int Token::count() const {
	return m_vecKeys.size();
}

inline void Token::add(const ttoken & key) {
	if (m_mapTokens.find(key) == m_mapTokens.end()) {
		m_mapTokens[key] = m_nWaterMark++;
		m_vecKeys.push_back(key);
	}
}

#endif
