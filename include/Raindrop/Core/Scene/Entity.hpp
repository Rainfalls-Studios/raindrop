#ifndef __RAINDROP_CORE_SCENE_ENTIY_HPP__
#define __RAINDROP_CORE_SCENE_ENTIY_HPP__

#include "common.hpp"

namespace Raindrop::Core::Scene{
	class Entity{
		public:
			Entity(Scene& scene, const EntityLUID& ID);
			Entity(const Entity& other);
			Entity(Scene& scene);
			~Entity();

			EntityLUID LUID() const;
			EntityGUID GUID() const;

		private:
			Scene& _scene;
			EntityLUID _ID;
	};
}

#endif