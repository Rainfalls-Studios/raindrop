#include "Raindrop/Raindrop.hpp"
#include <iostream>

int main(){

	try{
		Raindrop::Raindrop engine;
		engine.run();
	} catch (const std::exception& e){
		std::cerr << "CRITICAL ERROR : " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}