#ifndef __RAINDROP_CORE_REGISTRY_NODE_HPP__
#define __RAINDROP_CORE_REGISTRY_NODE_HPP__

#include <Raindrop/Core/Registry/common.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Raindrop::Core::Registry{
	class Node{
		public:
			Node(const Node& node);
			~Node();

			bool same(const Node& other) const;
			bool different(const Node& other) const;

			inline bool operator==(const Node& other) const;
			inline bool operator!=(const Node& other) const;

			Node get(const std::string& path);
			const Node get(const std::string& path) const;

			inline Node operator[](const std::string& path);
			inline const Node operator[](const std::string& path) const;

			void remove(const std::string& path);
			std::string format(const std::string& str);

			template<typename T> T as(){
				return _impl->tree->get_value<T>();
			}

			template<typename T> void set(const T& value){
				data() = std::to_string(value);
			}

			std::string& data();
			const std::string& data() const;

			void sort();
			void revers();
			void clear();

			uint32_t size() const;
			bool empty() const;

			void load(const std::string& path);
			void save(const std::string& path);

		private:
			struct Impl{
				std::shared_ptr<boost::property_tree::ptree> tree;

				Impl(const std::shared_ptr<boost::property_tree::ptree>& tree) : tree{tree}{}
			};

			Context& _context;
			std::unique_ptr<Impl> _impl;

			Node(Context& context, Impl impl);
		
		protected:
			Node(Context& context);
	};

	#include <Raindrop/Core/Registry/inl/Node.inl>
}

#endif