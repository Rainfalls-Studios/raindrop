#pragma once

#include "Raindrop/pch.pch"
#include "Manager.hpp"
#include "Inputs.hpp"

namespace Raindrop::Core{
	class Context;

	namespace Events{
		class Context{
			public:
				Context(Core::Context& core);
				~Context();

				Core::Context& getCore() noexcept;
				const Core::Context& getCore() const noexcept;

				std::shared_ptr<spdlog::logger>& getLogger() noexcept;
				const std::shared_ptr<spdlog::logger>& getLogger() const noexcept;
				

				Manager& getManager() noexcept;
				const Manager& getManager() const noexcept;

				Inputs& getInputs() noexcept;
				const Inputs& getInputs() const noexcept;

			private:
				Core::Context& _core;
				std::shared_ptr<spdlog::logger> _logger;

				Manager _manager;
				Inputs _inputs;
		};
	}
}