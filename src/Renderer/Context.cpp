#include <Raindrop/Renderer/Context.hpp>

namespace Raindrop::Renderer{
	Context::Context(::Raindrop::Context& core) :
		core{core},
		allocationCallbacks{nullptr},
		window{*this},
		instance(*this),
		device(*this),
		queues(*this),
		commandPools(*this),
		swapchain(*this),
		scene(*this),
		white(nullptr),
		materialSetlayout(*this)
	{
		{
			static constexpr uint32_t color = 0xFFFFFFFF;

			Texture::Texture::ConstructData data;
			data.width = 1;
			data.height = 1;
			data.channels = 4;
			data.data = &color;

			white = std::make_shared<Texture::Texture>(*this, data);
		}
	}
}