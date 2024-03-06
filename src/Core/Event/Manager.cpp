#include <Raindrop/Core/Event/Manager.hpp>
#include <Raindrop/Core/Event/KeyEvents.hpp>
#include <Raindrop/Core/Event/MouseEvents.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Core::Event{
	Manager::Manager(){
		spdlog::info("Constructing event manager ...");
	}
	
	Manager::~Manager(){
		spdlog::info("Destructing event manager ...");
	}

	void Manager::subscribe(const std::string& name, Callback callback){
		spdlog::trace("New subscriber to \"{}\" event", name);
		_nameToCallbacksMap[name].push_back(callback);
	}

	void Manager::trigger(const std::string& name){
		auto& subscribers = _nameToCallbacksMap[name];
		for (auto &callback : subscribers){
			try{
				callback();
			} catch (const std::exception &e){
				spdlog::warn("Exception encountered while triggering event \"{}\"", name);
			}
		}
	}
	
	KeyEvents& Manager::keyEvents(){
		return _keyEvents;
	}

	MouseEvents& Manager::mouseEvents(){
		return _mouseEvents;
	}
	
	const KeyEvents& Manager::keyEvents() const{
		return _keyEvents;
	}

	const MouseEvents& Manager::mouseEvents() const{
		return _mouseEvents;
	}
}