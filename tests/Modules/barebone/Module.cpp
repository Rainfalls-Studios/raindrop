// #include <Raindrop/Raindrop.hpp>
// #include <Raindrop/Modules/ModuleInterface.hpp>
// #include <Raindrop/Context.hpp>

// #include <iostream>

// class Interface : public Raindrop::Modules::ModuleInterface{
// 	public:
// 		Interface(){
// 			std::cout << "Interface created !" << std::endl;
// 		}

// 		~Interface(){
// 			std::cout << "Interface destroyed !" << std::endl;
// 		}

// 		// Since we do not have any render systems, it is better to not override this method
// 		// const std::unordered_map<std::string, RenderSystemCollection>& renderSystemCollections() const;
// };

// extern "C" {
// 	void startup(Raindrop::Context& context){
		
// 	}

// 	void shutdown(){
		
// 	}

// 	Interface* initializeInterface(){
// 		return new Interface();
// 	}

// 	void destroyInterface(Interface* interface){
// 		delete interface;
// 	}

// 	std::vector<std::string> getAliases(){
// 		return {
// 			"Module",
// 			"Mod"
// 		};
// 	}

// 	std::string getName(){
// 		return "barbone module";
// 	}

// 	std::string getDescription(){
// 		return "The simplest module";
// 	}

// 	Raindrop::Version getVersion(){
// 		return Raindrop::Version(0, 1, 0, 0);
// 	}
// }