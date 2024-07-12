#ifndef __RAINDROP_INTERNAL_CONTEXT_HPP__
#define __RAINDROP_INTERNAL_CONTEXT_HPP__

#include "common.hpp"
#include <Raindrop/Context.hpp>
#include <Raindrop/Event.hpp>
#include "Assets/common.hpp"
#include "Graphics/common.hpp"

namespace Raindrop{
	VkBool32 toVulkan(const bool& b);
}

namespace Raindrop::Internal{
	class Context{
		public:
			Context(Raindrop::Context& interface);
			~Context();

			void initialize();

			std::shared_ptr<spdlog::logger>& getLogger() noexcept;
			const std::shared_ptr<spdlog::logger>& getLogger() const noexcept;

			Event::Manager& getEventManager() noexcept;
			Assets::Manager& getAssetManager() noexcept;
			Graphics::Engine& getEngine() noexcept;
			const Graphics::Engine& getEngine() const noexcept;

			Raindrop::Context& getInterface() noexcept;
			
			void start();
			void stop();

		private:
			Raindrop::Context& _interface;

			std::shared_ptr<spdlog::logger> _logger;
			std::unique_ptr<Event::Manager> _eventManager;
			std::unique_ptr<Assets::Manager> _assetManager;
			std::unique_ptr<Graphics::Engine> _engine;

			Event _onTick;

			enum class State{
				RUNNING,
				STOPPED
			};

			State _state;
	};
}

#endif