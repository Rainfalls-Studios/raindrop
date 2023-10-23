#include <Raindrop/Graphics/Engine.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics{
	Engine::Engine(Core::Context& core){
		_context = std::make_unique<Context>(core);
		_context->logger.info("Initializing Graphics Engine...");

		_context->logger.info("Graphics Engine initialized without any critical error");
	}

	Engine::~Engine(){
		_context->logger.info("Terminating Graphics Engine...");

		_context->logger.info("Graphics Engine terminated without any critical error");
		_context.reset();
	}
}


// #include <Raindrop/Graphics/Renderer.hpp>
// #include <Raindrop/Graphics/FrameState.hpp>
// #include <Raindrop/Scene.hpp>

// #include <SDL2/SDL_vulkan.h>
// #include <glm/gtc/quaternion.hpp>
// #include <glm/gtx/quaternion.hpp>

// #include <Raindrop/Graphics/Components/Camera.hpp>
// #include <Raindrop/Graphics/Components/Model.hpp>
// #include <Raindrop/Graphics/Components/Spotlight.hpp>
// #include <Raindrop/Graphics/Components/LightPoint.hpp>
// #include <Raindrop/Graphics/Components/Sun.hpp>

// #ifdef RAINDROP_EDITOR
// 	#include <Raindrop/Graphics/Editor/Editor.hpp>
// #endif

// namespace Raindrop::Graphics{
// 	Renderer::Renderer(Core::EngineContext& context) :
// 		_context{context},
// 		_factories{_context}{
		
// 		_wireMesh = std::make_unique<BaseRender::WireMesh::Renderer>(_context);
// 	}

// 	Renderer::~Renderer(){

// 	}

// 	void Renderer::registerSceneComponents(Core::Scene::Scene& scene){
// 		uint32_t count = scene.maxEntityCount();

// 		scene.registerComponent<Components::Camera>(count, _context);
// 		scene.registerComponent<Components::Model>(count, _context);
// 		scene.registerComponent<Components::Sun>(count, _context);
// 		scene.registerComponent<Components::Spotlight>(count);
// 		scene.registerComponent<Components::LightPoint>(count);
// 	}

// 	FrameState Renderer::begin(){
// 		auto& window = _context.window;
// 		auto& swapchain = _context.swapchain;

// 		FrameState frameState;

// 		VkResult result = swapchain.acquireNextImage();
// 		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
// 			auto size = window.getSize();
// 			swapchain.setExtent(VkExtent2D{size.x, size.y});
// 			swapchain.rebuildSwapchain();
// 			return frameState;
// 		}

// 		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
// 			throw std::runtime_error("failed to acquire swap chain image!");
// 		}

// 		frameState.commandBuffer = _context.graphics.commandBuffers.getCommandBuffer(currentFrameID());
// 		VkCommandBufferBeginInfo beginInfo{};
// 		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

// 		if (vkBeginCommandBuffer(frameState.commandBuffer, &beginInfo) != VK_SUCCESS) {
// 			throw std::runtime_error("failed to begin recording command buffer!");
// 		}
// 		return frameState;
// 	}


// 	void Renderer::end(FrameState& state){
// 		_context.window.events();

// 		auto& window = _context.window;
// 		auto& swapchain = _context.swapchain;
// 		VkCommandBuffer& commandBuffer = state.commandBuffer;

// 		swapchain.beginRenderPass(commandBuffer);
// 		swapchain.endRenderPass(commandBuffer);


// 		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
// 			throw std::runtime_error("failed to record command buffer");
		
// 		VkResult result = swapchain.submitCommandBuffer(&commandBuffer);

// 		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.resized()){
// 			auto size = window.getSize();
// 			window.resetResizedFlag();
// 			swapchain.setExtent(VkExtent2D{size.x, size.y});
// 			swapchain.rebuildSwapchain();

// 		} else if (result != VK_SUCCESS){
// 			throw std::runtime_error("failed to submit the command buffer");
// 		}
// 	}

