#include <Raindrop/Renderer/Pipelines/Default.hpp>
#include <Raindrop/Renderer/Pipelines/GraphicsPipeline.hpp>
#include <Raindrop/Renderer/Context.hpp>
#include <Raindrop/Renderer/Model/Vertex.hpp>
#include <Raindrop/Renderer/Material/SetLayout.hpp>

#include <spdlog/spdlog.h>
#include <fstream>

namespace Raindrop::Renderer::Pipelines{
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

	Default::Default(Context& context) :
			_context{context},
			_layout{VK_NULL_HANDLE},
			_pipeline{nullptr},
			_fragmentModule{VK_NULL_HANDLE},
			_vertexModule{VK_NULL_HANDLE}{
		
		createPipelineLayout();
		createShaderModules();
		createPipeline();
	}

	Default::~Default(){
		_pipeline = nullptr;
		destroyShaderModules();
		destroyPipelineLayout();
	}

	void Default::createPipelineLayout(){
		VkPipelineLayoutCreateInfo info = {};

		info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.setLayoutCount = 0;

		VkPushConstantRange range;
		range.offset = 0;
		range.size = sizeof(glm::mat4) * 2;
		range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

		info.pushConstantRangeCount = 1;
		info.pPushConstantRanges = &range;

		VkDescriptorSetLayout layout = _context.materials.layout().get();
		info.setLayoutCount = 1;
		info.pSetLayouts = &layout;

		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (vkCreatePipelineLayout(device.get(), &info, allocationCallbacks, &_layout) != VK_SUCCESS){
			spdlog::error("Failed to create triange pipeline pipeline layout");
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void Default::destroyPipelineLayout(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_layout != VK_NULL_HANDLE){
			vkDestroyPipelineLayout(device.get(), _layout, allocationCallbacks);
			_layout = VK_NULL_HANDLE;
		}
	}

	void Default::createPipeline(){
		GraphicsPipelineConfigInfo info;
		GraphicsPipelineConfigInfo::defaultInfo(info);

		info.renderPass = _context.baseFramebuffer.renderPass();
		info.pipelineLayout = _layout;
		
		{
			VkPipelineColorBlendAttachmentState attachment = {};
			attachment.blendEnable = VK_FALSE;
			attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			
			info.colorAttachments.push_back(attachment);
		}

		{
			info.dynamicStateEnables.insert(info.dynamicStateEnables.end(), {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR});
		}

		{
			VkPipelineShaderStageCreateInfo frag{};
			frag.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			frag.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
			frag.pName = "main";
			frag.module = _fragmentModule;

			info.stages.push_back(frag);
		}

		{
			VkPipelineShaderStageCreateInfo vert{};
			vert.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vert.stage = VK_SHADER_STAGE_VERTEX_BIT,
			vert.pName = "main";
			vert.module = _vertexModule;

			info.stages.push_back(vert);
		}

		using Vertex = Model::Vertex;
		

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

		info.viewportInfo.viewportCount = 1;
		info.viewportInfo.pViewports = nullptr;
		info.viewportInfo.scissorCount = 1;
		info.viewportInfo.pScissors = nullptr;

		info.update();

		_pipeline = std::make_unique<GraphicsPipeline>(_context, info);
	}
	
	void Default::createShaderModules(){
		createFragmentModule();
		createVertexModule();
	}

	void Default::destroyShaderModules(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_fragmentModule != VK_NULL_HANDLE){
			vkDestroyShaderModule(device.get(), _fragmentModule, allocationCallbacks);
			_fragmentModule = VK_NULL_HANDLE;
		}

		if (_vertexModule != VK_NULL_HANDLE){
			vkDestroyShaderModule(device.get(), _vertexModule, allocationCallbacks);
			_vertexModule = VK_NULL_HANDLE;
		}
	}

	void Default::createFragmentModule(){
		auto code = readFile((std::filesystem::current_path() / "shaders/Default/shader.glsl.frag.spv").string());
		createShaderModule(code, _fragmentModule);
	}

	void Default::createVertexModule(){
		auto code = readFile((std::filesystem::current_path() / "shaders/Default/shader.glsl.vert.spv").string());
		createShaderModule(code, _vertexModule);
	}

	void Default::createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(device.get(), &createInfo, allocationCallbacks, &shaderModule) != VK_SUCCESS){
			spdlog::error("Failed to create shader module");
			throw std::runtime_error("failed to create shader module");
		}
	}

	void Default::bind(VkCommandBuffer commandBuffer){
		_pipeline->bind(commandBuffer);

		const auto& size = _context.window.getSize();

		VkViewport viewport;
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = size.x;
		viewport.height = size.y;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor;
		scissor.extent = {size.x, size.y};
		scissor.offset = {0, 0};

		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	VkPipelineLayout Default::layout() const{
		return _layout;
	}
}