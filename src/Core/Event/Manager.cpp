#include <Raindrop/Core/Event/Manager.hpp>
#include <Raindrop/Core/Event/KeyEvents.hpp>
#include <Raindrop/Core/Event/MouseEvents.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Core::Event{
	Manager::Manager(){
		spdlog::info("Constructing event manager ...");
	}
	
	Manager::~Manager(){
		spdlog::info("Destroying event manager ... ({} event(s))", _nameToEvent.size());
	}

	void Manager::subscribe(const std::string& eventName, std::function<void()> callback) {
		auto it = _nameToEvent.find(eventName);
		assert(it != _nameToEvent.end() && "Event not registred");
		assert(it->second.argCount == 0 && "Invalid argument count");

		it->second.subscribers.push_back(
			[=](const std::vector<void*>& args){
				callback();
			}
		);
	}
}