// 	void Renderer::renderScene(FrameState& state, const Scene& scene){
// 		auto cameras = scene.filterEntitiesWithComponent<Components::Camera>();
// 		for (auto &camera : cameras){
// 			auto& cameraComponent = camera.getComponent<Components::Camera>();
// 			renderSceneWithCamera(state, scene, cameraComponent.camera());
// 		}
// 	}

// 	void Renderer::renderSceneWithCamera(FrameState& state, const Scene& scene, Camera& camera){
// 		// auto& fbo = camera.framebuffer();
// 		// auto commandBuffer = state.commandBuffer;

// 		// if (fbo.beginRenderPass(commandBuffer)){
			
// 		// 	SceneRenderer::RenderWithTextureInfo info;
// 		// 	info.commandBuffer = commandBuffer;
// 		// 	info.viewTransform = camera.viewProjection();

// 		// 	// _sceneRenderer.renderWithTextures(scene.root(), info);
			
// 		// 	fbo.endRenderPass(commandBuffer);
// 		// }
// 	}

// 	uint32_t Renderer::currentFrameID(){
// 		return _context.swapchain.currentFrame();
// 	}

// 	// Renderer::Renderer(Core::EngineContext& context) : _interpreter{context}{
// 	// 	// el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics");
// 	// 	// customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

// 	// 	// CLOG(INFO, "Engine.Graphics") << "Creating renderer ...";
		
// 	// 	// _context = std::make_unique<GraphicsContext>(context, {});
// 	// 	// _gui = std::make_unique<ImGUI>(*_context);
// 	// 	// registerFactories();

// 	// 	// _pipelines = std::make_unique<Pipelines>(*_context);

// 	// 	// _context->scene.registerComponent<Components::Model>(1000, *_context);
// 	// 	// _context->scene.registerComponent<Components::Sun>(1000, *_context);
// 	// 	// _context->scene.registerComponent<Components::LightPoint>(1000);
// 	// 	// _context->scene.registerComponent<Components::Spotlight>(1000);

// 	// 	// #ifdef RAINDROP_EDITOR
// 	// 	// 	_editor = std::make_unique<Editor::Editor>(*_context, nullptr);
// 	// 	// 	_context->sceneRenderPass = _editor->renderPass();
// 	// 	// #else
// 	// 	// 	_context->sceneRenderPass = _context->swapchain.renderPass();
// 	// 	// #endif

// 	// 	// _gui->uploadFonts();

// 	// 	// _worldFramebuffer = std::make_unique<WorldFramebuffer>(*_context, 1080, 720);
// 	// 	// _sceneRenderer = std::make_unique<SceneRenderer>(*_context);
// 	// 	// _forwardShader = std::make_unique<ForwardShader::ForwardShader>(*_context);

// 	// 	// createDescriptorPool();
// 	// 	// createSetLayout();
// 	// 	// createDescriptorSet();
// 	// 	// createPipeline();

// 	// 	// _context->gRegistry["layout"] = _setLayout->get();
// 	// 	// createGraphicsCommandBuffers();

// 	// 	// _forwardShader->updateDescriptor(*_worldFramebuffer);

// 	// 	// CLOG(INFO, "Engine.Graphics") << "Created renderer with success !";
// 	// }

// 	// Renderer::~Renderer(){
// 	// 	CLOG(INFO, "Engine.Graphics") << "Destroying renderer ...";

// 	// 	_context->device.waitIdle();

// 	// 	eraseFactories();
// 	// 	destroyGraphicsCommandBuffers();

// 	// 	#ifdef RAINDROP_EDITOR
// 	// 		_editor.reset();
// 	// 	#endif
		
// 	// 	_context->scene.unregisterComponent<Components::Model>();
// 	// 	_context->scene.unregisterComponent<Components::Sun>();
// 	// 	_context->scene.unregisterComponent<Components::Spotlight>();
// 	// 	_context->scene.unregisterComponent<Components::LightPoint>();
		
