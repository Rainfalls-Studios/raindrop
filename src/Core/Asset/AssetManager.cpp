#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>
#include <Raindrop/Core/Asset/Context.hpp>

namespace Raindrop::Core::Asset{
	AssetManager::AssetManager(Core::Context& core){
		_context = std::make_unique<Context>(core);

		_context->logger().info("Initializing Asset manager...");
		_context->logger().info("Asset manager initialized without any critical error");
	}

	AssetManager::~AssetManager(){
		_context->logger().info("Terminating Asset manager...");
		_context->logger().info("Asset manager terminated without any critical error");
	}

	std::shared_ptr<Asset> AssetManager::loadOrGet(const std::filesystem::path& path){
		_context->logger().info("Requirering asset \"{}\"", path.string());

		auto it = _pathToAsset.find(path);
		if (it != _pathToAsset.end()){
			if (!it->second.expired()){
				_context->logger().trace("Succesfuly found asset \"{}\"", path.string().c_str());
				return it->second.lock();
			}
			_pathToAsset.erase(it);
		}

		AssetFactory* Factory = findFactory(path.extension());
		if (!Factory){
			_context->logger().error("Failed to find the asset factory corresponding to \"{}\" extension : \"{}\"", path.extension().string(), path.string());
			throw std::runtime_error("Failed to find asset factory");
		}

		_context->logger().trace("Loading \"{}\" from asset factory...", path.string());

		std::shared_ptr<Asset> asset;
		try{
			asset = Factory->createAsset(path);
		} catch (const std::exception &e){
			_context->logger().error("Failed to load asset \"{}\" : {}", path.string(), e.what());
			throw std::runtime_error("Failed to load asset");
		}

		_context->logger().info("Asset \"{}\" loaded with success !", path.string());


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

	void AssetManager::registerFactory(const std::shared_ptr<AssetFactory>& factory){
		auto exts = factory->extensions();

		for (const auto& e : exts){
			_extensionToFactory[e] = factory;
		}
	}

	void AssetManager::removeFactory(const std::shared_ptr<AssetFactory>& factory){

		std::erase_if(
			_extensionToFactory,
			[&](const auto& pair){
				return pair.second.lock() == factory;
			}
		);

		for (const auto& pair : _pathToAsset){
			const auto& asset = pair.second;

			if (factory->has(asset.lock())){
				_pathToAsset.erase(pair.first);
			}
		}
	}
}