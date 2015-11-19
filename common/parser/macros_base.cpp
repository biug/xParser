#include <map>
#include <set>
#include <stack>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "macros_base.h"

int encodeLinkDistance(const int & st, const int & n0) {
	int diff = n0 - st;
	if (diff > 10) {
		diff = 6;
	}
	else if (diff > 5) {
		diff = 5;
	}
	return diff;
}

int encodeLinkDistanceOrDirection(const int & hi, const int & di, bool dir) {
	int diff = hi - di;
	if (dir) {
		return diff > 0 ? 7 : -7;
	}
	if (diff < 0) {
		diff = -diff;
	}
	if (diff > 10) {
		diff = 6;
	}
	else if (diff > 5) {
		diff = 5;
	}
	if (hi < di) {
		diff = -diff;
	}
	return diff;
}

std::string nCharPrev(const Sentence & sent, int index, int n) {
	std::string str = "";
	--index;
	while (index >= 0 && n > 0) {
		const std::string & word = SENT_WORD(sent[index]);
		int i = word.length() - 1;
		while (i >= 0) {
			const unsigned char & c = word[i];
			if ((c >> 6) != 2) {
				--n;
				if (n == 0) {
					break;
				}
			}
			--i;
		}
		str = word.substr(i == -1 ? 0 : i) + str;
		--index;
	}
	while (n--) {
		str = "(P)" + str;
	}
	return str;
}

std::string nCharNext(const Sentence & sent, int index, int n) {
	std::string str = "";
	++index;
	while (index < sent.size() && n > 0) {
		const std::string & word = SENT_WORD(sent[index]);
		int i = 1;
		while (i < word.length()) {
			const unsigned char & c = word[i];
			if ((c >> 6) != 2) {
				--n;
				if (n == 0) {
					break;
				}
			}
			++i;
		}
		if (i == word.length()) {
			--n;
		}
		str = str + word.substr(0, i);
		++index;
	}
	while (n--) {
		str = str + "(N)";
	}
	return str;
}

std::istream & operator>>(std::istream & input, Sentence & sentence) {
	sentence.clear();
	ttoken line, token;
	std::getline(input, line);
	std::istringstream iss(line);
	while (iss >> token) {
		int i = token.rfind(SENT_SPTOKEN);
		sentence.push_back(POSTaggedWord(token.substr(0, i), token.substr(i + 1)));
	}
	return input;
}

std::istream & operator>>(std::istream & input, DependencyTree & tree) {
	tree.clear();
	ttoken line, token;
	while (true) {
		std::getline(input, line);
		if (line.empty()) {
			break;
		}
		DependencyTreeNode node;
		std::istringstream iss(line);
		iss >> TREENODE_WORD(node) >> TREENODE_POSTAG(node) >> TREENODE_HEAD(node) >> TREENODE_LABEL(node);
		tree.push_back(node);
	}
	return input;
}



std::ostream & operator<<(std::ostream & output, const Sentence & sentence) {
	auto itr = sentence.begin();
	while (true) {
		output << SENT_WORD(*itr) << SENT_SPTOKEN << SENT_POSTAG(*itr);
		if (++itr == sentence.end()) {
			break;
		}
		else {
			output << " ";
		}
	}
	output << std::endl;
	return output;
}

std::ostream & operator<<(std::ostream & output, const DependencyTree & tree) {
	for (auto itr = tree.begin(); itr != tree.end(); ++itr) {
		output << TREENODE_WORD(*itr) << "\t" << TREENODE_POSTAG(*itr) << "\t" << TREENODE_HEAD(*itr) << "\t" << TREENODE_LABEL(*itr) << std::endl;
	}
	output << std::endl;
	return output;
}

void nBackSpace(const std::string & str) {
	for (int i = 0; i < str.size(); ++i) {
		std::cout << '\b';
	}
	std::cout << std::flush;
}