// 	// 	_forwardShader.reset();
// 	// 	_setLayout.reset();
// 	// 	vkResetDescriptorPool(_context->device.get(), _descriptorPool->get(), 0);
// 	// 	_descriptorPool.reset();
// 	// 	_pipeline.reset();
// 	// 	_pipelines.reset();

// 	// 	_sceneRenderer.reset();
// 	// 	_worldFramebuffer.reset();
// 	// 	_gui.reset();
// 	// 	_context.reset();
		
// 	// 	CLOG(INFO, "Engine.Graphics") << "Destroyed renderer with success !";
// 	// }

// 	// void Renderer::openGUI(const std::filesystem::path& path){
// 	// 	_interpreter.parse(path);
// 	// }

// 	// void Renderer::createDescriptorPool(){
// 	// 	Builders::DescriptorPoolBuilder builder;
// 	// 	builder.setMaxSets(1);
// 	// 	builder.pushPoolSize({VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1});

// 	// 	_descriptorPool = builder.build(*_context);
// 	// }

// 	// void Renderer::createSetLayout(){
// 	// 	Builders::DescriptorSetLayoutBuilder builder;

// 	// 	VkDescriptorSetLayoutBinding binding;
// 	// 	binding.binding = 0;
// 	// 	binding.descriptorCount = 1;
// 	// 	binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
// 	// 	binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
// 	// 	binding.pImmutableSamplers = nullptr;
// 	// 	builder.pushBinding(binding);

// 	// 	_setLayout = builder.build(*_context);
// 	// }

// 	// void Renderer::createDescriptorSet(){
// 	// 	VkDescriptorSetAllocateInfo info{};
// 	// 	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
// 	// 	info.descriptorSetCount = 1;
// 	// 	info.descriptorPool = _descriptorPool->get();
		
// 	// 	VkDescriptorSetLayout layout = _setLayout->get();
// 	// 	info.pSetLayouts = &layout;
		
// 	// 	if (vkAllocateDescriptorSets(_context->device.get(), &info, &_descriptorSet) != VK_SUCCESS){
// 	// 		CLOG(ERROR, "Engine.Graphics") << "Failed to create renderer descriptor set";
// 	// 		throw std::runtime_error("Failed to create renderer descriptor set");
// 	// 	}

// 	// 	VkWriteDescriptorSet write = {};
// 	// 	VkDescriptorImageInfo imageInfo = _forwardShader->attachmentInfo();

// 	// 	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
// 	// 	write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
// 	// 	write.dstBinding = 0;
// 	// 	write.pImageInfo = &imageInfo;
// 	// 	write.descriptorCount = 1;
// 	// 	write.dstSet = _descriptorSet;

// 	// 	vkUpdateDescriptorSets(_context->device.get(), 1, &write, 0, nullptr);
// 	// }

	
// 	// FrameState Renderer::begin(){
// 	// 	_context->window.events(_gui.get());

// 	// 	FrameState frameState;
// 	// 	frameState.commandBuffer = beginFrame();

// 	// 	return frameState;
// 	// }

// 	// void Renderer::renderScene(const Scene& scene, FrameState& state){
// 	// 	// renderDepthMaps(scene, state);

// 	// 	// // _worldFramebuffer->beginRenderPass(scene, state);
// 	// 	// // renderScene(commandBuffer);
// 	// 	// // _worldFramebuffer->endRenderPass(commandBuffer);

// 	// 	// // _forwardShader->render(commandBuffer, _editor->cameraDirection(), _editor->cameraPosition());

// 	// 	// // renderGui();

// 	// 	// // #ifdef RAINDROP_EDITOR
// 	// 	// 	// if (_editor->beginRenderPass(commandBuffer)){
// 	// 	// 		renderFrame(commandBuffer);
// 	// 	// 		_editor->endRenderPass(commandBuffer);
// 	// 	// 	}
// 	// 	// #endif

