#ifndef __RAINDROP_EVENTS_MANAGER_HPP__
#define __RAINDROP_EVENTS_MANAGER_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Events{
	class Manager{
		public:
			struct CallbackInfo{
				std::function<bool(const Event&)> callback;
			};

			struct EventData{
				std::list<CallbackInfo> callbacks;
			};

			Manager() noexcept;
			~Manager();

			void prepare(Context& context);
			void initialize();
			void release();
			
			void subscribe(const std::type_index& id, const CallbackInfo& info);
			// void unsubscribe(const std::type_index& id, const Listener* listener);

			void trigger(const std::type_index& id, const Event& event);


			template<typename EventType>
			void subscribe(std::function<bool(const EventType&)> callback){
				static_assert(std::is_base_of<Event, EventType>::value, "The event type has to be derived from Raindrop::Events::Event");

				subscribe(
					typeid(EventType),
					CallbackInfo(
						[callback](const Event& event) -> bool {
							return callback(static_cast<const EventType&>(event));
						}
				));
			}

			template<typename EventType, typename ClassType>
			void subscribe(ClassType& instance, bool (ClassType::*memberFunction)(const EventType&)) {
				subscribe<EventType>([&instance, memberFunction](const EventType& event) -> bool {
					return (instance.*memberFunction)(event);
				});
			}

			template<typename EventType, typename ClassType>
			void subscribe(ClassType* instance, bool (ClassType::*memberFunction)(const EventType&)) {
				subscribe<EventType>([instance, memberFunction](const EventType& event) -> bool {
					return (instance->*memberFunction)(event);
				});
			}

			template<typename T>
			void trigger(const T& event){
				static_assert(std::is_base_of<Event, T>::value, "The event type has to be derived from Raindrop::Events::Event");
				trigger(typeid(T), static_cast<const Event&>(event));
			}
		
		private:
			Context* _context;
			std::unordered_map<std::type_index, EventData> _events;
	};
}

#endif