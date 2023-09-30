#include <Raindrop/Graphics/Factory/ShaderFactory.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/Shader.hpp>
#include <boost/filesystem.hpp>

struct CaseInsensitiveHash {
	std::size_t operator()(const std::string& str) const {
		std::wstring lowercaseStr;
		lowercaseStr.reserve(str.length());
		for (wchar_t ch : str) {
			lowercaseStr.push_back(std::tolower(ch, std::locale()));
		}
		return std::hash<std::wstring>()(lowercaseStr);
	}
};

struct CaseInsensitiveEqual {
	bool operator()(const std::string& str1, const std::string& str2) const {
		return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(),
			[](char ch1, char ch2) {
				return std::tolower(ch1, std::locale()) == std::tolower(ch2, std::locale());
			}
		);
	}
};

namespace Raindrop::Graphics::Factory{
	static const std::unordered_map<std::string, VkShaderStageFlagBits, CaseInsensitiveHash, CaseInsensitiveEqual> extensionToStageMap = {
		{"vert", VK_SHADER_STAGE_VERTEX_BIT},
		{"vertex", VK_SHADER_STAGE_VERTEX_BIT},
		{"vs", VK_SHADER_STAGE_VERTEX_BIT},
		{"vertexshader", VK_SHADER_STAGE_VERTEX_BIT},
		{"vp", VK_SHADER_STAGE_VERTEX_BIT},
		{"frag", VK_SHADER_STAGE_FRAGMENT_BIT},
		{"fragment", VK_SHADER_STAGE_FRAGMENT_BIT},
		{"fs", VK_SHADER_STAGE_FRAGMENT_BIT},
		{"fragshader", VK_SHADER_STAGE_FRAGMENT_BIT},
		{"fp", VK_SHADER_STAGE_FRAGMENT_BIT},
		{"geom", VK_SHADER_STAGE_GEOMETRY_BIT},
		{"geometry", VK_SHADER_STAGE_GEOMETRY_BIT},
		{"gs", VK_SHADER_STAGE_GEOMETRY_BIT},
		{"geometryshader", VK_SHADER_STAGE_GEOMETRY_BIT},
		{"gp", VK_SHADER_STAGE_GEOMETRY_BIT}
	};

	static std::vector<char> readFile(const std::filesystem::path &filepath){
		std::ifstream file(filepath, std::ios::ate | std::ios::binary);

		if (!file.is_open()){
			std::stringstream err;
			err << "Failed to open : " << filepath << " shader file";
			throw std::runtime_error(err.str());
		}
		
		size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	ShaderFactory::ShaderFactory(GraphicsContext& context) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Shader");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
	}

	ShaderFactory::~ShaderFactory(){
		_shaders.clear();
	}

	std::shared_ptr<Core::Asset::Asset> ShaderFactory::createAsset(const std::filesystem::path& path){
		CLOG(INFO, "Engine.Graphics.Shader") << "Loading a new shader : " << path;

		std::shared_ptr<Shader> shader;
		std::filesystem::path out = compiledName(path);

		if (!isShaderCompiled(path)){
			_context.shaderCompiler.compile(path, out);
		}

		shader = loadSPV(out);

		_shaders.push_back(shader);
		return shader;
	}

	void ShaderFactory::destroyAsset(std::shared_ptr<Core::Asset::Asset> asset){
		std::shared_ptr<Shader> shader = std::static_pointer_cast<Shader>(asset);
		CLOG(INFO, "Engine.Graphics.Shader") << "Destroying shader";
		_shaders.remove(shader);
	}

	std::shared_ptr<Shader> ShaderFactory::loadSPV(const std::filesystem::path& path){
		std::vector<char> code = readFile(path);
		VkShaderStageFlagBits stage = getStage(path);
		CLOG(INFO, "Engine.Graphics.Shader") << "Loading " << string_VkShaderStageFlagBits(stage) << " shader from " << path;
		return std::make_unique<Shader>(_context, code, stage);
	}

	VkShaderStageFlagBits ShaderFactory::getStage(const std::filesystem::path& path){
		std::regex extensionRegex("\\.([a-zA-Z0-9]+)");
		std::string strPath = path.string();

		std::smatch extensionsMatch;
		auto searchStart = strPath.cbegin();

		while (std::regex_search(searchStart, strPath.cend(), extensionsMatch, extensionRegex)) {
			std::string extension = extensionsMatch[1].str();
			VkShaderStageFlagBits flagBits = getExtensionStage(extension);
			if (flagBits != 0) return flagBits;
			searchStart = extensionsMatch.suffix().first;
		}
		return static_cast<VkShaderStageFlagBits>(0);
	}

	bool ShaderFactory::isShaderCompiled(const std::filesystem::path& path){
		std::filesystem::path compiledPath = compiledName(path);
		if (!std::filesystem::exists(compiledPath)) return false;
		return boost::filesystem::last_write_time(path.string()) < boost::filesystem::last_write_time(compiledPath.string());
	}

	std::filesystem::path ShaderFactory::compiledName(std::filesystem::path path){
		std::filesystem::path relPath = std::filesystem::relative(path, _context.context.dataDirectory);
		relPath.replace_extension(relPath.extension().string() + ".spv");
		std::string strPath = relPath.string();
		
		std::replace(strPath.begin(), strPath.end(), '\\', '_');
   		std::replace(strPath.begin(), strPath.end(), '/', '_');

		size_t pos = strPath.find("__");
		while (pos != std::string::npos) {
			strPath.replace(pos, 2, "_");
			pos = strPath.find("__", pos + 1);
		}

		return _context.context.config.compiledShaderDirectory / std::filesystem::path(strPath);
	}

	VkShaderStageFlagBits ShaderFactory::getExtensionStage(const std::string extension){
		auto it = extensionToStageMap.find(extension);
		return it != extensionToStageMap.end() ? it->second : throw std::runtime_error("unknown shader extension");
	}

	std::vector<const char*> ShaderFactory::extensions() const{
		std::vector<const char*> ext;
		ext.push_back(".spv");

		for (const auto& it : extensionToStageMap){
			ext.push_back((std::string(".") + it.first).c_str());
		}

		return ext;
	}
}