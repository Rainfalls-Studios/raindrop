#ifndef __RAINDROP_GRAPHICS_EDITOR_WORKSPACE_PANEL_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_WORKSPACE_PANEL_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	class WorkspacePanel{
		public:
			WorkspacePanel(EditorContext& context);
			~WorkspacePanel();

			void update();

		private:
			EditorContext& _context;

			std::unique_ptr<ViewportPanel> _viewport;
			std::unique_ptr<SceneEditorPanel> _sceneEditor;

	};
}

#endif