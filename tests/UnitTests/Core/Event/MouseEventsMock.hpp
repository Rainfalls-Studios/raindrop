#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_EVENT_MOUSE_EVENTS_HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_EVENT_MOUSE_EVENTS_HPP__

#include <common.hpp>
#include <Raindrop/Core/Event/MouseEvents.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Event{
	class MouseEventsMock : public ::Raindrop::Core::Event::MouseEvents{
		public:
			using ButtonState = ::Raindrop::Core::Event::ButtonState;
			using MouseButton = ::Raindrop::Core::Event::MouseButton;

			MouseEventsMock(::Raindrop::Core::Event::Context& context) : MouseEvents(context){}

			MOCK_METHOD(ButtonState&, state, (MouseButton));
			MOCK_METHOD(const ButtonState&, state, (MouseButton), (const));

			MOCK_METHOD(glm::vec2, pos, ());
			MOCK_METHOD(const glm::vec2&, pos, (), (const));

			MOCK_METHOD(glm::vec2, relPos, ());
			MOCK_METHOD(const glm::vec2&, relPos, (), (const));

			MOCK_METHOD(glm::vec2, scroll, ());
			MOCK_METHOD(const glm::vec2&, scroll, (), (const));
	};
}

#endif