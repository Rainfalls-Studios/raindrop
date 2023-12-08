// #include <Raindrop/Graphics/Editor/Fonts.hpp>

// namespace Raindrop::Graphics::Editor{
// 	Fonts::Fonts(){
// 		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Editor.Fonts");
// 		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
// 		CLOG(INFO, "Engine.Graphics.Editor.Fonts") << "Editor fonts handler created";
// 	}

// 	Fonts::~Fonts(){
// 		CLOG(INFO, "Engine.Graphics.Editor.Fonts") << "Destroying editor fonts handler...";

// 		_nameToFont.clear();
// 		ImGui::GetIO().Fonts->Clear();

// 		CLOG(INFO, "Engine.Graphics.Editor.Fonts") << "Editor fonts handler destroyed with success !";
// 	}

// 	ImFont* Fonts::get(const std::string& name){
// 		return _nameToFont[name];
// 	}

// 	void Fonts::load(const std::string& name, float size, const std::filesystem::path& path){
// 		CLOG(INFO, "Engine.Graphics.Editor.Fonts") << "Loading new font : " << path << " size : (" << size << ") name : " << name;
// 		_nameToFont[name] = ImGui::GetIO().Fonts->AddFontFromFileTTF(path.string().c_str(), size);
// 	}
// }