#include <algorithm>
#include <iostream>

#include "Parser.h"

using namespace ahoy;
using namespace std;

void Parser::example(string value) {
	_examples.push_back(value);
}

set<string> Parser::getAliases(string key) {
	map<string, set<string>>::iterator it = _reverseAliases.find(key);
	if (it != _reverseAliases.end()) {
		return it->second;
	}
	return set<string>();
}

string Parser::help() {
	string message;
	if (_options.size() > 0) {
		message += "Options:\n";
		vector<Option*> options;
		for (auto keyOption : _options) {
			options.push_back(keyOption.second);
		}
		sort(options.begin(), options.end(), [](const Option* left, const Option* right) {
			if (left->indexValue >= 0 && right->indexValue >= 0) {
				return left->indexValue < right->indexValue;
			}
			else if (left->indexValue >= 0 && right->indexValue < 0) {
				return true;
			}
			else if (left->indexValue < 0 && right->indexValue >= 0) {
				return false;
			}
			return left->key < right->key;
		});
		for (Option* option : options) {
			message += option->help() + "\n";
		}
		message += "\n";
	}
	if (_examples.size() > 0) {
		message += "Examples:\n";
		for (string example : _examples) {
			message += "  " + example;
		}
	}
	
	return message;
}

bool Parser::parse(const int argc, const char** argv) {
	int position = 0;
	set<string> matched;
	vector<const char*> looseStrings;
	string parseError;
	while (position < argc) {
		std::string arg = argv[position];
		std::string key = "";
		if (arg.length() > 2) {
			if (arg.at(0) == '-') {
				if (arg.at(1) == '-') {
					// String key
					key = arg.substr(2);
				}
				else {
					// Single-character key
					key = arg.at(1);
				}
			}
		}
		else {
			looseStrings.push_back(argv[position]);
		}
		position++;
		if (key != "") {
			Option* option = NULL;
			map<string, Option*>::iterator optionsIt = _options.find(key);
			if (optionsIt != _options.end()) {
				// Matched option
				option = optionsIt->second;
			}
			else {
				// Try aliases
				map<string, string>::iterator aliasesIt = _aliases.find(key);
				if (aliasesIt != _aliases.end()) {
					// Matched alias
					key = aliasesIt->second;
					optionsIt = _options.find(key);
					if (optionsIt != _options.end()) {
						// Matched option
						option = optionsIt->second;
					}
				}
			}
			if (option != NULL) {
				matched.insert(key);
				if (!option->parse(argc, argv, &position, &parseError)) {
					if (!_quiet) {
						cout << help() << "Error while parsing argument: " << key << " " << parseError << endl << endl;
					}
					return false;
				}
			}
			else if (_strict) {
				// Option isn't defined and in strict mode
				if (!_quiet) {
					string strictError = "Unknown argument in strict mode: " + key;
					cout << help() << strictError << endl << endl;
				}
				return false;
			}
		}
	}
	int internalIndex = 0;
	int index = 0;
	while (index < looseStrings.size()) {
		// Try to fill in any missing indexed arguments with the loose strings
		map<int, string>::iterator indicesIt = _indices.find(internalIndex);
		if (indicesIt != _indices.end()) {
			std::string key = indicesIt->second;
			Option* option = NULL;
			if (matched.find(key) == matched.end()) {
				// This key hasn't been filled
				map<string, Option*>::iterator optionsIt = _options.find(key);
				if (optionsIt != _options.end()) {
					// Matched option
					option = optionsIt->second;
				}
			}
			else {
				// Already matched this key, try the next index
				internalIndex++;
			}
			if (option != NULL) {
				matched.insert(key);
				if (!option->parse(looseStrings.size(), looseStrings.data(), &index, &parseError)) {
					if (!_quiet) {
						cout << help() << "Error while parsing argument: " << key << " " << parseError << endl << endl;
					}
					return false;
				}
			}
		}
		else {
			break;
		}
	}
	set<string> missingRequired;
	for (string required : _required) {
		if (matched.find(required) == matched.end()) {
			missingRequired.insert(required);
		}
	}
	if (missingRequired.size() > 0) {
		if (!_quiet) {
			string missingError = "Missing required arguments: ";
			bool first = true;
			for (string match : missingRequired) {
				if (!first) {
					missingError += ", ";
				}
				missingError += match;
				first = false;
			}
			cout << help() << missingError << endl << endl;
		}
		return false;
	}
	return true;
}

void Parser::registerAlias(string original, string alias) {
	map<string, string>::iterator indirectionIt = _aliases.find(original);
	while (indirectionIt != _aliases.end()) {
		// This is an alias of an alias, we want to point to the root key
		original = indirectionIt->second;
		indirectionIt = _aliases.find(original);
	}
	map<string, string>::iterator aliasesIt = _aliases.find(alias);
	if (aliasesIt != _aliases.end()) {
		// Overwriting, so remove from reverse aliases
		_reverseAliases[aliasesIt->second].erase(alias);
	}
	_aliases[alias] = original;
	map<string, set<string>>::iterator reverseAliasesIt = _reverseAliases.find(original);
	if (reverseAliasesIt == _reverseAliases.end()) {
		// Doesn't exist yet, add it
		_reverseAliases[original] = set<string>();
	}
	_reverseAliases[original].insert(alias);
}

void Parser::registerIndex(string key, int index) {
	_indices[index] = key;
}

void Parser::registerRequire(string key) {
	_required.insert(key);
};

Parser* Parser::strict(bool enabled) {
	_strict = enabled;
	return this;
}

Parser* Parser::quiet(bool enabled) {
	_quiet = enabled;
	return this;
}

