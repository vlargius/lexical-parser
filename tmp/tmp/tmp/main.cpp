#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::endl;

namespace {
    enum  WordClass {
        VERB,
        NOUN,
        UNION,
        ARTICLE,
        TERMINATOR,
		SENTENCE,
		ERROR
	};

	std::unordered_map<string, WordClass> type{
		{ "a", ARTICLE },
		{ "the", ARTICLE },
		{ "bird", NOUN },
		{ "fish", NOUN },
		{ "apple", NOUN },
		{ "cat", NOUN },
		{ "swim", VERB },
		{ "sleep", VERB },
		{ "drink", VERB },
		{ "fly", VERB },
		{ "and", UNION },
		{ ".", TERMINATOR }

	};
}

class Parser {
public: 
	Parser(const string& str) :
		is_empty(true),
		ss(str) {}

    string get_word() { 
        string word;
		if (is_empty) {
			ss >> word;
		}
		else {
			word = buff;
			is_empty = true;
		}
        return word;
    }

    void putback(const string& word) {
		is_empty = false;
		buff = word;
    } 

	bool is_dot() {
		auto w = get_word();
		if (w == ".") {
			return true;
		}
		else {
			putback(w);
			return false;
		}
	}

	bool is_union() {
		string w = get_word();
		if (type[w] == UNION) {
			return true;
		}
		else {
			putback(w);
			return false;
		}
	}

	bool is_compound_noun() {
		string w = get_word();
		switch (type[w]) {
		case ARTICLE:
		{
			string n = get_word();
			return type[n] == NOUN;
		}
		case NOUN:
		{
			return true;
		}
		default:
			putback(w);
			return false;
		}
	}

	bool is_verb() {
		string w = get_word();
		if (type[w] == VERB) {
			return true;
		}
		else {
			putback(w);
			return false;
		}
	}

	bool is_sentence() {
		return is_compound_noun() && is_verb();			
	}


	bool is_compund_sentence() {
		return is_sentence() && is_union() && is_sentence();
	}

	bool is_complete_sentence() {
		if (!is_sentence())
			return false;
		while(is_union()) {
			if (!is_sentence())
				return false;
		}
		is_dot();
	}	


private:
	std::stringstream ss;
	string buff;
	bool is_empty;
};

bool is_sentence(const string& str) {
	return Parser(str).is_complete_sentence();
}

int main() {
	cout << "Enter a test sentence e.g. : \"the bird fly .\"" << endl
		<< "Put the dot in the end of the sentence" << endl
		<< "Enter \"quit\" for exit" << endl
		<< "------------------------------------------------" << endl;
    while(true) {
       string str;
       std::getline(cin, str);
       if(str == "quit") {
           cout << "Bye!" << endl;
           return 0;
       }
       if(is_sentence(str)) {
            cout << "Yes" << endl;
       } else {
            cout << "No" << endl;
       }
    }

    return 0;
}

