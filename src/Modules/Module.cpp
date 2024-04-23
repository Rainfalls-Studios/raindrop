#include <Raindrop/Modules/Module.hpp>
#include <Raindrop/Exceptions/ResourceExceptions.hpp>
#include <Raindrop/Modules/ModuleInterface.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystem.hpp>
#include <Raindrop/Graphics/RenderSystems/RenderSystemCollection.hpp>

#ifdef _WIN32
    #include <windows.h>
#else
	#include <dlfcn.h>
#endif

#include <spdlog/spdlog.h>

namespace Raindrop::Modules{
	struct Module::__Module__{
		#ifdef _WIN32
			HINSTANCE libraryHandle = nullptr;

			void open(const Path& path){
				libraryHandle = ::LoadLibrary(libraryPath);
				if (!libraryHandle) {
					std::cerr << "Failed to load the library" << std::endl;
					throw;
				}
			}

			void* getFunc(const char* fnc){
				return ::GetProcAddress(libraryHandle, fnc);
			}

			~__Module__(){
				if (libraryHandle != nullptr){
					::FreeLibrary(libraryHandle);
					libraryHandle = nullptr;
				}
			}

		#else
			void* libraryHandle = nullptr;

			void open(const Path& path){
				libraryHandle = ::dlopen(path.string().c_str(), RTLD_LAZY);
				if (libraryHandle == nullptr) {
					const char* error = ::dlerror();
					spdlog::error("Failed to load library : {}", error);
					throw Exceptions::ResourceLoadException(path, "Module", error);
				}
			}
			
			void* getFunc(const char* fnc){
				void* symbol = ::dlsym(libraryHandle, fnc);
				if (const char* e = ::dlerror(); e){
					spdlog::error("dlsym error : {}", e);
				}
				return symbol;
			}

			~__Module__(){
				if (libraryHandle != nullptr){
					::dlclose(libraryHandle);
					libraryHandle = nullptr;
				}
			}
		#endif

		// Interface

		struct Interface{
			using InitFnc_t = ModuleInterface*(*)(Context&);
			using DestroyFnc_t = void(*)(ModuleInterface*);

			InitFnc_t init = nullptr;
			DestroyFnc_t destroy = nullptr;
		} interface;

		bool tryLoadInterface() noexcept{
			interface.init = reinterpret_cast<Interface::InitFnc_t>(getFunc("initializeInterface"));
			interface.destroy = reinterpret_cast<Interface::DestroyFnc_t>(getFunc("destroyInterface"));

			return interface.init && interface.destroy;
		}

		// Aliases

		struct Aliases{
			std::vector<std::string> aliases = {};
 		} aliases;

		bool tryLoadAliases() noexcept{
			using GetAliasesFnc_t = std::vector<std::string>(*)();
			GetAliasesFnc_t getAliases = reinterpret_cast<GetAliasesFnc_t>(getFunc("getAliases"));

			if (getAliases == nullptr){
				return false;
			}

			aliases.aliases = getAliases();
			return true;
		}

		// Metadata

		struct Metadata{
			std::string name = "None";
			std::string description = "None";
			Version version = Version(0, 1, 0, 0);
		} metadata;

		void tryLoadMetadata() noexcept{
			using GetNameFnc_t = std::string(*)(void);
			using GetDescriptionFnc_t = std::string(*)(void);
			using GetVersionFnc_t = Version(*)(void);

			if (GetNameFnc_t getName = reinterpret_cast<GetNameFnc_t>(getFunc("getName")); getName){
				metadata.name = getName();
			}

			if (GetDescriptionFnc_t getDescription = reinterpret_cast<GetDescriptionFnc_t>(getFunc("getDescription")); getDescription){
				metadata.description = getDescription();
			}

			if (GetVersionFnc_t getVersion = reinterpret_cast<GetVersionFnc_t>(getFunc("getVersion")); getVersion){
				metadata.version = getVersion();
			}
		}
	};

	Module::Module(Context& context, const Path& path) :
			_module{nullptr},
			_interface{nullptr}
		{

		_module = new __Module__();
		_module->open(path);

		if (_module->tryLoadInterface()){
			_interface = _module->interface.init(context);
		} else {
			spdlog::info("The module does not contain an accessible interface");
		}

		if (_module->tryLoadAliases() == false){
			spdlog::info("The module does not contain aliases");
		}

		_module->tryLoadMetadata();
	}

	Module::~Module(){
		if (_module != nullptr){
			if (_interface != nullptr){
				_module->interface.destroy(_interface);
				_interface = nullptr;
			}

			delete _module;
			_module = nullptr;
		}
	}

	ModuleInterface* Module::interface() const{
		return _interface;
	}

	const std::string& Module::name() const{
		assert(_module != nullptr && "The module is invalid !");
		return _module->metadata.name;
	}

	const std::string& Module::description() const{
		assert(_module != nullptr && "The module is invalid !");
		return _module->metadata.description;
	}

	const Version& Module::version() const{
		assert(_module != nullptr && "The module is invalid !");
		return _module->metadata.version;
	}

	const std::vector<std::string>& Module::aliases() const{
		assert(_module != nullptr && "The module is invalid !");
		return _module->aliases.aliases;
	}

	const std::vector<RenderSystemID>& Module::renderSystemIDs() const{
		assert(_interface != nullptr && "The module interface is invalid !");
		return _interface->renderSystemIDs();
	}

	bool Module::hasRenderSystem(const RenderSystemID& id){
		assert(_interface != nullptr && "The module interface is invalid !");
		auto& systems = renderSystemIDs();
		for (const auto& system : systems){
			if (system == id) return true;
		}
		return false;
	}

	const std::vector<RenderSystemCollection>& Module::renderSystemCollections() const{
		assert(_interface != nullptr && "The module interface is invalid !");
		return _interface->renderSystemCollections();
	}

	const RenderSystemCollection* Module::getRenderSystemCollection(const std::string& name) const{
		assert(_interface != nullptr && "The module interface is invalid !");
		auto& collections = renderSystemCollections();
		for (const auto& collection : collections){
			if (collection.name() == name){
				return &collection;
			}
		}
		return nullptr;
	}
}