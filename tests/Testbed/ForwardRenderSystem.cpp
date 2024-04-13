#include "ForwardRenderSystem.hpp"
#include <Raindrop/Components/Model.hpp>
#include <Raindrop/Components/Transformation.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop/Graphics/Pipelines/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Materials/SetLayout.hpp>
#include <Raindrop/Graphics/Models/Vertex.hpp>
#include <Raindrop/Graphics/Models/Mesh.hpp>

#include <spdlog/spdlog.h>
#include <fstream>

#define MAX_BOUND_SCENE_COUNT 100


struct PushConstant{
	Raindrop::Maths::mat4 localTransform;

	static VkPushConstantRange range(){
		return VkPushConstantRange{
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.offset = 0,
			.size = sizeof(PushConstant)
		};
	}
};

// TODO: really need to rework on shader modules loading
std::vector<char> readFile(const std::string& filepath) {
	std::ifstream file{filepath, std::ios::ate | std::ios::binary};

	if (!file.is_open()) {
		spdlog::warn("Failed to open \"{}\" shader file", filepath);
		throw std::runtime_error("failed to open file");
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

ForwardRenderSystem::ForwardRenderSystem(Raindrop::Raindrop& engine) :
	_engine{engine},
	_descriptorSetLayout{VK_NULL_HANDLE},
	_descriptorPool{VK_NULL_HANDLE},
	_descriptorsBuffer{VK_NULL_HANDLE},
	_descriptorsMemory{VK_NULL_HANDLE},
	_vertexShader{VK_NULL_HANDLE},
	_fragmentShader{VK_NULL_HANDLE}{
	
	spdlog::info("Constructing Forward render system ...");

	createDescriptorSetLayout();
	createDescriptorPool();
	createDescriptorBuffer();
	allocateDescriptorSets();
	updateDescriptorSets();
	createPipelineLayout();
	createShaderModules();
	createPipeline();
}

ForwardRenderSystem::~ForwardRenderSystem(){
	spdlog::info("Destroying Forward render system ...");

	destroyPipeline();
	destroyShaderModules();
	destroyPipelineLayout();
	destroyDescriptorPool();
	destroyDescriptorBuffer();
	destroyDescriptorSetLayout();
}

void ForwardRenderSystem::createDescriptorSetLayout(){
	spdlog::info("Constructing Forward renderer system descriptor scene properties set layout ...");
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	VkDescriptorSetLayoutBinding bindings[] = {
		{
			.binding = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = 1,
			.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT
		},
	};

	VkDescriptorSetLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.bindingCount = sizeof(bindings) / sizeof(bindings[0]);
	info.pBindings = bindings;
	
	if (vkCreateDescriptorSetLayout(device.get(), &info, allocationCallbacks, &_descriptorSetLayout) != VK_SUCCESS){
		spdlog::error("Failed to create forward render system descriptor set layout");
		throw std::runtime_error("Failed to create descriptor set layout");
	}
}

void ForwardRenderSystem::destroyDescriptorSetLayout(){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	if (_descriptorSetLayout != VK_NULL_HANDLE){
		vkDestroyDescriptorSetLayout(device.get(), _descriptorSetLayout, allocationCallbacks);
		_descriptorSetLayout = VK_NULL_HANDLE;
	}
}

void ForwardRenderSystem::createDescriptorPool(){
	spdlog::info("Constructing Forward renderer system descriptor scene properties descriptor pool ...");
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	VkDescriptorPoolSize sizes[] = {
		{
			.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.descriptorCount = static_cast<uint32_t>(MAX_BOUND_SCENE_COUNT)
		}
	};

	VkDescriptorPoolCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.maxSets = MAX_BOUND_SCENE_COUNT;
	info.pPoolSizes = sizes;
	info.poolSizeCount = sizeof(sizes) / sizeof(sizes[0]);

	if (vkCreateDescriptorPool(device.get(), &info, allocationCallbacks, &_descriptorPool) != VK_SUCCESS){
		spdlog::error("Failed to create forward renderer descriptor pool (size : {})", MAX_BOUND_SCENE_COUNT);
		throw std::runtime_error("Failed to create descriptor pool");
	}
}

void ForwardRenderSystem::destroyDescriptorPool(){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;
	
	if (_descriptorPool != VK_NULL_HANDLE){
		vkDestroyDescriptorPool(device.get(), _descriptorPool, allocationCallbacks);
		_descriptorPool = VK_NULL_HANDLE;
	}
}

void ForwardRenderSystem::createDescriptorBuffer(){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	{
		const std::size_t alignement = context.physicalDevice.limits().minUniformBufferOffsetAlignment;
		const std::size_t rest = sizeof(ForwardRenderSceneProperties::data) % alignement;
		_alignedDescriptorSize = rest != 0 ? sizeof(ForwardRenderSceneProperties::data) + (alignement - rest) : sizeof(ForwardRenderSceneProperties::data);
	}

	spdlog::info("Constructing Forward renderer system descriptor scene properties buffer and memory ... (count : {} | instance size : {} | aligned instance size : {})", MAX_BOUND_SCENE_COUNT, sizeof(ForwardRenderSceneProperties::data), _alignedDescriptorSize);

	{
		std::size_t size = _alignedDescriptorSize * MAX_BOUND_SCENE_COUNT;

		VkBufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.size = size;
		info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		if (vkCreateBuffer(device.get(), &info, allocationCallbacks, &_descriptorsBuffer) != VK_SUCCESS){
			spdlog::error("Failed to create forward renderer uniform buffer ! (size : {})", size);
			throw std::runtime_error("Failed to create buffer !");
		}
	}

	{
		VkMemoryRequirements requirements;
		vkGetBufferMemoryRequirements(device.get(), _descriptorsBuffer, &requirements);

		VkMemoryAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.allocationSize = requirements.size;
		info.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		
		if (vkAllocateMemory(device.get(), &info, allocationCallbacks, &_descriptorsMemory) != VK_SUCCESS){
			spdlog::error("Failed to allocate forward renderer uniform buffer memory !");
			throw std::runtime_error("Failed to allocate memory !");
		}

		if (vkBindBufferMemory(device.get(), _descriptorsBuffer, _descriptorsMemory, 0) != VK_SUCCESS){
			spdlog::error("Failed to bind forward renderer uniform buffer memory !");
			throw std::runtime_error("Failed to bind buffer memory !");
		}
	}
}

void ForwardRenderSystem::destroyDescriptorBuffer(){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	if (_descriptorsBuffer != VK_NULL_HANDLE){
		vkDestroyBuffer(device.get(), _descriptorsBuffer, allocationCallbacks);
		_descriptorsBuffer = VK_NULL_HANDLE;
	}

	if (_descriptorsMemory != VK_NULL_HANDLE){
		vkFreeMemory(device.get(), _descriptorsMemory, allocationCallbacks);
		_descriptorsMemory = VK_NULL_HANDLE;
	}
}

void ForwardRenderSystem::allocateDescriptorSets(){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	_sets.resize(MAX_BOUND_SCENE_COUNT);
	std::vector<VkDescriptorSetLayout> setLayouts(MAX_BOUND_SCENE_COUNT);
	std::fill(setLayouts.begin(), setLayouts.end(), _descriptorSetLayout);

	VkDescriptorSetAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	info.descriptorPool = _descriptorPool;
	info.descriptorSetCount = static_cast<uint32_t>(MAX_BOUND_SCENE_COUNT);
	info.pSetLayouts = setLayouts.data();

	if (vkAllocateDescriptorSets(device.get(), &info, _sets.data()) != VK_SUCCESS){
		spdlog::error("Failed to allocate forward renderer descriptor sets (count : {})", MAX_BOUND_SCENE_COUNT);
		throw std::runtime_error("Failed to allocate descriptor sets !");
	}

	for (std::size_t i=0; i<MAX_BOUND_SCENE_COUNT; i++){
		_freeSetIDs.push_back(i);
	}
}

void ForwardRenderSystem::updateDescriptorSets(){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	std::vector<VkDescriptorBufferInfo> infos(MAX_BOUND_SCENE_COUNT);
	std::vector<VkWriteDescriptorSet> writes(MAX_BOUND_SCENE_COUNT);

	for (std::size_t i=0; i<MAX_BOUND_SCENE_COUNT; i++){
		infos[i] = {
			.buffer = _descriptorsBuffer,
			.offset = _alignedDescriptorSize * i,
			.range = sizeof(ForwardRenderSceneProperties::data)
		};

		writes[i] = {
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = _sets[i],
			.dstBinding = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pBufferInfo = &infos[i]
		};
	}

	vkUpdateDescriptorSets(
		device.get(),
		MAX_BOUND_SCENE_COUNT,
		writes.data(),
		0,
		nullptr
	);
}

void ForwardRenderSystem::populateInternProperty(ForwardRenderSceneProperties& prop){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	if (_freeSetIDs.empty()){
		throw std::runtime_error("Out of descriptor sets");
	}

	auto& ID = prop.__internal__.setID;
	ID = _freeSetIDs.front();
	_freeSetIDs.pop_front();
}


void ForwardRenderSystem::createPipelineLayout(){
	auto& context = _engine.renderer().context();

	Raindrop::Graphics::Pipelines::PipelineLayoutConfigInfo info;
	info.setLayouts = {
		context.materials.layout().get(),
		_descriptorSetLayout
	};

	info.pushConstants = {
		PushConstant::range()
	};

	_pipelineLayout = _engine.renderer().createPipelineLayout(info);
}

void ForwardRenderSystem::destroyPipelineLayout(){
	_engine.renderer().destroyPipelineLayout(_pipelineLayout);	
}

void ForwardRenderSystem::createShaderModule(const std::vector<char>& code, VkShaderModule& module){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(device.get(), &createInfo, allocationCallbacks, &module) != VK_SUCCESS){
		spdlog::error("Failed to create shader module");
		throw std::runtime_error("failed to create shader module");
	}
}

void ForwardRenderSystem::createShaderModules(){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	auto code = readFile((std::filesystem::current_path() / "shaders/forwardRenderer/shader.glsl.vert.spv").string());
	createShaderModule(code, _vertexShader);

	code = readFile((std::filesystem::current_path() / "shaders/forwardRenderer/shader.glsl.frag.spv").string());
	createShaderModule(code, _fragmentShader);
}

void ForwardRenderSystem::destroyShaderModules(){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	if (_vertexShader != VK_NULL_HANDLE){
		vkDestroyShaderModule(device.get(), _vertexShader, allocationCallbacks);
		_vertexShader = VK_NULL_HANDLE;
	}

	if (_fragmentShader != VK_NULL_HANDLE){
		vkDestroyShaderModule(device.get(), _fragmentShader, allocationCallbacks);
		_fragmentShader = VK_NULL_HANDLE;
	}
}

void ForwardRenderSystem::createPipeline(){
	Raindrop::Graphics::Pipelines::GraphicsPipelineConfigInfo info;
	info.defaultInfo(info);
	auto& context = _engine.renderer().context();

	info.pipelineLayoutID = _pipelineLayout.ID();
	info.renderPass = context.baseFramebuffer.renderPass();

	info.viewportInfo.viewportCount = 1;
	info.viewportInfo.pViewports = nullptr;
	info.viewportInfo.scissorCount = 1;
	info.viewportInfo.pScissors = nullptr;

	info.colorAttachments = {
		VkPipelineColorBlendAttachmentState{
			.blendEnable = VK_FALSE,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		}
	};

	info.dynamicStateEnables = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	
	{
		VkPipelineShaderStageCreateInfo frag{};
		frag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		frag.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		frag.pName = "main";
		frag.module = _fragmentShader;

		info.stages.push_back(frag);
	}

	{
		VkPipelineShaderStageCreateInfo vert{};
		vert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vert.stage = VK_SHADER_STAGE_VERTEX_BIT,
		vert.pName = "main";
		vert.module = _vertexShader;

		info.stages.push_back(vert);
	}

	// TODO: user should not have to do this
	using Vertex = Raindrop::Graphics::Models::Vertex;

	info.vertices = {
		VkVertexInputAttributeDescription{
			.location = 0,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(Vertex, position)
		},

		VkVertexInputAttributeDescription{
			.location = 1,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(Vertex, normal)
		},

		VkVertexInputAttributeDescription{
			.location = 2,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(Vertex, color)
		},

		VkVertexInputAttributeDescription{
			.location = 3,
			.binding = 0,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = offsetof(Vertex, UV)
		}
	};

	info.bindings = {
		VkVertexInputBindingDescription{
			.binding = 0,
			.stride = sizeof(Vertex),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
		}
	};

	info.update();

	_pipeline = _engine.renderer().createGraphicsPipeline(info);
}

void ForwardRenderSystem::destroyPipeline(){
	_engine.renderer().destroyGraphicsPipeline(_pipeline);
}

void ForwardRenderSystem::bind(Raindrop::SceneWrapper scene){
	ForwardRenderSceneProperties* properties = scene.getProperty<ForwardRenderSceneProperties>();
	if (properties == nullptr){
		properties = scene.addProperty<ForwardRenderSceneProperties>();
	}

	try{
		populateInternProperty(*properties);
	} catch (const std::exception &e){
		spdlog::error("Tried to bind scene to forward render system, but failed to : ", e.what());
		throw std::runtime_error("Failed to bind scene");
	}
}

void ForwardRenderSystem::release(Raindrop::SceneWrapper scene){
	auto& context = _engine.renderer().context();
	auto& device = context.device;
	auto& allocationCallbacks = context.allocationCallbacks;

	ForwardRenderSceneProperties* properties = scene.getProperty<ForwardRenderSceneProperties>();
	if (properties == nullptr) return;

	_freeSetIDs.push_back(properties->__internal__.setID);
}

void ForwardRenderSystem::updateScene(VkCommandBuffer commandBuffer, Raindrop::SceneWrapper scene){
	ForwardRenderSceneProperties* properties = scene.getProperty<ForwardRenderSceneProperties>();
	if (properties == nullptr){
		throw std::runtime_error("Cannot update a scene that does not contains a \"ForwardRenderSceneProperties\" property");
	}

	std::size_t setID = properties->__internal__.setID;

	vkCmdUpdateBuffer(
		commandBuffer,
		_descriptorsBuffer,
		setID * _alignedDescriptorSize,
		sizeof(ForwardRenderSceneProperties::data),
		&properties->data
	);
}

void ForwardRenderSystem::render(VkCommandBuffer commandBuffer, Raindrop::SceneWrapper scene){
	ForwardRenderSceneProperties* properties = scene.getProperty<ForwardRenderSceneProperties>();
	if (properties == nullptr){
		throw std::runtime_error("Cannot render a scene that does not contains a \"ForwardRenderSceneProperties\" property");
	}

	std::size_t setID = properties->__internal__.setID;

	// We create a view of the scene
	// A view is basicaly put a filter that only iterate through entities with the required components
	auto view = scene->view<Raindrop::Components::Model, Raindrop::Components::Transformation>();

	// If there is nothing to render, it would be quit useless to bind everything
	// Plus, binding non used shared can lead to unexpected issues
	if (view.size_hint() == 0) return;

	// We bind the pipeline
	_pipeline->bind(commandBuffer);

	// Then we specify the viewport and scissor
	// We added the dynamic viewprot and scissor to the pipeline to support multiple size of framebuffer
	// And so, we need to update it
	{
		auto& context = _engine.renderer().context();
		auto size = context.baseFramebuffer.size();

		VkViewport viewport{
			.x = 0,
			.y = 0,
			.width = static_cast<float>(size.x),
			.height = static_cast<float>(size.y),
			.minDepth = 0.f,
			.maxDepth = 1.f
		};

		vkCmdSetViewport(
			commandBuffer,
			0,
			1,
			&viewport
		);

		VkRect2D scissor{
			.offset = VkOffset2D{
				.x = 0,
				.y = 0
			},
			.extent = VkExtent2D{
				.width = static_cast<uint32_t>(size.x),
				.height = static_cast<uint32_t>(size.y)
			}
		};

		vkCmdSetScissor(
			commandBuffer,
			0,
			1,
			&scissor
		);
	}

	PushConstant pushConstant;

	// Iterate through every entities
	for (auto& id : view){

		// Get the components
		auto& model = scene->get<Raindrop::Components::Model>(id);
		auto& transformation = scene->get<Raindrop::Components::Transformation>(id);

		// Here we use 'useless' tabulations to make the loop more understandable
		{
			// We need to update and push the push constant
			pushConstant.localTransform = transformation.matrix;

			vkCmdPushConstants(
				commandBuffer,
				_pipelineLayout->get(),
				VK_SHADER_STAGE_VERTEX_BIT,
				0,
				sizeof(PushConstant),
				&pushConstant
			);
		}

		// We can't really know if a model has been removed / invalidated
		// So we check if the model is valid
		auto modelRef = model.model.lock();
		if (modelRef != nullptr){

			// We iterate through every meshes
			for (auto& mesh : *modelRef){
				// We need to query the correct material ID to bind the proper material descriptor set
				const auto materialID = mesh->materialID();
				const auto& materialDescriptorSet = _engine.renderer().materials().getDescriptorSet(materialID);

				VkDescriptorSet sets[] = {
					materialDescriptorSet,
					_sets[setID]
				};

				// We bind the corresponding descriptor sets
				vkCmdBindDescriptorSets(
					commandBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					_pipelineLayout->get(),
					0,
					2,
					sets,
					0,
					nullptr
				);

				// And then, finaly, we can draw the mesh
				mesh->render(commandBuffer);
			}
		}
	}
}