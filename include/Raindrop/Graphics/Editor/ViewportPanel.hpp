#ifndef __RAINDROP_GRAPHICS_EDITOR_VIEWPORT_PANEL_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_VIEWPORT_PANEL_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	class ViewportPanel{
		public:
			ViewportPanel(EditorContext& context);
			~ViewportPanel();

			void update();

		private:
			EditorContext& _context;

	};
}

#endif