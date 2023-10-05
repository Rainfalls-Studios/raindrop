#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Core/Context.hpp>

namespace Raindrop::Graphics{
	Context::Context(Core::Context& core) :
		logger{"Graphics"},
		core{core},
		registry{core.registry["Graphics"]}
		// window{*this},
		// instance{*this},
		// device{*this},
		// formats{*this},
		// swapchain{*this},
		// graphics{*this},
		// present{*this},
		// transfert{*this},
		// // layouts{*this},
		// renderPasses{*this},
		// descriptorPool{*this}
		// // dummyTexture{*this},
		// // shaderCompiler{*this}{
	{}

	// Context::~Context(){
	// 	device.waitIdle();
	// }
}