#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Events/Manager.hpp>
#include <Raindrop_internal/Assets/Manager.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Internal{
	Context::Context(Raindrop::Context& interface) : _interface{interface}{
		_logger = spdlog::stdout_color_mt("Raindrop");
		_eventManager = std::make_unique<Events::Manager>(*this);
		_assetManager = std::make_unique<Assets::Manager>(*this); 
		_renderer = std::make_unique<Graphics::Renderer>(*this);
	}

	Context::~Context(){
		_renderer.reset();
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

	Events::Manager& Context::getEventManager(){
		return *_eventManager;	
	}

	Assets::Manager& Context::getAssetManager(){
		return *_assetManager;
	}

	Graphics::Renderer& Context::getRenderer(){
		return *_renderer;
	}

	Raindrop::Context& Context::getInterface(){
		return _interface;
	}

	void Context::start(){
		_state = State::RUNNING;

		while (_state == State::RUNNING){
			_renderer->render();
			_renderer->events();
		};
	}

	void Context::stop(){
		_state = State::STOPPED;
	}

}