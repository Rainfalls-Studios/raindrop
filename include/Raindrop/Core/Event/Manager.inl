#ifndef __RAINDROP_CORE_EVENT_MANAGER_INL__
#define __RAINDROP_CORE_EVENT_MANAGER_INL__

#include "Manager.hpp"

namespace Raindrop::Core::Event{
	template<typename... Args>
	void Manager::subscribe(const std::string& eventName, std::function<void(Args...)> callback) {
		auto it = _nameToEvent.find(eventName);
		
		// Ensure the event is valid
		assert(it != _nameToEvent.end() && "Event not registred");
		assert(sizeof...(Args) == it->second.argCount && "Invalid argument count");

		// push the callback into the event's subscriber list
		it->second.subscribers.push_back(
			[=](const std::vector<void*>& args){

				// pushes the event's arguments
				std::size_t i = args.size()-1;
				callback(*reinterpret_cast<Args*>(args[i--])...);
			}
		);
	}

	template<typename F>
	void Manager::subscribe(const std::string& eventName, F&& callback){
		std::function func(callback);
		subscribe(eventName, func);
	}

	template<typename... Args>
	void Manager::trigger(const std::string& eventName, Args... args) {
		auto it = _nameToEvent.find(eventName);

		// Ensure the event is valid
		assert(it != _nameToEvent.end() && "Event not registred");
		assert(sizeof...(Args) == it->second.argCount && "Invalid argument count");

		// Stores arguments as an array
		std::vector<void*> arguments = {reinterpret_cast<void*>(&args)...};

		// Call subscribers
		for (auto& callback : it->second.subscribers){
			callback(arguments);
		}
	}

	template<typename... Args>
	void Manager::registerEvent(const std::string& eventName){
		auto it = _nameToEvent.find(eventName);

		// Ensure the event does not already exixts
		assert(it == _nameToEvent.end() && "Event already regsitred");
		
		_nameToEvent[eventName].argCount = sizeof...(Args);
	}


	inline KeyEvents& Manager::keyEvents(){
		return _keyEvents;
	}

	inline MouseEvents& Manager::mouseEvents(){
		return _mouseEvents;
	}
	
	inline const KeyEvents& Manager::keyEvents() const{
		return _keyEvents;
	}

	inline const MouseEvents& Manager::mouseEvents() const{
		return _mouseEvents;
	}
}

#endif