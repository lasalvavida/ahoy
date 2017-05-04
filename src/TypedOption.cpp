#include <vector>

#include "TypedOption.h"

using namespace ahoy;
using namespace std;

bool TypedOption<bool>::parse(const int argc, const char** argv, int* positionPtr, string* parseError) {
	*output = true;
	return true;
}

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

string TypedOption<bool>::help() {
	return Option::help() + " [bool]" + (hasDefault ? (" [default: " + string(defaultValue ? "true" : "false") + "]") : "");
}

string TypedOption<double>::help() {
	return Option::help() + " [double]" + (hasDefault ? (" [default: " + to_string(defaultValue) + "]") : "");
}

string TypedOption<int>::help() {
	return Option::help() + " [int]" + (hasDefault ? (" [default: " + to_string(defaultValue) + "]") : "");
}

string TypedOption<string>::help() {
	return Option::help() + " [string]" + (hasDefault ? (" [default: \"" + defaultValue + "\"]") : "");
}

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
	return Option::help() + " [vec<int>" + (countValue < 0 ? "" : ", " + countValue) + "]" + (hasDefault ? (" [default: " + defaultString + "]") : "");
}

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