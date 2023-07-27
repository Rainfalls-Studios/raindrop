#ifndef __RAINDROP_GRAPHCIS_EDITOR_SCENE_EDITOR_PANEL_HPP__
#define __RAINDROP_GRAPHCIS_EDITOR_SCENE_EDITOR_PANEL_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	class SceneEditorPanel{
		public:
			SceneEditorPanel(EditorContext& context);
			~SceneEditorPanel();

			void update();

		private:
			EditorContext& _context;
			
			std::unique_ptr<SceneHierarchyPanel> _sceneHierarchy;
			std::unique_ptr<ComponentEditorPanel> _componentEditor;

	};
}

#endif