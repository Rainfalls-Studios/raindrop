#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_EVENT_KEY_EVENTS_HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_EVENT_KEY_EVENTS_HPP__

#include <common.hpp>
#include <Raindrop/Core/Event/KeyEvents.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Event{
	class KeyEventsMock : public ::Raindrop::Core::Event::KeyEvents{
		public:
			using KeyState = ::Raindrop::Core::Event::KeyState;
			using Key = ::Raindrop::Core::Event::Key;

			KeyEventsMock(::Raindrop::Core::Event::Context& context) : KeyEvents(context){}

			MOCK_METHOD(KeyState&, state, (Key));
			MOCK_METHOD(const KeyState&, state, (Key), (const));

			MOCK_METHOD(void, clear, ());
	};
}

#endif