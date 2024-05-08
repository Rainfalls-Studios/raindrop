#ifndef __RAINDROP_INLINE_EVENT_INL__
#define __RAINDROP_INLINE_EVENT_INL__

#include "../Event.hpp"

namespace Raindrop{
	template<typename... Args>
	Event Event::Register(Context& context, const std::string& name){
		return Event(context, _Register(context, sizeof...(Args), name));
	}

	template<typename... Args>
	void Event::Subscribe(Context& context, const Event& event, std::function<void(Args...)> callback){
		_Subscribe(context, event.getID(),
			[=](const std::vector<void*>& args){
				std::size_t i = args.size()-1;
				callback(*reinterpret_cast<Args*>(args[i--])...);
			});
	}

	template<typename F>
	void Event::Subscribe(Context& context, const Event& event, F&& callback){
		Subscribe(context, event, std::function(callback));
	}

	template<typename... Args>
	void Event::Subscribe(Context& context, const std::string& eventName, std::function<void(Args...)> callback){
		Subscribe<Args...>(context, Get(context, eventName), callback);
	}

	template<typename F>
	void Event::Subscribe(Context& context, const std::string& eventName, F&& callback){
		Subscribe(context, Get(context, eventName), std::function(callback));
	}

	template<typename... Args>
	void Event::Trigger(Context& context, const Event& event, Args... args){
		_Trigger(context, event.getID(), {reinterpret_cast<void>(&args)...});
	}


	template<typename... Args>
	void Event::Trigger(Context& context, const std::string& eventName, Args... args){
		Trigger(context, Get(context, eventName), std::forward<Args>(args)...);
	}

	template<typename... Args>
	void Event::subscribe(std::function<void(Args...)> callback){
		Subscribe<Args...>(*_context, *this, callback);
	}

	template<typename F>
	void Event::subscribe(F&& callback){
		Subscribe(*_context, *this, callback);
	}

	template<typename... Args>
	void Event::trigger(Args... args){
		Trigger<Args...>(*_context, *this, std::forward<Args>(args)...);
	}
}

#endif
