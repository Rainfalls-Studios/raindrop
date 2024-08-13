#include <Raindrop/Graphics/ShaderModuleFactory.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/ShaderModule.hpp>

#include <fstream>

namespace Raindrop::Graphics{
	std::string read_file(const std::filesystem::path& path){
		spdlog::trace("Loading file \"{}\" ...", path.string());
		std::ifstream file(path, std::ios_base::binary | std::ios_base::in);
		if (!file.is_open()){
			spdlog::warn("Could not open file \"{}\" : {}", path.string(), strerror(errno));
			throw std::runtime_error("Could not open file");
		}

		std::stringstream streamBuf;
		streamBuf << file.rdbuf();

		spdlog::trace("File \"{}\" loaded !", path.string());

		return streamBuf.str();
	}

	ShaderModuleFactory::ShaderModuleFactory(Context& context) : 
		_context{&context},
		_modules{}
	{}

	ShaderModuleFactory::~ShaderModuleFactory(){
		_modules.clear();
	}

	std::shared_ptr<Assets::Asset> ShaderModuleFactory::getOrCreate(const std::filesystem::path& path){
		{
			auto it = _modules.find(path);
			
			if (it != _modules.end()){
				return it->second;
			}
		}

		std::shared_ptr<ShaderModule> module = ShaderModule::create(*_context->raindrop);
		module->prepare(*_context);

		std::filesystem::path extension = path.extension();
		
		// TODO: insensitive case compare
		if (extension == ".spv"){
			module->setCode(loadSPVCode(path));
		} else {
			_context->logger->warn("Could not create shader module from \"{}\", the extensions is not recognized", path.string());
			throw std::runtime_error("Unknown extension");
		}

		_modules[path] = module;
		return std::static_pointer_cast<Assets::Asset>(module);
	}

	std::string ShaderModuleFactory::loadSPVCode(const std::filesystem::path& path){
		// Since the code is already in spv format, no need for compilation
		_context->logger->info("Reading file \"{}\"", path.string());

		try{
			return read_file(path);
		} catch (const std::exception& e){
			_context->logger->warn("Could not open spv shader file : {}", e.what());
			throw std::runtime_error("Could not open SPV shader file");
		}
	}
}