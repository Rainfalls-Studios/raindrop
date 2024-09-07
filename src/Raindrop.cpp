#include <Raindrop/Raindrop.hpp>

namespace Raindrop{
	Raindrop::Setup::Setup(){
		spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%t] [%^%l%$] [%s %#] :: %v");
	}

	Raindrop::Raindrop() : 
		_setup(),
		_core(),
		_graphics(_core)
	{}

	Raindrop::~Raindrop(){

	}

	void Raindrop::run(){
		
	}
}