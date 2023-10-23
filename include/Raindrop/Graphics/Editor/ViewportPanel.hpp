#ifndef __RAINDROP_GRAPHICS_EDITOR_VIEWPORT_PANEL_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_VIEWPORT_PANEL_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	class ViewportPanel{
		public:
			ViewportPanel(EditorContext& context);
			~ViewportPanel();

			void update();
			
			glm::u32vec2 start() const;
			glm::u32vec2 size() const;

		private:
			EditorContext& _context;

			glm::u32vec2 _start;
			glm::u32vec2 _size;

			void guizmo();

			int _currentGuizmoMode = 0;
			int _currentGuizmoOperation = 0;
			

	};
}

#endif