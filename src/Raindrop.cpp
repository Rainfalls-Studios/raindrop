#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop{
	Raindrop::Raindrop(){
		_context = new Context();
	}

	Raindrop::~Raindrop(){
		delete _context;
	}
}