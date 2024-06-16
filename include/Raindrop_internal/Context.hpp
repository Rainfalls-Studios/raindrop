#ifndef __RAINDROP_INTERNAL_CONTEXT_HPP__
#define __RAINDROP_INTERNAL_CONTEXT_HPP__

#include "common.hpp"
#include <Raindrop/Context.hpp>
#include "Events/common.hpp"
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

			std::shared_ptr<spdlog::logger>& getLogger() noexcept;
			const std::shared_ptr<spdlog::logger>& getLogger() const noexcept;

			Events::Manager& getEventManager();
			Assets::Manager& getAssetManager();
			Graphics::Engine& getEngine();

			Raindrop::Context& getInterface();
			
			void start();
			void stop();

		private:
			Raindrop::Context& _interface;

			std::shared_ptr<spdlog::logger> _logger;
			std::unique_ptr<Events::Manager> _eventManager;
			std::unique_ptr<Assets::Manager> _assetManager;
			std::unique_ptr<Graphics::Engine> _engine;

			enum class State{
				RUNNING,
				STOPPED
			};

			State _state;
	};
}

#endif