// 	// 	// swapchain.beginRenderPass(commandBuffer);
// 	// 	// renderSwapchain(commandBuffer);
// 	// 	// swapchain.endRenderPass(commandBuffer);
// 	// }

// 	// void Renderer::end(FrameState& state){
// 	// 	endFrame();
// 	// }

// 	// void Renderer::update(){
// 	// 	auto& swapchain = _context->swapchain;

// 	// 	_context->window.events(_gui.get());

// 	// 	if (VkCommandBuffer commandBuffer = beginFrame()){

// 	// 		_gui->newFrame();

// 	// 		endFrame();
// 	// 	}

// 	// 	_context->executPostRenderProcs();
// 	// }

// 	// void Renderer::renderGui(){
// 	// }

// 	// void Renderer::renderSwapchain(VkCommandBuffer commandBuffer){
// 	// 	#ifndef RAINDROP_EDITOR
// 	// 		renderFrame(commandBuffer);
// 	// 	#else
// 	// 		_editor->render();
// 	// 	#endif

// 	// 	_gui->render(commandBuffer);
// 	// }

// 	// void Renderer::renderFrame(VkCommandBuffer commandBuffer){
// 	// 	_pipeline->bind(commandBuffer);
// 	// 	vkCmdBindDescriptorSets(
// 	// 		commandBuffer,
// 	// 		VK_PIPELINE_BIND_POINT_GRAPHICS,
// 	// 		_pipeline->layout(),
// 	// 		0,
// 	// 		1,
// 	// 		&_descriptorSet,
// 	// 		0,
// 	// 		nullptr
// 	// 	);

// 	// 	vkCmdDraw(commandBuffer, 6, 1, 0, 0);
// 	// }

// 	// VkCommandBuffer Renderer::beginFrame(){
// 	// 	auto& window = _context->window;
// 	// 	auto& swapchain = _context->swapchain;

// 	// 	VkResult result = swapchain.acquireNextImage();
// 	// 	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
// 	// 		auto size = window.getSize();
// 	// 		swapchain.setExtent(VkExtent2D{size.x, size.y});
// 	// 		swapchain.rebuildSwapchain();
// 	// 		return nullptr;
// 	// 	}

// 	// 	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
// 	// 		throw std::runtime_error("failed to acquire swap chain image!");
// 	// 	}

// 	// 	auto commandBuffer = getCurrentGraphicsCommandBuffer();
// 	// 	VkCommandBufferBeginInfo beginInfo{};
// 	// 	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

// 	// 	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
// 	// 		throw std::runtime_error("failed to begin recording command buffer!");
// 	// 	}
// 	// 	return commandBuffer;
// 	// }

// 	// void Renderer::endFrame(){
// 	// 	auto& window = _context->window;
// 	// 	auto& swapchain = _context->swapchain;

// 	// 	VkCommandBuffer commandBuffer = getCurrentGraphicsCommandBuffer();

// 	// 	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
// 	// 		throw std::runtime_error("failed to record command buffer");
		
// 	// 	VkResult result = swapchain.submitCommandBuffer(&commandBuffer);

// 	// 	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.resized()){
// 	// 		auto size = window.getSize();
// 	// 		window.resetResizedFlag();
// 	// 		swapchain.setExtent(VkExtent2D{size.x, size.y});
// 	// 		swapchain.rebuildSwapchain();
// 	// 	} else if (result != VK_SUCCESS){
// 	// 		throw std::runtime_error("failed to submit the command buffer");
// 	// 	}
// 	// }

// 	// VkCommandBuffer Renderer::getCurrentGraphicsCommandBuffer(){
// 	// 	return _graphicsCommandBuffers[_context->swapchain.currentFrame()];
// 	// }

