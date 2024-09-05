#pragma once

#include "Raindrop/pch.pch"

#include "Assets/Context.hpp"
#include "Events/Context.hpp"
#include "Scenes/Context.hpp"
#include "UUIDRegistry.hpp"

namespace Raindrop::Core{
	class Context{
		public:
			Context();
			~Context();

			std::shared_ptr<spdlog::logger>& getLogger() noexcept;
			const std::shared_ptr<spdlog::logger>& getLogger() const noexcept;

			Assets::Context& getAssets() noexcept;
			const Assets::Context& getAssets() const noexcept;

			Events::Context& getEvents() noexcept;
			const Events::Context& getEvents() const noexcept;

			Scenes::Context& getScenes() noexcept;
			const Scenes::Context& getScenes() const noexcept;

			UUIDRegistry& getRegistry() noexcept;
			const UUIDRegistry& getRegistry() const noexcept;

		private:
			std::shared_ptr<spdlog::logger> _logger;

			UUIDRegistry _registry;
			std::unique_ptr<Assets::Context> _assets;
			std::unique_ptr<Events::Context> _events;
			std::unique_ptr<Scenes::Context> _scenes;
			
	};
}