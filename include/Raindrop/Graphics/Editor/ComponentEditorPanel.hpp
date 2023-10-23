#ifndef __RAINDROP_GRAPHICS_EDITOR_COMPONENT_EDITOR_PANEL_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_COMPONENT_EDITOR_PANEL_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>
#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Graphics::Editor{
	class ComponentEditorPanel{
		public:
			ComponentEditorPanel(EditorContext& context);
			~ComponentEditorPanel();

			void update(Core::Scene::Entity entity);

		private:
			EditorContext& _context;
	};
}

#endif