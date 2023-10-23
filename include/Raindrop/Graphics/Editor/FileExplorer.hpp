#ifndef __RAINDROP_GRAPHICS_EDITOR_FILE_EXPLORER_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_FILE_EXPLORER_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	class FileExplorer{
		public:
			FileExplorer(EditorContext& context);
			~FileExplorer();

			void update();

		private:
			EditorContext& _context;

			void hierachy();
			void content();

			bool renderFolder(const std::filesystem::path& path);

			std::filesystem::path _selectedDirectory;
			std::stack<std::filesystem::path> _lastSelections;
			std::stack<std::filesystem::path> _nextSelections;

			void select(const std::filesystem::path& dir);

	};
}

#endif