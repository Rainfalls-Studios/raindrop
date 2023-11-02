#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_EVENT_CONTEXT_HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_EVENT_CONTEXT_HPP__

#include <common.hpp>
#include <Raindrop/Core/Event/Context.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Event{
	class ContextMock : public ::Raindrop::Core::Event::Context{
		public:
			ContextMock(::Raindrop::Core::Context& context) : Context(context){};

			MOCK_METHOD(::Raindrop::Core::Context&, core, ());
	};
}

#endif