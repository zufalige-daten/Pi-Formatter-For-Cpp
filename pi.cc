#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <pi.hh>

namespace pi{
	rule::rule(rule_type type, std::string parameter){
		this->type = type;
		this->parameter = parameter;
	}

	std::tuple<bool, rule_stored, std::string> rule::does_match(std::string str, int index){
		int i;

		std::string out_str = "";

		switch(this->type){
			case EQUATE:
				if(std::strncmp(str.c_str() + index, parameter.c_str(), parameter.length()) != 0){
					return std::tuple(false, rule_stored::NOT_STORED, "");
				}

				out_str = parameter;
				break;
			case ANY_OF:
				for(i = index; i < str.length() && parameter.find(str[i]) != parameter.npos; i++){
					out_str += str[i];
				}

				if(i == index){
					return std::tuple(false, rule_stored::STORED, "");
				}

				return std::tuple(true, rule_stored::STORED, out_str);
				break;
			case OPTION_ANY_OF:
				for(i = index; i < str.length() && parameter.find(str[i]) != parameter.npos; i++){
					out_str += str[i];
				}
				break;
			case ANY_NOT_OF:
				for(i = index; i < str.length() && parameter.find(str[i]) == parameter.npos; i++){
					out_str += str[i];
				}

				if(i == index){
					return std::tuple(false, rule_stored::STORED, "");
				}

				return std::tuple(true, rule_stored::STORED, out_str);
				break;
			case OPTION_ANY_NOT_OF:
				for(i = index; i < str.length() && parameter.find(str[i]) == parameter.npos; i++){
					out_str += str[i];
				}
				break;
		}

		return std::tuple(true, rule_stored::NOT_STORED, out_str);
	}

	void assert_q(bool condition, std::string err){
		if(!condition){
			std::cerr << "Error: " << err << "\n";
			std::exit(-1);
		}
	}

	std::vector<rule> generate_rules(std::string formatter){
		std::vector<rule> rules;

		int i = 0;

		std::string equate = "";

		while(i < formatter.length()){
			if(formatter[i] == '%' || formatter[i] == ' ' || formatter[i] == '\t' || formatter[i] == '\n'){
				if(equate != ""){
					rules.push_back(rule(rule_type::EQUATE, equate));
					equate = "";
				}

				if(formatter[i] == ' ' || formatter[i] == '\t' || formatter[i] == '\n'){
					rules.push_back(rule(rule_type::OPTION_ANY_OF, " \t\n"));
					i++;
				}
				else{
					i++;
					assert_q(i < formatter.length() && formatter[i] == '[', "Expected '[' within formatstring.");
					i++;
					assert_q(i < formatter.length(), "Expected '^' within formatstring.");
					if(formatter[i] == '^'){
						i++;
						assert_q(i < formatter.length(), "Expected ']' within formatstring.");
						std::string outstr = "";
						for(; i < formatter.length() && formatter[i] != ']'; i++){
							outstr += formatter[i];
						}
						assert_q(i < formatter.length() && formatter[i] == ']', "Expected ']' within formatstring.");
						i++;
						rules.push_back(rule(rule_type::ANY_NOT_OF, outstr));
					}
					else{
						std::string outstr = "";
						for(; i < formatter.length() && formatter[i] != ']'; i++){
							outstr += formatter[i];
						}
						assert_q(i < formatter.length() && formatter[i] == ']', "Expected ']' within formatstring.");
						i++;
						rules.push_back(rule(rule_type::ANY_OF, outstr));
					}
				}
			}
			else{
				equate += formatter[i];
				i++;
			}
		}

		if(equate != ""){
			rules.push_back(rule(rule_type::EQUATE, equate));
			equate = "";
		}

		return rules;
	}

	bool is_matched(std::string str, std::vector<rule> rules, std::vector<std::string> *output){
		int index = 0;

		int c_rule = 0;

		for(rule rule_singular : rules){
			c_rule++;

			std::tuple<bool, rule_stored, std::string> returned_status = rule_singular.does_match(str, index);

			if(!std::get<0>(returned_status)){
				return false;
			}

			if(std::get<1>(returned_status) == rule_stored::STORED){
				output->push_back(std::get<2>(returned_status));
			}

			index += std::get<2>(returned_status).length();

			if(index >= str.length()){
				break;
			}
		}

		if(index != str.length() || c_rule != rules.size()){
			return false;
		}

		return true;
	}
}

