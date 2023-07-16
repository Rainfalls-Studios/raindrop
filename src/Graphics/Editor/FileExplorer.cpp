#include <Raindrop/Graphics/Editor/FileExplorer.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <Raindrop/Graphics/Editor/Icon.hpp>
#include <Raindrop/Graphics/Editor/Widgets.hpp>
#include <imgui/imgui_internal.h>

namespace Raindrop::Graphics::Editor{
	FileExplorer::FileExplorer(EditorContext& context): _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Editor.FileExplorer");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
		CLOG(INFO, "Engine.Graphics.Editor.FileExplorer") << "Initialized editor file explorer";
	}

	FileExplorer::~FileExplorer(){
		CLOG(INFO, "Engine.Graphics.Editor.FileExplorer") << "Editor file explorer destroyed";
	}

	void FileExplorer::update(){

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		bool begin = ImGui::Begin("FileExplorer");

		ImGui::PopStyleVar(3);

		if (begin){
			ImGuiID dockspace_id = ImGui::GetID("Dockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking);

			static auto first_time = true;
			if (first_time) {
				first_time = false;
				ImGui::DockBuilderRemoveNode(dockspace_id);
				ImGui::DockBuilderAddNode(dockspace_id);
				ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());

				auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.23f, nullptr, &dockspace_id);
				auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.76f, nullptr, &dockspace_id);
				ImGui::DockBuilderDockWindow("Hierachy", dock_id_left);
				ImGui::DockBuilderDockWindow("Content", dock_id_right);

				ImGui::DockBuilderFinish(dockspace_id);
			}

			hierachy();
			content();
		}
		ImGui::End();
	}

	void FileExplorer::hierachy(){
		if (ImGui::Begin("Hierachy")){
			ImGui::SetCursorPos(ImVec2(15, 5));
			
			ImGui::PushFont(_context.fonts.get("Inter SemiBold 17"));
			ImGui::Text("My New Project");
			ImGui::PopFont();

			ImGui::TreePush("Project");

			try{
				//TODO: add project folder - https://trello.com/c/JN4lyKvZ/7-add-project-folder
				for (auto entry : std::filesystem::directory_iterator(RAINDROP_EDITOR_DIR)) {
					if (entry.is_directory()) {
						renderFolder(entry.path());
					}
				}
			} catch (const std::exception& e){
				CLOG(WARNING, "Engine.Graphics.Editor.FileExplorer") << "ERROR : " << e.what();
			}


			ImGui::TreePop();
		}
		ImGui::End();
	}

	bool FileExplorer::renderFolder(const std::filesystem::path& path){
		Icon& folderIcon = _context.icons.get("Bold folder");
		// Icon& hardDriveIcon = _context.icons.get("Hard drive");

		std::string label = path.filename().string();

		ImGui::PushID(label.c_str());
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGuiStorage* storage = ImGui::GetStateStorage();

		bool* opened = storage->GetBoolRef(ImGui::GetID("opened"), 0);

		ImGui::BeginGroup();

		// the arrow button
		{
			bool hasSubdirectories = false;
			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				if (entry.is_directory()) {
					hasSubdirectories = true;
					break;
				}
			}

			if (hasSubdirectories){
				if (ImGui::ArrowButton("arrow button", *opened ? ImGuiDir_Down : ImGuiDir_Right)){
					*opened = !*opened;
				}
			} else {
				float size = ImGui::GetFrameHeight();
				ImGui::Dummy(ImVec2(size, size));
			}
			ImGui::SameLine();
		}

		float x = ImGui::GetCursorPosX();
		if (ImGui::InvisibleButton("select button", ImVec2(-1, ImGui::GetFontSize() * 1.2))){
			select(path / "");
		}

		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();

		if (hovered || active){
			ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImGui::GetColorU32(active ? rgb(133, 133, 133) : rgb(168, 168, 168)));
		}

		// Icon, text
		ImGui::SameLine();
		ImGui::SetCursorPosX(x + 5);
		float size = ImGui::GetFrameHeight();
		ImGui::Image(folderIcon.texture(), ImVec2(size, size), folderIcon.uv1(), folderIcon.uv2());
		ImGui::SameLine();

		if (opened){
			ImGui::Text(label.c_str());
		} else {
			ImGui::TextDisabled(label.c_str());
		}

		ImGui::EndGroup();

		if (*opened){
			ImGui::TreePush(label.c_str());
			for (auto entry : std::filesystem::directory_iterator(path)) {
				if (entry.is_directory()) {
					renderFolder(entry.path());
				}
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
			
		return opened != 0;
	}

	void FileExplorer::content(){
		if (ImGui::Begin("Content")){
			Icon& folderIcon = _context.icons.get("Folder");
			
			// top bar
			{
				bool disable = _lastSelections.empty();
				if (disable) ImGui::BeginDisabled();
				if (ImGui::ArrowButton("##undo", ImGuiDir_Left)){
					_nextSelections.push(_selectedDirectory);
					_selectedDirectory = _lastSelections.top();
					_lastSelections.pop();
				}
				if (disable) ImGui::EndDisabled();

				ImGui::SameLine();

				disable = _nextSelections.empty();
				if (disable) ImGui::BeginDisabled();
				if (ImGui::ArrowButton("##redo", ImGuiDir_Right)){
					_lastSelections.push(_selectedDirectory);
					_selectedDirectory = _nextSelections.top();
					_nextSelections.pop();
				}
				if (disable) ImGui::EndDisabled();

				ImGui::SameLine();

				disable = !_selectedDirectory.has_parent_path();
				if (disable) ImGui::BeginDisabled();
				if (ImGui::ArrowButton("##top", ImGuiDir_Up)){
					select((_selectedDirectory / "").parent_path());
				}
				if (disable) ImGui::EndDisabled();

				ImGui::SameLine();
				static char buff[255];
				if (ImGui::InputTextWithHint("##path", _selectedDirectory.string().c_str(), buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll)){
					std::filesystem::path path = buff;
					if (std::filesystem::exists(path)){
						std::filesystem::directory_entry entry(path);
						if (!entry.is_directory()){
							entry.replace_filename(path.parent_path());
						}
						_selectedDirectory = entry;
					}
					buff[0] = '\0';
				}
			}

			
			float cellSize = 160.f;
			float panelWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = static_cast<int>(panelWidth / cellSize);
			if (ImGui::BeginTable("##content", columnCount)){
				ImGui::TableNextColumn();
				try{
					if (std::filesystem::exists(_selectedDirectory)){
						for (auto entry : std::filesystem::directory_iterator(_selectedDirectory)) {
							std::string filename = entry.path().filename().string();
							if (entry.is_directory()){
								ImGui::ImageButton(folderIcon.texture(), ImVec2(150, 150), folderIcon.uv1(), folderIcon.uv2());

								// ImGui::Button(entry.path().filename().string().c_str(), ImVec2(150, 150));

								if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
									select(entry.path() / "");
									break;
								}

							} else if (entry.is_regular_file()){
								ImGui::Button(entry.path().filename().string().c_str(), ImVec2(150, 150));
							}

							
							float textWidth = ImGui::CalcTextSize(filename.c_str()).x;
							float ident = std::max(0.f, (150 / 2.f) - (textWidth / 2.f));

							ImGui::Indent(ident);
							ImGui::TextWrapped(entry.path().filename().string().c_str());
							ImGui::Unindent(ident);

							ImGui::TableNextColumn();
						}
					}
				} catch (const std::exception &){}
				ImGui::EndTable();
			}

		}
		ImGui::End();
	}

	void FileExplorer::select(const std::filesystem::path& dir){
		std::filesystem::directory_entry entry;
		if (!entry.is_directory()){
			entry.replace_filename(dir.parent_path());
		}
		
		if (_selectedDirectory != entry){
			_lastSelections.push(entry);
		}

		_selectedDirectory = entry;
	}
}