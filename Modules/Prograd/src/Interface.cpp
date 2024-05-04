#include <Prograd/Interface.hpp>
#include <iostream>

using namespace Prograd;

Interface::Interface(){
	std::cout << "Prograd instance created !" << std::endl;
}

Interface::~Interface(){
	std::cout << "Prograd instance destroyed !" << std::endl;
}

extern "C"{
	Interface* initializeInterface(Raindrop::Context& context){
		raindropContext() = &context;
		return new Interface();
	}

	void destroyInterface(Interface* interface){
		delete interface;
	}

	void startup(Raindrop::Context& context){
		raindropContext() = &context;
	}

	void shutdown(){
		raindropContext() = nullptr;
	}

	std::vector<std::string> getAliases(){
		return {
			"Prograd"
		};
	}

	std::string getName(){
		return "Prograd";
	}

	std::string getDescription(){
		return "Raindrop's graphics module";
	}

	Raindrop::Version getVersion(){
		return Raindrop::Version(0, 0, 1, 0);
	}
}