// 	// void Renderer::registerFactories(){
// 	// 	CLOG(INFO, "Engine.Graphics") << "registering renderer asset factories ...";
// 	// 	registerShaderFactory();
// 	// 	registerGraphicsPipelineFactory();
// 	// 	registerModelFactory();
// 	// 	registerTextureFactory();
// 	// 	CLOG(INFO, "Engine.Graphics") << "registred renderer asset factories with success !";
// 	// }

// 	// void Renderer::registerShaderFactory(){
// 	// 	_shaderFactory = std::make_shared<Factory::ShaderFactory>(*_context);
// 	// 	_context->context.assetManager.linkFactory(".spv", _shaderFactory);
// 	// }

// 	// void Renderer::registerGraphicsPipelineFactory(){
// 	// 	_graphicsPipelineFactory = std::make_unique<Factory::GraphicsPipelineFactory>(*_context);
// 	// 	_graphicsPipelineFactory->registerExtensions(_graphicsPipelineFactory);
// 	// }

// 	// void Renderer::registerModelFactory(){
// 	// 	_modelFactory = std::make_unique<Factory::ModelFactory>(*_context);
// 	// 	_modelFactory->registerExtensions(_modelFactory);
// 	// }

// 	// void Renderer::registerTextureFactory(){
// 	// 	_textureFactory = std::make_unique<Factory::TextureFactory>(*_context);
// 	// 	_textureFactory->registerExtensions(_textureFactory);
// 	// }

// 	// void Renderer::eraseFactories(){
// 	// 	_shaderFactory.reset();
// 	// 	_graphicsPipelineFactory.reset();
// 	// 	_modelFactory.reset();
// 	// 	_textureFactory.reset();
// 	// }

// 	// void Renderer::createGraphicsCommandBuffers(){
// 	// 	_graphicsCommandBuffers.resize(_context->swapchain.frameCount());

// 	// 	VkCommandBufferAllocateInfo info{};
// 	// 	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
// 	// 	info.commandPool = _context->graphicsCommandPool.primary();
// 	// 	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
// 	// 	info.commandBufferCount = _graphicsCommandBuffers.size();

// 	// 	if (vkAllocateCommandBuffers(_context->device.get(), &info, _graphicsCommandBuffers.data()) != VK_SUCCESS){
// 	// 		CLOG(ERROR, "Engine.Graphics") << "Failed to allocate graphics command buffer";
// 	// 		throw std::runtime_error("Failed to allocate graphics command buffers");
// 	// 	}
// 	// }

// 	// void Renderer::destroyGraphicsCommandBuffers(){
// 	// 	vkFreeCommandBuffers(_context->device.get(), _context->graphicsCommandPool.primary(), _graphicsCommandBuffers.size(), _graphicsCommandBuffers.data());
// 	// 	_graphicsCommandBuffers.clear();
// 	// }

// 	// void Renderer::renderScene(VkCommandBuffer commandBuffer){
// 	// 	auto& scene = _context->scene;

// 	// 	glm::mat4 viewTransform;
// 	// 	#ifdef RAINDROP_EDITOR
// 	// 		viewTransform = _editor->cameraViewProjection();
// 	// 	#else
// 	// 		{
// 	// 			auto& list = scene.componentEntities<Core::Scene::Components::Camera>();
// 	// 			if (!list.empty()){
// 	// 				auto entity = Core::Scene::Entity(list.front(), &scene);
// 	// 				auto& component = entity.getComponent<Core::Scene::Components::Camera>();
// 	// 				component.update(entity.getComponent<Core::Scene::Components::Transform>());
// 	// 				viewTransform = component.viewProjection;
// 	// 			} else {
// 	// 				viewTransform = glm::scale(glm::mat4(1.f), glm::vec3(1/100.f));
// 	// 			}
// 	// 		}
// 	// 	#endif
		
// 	// 	auto weak_pipeline = _context->context.registry["Pipeline"].as<std::weak_ptr<Raindrop::Core::Asset::Asset>>();
// 	// 	if (auto pipeline = std::static_pointer_cast<GraphicsPipeline>(weak_pipeline.lock())){
// 	// 		pipeline->bind(commandBuffer);
			
