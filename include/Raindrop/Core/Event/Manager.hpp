#ifndef __RAINDROP_CORE_EVENT_MANAGER_HPP__
#define __RAINDROP_CORE_EVENT_MANAGER_HPP__

#include "common.hpp"
#include "KeyEvents.hpp"
#include "MouseEvents.hpp"

namespace Raindrop::Core::Event{
	class Manager{
		public:
			Manager();
			~Manager();

			void subscribe(const std::string& name, Callback callback);
			void trigger(const std::string& name);

			KeyEvents& keyEvents();
			MouseEvents& mouseEvents();

			const KeyEvents& keyEvents() const;
			const MouseEvents& mouseEvents() const;

		private:
			std::unordered_map<std::string, std::list<Callback>> _nameToCallbacksMap;
			KeyEvents _keyEvents;
			MouseEvents _mouseEvents;
	};
}

#endif