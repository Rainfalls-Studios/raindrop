#ifndef __RAINDROP_CORE_EVENT_MANAGER_HPP__
#define __RAINDROP_CORE_EVENT_MANAGER_HPP__

#include "common.hpp"
#include "KeyEvents.hpp"
#include "MouseEvents.hpp"

#include <functional>

namespace Raindrop::Core::Event{
	class Manager{
		public:
			template<typename... Args>
			using EventCallback = std::function<void(Args...)>;

			Manager();
			~Manager();

			void subscribe(const std::string& eventName, std::function<void()> subscriber) {
				auto it = _nameToEvent.find(eventName);
				assert(it != _nameToEvent.end() && "Event not registred");
				assert(it->second.argCount == 0 && "Invalud argument count");

				it->second.subscribers.push_back(
					[=](const std::vector<void*>& args){
						subscriber();
					}
				);
			}

			template<typename... Args>
			void subscribe(const std::string& eventName, std::function<void(Args...)> subscriber) {
				auto it = _nameToEvent.find(eventName);
				assert(it != _nameToEvent.end() && "Event not registred");
				assert(sizeof...(Args) == it->second.argCount && "Invalid argument count");

				it->second.subscribers.push_back(
					[=](const std::vector<void*>& args){
						std::size_t i = args.size()-1;
						subscriber(*reinterpret_cast<Args*>(args[i--])...);
					}
				);
			}

			template<typename F>
			void subscribe(const std::string& eventName, F&& callback){
				std::function func(callback);
				subscribe(eventName, func);
			}

			// Trigger an event
			template<typename... Args>
			void trigger(const std::string& eventName, Args... args) {
				auto it = _nameToEvent.find(eventName);
				assert(it != _nameToEvent.end() && "Event not registred");

				assert(sizeof...(Args) == it->second.argCount && "Invalid argument count");
				std::vector<void*> arguments = {reinterpret_cast<void*>(&args)...};

				for (auto& subscriber : it->second.subscribers){
					subscriber(arguments);
				}
			}

			template<typename... Args>
			void registerEvent(const std::string& eventName){
				auto it = _nameToEvent.find(eventName);
				assert(it == _nameToEvent.end() && "Event already regsitred");
				
				_nameToEvent[eventName].argCount = sizeof...(Args);
			}

			KeyEvents& keyEvents();
			MouseEvents& mouseEvents();

			const KeyEvents& keyEvents() const;
			const MouseEvents& mouseEvents() const;

		private:
			struct EventInfo{
				std::list<std::function<void(const std::vector<void*>&)>> subscribers;
				std::size_t argCount;
			};

			std::unordered_map<std::string, EventInfo> _nameToEvent;
			KeyEvents _keyEvents;
			MouseEvents _mouseEvents;
	};
}

#endif