// 	// 		VkRect2D scissor = VkRect2D{{0, 0}, {1080, 720}};
// 	// 		VkViewport viewport = VkViewport{0, 0, 1080, 720, 0, 1};

// 	// 		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
// 	// 		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

// 	// 		SceneRenderer::RenderWithTextureInfo info;
// 	// 		info.layout = pipeline->layout();
// 	// 		info.commandBuffer = commandBuffer;
// 	// 		info.viewTransform = viewTransform;

// 	// 		_sceneRenderer->renderWithTextures(Core::Scene::Entity(scene.root(), &scene), info);
// 	// 	}
// 	// }

// 	// void Renderer::createPipeline(){
// 	// 	Builders::GraphicsPipelineBuilder builder;
// 	// 	builder.addShader(std::static_pointer_cast<Shader>(_context->context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/worldFramebuffer/default.glsl.frag.spv").lock()));
// 	// 	builder.addShader(std::static_pointer_cast<Shader>(_context->context.assetManager.loadOrGet("C:/Users/aalee/Documents/raindrop/tests/resources/shaders/worldFramebuffer/default.glsl.vert.spv").lock()));

// 	// 	builder.setName("quad frame render");
// 	// 	builder.setRenderPass(_context->sceneRenderPass);

// 	// 	builder.addDescriptorSetLayout(_setLayout->get());
// 	// 	builder.setAttachmentCount(1);

// 	// 	builder.setVertexAttribtes({});
// 	// 	builder.setVertexBindings({});

// 	// 	_pipeline = builder.build(*_context);
// 	// }

// 	// void Renderer::renderDepthMaps(VkCommandBuffer commandBuffer){
// 	// 	auto suns = _context->scene.componentEntities<Components::Sun>();
// 	// 	auto shadowsIT = std::partition(suns.begin(), suns.end(), [&](Core::Scene::EntityID entity){
// 	// 		const auto& sun = _context->scene.getComponent<Components::Sun>(entity);
// 	// 		return sun.hasShadowMap();
// 	// 	});

// 	// 	_pipelines->sun.get().bind(commandBuffer);

// 	// 	for (auto it = suns.begin(); it!=shadowsIT; it++){
// 	// 		Core::Scene::Entity entity = Core::Scene::Entity(*it, &_context->scene);
// 	// 		renderSunDepthMap(commandBuffer, entity);
// 	// 	}
// 	// }

// 	// void Renderer::renderSunDepthMap(VkCommandBuffer commandBuffer, Core::Scene::Entity entity){
// 	// 	auto& transform = entity.getComponent<Core::Scene::Components::Transform>();
// 	// 	auto& sun = entity.getComponent<Components::Sun>();
// 	// 	auto& scene = _context->scene;

// 	// 	sun.shadowMap().beginRenderPass(commandBuffer);
		
// 	// 	glm::vec3 direction = glm::rotate(transform.rotation, glm::vec3(0.0f, 0.0f, -1.0f));
// 	// 	direction = glm::normalize(direction);

// 	// 	float near_plane = -150.f, far_plane = 150.f;
// 	// 	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
// 	// 	glm::mat4 lightView = glm::lookAt(direction * -50.f, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)); 
// 	// 	glm::mat4 lightSpaceMatrix = lightProjection * lightView; 

// 	// 	SceneRenderer::RenderForDepthMapInfo info;
// 	// 	info.commandBuffer = commandBuffer;
// 	// 	info.layout = _pipelines->sun.get().layout();
// 	// 	info.viewTransform = lightSpaceMatrix;

// 	// 	_sceneRenderer->renderForDepthMap(Core::Scene::Entity(scene.root(), &scene), info);

// 	// 	sun.shadowMap().endRenderPass(commandBuffer);
// 	// }
// }