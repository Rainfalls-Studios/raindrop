#ifndef __RAINDROP_CORE_REGISTRY_NODE_HPP__
#define __RAINDROP_CORE_REGISTRY_NODE_HPP__

#include <Raindrop/Core/Registry/common.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Raindrop::Core::Registry{
	class Node{
		public:
			Node(Registry& registry, boost::property_tree::ptree &tree, const std::string& path);
			~Node();

			const std::string& path() const;

			template<typename T>
			T get(const std::string& path) const;

		private:
			Registry& _registry;
			boost::property_tree::ptree &_tree;
			std::string _path;

	};
}

#endif