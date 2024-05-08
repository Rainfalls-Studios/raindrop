#include <Raindrop_internal/Events/Manager.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop/Exceptions/EventsExceptions.hpp>

namespace Raindrop::Internal::Events{
	Manager::Manager(Context& context) : _context{context}{
		_context.getLogger()->info("Constructing event manager...");
	}

	Manager::~Manager(){
		_context.getLogger()->info("Destroying event manager...");
		_nameToEventID.clear();
	}

	EventID Manager::registerEvent(const std::size_t& argCount, const std::string& name){
		_context.getLogger()->trace("Registring event ... (\"{}\")", name);

		auto it = _nameToEventID.find(name);
		if (it != _nameToEventID.end()){
			_context.getLogger()->warn("The event name \"{}\" is already used, cannot overwrite events", name);
			throw Exceptions::EventNameAlreadyUsed(name);
		}


		EventInfo info{
			.argumentCount = argCount,
			.name = name
		};

		EventID id = _eventInfoRegistry.insert(info);
		_nameToEventID.insert({name, id});

		return id;
	}

	void Manager::unregisterEvent(const EventID& id){
		_context.getLogger()->trace("Unregistring event ... (\"{}\")", id);
		
		const EventInfo& info = _eventInfoRegistry.get(id);
		_nameToEventID.erase(info.name);
	}

	void Manager::subscribe(const EventID& id, std::function<void(const std::vector<void*>&)> callback){
		
		EventInfo& info = _eventInfoRegistry.get(id);
		info.subscribers.push_back(callback);
	}

	void Manager::trigger(const EventID& id, const std::vector<void*>& args){
		EventInfo& info = _eventInfoRegistry.get(id);
		
		for (const auto& subscriber : info.subscribers){
			subscriber(args);
		}
	}

	EventID Manager::getEventID(const std::string& name){
		try{
			return _nameToEventID.at(name);
		} catch (const std::out_of_range& e){
			_context.getLogger()->warn("Cannot get event named \"{}\" id", name);
			throw std::runtime_error("Event name not registred");
		}
	}
}