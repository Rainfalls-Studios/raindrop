#ifndef __RAINDROP_CORE_SCENE_ENTITY_HPP__
#define __RAINDROP_CORE_SCENE_ENTITY_HPP__

#include <Raindrop/Core/Scene/common.hpp>
#include <Raindrop/Core/Scene/Scene.hpp>

#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <Raindrop/Core/Scene/Components/Tag.hpp>
#include <Raindrop/Core/Scene/Components/Hierarchy.hpp>

namespace Raindrop::Core::Scene{
	class Entity{
		public:
			class Iterator{
				public:
					using iterator_category = std::forward_iterator_tag;
					using value_type = std::list<EntityID>::iterator;
					using difference_type = std::ptrdiff_t;
					using pointer = std::list<std::unique_ptr<Entity>>::iterator*;
					using reference = std::list<std::unique_ptr<Entity>>::iterator&;

					Iterator(value_type it, Scene* scene) : _it{it}, _scene{scene}{}
				
					Iterator operator++() {return Iterator(_it++, _scene);}
					Entity operator*() {return Entity(*_it, _scene);}
					bool operator==(const Iterator& other) {return _it == other._it;}
					bool operator!=(const Iterator& other) {return _it != other._it;}
					Entity operator->(){return Entity(*_it, _scene);}

				private:
					Scene* _scene;
					value_type _it;
			};

			Entity();
			Entity(EntityID id, Scene* scene);
			Entity(const Entity &other);
			~Entity();

			bool hasComponent(ComponentID component);
			void* getComponent(ComponentID component);
			void* createComponent(ComponentID component);
			void destroyComponent(ComponentID component);

			template<typename T> bool hasComponent();
			template<typename T> T& getComponent();
			template<typename T> const T& getComponent() const;
			template<typename T> T& createComponent();
			template<typename T> void destroyComponent();

			void reset();

			Components::Transform& transform();
			Components::Tag& tag();
			Components::Hierarchy& hierarchy();

			Iterator begin();
			Iterator end();

			EntityID id() const;
			Scene* scene() const;
			Entity createChild();

			operator EntityID() const;
			operator bool() const;

			bool operator==(const EntityID& other) const;
			bool operator==(const Entity& other) const;
			bool operator!=(const Entity& other) const;
			bool operator!=(const EntityID& other) const;

		private:
			EntityID _id;
			Scene* _scene;
	};

	#include <Raindrop/Core/Scene/tpp/Entity.tpp>
}

#endif