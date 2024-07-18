#include <Raindrop/Events/Manager.hpp>

namespace Raindrop::Events{

	Manager::Manager() noexcept :
		_context{nullptr},
		_events{}
	{}

	Manager::~Manager(){
		release();
	}

	void Manager::prepare(Context& context){
		_context = &context;
	}

	void Manager::initialize(){

	}

	void Manager::release(){
		_events.clear();
	}
	
	void Manager::subscribe(const std::type_index& id, const CallbackInfo& info){
		_events[id].callbacks.push_back(info);
	}

	void Manager::unsubscribe(const std::type_index& id, const Listener* listener){
		auto& callbacks = _events[id].callbacks;

		callbacks.remove_if(
			[listener](const CallbackInfo& info){
				return info.listener == listener;
			}
		);
	}

	void Manager::trigger(const std::type_index& id, const Event& event){
		auto& callbacks = _events[id].callbacks;
		for (const auto& callback : callbacks){
			callback.callback(*callback.listener, event);
		}
	}
}