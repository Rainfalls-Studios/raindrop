#include <Raindrop/Core/Registry/Registry.hpp>
#include <Raindrop/Core/Registry/Context.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

namespace Raindrop::Core::Registry{
	Registry::Registry(Core::Context& core) :
		_context{std::make_unique<Context>(core)},
		Node(*_context){}
}