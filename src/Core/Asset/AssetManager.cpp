#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>
#include <Raindrop/Core/Asset/Context.hpp>

namespace Raindrop::Core::Asset{
	struct AssetManager::FactoryData{
		std::shared_ptr<AssetFactory> factory;
		std::size_t typeID;
	};

	AssetManager::AssetManager(Core::Context& core){
		_context = std::make_unique<Context>(core);

		_context->logger.info("Initializing Asset manager...");
		_context->logger.info("Asset manager initialized without any critical error");
	}

	AssetManager::~AssetManager(){
		_context->logger.info("Terminating Asset manager...");
		_factories.clear();
		_context->logger.info("Asset manager terminated without any critical error");
	}

	std::weak_ptr<Asset> AssetManager::loadOrGet(const std::filesystem::path& path){
		_context->logger.info("Requirering asset %s", path.string().c_str());

		auto it = _pathToAsset.find(path);
		if (it != _pathToAsset.end()){
			if (!it->second.expired()){
				_context->logger.trace("Succesfuly found asset file %s", path.string().c_str());
				return it->second;
			}
			_pathToAsset.erase(it);
		}

		AssetFactory* Factory = findFactory(path.extension());
		if (!Factory){
			_context->logger.error("Failed to find the asset factory corresponding to %s extension : %s", path.extension().string().c_str(), path.string().c_str());
			throw std::runtime_error("Failed to find asset factory");
		}

		_context->logger.info("Loading %s from asset factory...", path.string().c_str());

		std::shared_ptr<Asset> asset;
		try{
			asset = Factory->createAsset(path);
		} catch (const std::exception &e){
			_context->logger.error("Failed to load asset %s : %s", path.string().c_str(), e.what());
			throw std::runtime_error("Failed to load asset");
		}

		_context->logger.info("Asset %s loaded with success !", path.string().c_str());


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