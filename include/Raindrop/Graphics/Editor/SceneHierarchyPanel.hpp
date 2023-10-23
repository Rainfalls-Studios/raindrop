#ifndef __RAINDROP_GRAPHICS_EDITOR_SCENE_HIERARCHY_PANEL_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_SCENE_HIERARCHY_PANEL_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>

namespace Raindrop::Graphics::Editor{
	class SceneHierarchyPanel{
		public:
			SceneHierarchyPanel(EditorContext& context);
			~SceneHierarchyPanel();

			void update();

		private:
			EditorContext& _context;
			ImGuiID _dockspace;

			char _renameBuffer[255];
			Core::Scene::Entity _renamingEntity;

			void drawScene(Core::Scene::Scene* scene);
			void drawEntity(Core::Scene::Entity entity);
			void entitySettings(Core::Scene::Entity entity);
			void drawSceneSettings(Core::Scene::Scene* scene);

			void dragDrop(Core::Scene::Entity entity);

			void selectEntity(Core::Scene::Entity entity);
			void addComponentEntity(Core::Scene::Entity entity);
			void addChildEntity(Core::Scene::Entity entity);
			void lookAtEntity(Core::Scene::Entity entity);
			void copyEntity(Core::Scene::Entity entity);
			void cutEntity(Core::Scene::Entity entity);
			void copyEntityID(Core::Scene::Entity entity);
			void renameEntity(Core::Scene::Entity entity);
			void removeEntity(Core::Scene::Entity entity);

			void addCube(Core::Scene::Scene* scene);
			void addCone(Core::Scene::Scene* scene);
			void addPlane(Core::Scene::Scene* scene);
			void addPyramide(Core::Scene::Scene* scene);
			void addModel(Core::Scene::Scene* scene);
			void addText(Core::Scene::Scene* scene);
			void add3DText(Core::Scene::Scene* scene);
			void addCamera(Core::Scene::Scene* scene);
			void addLightPoint(Core::Scene::Scene* scene);
			void addSpotLight(Core::Scene::Scene* scene);
			void addSun(Core::Scene::Scene* scene);

			Core::Scene::Entity createEntity(Core::Scene::Scene* scene);
	};
}

#endif