#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>

#include "Controller.h"
#include "TypedOption.h"

namespace ahoy {
	class Parser : Controller {
	private:
		bool _strict = false;
		bool _quiet = false;
		std::map<std::string, Option*> _options;
		std::map<std::string, std::string> _aliases;
		std::map<std::string, std::set<std::string>> _reverseAliases;
		std::map<int, std::string> _indices;
		std::vector<std::string> _examples;
		std::set<std::string> _required;

	public:
		template <class T>
		TypedOption<T>* define(std::string key, T* output) {
			TypedOption<T>* option = new TypedOption<T>(key, output);
			option->_controller = this;
			_options[key] = option;
			return option;
		}
		
		void example(std::string value);
		virtual std::set<std::string> getAliases(std::string key);
		std::string help();
		bool parse(const int argc, const char** argv);
		virtual void registerAlias(std::string original, std::string alias);
		virtual void registerIndex(std::string key, int index);
		virtual void registerRequire(std::string key);
		Parser* strict(bool enabled);
		Parser* quiet(bool enabled);
	};
}