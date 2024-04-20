#include <Raindrop/Modules/Module.hpp>
#include <Raindrop/Exceptions/ResourceExceptions.hpp>
#include <Raindrop/Modules/ModuleInterface.hpp>

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

		struct Interface{
			using InitFnc_t = ModuleInterface*(*)(Context&);
			using DestroyFnc_t = void(*)(ModuleInterface*);

			InitFnc_t init = nullptr;
			DestroyFnc_t destroy = nullptr;
		} interface;

		struct Aliases{
			uint32_t lenght;
			const char** aliases;
 		} aliases;

		bool tryLoadInterface() noexcept{
			interface.init = reinterpret_cast<Interface::InitFnc_t>(getFunc("initializeInterface"));
			interface.destroy = reinterpret_cast<Interface::DestroyFnc_t>(getFunc("destroyInterface"));

			return interface.init && interface.destroy;
		}

		bool tryLoadAliases() noexcept{
			using GetAliasesFnc_t = const char**(*)(uint32_t*);
			GetAliasesFnc_t getAliases = reinterpret_cast<GetAliasesFnc_t>(getFunc("getAliases"));

			if (getAliases == nullptr){
				return false;
			}

			aliases.aliases = getAliases(&aliases.lenght);
			return true;
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
			spdlog::info("Non interface module loaded");
		}

		_module->tryLoadAliases();
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

	const char** Module::aliases(std::size_t& l) const{
		assert(_module != nullptr && "The module is invalid !");
		l = static_cast<std::size_t>(_module->aliases.lenght);
		return _module->aliases.aliases;
	}
}