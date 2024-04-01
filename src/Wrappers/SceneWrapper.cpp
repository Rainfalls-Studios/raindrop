#include <Raindrop/Wrappers/SceneWrapper.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Wrappers{
	using namespace Core::Scenes;

	SceneWrapper::SceneWrapper() : _context{nullptr}, _ID{INVALID_SCENE_ID}{}
	SceneWrapper::SceneWrapper(Context& context, const SceneID& ID) : _context{&context}, _ID{ID}{}
	SceneWrapper::SceneWrapper(const SceneWrapper& other) : _context{other._context}, _ID{other._ID}{}

	SceneWrapper& SceneWrapper::operator=(const SceneWrapper& other){
		_context = other._context;
		_ID = other._ID;
		return *this;
	}

	SceneWrapper& SceneWrapper::operator=(const SceneID& ID){
		_ID = ID;
		return *this;
	}

	SceneWrapper::~SceneWrapper(){}

	Scene& SceneWrapper::get(){
		assert(_context != nullptr && "The scene wrapper context is invalid");
		return _context->scenes.get(_ID);
	}

	const Scene& SceneWrapper::get() const{
		assert(_context != nullptr && "The scene wrapper context is invalid");
		return _context->scenes.get(_ID);
	}

	SceneID SceneWrapper::ID() const{
		return _ID;
	}

	Scene* SceneWrapper::operator->(){
		return &get();
	}

	const Scene* SceneWrapper::operator->() const{
		return &get();
	}
}