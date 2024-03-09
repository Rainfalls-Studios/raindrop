#include <Raindrop/Renderer/Scene/Scene.hpp>
#include <Raindrop/Renderer/Context.hpp>

namespace Raindrop::Renderer::Scene{
	Scene::Scene(::Raindrop::Renderer::Context& renderer) : 
		_renderer{renderer},
		_context{renderer}{}
		
	Scene::~Scene(){}
}