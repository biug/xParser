#ifndef _SUPERTAG_H
#define _SUPERTAG_H
#include "token.h"

#define NULL_SUPERTAG	"_"

class TSuperTag {
protected:
	static Token tokenizer;

public:
	TSuperTag();
	~TSuperTag();

	static int code(const ttoken & s);
	static const ttoken & key(const int & token);
	static int count();
	static const int START = 1;
	static const int ROOT = 1;
	static std::string null_tag;
	static void add(const ttoken & s);

	static Token & getTokenizer();
};

inline TSuperTag::TSuperTag() = default;

inline TSuperTag::~TSuperTag() = default;

inline int TSuperTag::count() {
	return TSuperTag::tokenizer.count();
}

inline int TSuperTag::code(const ttoken & s) {
	return s == null_tag ? 0 : TSuperTag::tokenizer.lookup(s);
}

inline const ttoken & TSuperTag::key(const int & index) {
	return index == 0 ? null_tag : TSuperTag::tokenizer.key(index);
}

inline Token & TSuperTag::getTokenizer() {
	return tokenizer;
}

inline void TSuperTag::add(const ttoken & s) {
	tokenizer.add(s);
}

#endif
