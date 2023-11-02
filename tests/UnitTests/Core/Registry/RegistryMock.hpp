#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_REGISTRY_REGISTRY_MOCK_HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_REGISTRY_REGISTRY_MOCK_HPP__


#include <common.hpp>
#include <Core/Context.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::UnitTests::Core::Registry{
	class RegistryMock : public ::Raindrop::Core::Registry::Registry{
		public:
			using Node = ::Raindrop::Core::Registry::Node;

			RegistryMock(::Raindrop::Core::Context& context) : Registry(context){};

			MOCK_METHOD(bool, same, (const Node&), (const));
			MOCK_METHOD(bool, different, (const Node&), (const));

			MOCK_METHOD(Node, get, (const std::filesystem::path&));
			MOCK_METHOD(const Node, get, (const std::filesystem::path&), (const));

			MOCK_METHOD(void, remove, (const std::string&));
			MOCK_METHOD(std::string, format, (const std::string&));

			MOCK_METHOD(void, sort, ());
			MOCK_METHOD(void, revers, ());
			MOCK_METHOD(void, clear, ());

			MOCK_METHOD(uint32_t, size, (), (const));
			MOCK_METHOD(bool, empty, (), (const));

			MOCK_METHOD(void, load, (const std::filesystem::path&));
			MOCK_METHOD(void, save, (const std::filesystem::path&));
	};
}
#endif