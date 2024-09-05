#include "Raindrop/Graphics/Vulkan/Surface.hpp"
#include "Raindrop/Graphics/Context.hpp"

namespace Raindrop::Graphics::Vulkan{
	Surface::Surface(Context& context, VkSurfaceKHR surface) noexcept :
		_context{context},
		_surface{surface}
	{}

	Surface::~Surface(){
		if (_surface != VK_NULL_HANDLE){
			vkDestroySurfaceKHR(_context.getInstance().get(), _surface, nullptr);
			_surface = VK_NULL_HANDLE;
		}
	}

	VkSurfaceKHR& Surface::get() noexcept{
		return _surface;
	}

	const VkSurfaceKHR& Surface::get() const noexcept{
		return _surface;
	}
}