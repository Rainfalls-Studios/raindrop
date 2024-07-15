#ifndef __RAINDROP_GRAPHICS_CORE_INSTANCE_HPP__
#define __RAINDROP_GRAPHICS_CORE_INSTANCE_HPP__

#include "common.hpp"
#include <Raindrop/Graphics/pch.pch>
#include <VkBootstrap.h>

namespace Raindrop::Graphics::Core{
	class Instance{
		public:
			Instance() noexcept;
			~Instance();

			Instance(const Instance&) = delete;
			Instance& operator=(const Instance&) = delete;

			vkb::InstanceBuilder* operator->() noexcept;
			vkb::InstanceBuilder& operator*() noexcept;

			void prepare(Context& context);
			void initialize();
			void release();

			const VkInstance& get() const noexcept;
			const vkb::Instance& getVkb() const noexcept;

		private:
			Context* _context;
			vkb::Instance _instance;
			std::unique_ptr<vkb::InstanceBuilder> _builder;

	};
}

#endif