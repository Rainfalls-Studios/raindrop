#pragma once

#include "Raindrop/pch.pch"
#include "Scene.hpp"

namespace Raindrop::Core{
	class Context;
	namespace Scenes{
		class Context{
			public:
				Context(Core::Context& core);
				~Context();

				Core::Context& getCore() noexcept;
				const Core::Context& getCore() const noexcept;

				std::shared_ptr<spdlog::logger>& getLogger() noexcept;
				const std::shared_ptr<spdlog::logger>& getLogger() const noexcept;

			private:
				Core::Context& _core;	
				std::shared_ptr<spdlog::logger> _logger;

				void createLogger();
		};
	}
}