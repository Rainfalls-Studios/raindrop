#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Core/Context.hpp>

namespace Raindrop::Graphics{
	Context::Context(Core::Context& core) :
		core{core},
		logger("Raindrop::Graphics"),
		registry{core.registry["Graphics"]},
		internal(*this),
		utils(*this)
	{}

	// Context::~Context(){
	// 	device.waitIdle();
	// }
}