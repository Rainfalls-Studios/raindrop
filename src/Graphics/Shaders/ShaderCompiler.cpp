#include <Raindrop/Graphics/Shaders/ShaderCompiler.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <shaderc/shaderc.hpp>

namespace Raindrop::Graphics::Shaders{
	static std::string readFile(const std::filesystem::path &filepath){
		std::ifstream file(filepath, std::ios::ate | std::ios::binary);

		if (!file.is_open()){
			std::stringstream err;
			err << "Failed to open : " << filepath << " shader file";
			throw std::runtime_error(err.str());
		}
		
		size_t fileSize = file.tellg();
		std::string buffer;
		buffer.resize(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	struct __ShaderCompileImpl{
		shaderc::Compiler compiler;
	};

	ShaderCompiler::ShaderCompiler(GraphicsContext& context) : _context{context}{
		_impl = new __ShaderCompileImpl();
	}

	ShaderCompiler::~ShaderCompiler(){
		delete _impl;
	}

	std::vector<uint32_t> ShaderCompiler::compile(const std::string& source, const char* name){
		auto& compiler = _impl->compiler;
		shaderc::CompileOptions options;

		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

		auto result = compiler.CompileGlslToSpv(source, shaderc_glsl_infer_from_source, name, options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success){
			std::cerr << result.GetErrorMessage() << std::endl;
		}

		return std::vector<uint32_t>(result.begin(), result.end());
	}

	void ShaderCompiler::compile(const std::filesystem::path& source, const std::filesystem::path& destination){
		std::string code = readFile(source);
		std::vector<uint32_t> output = compile(code, source.string().c_str());

		std::basic_ofstream<uint32_t> out(destination);
		out.write(output.data(), output.size());
		out.close();
	}
}