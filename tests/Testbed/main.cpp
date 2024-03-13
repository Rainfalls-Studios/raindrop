#include <iostream>
#include <Raindrop/Raindrop.hpp>
#include "config.h"

void testbed(){
	std::filesystem::current_path(PATH);
	Raindrop::Raindrop engine;

	engine.run();
}

int main(){
	std::cout << "==========================================" << std::endl;
	std::cout << "=                 TESTBED                =" << std::endl;
	std::cout << "==========================================" << std::endl;
	std::cout << std::endl;
	
	testbed();

	std::cout << std::endl;
	std::cout << "==========================================" << std::endl;
	std::cout << "=                 TESTBED                =" << std::endl;
	std::cout << "==========================================" << std::endl;

	return 0;
}