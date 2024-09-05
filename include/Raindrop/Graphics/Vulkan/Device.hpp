#pragma once

#include "Raindrop/pch.pch"

namespace Raindrop::Graphics{
	class Context;
	
	namespace Vulkan{
		class Device{
			public:
				struct QueueInfo{
					VkQueue queue;
					std::uint32_t familyIndex;
				};

				Device() noexcept;
				~Device();

				Device(const Device&) = delete;
				Device& operator=(const Device&) = delete;

				Device& prepare(Context& context);
				void initialize();
				void release();

				const VkDevice& get() const noexcept;
				const vkb::Device& getVkb() const noexcept;

				void waitIdle();

				uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

				QueueInfo graphicsQueue;
				QueueInfo presentQueue;
				QueueInfo transfetQueue;

			private:
				Context* _context;
				vkb::Device _device;
				std::unique_ptr<vkb::DeviceBuilder> _builder;

				void checkQueuePresence();
				void getQueues();
		};
	}
}