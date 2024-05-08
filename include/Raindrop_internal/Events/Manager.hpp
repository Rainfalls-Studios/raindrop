#ifndef __RAINDROP_INTERNAL_EVENTS_MANAGER_HPP__
#define __RAINDROP_INTERNAL_EVENTS_MANAGER_HPP__

#include "common.hpp"
#include "../Templates/Registry.hpp"

namespace Raindrop::Internal::Events{
	class Manager{
		public:
			Manager(Context& context);
			~Manager();

			template<typename... Args>
			EventID registerEvent(const std::string& name){
				return registerEvent(sizeof...(Args), name);
			}

			template<typename... Args>
			void subscribe(const EventID& id, std::function<void(Args...)> callback){
				subscribe(id,
					[=](const std::vector<void*>& args){
						std::size_t i = args.size()-1;
						callback(*reinterpret_cast<Args*>(args[i--])...);
					});
			}

			template<typename F>
			void subscribe(const EventID& event, std::function<void()> callback){
				subscribe(event, [&callback](const std::vector<void*>&){callback();});
			}

			template<typename F>
			void subscribe(const EventID& event, F&& callback){
				subscribe(event, std::function(callback));
			}

			template<typename... Args>
			void trigger(const EventID& event, Args... args){
				std::vector<void*> arguments = {reinterpret_cast<void*>(&args)...};
				trigger(event, arguments);
			}

			EventID registerEvent(const std::size_t& argCount, const std::string& name);
			void unregisterEvent(const EventID& id);
			
			void subscribe(const EventID& id, std::function<void(const std::vector<void*>&)> callback);
			void trigger(const EventID& id, const std::vector<void*>& args);

			EventID getEventID(const std::string& name);

		private:
			Context& _context;

			struct EventInfo{
				std::list<std::function<void(const std::vector<void*>&)>> subscribers;
				std::size_t argumentCount;
				std::string name;
			};

			Templates::Registry<EventInfo> _eventInfoRegistry;
			std::unordered_map<std::string, EventID> _nameToEventID;
	};
}

#endif