#pragma once

#include "Raindrop/pch.pch"

namespace Raindrop::Graphics{
	class Context;

	namespace Vulkan{	
		class Instance{
			public:
				Instance(Context& context, const std::vector<const char*> extensions);
				~Instance();

				Instance(const Instance&) = delete;
				Instance& operator=(const Instance&) = delete;

				const VkInstance& get() const noexcept;
				const vkb::Instance& getVkb() const noexcept;

			private:
				Context& _context;
				vkb::Instance _instance;
		};
	}
}