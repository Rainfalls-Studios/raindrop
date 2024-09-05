#pragma once

#include "Raindrop/pch.pch"

#include "Window.hpp"

namespace Raindrop{
	namespace Core{class Context;}

	namespace Graphics{
		class Context{
			public:
				Context(Core::Context& core);
				~Context();

				std::shared_ptr<spdlog::logger>& getLogger() noexcept;
				const std::shared_ptr<spdlog::logger>& getLogger() const noexcept;

			private:
				Core::Context& _core;
				std::shared_ptr<spdlog::logger> _logger;

				Window _window;
		};
	}
}