#ifndef __RAINDROP_INLINE_EVENT_INL_HPP__
#define __RAINDROP_INLINE_EVENT_INL_HPP__

#include <Raindrop/Event.hpp>

namespace Raindrop{
	template<typename F>
	void Event::Listener::subscribe(const std::string& eventName, const F& callback){
		_manager->getData(_manager->getID(eventName)).subscribe(static_cast<Listener*>(this), reinterpret_cast<void*>(callback));
	}

	template<typename F>
	void Event::Listener::subscribe(const Event::ID& eventID, const F& callback){
		_manager->getData(eventID).subscribe(static_cast<Listener*>(this), reinterpret_cast<void*>(callback));
	}

	template<typename F>
	void Event::Listener::subscribe(Event& event, const F& callback){
		event.getData().subscribe(static_cast<Listener*>(this), reinterpret_cast<void*>(callback));
	}

	template<typename... Args>
	Event Event::Manager::create(const std::string& name){
		Event event = create(name);
		event.getData().setArguments<Args...>();

		return event;
	}
	
	template<typename... Args>
	void Event::Manager::EventData::setArguments(){
		// Make sure the argument list if empty
		arguments.clear();

		// prepare enough memory
		arguments.reserve(sizeof...(Args));

		// store agruments info
		arguments.insert(
			arguments.begin(),
			{typeid(Args)...}
		);
	}

	template<typename... Args>
	void Event::trigger(Args&&... args){
		// Checks if given arguments are of valid types
		checkArgs<Args...>();
		
		// call each subscribers
		const auto& subscribers = getData().subscribers;

		for (const auto& s : subscribers){
			const auto fnc = reinterpret_cast<void(*)(void*, Args...)>(s.callback);
			fnc(s.listener, std::forward<Args>(args)...);
		}
	}

	template<typename F>
	void Event::subscribe(Listener* listener, const F& function){
		getData().subscribe(listener, reinterpret_cast<void*>(&function));
	}

	template<typename... Args>
	void Event::checkArgs() const{
		const auto& arguments = getData().arguments;

		if (sizeof...(Args) != arguments.size()){
			throw std::invalid_argument("Invalid argument count :: Expected " + std::to_string(arguments.size()) + " but recived " + std::to_string(sizeof...(Args)));
		}

		std::size_t i=0;

		([&]
		{
			const std::type_index current = typeid(Args);
			const std::type_index expected = arguments[i];

			// TODO : check castability

			if (current != expected){
				throw std::invalid_argument("Invalid argument type for paramater " + std::to_string(i) + ", expected type '" + demangle(expected) + "' but recived '" + demangle(current) + "'");
			}

			++i;
		} (), ...);
	}

	template<typename... Args>
	Event Event::Create(Context& context, const std::string& name){
		return _GetManager(context).create<Args...>(name);
	}

	template<typename F>
	void Event::Subscribe(Context& context, Event& event, Listener* listener, const F& function){
		event.subscribe(listener, function);
	}

	template<typename F>
	void Event::Subscribe(Context& context, const std::string& name, Listener* listener, const F& function){
		_GetManager(context).get(name).subscribe(listener, function);
	}

	template<typename F>
	void Event::Subscribe(Context& context, const ID& id, Listener* listener, const F& function){
		_GetManager(context).get(id).subscribe(listener, function);
	}

	template<typename... Args>
	void Event::Trigger(Context& context, Event& event, Args&&... args){
		event.trigger<Args...>(args...);
	}
	
	template<typename... Args>
	void Event::Trigger(Context& context, const std::string& name, Args&&... args){
		_GetManager(context).get(name).trigger<Args...>(args...);
	}

	template<typename... Args>
	void Event::Trigger(Context& context, const ID& id, Args&&... args){
		_GetManager(context).get(id).trigger<Args...>(args...);
	}
}

#endif