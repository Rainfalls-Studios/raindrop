#include <Raindrop/Core/Event/Manager.hpp>
#include <Raindrop/Core/Event/KeyEvents.hpp>
#include <Raindrop/Core/Event/MouseEvents.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Core::Event{
	Manager::Manager(){
		spdlog::info("Constructing event manager ...");
	}
	
	Manager::~Manager(){
		spdlog::info("Destroying event manager ...");
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