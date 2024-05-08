#include <Raindrop/Event.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop_internal/Events/Manager.hpp>

#define EVENT_MANAGER context.getInternalContext()->getEventManager()

namespace Raindrop{
	Event::ID Event::_Register(Context& context, const std::size_t argCount, const std::string& name){
		return static_cast<ID>(EVENT_MANAGER.registerEvent(argCount, name));
	}

	void Event::_Subscribe(Context& context, const ID& event, const std::function<void(const std::vector<void*>&)>& callback){
		EVENT_MANAGER.subscribe(static_cast<Internal::Events::EventID>(event), callback);
	}

	void Event::_Trigger(Context& context, const ID& event, const std::vector<void*>& arguments){
		EVENT_MANAGER.trigger(static_cast<Internal::Events::EventID>(event), arguments);
	}

	void Event::Unregister(Context& context, const Event& event){
		EVENT_MANAGER.unregisterEvent(static_cast<Internal::Events::EventID>(event.getID()));
	}

	void Event::Subscribe(Context& context, const Event& event, std::function<void()> callback){
		EVENT_MANAGER.subscribe(event.getID(), [=](const std::vector<void*>&){callback();});
	}

	void Event::Subscribe(Context& context, const std::string& eventName, std::function<void()> callback){
		Subscribe(context, Get(context, eventName), callback);
	}

	Event Event::Get(Context& context, const std::string& name){
		return Event(context, static_cast<ID>(EVENT_MANAGER.getEventID(name)));
	}

	Event::Event(const Event& other) : _context{other._context}, _id{other._id}{}
	Event::Event(Context& context, const ID& id) : _context{&context}, _id{id}{}
	Event::Event() : _context{nullptr}, _id{INVALID_ID}{}

	Event::ID Event::getID() const{
		return _id;
	}

}