#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_EVENT_EVENT_MANAGER_HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_EVENT_EVENT_MANAGER_HPP__

#include <common.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Event{
	class EventManagerMock : public ::Raindrop::Core::Event::EventManager{
		public:
			using Callback = ::Raindrop::Core::Event::Callback;
			EventManagerMock(::Raindrop::Core::Context& context) : EventManager(context){}

			MOCK_METHOD(void, subscribe, (const std::string&, Callback));
			MOCK_METHOD(void, trigger, (const std::string&));

			MOCK_METHOD(::Raindrop::Core::Event::KeyEvents&, keyEvents, ());
			MOCK_METHOD(::Raindrop::Core::Event::MouseEvents&, mouseEvents, ());

			MOCK_METHOD(const ::Raindrop::Core::Event::KeyEvents&, keyEvents, (), (const));
			MOCK_METHOD(const ::Raindrop::Core::Event::MouseEvents&, mouseEvents, (), (const));

			
	};
}

#endif