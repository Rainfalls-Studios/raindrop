#include <Raindrop/Wrappers/SceneWrapper.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Wrappers{
	using namespace Core::Scenes;
	
	Scene& SceneWrapper::get(){
		return Wrapper::_context->scenes.get(_ID);
	}

	const Scene& SceneWrapper::get() const{
		return Wrapper::_context->scenes.get(_ID);
	}
}