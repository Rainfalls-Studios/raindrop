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

	void Manager::insertFactory(const std::type_info& typeInfo, std::shared_ptr<Factory>&& factory){
		if (_factories.count(typeInfo.hash_code()) == 1){
			_context->logger->warn("Overwriting factory linked with type {}", typeInfo.name());
		}
		_factories[typeInfo.hash_code()] = factory;
	}

	std::shared_ptr<Asset> Manager::loadOrGet(const std::type_info& typeInfo, const std::filesystem::path& path){
		auto it = _factories.find(typeInfo.hash_code());
		if (it == _factories.end()){
			_context->logger->warn("Could not find the factory corresponding to the data type {}. Could not load \"{}\"", typeInfo.name(), path.string());
			throw std::out_of_range("Could not find the factory");
		}

		auto factory = it->second.lock();
		if (!factory){
			_context->logger->warn("The factory corresponding to the data type \"{}\" has been destroy and is no longer valid. Could not load \"{}\"", typeInfo.name(), path.string());
			throw std::out_of_range("The corresponding factory has been destroyed and is no longer valid");
		}

		return factory->getOrCreate(path);
	}
}