#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Events/Manager.hpp>
#include <Raindrop_internal/Assets/Manager.hpp>
#include <Raindrop_internal/Graphics/Engine.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

#include <chrono>

namespace Raindrop::Internal{
	Context::Context(Raindrop::Context& interface) : _interface{interface}{
		_logger = spdlog::stdout_color_mt("Raindrop");
		_eventManager = std::make_unique<Events::Manager>(*this);
		_assetManager = std::make_unique<Assets::Manager>(*this); 
		_engine = std::make_unique<Graphics::Engine>(*this);

		_onTick = _eventManager->registerEvent("OnTick");
	}

	Context::~Context(){
		_engine.reset();
		_assetManager.reset();
		_eventManager.reset();
		_logger.reset();
	}

	std::shared_ptr<spdlog::logger>& Context::getLogger() noexcept{
		return _logger;
	}

	const std::shared_ptr<spdlog::logger>& Context::getLogger() const noexcept{
		return _logger;
	}

	Events::Manager& Context::getEventManager() noexcept{
		return *_eventManager;	
	}

	Assets::Manager& Context::getAssetManager() noexcept{
		return *_assetManager;
	}

	Graphics::Engine& Context::getEngine() noexcept{
		return *_engine;
	}

	const Graphics::Engine& Context::getEngine() const noexcept{
		return *_engine;
	}

	Raindrop::Context& Context::getInterface() noexcept{
		return _interface;
	}

	void Context::start(){
		_state = State::RUNNING;

		std::uint32_t frameCount = 0;
		auto t = std::chrono::steady_clock::duration();

		while (_state == State::RUNNING){
			auto begin = std::chrono::steady_clock::now();

			this->_eventManager->trigger(_onTick);

			_engine->render();
			_engine->events();

			frameCount++;

			if (t > std::chrono::milliseconds(500)){
				t = std::chrono::milliseconds(0);
				spdlog::info("fps : {}", frameCount * 2);
				frameCount = 0;
			}

			auto end = std::chrono::steady_clock::now();
			t += (end - begin);
		};
	}

	void Context::stop(){
		_state = State::STOPPED;
	}

}