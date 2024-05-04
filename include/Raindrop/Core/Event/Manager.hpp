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

			void subscribe(const std::string& eventName, std::function<void()> callback);

			template<typename... Args>
			void subscribe(const std::string& eventName, std::function<void(Args...)> callback);

			/**
			 * @brief Subscribes a callback to an event
			 * 
			 * @tparam F The function callbacks type
			 * @param eventName The name of the event to subscribe to
			 * @param callback The callback itself
			 */
			template<typename F>
			void subscribe(const std::string& eventName, F&& callback);

			/**
			 * @brief Triggers an event, all the callbacks linked to this event will be called
			 * 
			 * @tparam Args The event paramaters
			 * @param eventName The name of the event to trigger
			 * @param args The event arguments
			 */
			template<typename... Args>
			void trigger(const std::string& eventName, Args... args);

			/**
			 * @brief Registers an event, predefines the event arguments
			 * 
			 * @tparam Args The event's argument types
			 * @param eventName The event's name
			 */
			template<typename... Args>
			void registerEvent(const std::string& eventName);

			
			inline KeyEvents& keyEvents();
			inline const KeyEvents& keyEvents() const;

			inline MouseEvents& mouseEvents();
			inline const MouseEvents& mouseEvents() const;

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

#include "Manager.inl"

#endif