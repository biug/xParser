#ifndef _EMP_H
#define _EMP_H
#include "token.h"

class TEmptyTag {
protected:
	static Token tokenizer;

public:
	TEmptyTag();
	~TEmptyTag();

	static int code(const ttoken & s);
	static const ttoken & key(const int & token);
	static int count();
	static const int START = 1;

	static Token & getTokenizer();
};

inline TEmptyTag::TEmptyTag() = default;

inline TEmptyTag::~TEmptyTag() = default;

inline int TEmptyTag::count() {
	return TEmptyTag::tokenizer.count();
}

inline int TEmptyTag::code(const ttoken & s) {
	return TEmptyTag::tokenizer.lookup(s);
}

inline const ttoken & TEmptyTag::key(const int & index) {
	return TEmptyTag::tokenizer.key(index);
}

inline Token & TEmptyTag::getTokenizer() {
	return tokenizer;
}

#endif
