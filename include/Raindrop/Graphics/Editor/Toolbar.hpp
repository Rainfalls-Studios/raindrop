#ifndef __RAINDROP_GRAPHICS_EDITOR_TOOLBAR_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_TOOLBAR_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	class Toolbar{
		public:
			Toolbar(EditorContext& context);
			~Toolbar();

			void update();

			uint32_t guizmoMode;
			uint32_t guizmoOperation;
		
		private:
			EditorContext& _context;

			void modeButton();
			void operationButton();

	};
}

#endif