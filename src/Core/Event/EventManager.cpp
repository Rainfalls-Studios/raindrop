#include <Raindrop/Core/Event/EventManager.hpp>
#include <Raindrop/Core/Event/Context.hpp>

namespace Raindrop::Core::Event{
	EventManager::EventManager(Core::Context& core){
		_context = std::make_unique<Context>(core);
		_context->logger.info("Intializing Event manager ...");

		_keyEvents = std::make_unique<KeyEvents>();	
		_mouseEvents = std::make_unique<MouseEvents>();

		_context->logger.info("Event manager initialized without any critical error");
	}
	
	EventManager::~EventManager() {
		_context->logger.info("Terminating Event manager...");

		_keyEvents.reset();
		_mouseEvents.reset();

		_context->logger.info("Event manager terminated without any critcal error");
		_context.reset();
	}

	void EventManager::subscribe(const std::string& name, Callback callback){
		_context->logger.trace("New subscriber to %s event", name.c_str());
		_nameToCallbacksMap[name].push_back(callback);
	}

	void EventManager::trigger(const std::string& name){
		_context->logger.debug("%s triggered", name.c_str());
		auto& subscribers = _nameToCallbacksMap[name];
		for (auto &callback : subscribers){
			try{
				callback();
			} catch (const std::exception &e){
				_context->logger.warn("Exception encountered when triggering %s", name.c_str());
			}
		}
	}
	
	KeyEvents& EventManager::keyEvents(){
		return *_keyEvents;
	}

	MouseEvents& EventManager::mouseEvents(){
		return *_mouseEvents;
	}
	
	const KeyEvents& EventManager::keyEvents() const{
		return *_keyEvents;
	}

	const MouseEvents& EventManager::mouseEvents() const{
		return *_mouseEvents;
	}
}