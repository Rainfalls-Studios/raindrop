#include "Raindrop/Core/Events/Manager.hpp"

namespace Raindrop::Core::Events{

	Manager::Manager(Context& context) noexcept :
		_context{context},
		_events{}
	{}

	Manager::~Manager(){}
	
	void Manager::subscribe(const std::type_index& id, const CallbackInfo& info){
		_events[id].callbacks.push_back(info);
	}

	void Manager::trigger(const std::type_index& id, const Event& event){
		auto& callbacks = _events[id].callbacks;
		for (const auto& callback : callbacks){
			callback.callback(event);
		}
	}
}