#ifndef __RAINDROP_CORE_REGISTRY_NODE_HPP__
#define __RAINDROP_CORE_REGISTRY_NODE_HPP__

#include <Raindrop/Core/Registry/common.hpp>

namespace Raindrop::Core::Registry{
	class Node{
		public:
			Node(const Node& node);
			~Node();

			bool operator==(const Node& other) const;
			bool operator!=(const Node& other) const;

			Node get(const std::string& path);
			const Node get(const std::string& path) const;

			Node operator[](const std::string& path);
			const Node operator[](const std::string& path) const;

			void remove(const std::string& path);
			std::string format(const std::string& str);

			template<typename T> T& as() const;

			void sort();
			void revers();
			void clear();

			uint32_t size() const;
			bool empty() const;

			void load(const std::filesystem::path& path);
			void save(const std::filesystem::path& path);

		private:
			struct Impl;

			Context& _context;
			std::unique_ptr<Impl> _impl;

			Node(Context& context, Impl impl);
		
		protected:
			Node(Context& context);
	};
}

#endif