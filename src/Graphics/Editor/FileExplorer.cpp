#include <Raindrop/Graphics/Editor/FileExplorer.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <imgui/imgui_internal.h>

namespace Raindrop::Graphics::Editor{
	FileExplorer::FileExplorer(EditorContext& context): _context{context}{

	}

	FileExplorer::~FileExplorer(){

	}

	void FileExplorer::update(){

		// ImGui::SetNextWindowDockID(ImGui::GetID("DockSpace"));
		if (ImGui::Begin("FileExplorer")){
			ImGui::Text("aaaaa");
		}
		ImGui::End();
	}
}