#pragma once

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <tuple>
#include <vector>
#include <string>

namespace pi{
	enum rule_type{ EQUATE, ANY_OF, OPTION_ANY_OF, ANY_NOT_OF, OPTION_ANY_NOT_OF };

	enum rule_stored{ NOT_STORED, STORED };

	class rule{
private:
		rule_type type;
		std::string parameter;
public:
		rule(rule_type type, std::string parameter);

		std::tuple<bool, rule_stored, std::string> does_match(std::string str, int index);
	};

	void assert_q(bool condition, std::string err);

	std::vector<rule> generate_rules(std::string formatter);

	bool is_matched(std::string str, std::vector<rule> rules, std::vector<std::string> *output);
}

