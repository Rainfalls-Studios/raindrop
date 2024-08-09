#include <Raindrop/Assets/Manager.hpp>
#include <Raindrop/Assets/Context.hpp>
#include <Raindrop/Assets/Factory.hpp>

namespace Raindrop::Assets{
	Manager::Manager() noexcept :
		_context{nullptr},
		_factories{}
	{}

	Manager::~Manager(){
		release();
	}

	Manager& Manager::prepare(Context& context){
		_context = &context;
		return *this;
	}

	void Manager::initialize(){

	}
	
	void Manager::release(){
		_factories.clear();
		_context = nullptr;
	}

	void Manager::insertFactory(const std::size_t& typeID, std::unique_ptr<Factory>&& factory){
		if (_factories.count(typeID) == 1){
			_context->logger->warn("Overwriting factory linked with typeID {}", typeID);
		}
		_factories[typeID] = std::move(factory);
	}


	void Manager::removeFactory(const std::size_t& typeID){
		_factories.erase(typeID);
	}

	std::shared_ptr<Asset> Manager::loadOrGet(const std::size_t& typeID, const std::filesystem::path& path){
		auto it = _factories.find(typeID);
		if (it == _factories.end()){
			_context->logger->warn("Could not find the factory corresponding to the data type {}. Could not load \"{}\"", typeID, path.string());
			throw std::out_of_range("Could not find the factory");
		}

		auto& factory = it->second;
		return factory->getOrCreate(path);
	}
}