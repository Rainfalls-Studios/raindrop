#include <Raindrop/Modules/Loader.hpp>
#include <Raindrop/Modules/Module.hpp>
#include <spdlog/spdlog.h>

#ifdef _WIN32
	inline constexpr const char* LIB_EXTENSION = ".dll";
#elif defined(__linux__)
	inline constexpr const char* LIB_EXTENSION = ".so";
#elif defined(__APPLE__)
	inline constexpr const char* LIB_EXTENSION = ".dylib";
#else
    #error Unsupported platform
#endif

namespace Raindrop::Modules{
	Loader::Loader(Context &context) : _context{context}{
		spdlog::info("Constructing module loader ...");
	}

	Loader::~Loader(){
		spdlog::info("Destroying module loader ... ({} module(s))", _modules.size());
		_modules.clear();
	}

	std::shared_ptr<Loader::Asset> Loader::load(const Path& original_path){
		Path path = original_path;
		spdlog::info("Loading module from \"{}\"", path.string());

		if (!path.has_extension()){
			path += LIB_EXTENSION;
		} else {
			spdlog::warn("predefining module extension could lead to cross-platform issues");
		}
		
		std::shared_ptr<Module> module;
		try{
			module = std::make_shared<Module>(_context, path);
		} catch (const std::exception& e){
			spdlog::error("Failed to load module from \"{}\" :: reason : \"{}\"", path.string(), e.what());
			throw std::runtime_error("Failed to load module");
		}

		spdlog::info("Loaded module \"{}\"", path.string());
		_modules.push_back(module);
		loadAliases(module);

		return std::static_pointer_cast<Loader::Asset>(module);
	}

	void Loader::registerAsset(const Path& path, const std::shared_ptr<Asset>& asset){
		auto module = std::static_pointer_cast<Module>(asset);
		_modules.push_back(module);
	}

	void Loader::collectGarbage(){
		auto it = _modules.begin();
		std::size_t count = 0;
		while (it != _modules.end()){
			if (it->unique()){
				count++;
				_modules.erase(it);
				continue;
			}
			it++;
		}

		spdlog::trace("Cleared {} module assets from module loader garbage collection", count);
	}

	void Loader::loadAliases(const std::shared_ptr<Module>& module){
		std::size_t aliasCount;
		const char** const aliases = module->aliases(aliasCount);

		for (std::size_t i=0; i<aliasCount; i++){
			const auto& alias = aliases[i];
			
			auto it = _aliasToModule.find(alias);
			if (it != _aliasToModule.end()){
				// If it is expired, there is no need to worry
				if (!it->second.expired()){
					spdlog::warn("Possibly critical warning ! Overwriting \"{}\" module alias", alias);
				}
			}

			_aliasToModule[alias] = module;
		}
	}

	std::shared_ptr<Module> Loader::get(const std::string& alias){
		auto it = _aliasToModule.find(alias);
		if (it == _aliasToModule.end()){
			spdlog::warn("No module tagged \"{}\"", alias);
			return nullptr;	
		}

		return it->second.lock();
	}
}