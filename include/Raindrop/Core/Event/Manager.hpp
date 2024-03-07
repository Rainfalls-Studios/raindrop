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
				subscribers[eventName].push_back([=](const std::vector<void*>& args) {
					subscriber();
				});
			}

			template<typename... Args>
			void subscribe(const std::string& eventName, std::function<void(Args...)> subscriber) {
				subscribers[eventName].push_back([=](const std::vector<void*>& args) {
					subscriber(*reinterpret_cast<Args*>(args[0])...);
				});
			}

			// Trigger an event
			template<typename... Args>
			void trigger(const std::string& eventName, Args... args) {
				auto it = subscribers.find(eventName);
				if (it != subscribers.end()) {
					std::vector<void*> arguments = {reinterpret_cast<void*>(&args)...};
					for (auto& subscriber : it->second) {
						subscriber(arguments);
					}
				}
			}

			KeyEvents& keyEvents();
			MouseEvents& mouseEvents();

			const KeyEvents& keyEvents() const;
			const MouseEvents& mouseEvents() const;

		private:
			std::unordered_map<std::string, std::vector<std::function<void(const std::vector<void*>&)>>> subscribers;
			KeyEvents _keyEvents;
			MouseEvents _mouseEvents;

			void triggerException(const std::string& event);
	};
}

#endif