#include <Raindrop/Graphics/Wrappers/MaterialWrapper.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Wrappers{
	Materials::Material& MaterialWrapper::get(){
		assert(_context != nullptr && "The material wrapper context is invalid");
		return _context->materials.getMaterial(_ID);
	}

	const Materials::Material& MaterialWrapper::get() const{
		assert(_context != nullptr && "The material wrapper context is invalid");
		return _context->materials.getMaterial(_ID);
	}

	void MaterialWrapper::update(){
		assert(_context != nullptr && "The material wrapper context is invalid");
		_context->materials.updateMaterial(_ID);
	}
	
	void MaterialWrapper::update(const Material& other){
		assert(_context != nullptr && "The material wrapper context is invalid");
		_context->materials.updateMaterial(_ID, other);
	}

	void MaterialWrapper::update(const MaterialWrapper& other){
		assert(_context != nullptr && "The material wrapper context is invalid");
		_context->materials.updateMaterial(_ID, other.get());
	}
}