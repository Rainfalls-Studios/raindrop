#ifndef __RAINDROP_ENTITY_HPP__
#define __RAINDROP_ENTITY_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>

namespace Raindrop{
	class Entity{
		public:
			class Iterator{
				public:
					using iterator_category = std::forward_iterator_tag;
					using value_type = Core::Scene::Entity::Iterator;
					using difference_type = std::ptrdiff_t;
					using pointer = Core::Scene::Entity::Iterator*;
					using reference = Core::Scene::Entity::Iterator&;

					Iterator(value_type it);
				
					Iterator operator++();
					Entity operator*();
					Entity operator->();
					bool operator==(const Iterator& other);
					bool operator!=(const Iterator& other);

				private:
					value_type _it;
			};

			Entity(const Core::Scene::Entity& entity);
			Entity(const Entity& other);
			Entity();

			~Entity();

			template<typename T> bool hasComponent() const;
			template<typename T> T& getComponent();
			template<typename T> const T& getComponent() const;
			template<typename T> T& createComponent();
			template<typename T> void destroyComponent();

			bool operator==(const Entity& other) const;
			bool operator!=(const Entity& other) const;

			Entity& operator=(const Entity& other);
			Entity& operator=(const Core::Scene::Entity& entity);

			Entity createChild();

			Entity parent();
			const Entity parent() const;

			uint64_t UUID() const;

			std::string& tag();
			const std::string& tag() const;

			void destroy();
			
			Iterator begin();
			Iterator end();

		private:
			Core::Scene::Entity _entity;
	};

	#include <Raindrop/tpp/Entity.tpp>
}

#endif