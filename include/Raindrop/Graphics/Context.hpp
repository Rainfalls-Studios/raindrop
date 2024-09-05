#pragma once

#include "Raindrop/pch.pch"

#include "Window.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/Surface.hpp"

namespace Raindrop{
	namespace Core{class Context;}

	namespace Graphics{
		class Context{
			public:
				Context(Core::Context& core);
				~Context();

				std::shared_ptr<spdlog::logger>& getLogger() noexcept;
				const std::shared_ptr<spdlog::logger>& getLogger() const noexcept;

				Window& getWindow() noexcept;
				const Window& getWindow() const noexcept;

				Vulkan::Instance& getInstance() noexcept;
				const Vulkan::Instance& getInstance() const noexcept;

				Vulkan::Surface& getSurface() noexcept;
				const Vulkan::Surface& getSurface() const noexcept;

			private:
				Core::Context& _core;
				std::shared_ptr<spdlog::logger> _logger;

				Window _window;
				Vulkan::Instance _vkInstance;
				Vulkan::Surface _vkSurface;
		};
	}
}