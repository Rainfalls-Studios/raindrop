#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Modules/ModuleInterface.hpp>
#include <Raindrop/Context.hpp>

#include <iostream>

class Interface : public Raindrop::Modules::ModuleInterface{
	public:
		Interface(){
			std::cout << "Interface created !" << std::endl;
		}

		~Interface(){
			std::cout << "Interface destroyed !" << std::endl;
		}
};

extern "C" {
	Interface* initializeInterface(Raindrop::Context& context){
		return new Interface();
	}

	void destroyInterface(Interface* interface){
		delete interface;
	}

	const char** getAliases(uint32_t* lenght){
		assert(lenght);

		static const char* aliases[] = {
			"Module",
			"Mod"
		};

		*lenght = sizeof(aliases) / sizeof(aliases[0]);
		return aliases;
	}
}