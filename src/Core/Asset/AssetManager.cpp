#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop::Core::Asset{
	AssetManager::AssetManager(){
		el::Logger* customLogger = el::Loggers::getLogger("AssetManager");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
		CLOG(INFO, "AssetManager") << "New Asset Manager";
	}

	AssetManager::~AssetManager(){
		CLOG(INFO, "AssetManager") << "Destroying Asset Manager";
	}

	void AssetManager::destroyLink(const std::filesystem::path& extension){
		_extensionToFactory.erase(extension);
	}

	std::weak_ptr<Asset> AssetManager::loadOrGet(const std::filesystem::path& path){
		CLOG(INFO, "AssetManager") << "requirering file : " << path;

		auto it = _pathToAsset.find(path);
		if (it != _pathToAsset.end()){
			if (!it->second.expired()){
				CLOG(TRACE, "AssetManager") << "Succefuly found asset file : " << path;
				return it->second;
			}
			_pathToAsset.erase(it);
		}

		AssetFactory* Factory = findFactory(path.extension());
		if (!Factory){
			std::stringstream err;
			err << "Failed to find the asset Factory corresponding to " << path.extension() << " extension : " << path;
			throw std::runtime_error(err.str());
		}
		auto asset = Factory->createAsset(path);
		_pathToAsset[path] = asset;

		asset->_path = path;
		return asset;
	}

	AssetFactory* AssetManager::findFactory(const std::filesystem::path& extension){
		auto it = _extensionToFactory.find(extension);
		if (it == _extensionToFactory.end()){
			return nullptr;
		}

		if (auto Factory = it->second.lock()){
			return Factory.get();
		}

		_extensionToFactory.erase(extension);
		return nullptr;
	}

	void AssetManager::registerFactory(const std::shared_ptr<AssetFactory>& factory, std::size_t typeID){
		FactoryData data;
		data.factory = factory;
		data.typeID = typeID;

		_factories.push_back(data);
		auto exts = factory->extensions();

		for (const auto& e : exts){
			_extensionToFactory[e] = factory;
		}
	}

	void AssetManager::removeFactory(std::size_t typeID){
		auto it = std::find_if(
			_factories.begin(),
			_factories.end(),
			[typeID](const FactoryData& a){return a.typeID == typeID;}
		);

		if (it == _factories.end()) return;
		_factories.erase(it);
	}

}