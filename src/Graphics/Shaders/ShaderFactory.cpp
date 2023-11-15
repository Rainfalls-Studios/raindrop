#include <Raindrop/Graphics/Shaders/ShaderFactory.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>
#include <Raindrop/Graphics/Shaders/Shader.hpp>

namespace Raindrop::Graphics::Shaders{
	ShaderFactory::ShaderFactory(Context& context) : _context{context}{}
	ShaderFactory::~ShaderFactory(){}

	static std::vector<char> readFile(const std::filesystem::path& filePath) {
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file");
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	VkShaderStageFlagBits findStage(const std::string& filename){
		if (filename.find("frag") != std::string::npos) return VK_SHADER_STAGE_FRAGMENT_BIT;
		if (filename.find("vert") != std::string::npos) return VK_SHADER_STAGE_VERTEX_BIT;
		throw std::runtime_error("Unknown shader stage");
	}

	std::shared_ptr<Core::Asset::Asset> ShaderFactory::createAsset(const std::filesystem::path& path){
		// std::shared_ptr<Shader>

		if (path.extension() != ".spv"){
			_context.logger().error("Only the compiled .spv shaders are supported");
			throw std::runtime_error("Only the compiled .spv shaders are supported");
		}

		VkShaderStageFlagBits stage = findStage(path.filename());
		std::vector<char> content = readFile(path);

		std::shared_ptr<Shader> shader = std::make_unique<Shader>(_context, content, stage);
		_shaders.push_back(shader);
		return shader;
	}

	void ShaderFactory::destroyAsset(std::shared_ptr<Core::Asset::Asset> asset){
		_shaders.remove(std::static_pointer_cast<Shader>(asset));
	}

	std::vector<const char*> ShaderFactory::extensions() const{
		return {
			".spv"
		};
	}

	bool ShaderFactory::has(const std::shared_ptr<Core::Asset::Asset>& asset) const{
		return std::find(_shaders.begin(), _shaders.end(), asset) != _shaders.end();
	}
}