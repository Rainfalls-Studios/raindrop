#include <Raindrop/Graphics/Editor/DragDropHandler.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>

namespace Raindrop::Graphics::Editor{


	DragDropHandler::DragDropHandler(EditorContext& context) : _context{context}{}
	DragDropHandler::~DragDropHandler(){}

	enum FileType{
		FILE,
		MODEL,
		FONT,
		IMAGE,
		SCENE,
	};

	static FileType pathToType(const std::filesystem::path& path){
		static std::unordered_map<std::filesystem::path, FileType> map = {
			{".obj", MODEL},

			{".ttf", FONT},

			{".png", IMAGE},
			{".jpg", IMAGE},
			{".jpeg", IMAGE},
			{".bmp", IMAGE},
		};

		auto extension = path.extension();
		auto it = map.find(extension);

		if (it == map.end()){
			return FILE;
		}

		return it->second;
	} 

	void DragDropHandler::pathSource(const std::filesystem::path& path){
		if (!ImGui::BeginDragDropSource()) return;

		FileType type = pathToType(path);
		std::string payloadName = FILE_PATH_TYPE;
		std::string strPath = path.string();

		switch (type){
			case MODEL: payloadName = MODEL_PATH_TYPE; break;
			case IMAGE: payloadName = IMAGE_PATH_TYPE; break;
			case FONT: payloadName = FONT_PATH_TYPE; break;
		}

		ImGui::SetDragDropPayload(payloadName.c_str(), strPath.c_str(), strPath.size());

		ImGui::EndDragDropSource();
	}

	bool getDragDrop(const char* type, std::filesystem::path& path, ImGuiDragDropFlags flags){
		if (!ImGui::BeginDragDropTarget()) return false;
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type, flags);
		
		if (payload){
			path = static_cast<char*>(payload->Data);
		}
		
		ImGui::EndDragDropTarget();
		return payload;
	}

	bool DragDropHandler::pathFileTarget(std::filesystem::path& path, ImGuiDragDropFlags flags){
		return getDragDrop(FILE_PATH_TYPE, path, flags);
	}

	bool DragDropHandler::pathModelTarget(std::filesystem::path& path, ImGuiDragDropFlags flags){
		return getDragDrop(MODEL_PATH_TYPE, path, flags);
	}

	bool DragDropHandler::pathImageTarget(std::filesystem::path& path, ImGuiDragDropFlags flags){
		return getDragDrop(IMAGE_PATH_TYPE, path, flags);
	}

	bool DragDropHandler::pathFontTarget(std::filesystem::path& path, ImGuiDragDropFlags flags){
		return getDragDrop(FONT_PATH_TYPE, path, flags);
	}
}