#include <iostream>

#include "arrrgh.hpp"


int main(int argc, const char * argv[]) {
	std::cout << "Initializing arguments..." << std::endl;

	arrrgh::add_option("source");
	arrrgh::add_flag("flag");
	arrrgh::add_flag("brag");
	arrrgh::add_list("tag");
	arrrgh::add_integer("number", 123);

	arrrgh::add_alias('s', "source");
	arrrgh::add_alias('f', "flag");
	arrrgh::add_alias('b', "brag");
	arrrgh::add_alias('t', "tag");
	arrrgh::add_alias('n', "number");

	std::cout << "Parsing arguments..." << std::endl;

	arrrgh::parse(argv, argv + argc);

	std::cout << "Checking options..." << std::endl;

	for (auto it : arrrgh::options<arrrgh::StringLike>) {
		std::cout << it.first << " -> \"" << it.second << '\"' << std::endl;
	}

	for (auto it : arrrgh::options<int>) {
		std::cout << it.first << " -> \"" << it.second << '\"' << std::endl;
	}

	for (auto it : arrrgh::options<bool>) {
		std::cout << it.first << " -> " << it.second << std::endl;
	}

	for (auto it : arrrgh::options<arrrgh::List>) {
		std::cout << it.first << " -> [" << std::endl;

		for (auto that : it.second) {
			std::cout << "    " << that << std::endl;
		}

		std::cout << ']' << std::endl;
	}

	std::cout << "Checking aliases..." << std::endl;

	for (auto it : arrrgh::aliases) {
		std::cout << it.first << " -> " << it.second << std::endl;
	}

	std::cout << "Checking parameters..." << std::endl;

	for (auto it : arrrgh::parameters) {
		std::cout << " * " << it << std::endl;
	}

	return 0;
}
