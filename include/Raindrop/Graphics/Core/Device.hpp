#ifndef __RAINDROP_GRAPHICS_CORE_DEVICE_HPP__
#define __RAINDROP_GRAPHICS_CORE_DEVICE_HPP__

#include "prototypes.hpp"
#include <Raindrop/Graphics/pch.pch>

namespace Raindrop::Graphics::Core{
	class Device{
		public:
			Device() noexcept;
			~Device();

			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;

			void requireExtension(const char* extension);
			void requireLayer(const char* layer);

			void initialize(Context& context);
			void release();

			const VkDevice& get() const noexcept;
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
			void waitIdle();

		private:
			Context* _context;
			VkDevice _device;

			std::vector<const char*> _requiredExtensions;
			std::vector<const char*> _requiredLayers;


			void findQueues();
	};
}

#endif