#ifndef __RAINDROP_GRAPHICS_EDITOR_DRAG_DROP_HANDLER_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_DRAG_DROP_HANDLER_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	static constexpr const char* FILE_PATH_TYPE = "FILE_PATH";
	static constexpr const char* MODEL_PATH_TYPE = "MODEL_PATH";
	static constexpr const char* IMAGE_PATH_TYPE = "IMAGE_PATH";
	static constexpr const char* FONT_PATH_TYPE = "FONT_PATH";


	class DragDropHandler{
		public:
			DragDropHandler(EditorContext& context);
			~DragDropHandler();

			void pathSource(const std::filesystem::path& path);
			
			bool pathFileTarget(std::filesystem::path& path, ImGuiDragDropFlags flags = 0);
			bool pathModelTarget(std::filesystem::path& path, ImGuiDragDropFlags flags = 0);
			bool pathImageTarget(std::filesystem::path& path, ImGuiDragDropFlags flags = 0);
			bool pathFontTarget(std::filesystem::path& path, ImGuiDragDropFlags flags = 0);
		
		private:
			EditorContext& _context;
	};
}

#endif