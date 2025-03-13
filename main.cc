#include <iostream>
#include <pi.hh>

int main(void){
	std::string input;
	std::string format;

	while(true){
		std::cin >> format;
		std::cin >> input;

		std::cout << "Format " << input << " based on format " << format << ".:\n";

		std::vector<pi::rule> rules = pi::generate_rules(format);

		std::vector<std::string> output;

		bool is_matched = pi::is_matched(input, rules, &output);

		std::cout << is_matched << " by length " << output.size() << ".\n";

		for(std::string str : output){
			std::cout << str << ".\n";
		}
	}
}

