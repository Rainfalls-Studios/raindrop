#include "Raindrop/Core/Assets/Manager.hpp"
#include "Raindrop/Core/Assets/Context.hpp"
#include "Raindrop/Core/Assets/Factory.hpp"

namespace Raindrop::Core::Assets{
	Manager::Manager(Context& context) noexcept :
		_context{context},
		_factories{}
	{}

	Manager::~Manager(){}

	void Manager::insertFactory(const std::type_info& typeInfo, std::shared_ptr<Factory>&& factory){
		if (_factories.count(typeInfo.hash_code()) == 1){
			SPDLOG_LOGGER_WARN(_context.getLogger(), "Overwriting factory linked with type {}", typeInfo.name());
		}
		_factories[typeInfo.hash_code()] = factory;
	}

	std::shared_ptr<Asset> Manager::loadOrGet(const std::type_info& typeInfo, const std::filesystem::path& path){
		auto it = _factories.find(typeInfo.hash_code());
		if (it == _factories.end()){
			SPDLOG_LOGGER_WARN(_context.getLogger(), "Could not find the factory corresponding to the data type {}. Could not load \"{}\"", typeInfo.name(), path.string());
			throw std::out_of_range("Could not find the factory");
		}

		auto factory = it->second.lock();
		if (!factory){
			SPDLOG_LOGGER_WARN(_context.getLogger(), "The factory corresponding to the data type \"{}\" has been destroy and is no longer valid. Could not load \"{}\"", typeInfo.name(), path.string());
			throw std::out_of_range("The corresponding factory has been destroyed and is no longer valid");
		}

		return factory->getOrCreate(path);
	}
}