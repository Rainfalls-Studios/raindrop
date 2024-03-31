#include <Raindrop/Graphics/Materials/Registry.hpp>
#include <Raindrop/Graphics/Materials/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Materials{

	Registry::MaterialPair::MaterialPair(Context& context) :
		material(context), 
		set{VK_NULL_HANDLE}
	{}

	Registry::MaterialPair::MaterialPair(const Material& material, VkDescriptorSet set) : 
		material{material},
		set{set}
	{}

	Registry::Registry(Context& context) : _context{context}{
		_handles.resize(_context.instanceCount, MaterialPair(context.renderer));
		for (std::size_t i=0; i<_context.instanceCount; i++){
			_handles[i].set = VK_NULL_HANDLE;
			_freeHandles.push(static_cast<MaterialID>(i));
		}
	}
	Registry::~Registry(){}

	MaterialID Registry::registerMaterial(){
		MaterialID ID = popNextID();
		return ID;
	}

	void Registry::updateMaterial(const MaterialID& ID, const MaterialPair& data){
		assert(ID < _handles.size() && "The material ID is invalid");
		_handles[ID] = data;
	}

	void Registry::unregisterMaterial(const MaterialID& ID){
		assert(ID < _handles.size() && "The material ID is invalid");
		assert(_handles[ID].set != VK_NULL_HANDLE && "The material ID has already been unregistred");
		_handles[ID].set = VK_NULL_HANDLE;
		_freeHandles.push(ID);
	}

	Registry::MaterialPair* Registry::get(const MaterialID& ID){
		assert(ID < _handles.size() && "The material ID is invalid");
		auto& pair = _handles[ID];
		if (pair.set == VK_NULL_HANDLE){
			return nullptr;
		} else {
			return &pair;
		}
	}

	std::size_t Registry::popNextID(){
		if (_freeHandles.empty()){
			spdlog::error("The material pool is empty, cannot register a new one");
			throw std::runtime_error("the pool is empty");
		}

		std::size_t nextID = _freeHandles.front();
		_freeHandles.pop();
		return nextID;
	}
}