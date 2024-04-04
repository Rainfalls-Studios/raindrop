#include <Raindrop/Graphics/Wrappers/Material.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics::Wrappers{
	MaterialWrapper::MaterialWrapper(Context& context) : 
		_context{&context},
		_ID{Materials::PLACEHOLDER_MATERIAL_ID}{}
	
	MaterialWrapper::MaterialWrapper(Context& context, const MaterialID& ID) : 
		_context{&context},
		_ID{ID}{}
	
	MaterialWrapper::MaterialWrapper(const MaterialWrapper& other) : 
		_context{other._context},
		_ID{other._ID}{}

	MaterialWrapper& MaterialWrapper::operator=(const MaterialWrapper& other){
		_context = other._context;
		_ID = other._ID;
		return *this;
	}

	MaterialWrapper& MaterialWrapper::operator=(const MaterialID& ID){
		_ID = ID;
		return *this;
	}

	MaterialWrapper::~MaterialWrapper(){}

	Materials::Material& MaterialWrapper::get(){
		assert(_context != nullptr && "The material wrapper context is invalid");
		return _context->materials.getMaterial(_ID);
	}

	const Materials::Material& MaterialWrapper::get() const{
		assert(_context != nullptr && "The material wrapper context is invalid");
		return _context->materials.getMaterial(_ID);
	}

	MaterialWrapper::MaterialID MaterialWrapper::ID() const{
		return _ID;
	}

	MaterialWrapper::Material* MaterialWrapper::operator->(){
		return &get();
	}

	const MaterialWrapper::Material* MaterialWrapper::operator->() const{
		return &get();
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