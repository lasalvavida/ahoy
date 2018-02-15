#include <cstdlib>
#include <vector>

#include "ahoy/TypedOption.h"

using namespace std;

namespace ahoy {
	template <>
	bool TypedOption<bool>::parse(const int argc, const char** argv, int* positionPtr, string* parseError) {
		*output = true;
		return true;
	}

	template <>
	bool TypedOption<double>::parse(const int argc, const char** argv, int* positionPtr, string* parseError) {
		int position = *positionPtr;
		if (position == argc) {
			*parseError = "Requires one argument";
			return false;
		}
		*output = atof(argv[position]);
		(*positionPtr)++;
		return true;
	}

	template <>
	bool TypedOption<int>::parse(const int argc, const char** argv, int* positionPtr, string* parseError) {
		int position = *positionPtr;
		if (position == argc) {
			*parseError = "Requires one argument";
			return false;
		}
		*output = atoi(argv[position]);
		(*positionPtr)++;
		return true;
	}

	template <>
	bool TypedOption<string>::parse(const int argc, const char** argv, int* positionPtr, string* parseError) {
		int position = *positionPtr;
		if (position == argc) {
			*parseError = "Requires one argument";
			return false;
		}
		*output = argv[position];
		(*positionPtr)++;
		return true;
	}

	template <>
	bool TypedOption<vector<int>>::parse(const int argc, const char** argv, int* positionPtr, string* parseError) {
		int position = *positionPtr;
		int consumed = 0;
		output->clear();
		while (position < argc && (countValue < 0 || consumed < countValue)) {
			const char* arg = argv[position];
			if (arg[0] == '-') {
				break;
			}
			output->push_back(atoi(arg));
			position++;
			consumed++;
		}
		*positionPtr = position;
		return true;
	}

	template<>
	bool TypedOption<vector<string>>::parse(const int argc, const char** argv, int* positionPtr, string* parseError) {
		int position = *positionPtr;
		int consumed = 0;
		output->clear();
		while (position < argc && (countValue < 0 || consumed < countValue)) {
			const char* arg = argv[position];
			if (arg[0] == '-') {
				break;
			}
			output->push_back(arg);
			position++;
			consumed++;
		}
		*positionPtr = position;
		return true;
	}

	template<>
	string TypedOption<bool>::help() {
		return Option::help() + " [bool]" + (hasDefault ? (" [default: " + string(defaultValue ? "true" : "false") + "]") : "");
	}

	template<>
	string TypedOption<double>::help() {
		return Option::help() + " [double]" + (hasDefault ? (" [default: " + to_string(defaultValue) + "]") : "");
	}

	template<>
	string TypedOption<int>::help() {
		return Option::help() + " [int]" + (hasDefault ? (" [default: " + to_string(defaultValue) + "]") : "");
	}

	template<>
	string TypedOption<string>::help() {
		return Option::help() + " [string]" + (hasDefault ? (" [default: \"" + defaultValue + "\"]") : "");
	}

	template<>
	string TypedOption<vector<int>>::help() {
		string defaultString = "[";
		bool first = true;
		for (int value : defaultValue) {
			if (!first) {
				defaultString += ", ";
			}
			else {
				first = false;
			}
			defaultString += to_string(value);
		}
		defaultString += "]";
		return Option::help() + " [vec<int>" + (countValue < 0 ? "" : ", " + to_string(countValue)) + "]" + (hasDefault ? (" [default: " + defaultString + "]") : "");
	}

	template<>
	string TypedOption<vector<string>>::help() {
		string defaultString = "[";
		bool first = true;
		for (string value : defaultValue) {
			if (!first) {
				defaultString += ", ";
			}
			else {
				first = false;
			}
			defaultString += value;
		}
		defaultString += "]";
		return Option::help() + " [vec<string>" + (countValue < 0 ? "" : ", " + to_string(countValue)) + "]" + (hasDefault ? (" [default: " + defaultString + "]") : "");
	}
}
