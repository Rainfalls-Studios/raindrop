#ifndef __RAINDROP_GRAPHICS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CONTEXT_HPP__

#include "types.hpp"
#include "Core/Context.hpp"
#include "Window/Context.hpp"
#include "Formats.hpp"
#include "CommandPool.hpp"
#include <Raindrop/Events/types.hpp>

namespace Raindrop::Graphics{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;

		Core::Context core;
		Window::Context window;
		Formats formats;

		CommandPool transfertCommandPool;

		void createLogger();
		void initialize(Events::Context& events);
		
		Core::Instance& getInstance() noexcept;
		const Core::Instance& getInstance() const noexcept;

		Core::PhysicalDevice& getPhysicalDevice() noexcept;
		const Core::PhysicalDevice& getPhysicalDevice() const noexcept;

		Core::Device& getDevice() noexcept;
		const Core::Device& getDevice() const noexcept;
	};
}

#endif