#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Context.hpp>
#include <spdlog/spdlog.h>

namespace Raindrop{
	Raindrop::Raindrop(){
		spdlog::info("Constructing Raindrop engine");
		_context = new Context();
	}

	Raindrop::~Raindrop(){
		spdlog::info("Destructing Raindrop engine");
		delete _context;
	}
}