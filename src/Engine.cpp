#include <Raindrop/Engine.hpp>

#include <Raindrop/Graphics/RenderPass.hpp>
#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/DescriptorSetLayout.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/ShaderModule.hpp>
#include <Raindrop/Graphics/Memory.hpp>
#include <Raindrop/Graphics/Memory.hpp>
#include <Raindrop/Graphics/Image.hpp>
#include <Raindrop/Graphics/ImageView.hpp>
#include <Raindrop/Graphics/Sampler.hpp>
#include <Raindrop/Graphics/DescriptorPool.hpp>
#include <Raindrop/Graphics/MeshData.hpp>
#include <Raindrop/Graphics/Mesh.hpp>
#include <Raindrop/Graphics/VertexLayout.hpp>
#include <Raindrop/Graphics/ModelLayoutConfig.hpp>
#include <Raindrop/Graphics/PipelineLayout.hpp>

#define create_graphics_interface(type) std::shared_ptr<Graphics::type> Engine::createGraphics##type(){ std::shared_ptr<Graphics::type> instance = getUUIDRegistry().emplace<Graphics::type>(); instance->prepare(getGraphicsContext()); return std::move(instance);}

namespace Raindrop{
	Engine::Engine() :
		_context{nullptr}
	{}

	Engine::~Engine(){
		release();
	}

	void Engine::initialize(){
		_context = std::make_unique<Context>();
		_context->initialize();
	}

	void Engine::release(){
		_context.reset();
	}

	// ======================= CORE ==============================

	Context& Engine::getContext(){
		if (!_context){
			throw std::runtime_error("Core has not been initialized");
		}
		return *_context;
	}

	const Context& Engine::getContext() const{
		if (!_context){
			throw std::runtime_error("Core has not been initialized");
		}
		return *_context;
	}

	UUIDRegistry& Engine::getUUIDRegistry(){
		return getContext().registry;
	}

	// ==================== GRAPHICS ============================

	Graphics::Context& Engine::getGraphicsContext(){
		auto& graphics = getContext().graphics;
		if (!graphics){
			throw std::runtime_error("Graphics context has not been initialized");
		}
		return *graphics;
	}

	const Graphics::Context& Engine::getGraphicsContext() const{
		const auto& graphics = getContext().graphics;
		if (!graphics){
			throw std::runtime_error("Graphics context has not been initialized");
		}
		return *graphics;
	}


	Graphics::Core::Instance& Engine::getGraphicsInstance(){
		return getGraphicsContext().core.instance;
	}

	Graphics::Core::PhysicalDevice& Engine::getGraphicsPhysicalDevice(){
		return getGraphicsContext().core.physicalDevice;
	}

	Graphics::Core::Device& Engine::getGraphicsDevice(){
		return getGraphicsContext().core.device;
	}

	Graphics::Window::Window& Engine::getGraphicsWindow(){
		return getGraphicsContext().window.window;
	}

	Graphics::Window::Surface& Engine::getGraphicsWindowSurface(){
		return getGraphicsContext().window.surface;
	}

	Graphics::Window::Swapchain& Engine::getGraphicsWindowSwapchain(){
		return getGraphicsContext().window.swapchain;
	}

	Graphics::Formats& Engine::getGraphicsFormats(){
		return getGraphicsContext().formats;
	}

	Graphics::ModelLoader& Engine::getGraphicsModelLoader(){
		return getGraphicsContext().modelLoader;
	}

	create_graphics_interface(RenderPass);
	create_graphics_interface(Buffer);
	create_graphics_interface(DescriptorSetLayout);
	create_graphics_interface(PipelineLayout);
	create_graphics_interface(GraphicsPipeline);
	create_graphics_interface(ShaderModule);
	create_graphics_interface(Memory);
	create_graphics_interface(Image);
	create_graphics_interface(Sampler);
	create_graphics_interface(DescriptorPool);
	create_graphics_interface(MeshData);
	
	Graphics::ModelLayoutConfig Engine::createGraphicsModelLayoutConfig(){
		return Graphics::ModelLayoutConfig();
	}

	Graphics::VertexLayout Engine::createGraphicsVertexLayout(){
		return Graphics::VertexLayout();
	}

	// ==================== EVENTS ============================

	Events::Context& Engine::getEventsContext(){
		auto& events = getContext().events;
		if (!events){
			throw std::runtime_error("Events context has not been initialized");
		}
		return *events;
	}

	const Events::Context& Engine::getEventsContext() const{
		auto& events = getContext().events;
		if (!events){
			throw std::runtime_error("Events context has not been initialized");
		}
		return *events;
	}


	// ==================== SCENES ============================

	Scenes::Context& Engine::getScenesContext(){
		auto& scenes = getContext().scenes;
		if (!scenes){
			throw std::runtime_error("Scenes context has not been initialized");
		}
		return *scenes;
	}

	const Scenes::Context& Engine::getScenesContext() const{
		auto& scenes = getContext().scenes;
		if (!scenes){
			throw std::runtime_error("Scenes context has not been initialized");
		}
		return *scenes;
	}

	std::shared_ptr<Scenes::Scene> Engine::createScene(){
		std::shared_ptr<Scenes::Scene> scene = _context->registry.emplace<Scenes::Scene>();
		scene->prepare(getScenesContext());
		return scene;
	}


	Assets::Context& Engine::getAssetsContext(){
		auto& assets = getContext().assets;
		if (!assets){
			throw std::runtime_error("Assets context has not been initialized");
		}
		return *assets;
	}

	const Assets::Context& Engine::getAssetsContext() const{
		auto& assets = getContext().assets;
		if (!assets){
			throw std::runtime_error("Assets context has not been initialized");
		}
		return *assets;
	}
}