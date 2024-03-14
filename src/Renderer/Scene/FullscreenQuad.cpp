#include <Raindrop/Renderer/Scene/FullscreenQuad.hpp>
#include <Raindrop/Renderer/Context.hpp>
#include <Raindrop/Renderer/Pipelines/GraphicsPipeline.hpp>

#include <spdlog/spdlog.h>
#include <fstream>

namespace Raindrop::Renderer::Scene{
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

	FullscreenQuad::FullscreenQuad(Context& context) : 
		_context{context},
		_layout{VK_NULL_HANDLE},
		_pipeline{nullptr},
		_fragmentModule{VK_NULL_HANDLE},
		_vertexModule{VK_NULL_HANDLE}
	{
		spdlog::info("Creating fullscreen quad scene pipeline");

		createPipelineLayout();
		createShaderModules();
		createPipeline();
	}
	
	FullscreenQuad::~FullscreenQuad(){
		spdlog::info("Destroying fullscreen quad scene pipeline");

		_pipeline = nullptr;
		destroyShaderModules();
		destroyPipelineLayout();
	}

	void FullscreenQuad::render(VkCommandBuffer commandBuffer){
		_pipeline->bind(commandBuffer);

		VkDescriptorSet set = _context.set.set();
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _layout, 0, 1, &set, 0, nullptr);

		vkCmdDraw(commandBuffer, 6, 1, 0, 0);
	}

	void FullscreenQuad::createPipelineLayout(){
		VkPipelineLayoutCreateInfo info = {};

		info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.pushConstantRangeCount = 0;

		VkDescriptorSetLayout layout = _context.set.layout();
		info.pSetLayouts = &layout; 
		info.setLayoutCount = 1;

		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (vkCreatePipelineLayout(device.get(), &info, allocationCallbacks, &_layout) != VK_SUCCESS){
			spdlog::error("Failed to create scene fullscreen quad pipeline layout");
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void FullscreenQuad::destroyPipelineLayout(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_layout != VK_NULL_HANDLE){
			vkDestroyPipelineLayout(device.get(), _layout, allocationCallbacks);
			_layout = VK_NULL_HANDLE;
		}
	}

	void FullscreenQuad::createPipeline(){
		Pipelines::GraphicsPipelineConfigInfo info;
		Pipelines::GraphicsPipelineConfigInfo::defaultInfo(info);

		info.renderPass = _context.renderer.swapchain.renderPass();
		info.pipelineLayout = _layout;
		
		{
			VkPipelineColorBlendAttachmentState attachment = {};
			attachment.blendEnable = VK_FALSE;
			attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT;
			
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

		info.viewportInfo.viewportCount = 1;
		info.viewportInfo.pViewports = nullptr;
		info.viewportInfo.scissorCount = 1;
		info.viewportInfo.pScissors = nullptr;

		info.update();

		_pipeline = std::make_unique<Pipelines::GraphicsPipeline>(_context.renderer, info);
	}

	void FullscreenQuad::createShaderModules(){
		createFragmentModule();
		createVertexModule();
	}

	void FullscreenQuad::destroyShaderModules(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_fragmentModule != VK_NULL_HANDLE){
			vkDestroyShaderModule(device.get(), _fragmentModule, allocationCallbacks);
			_fragmentModule = VK_NULL_HANDLE;
		}

		if (_vertexModule != VK_NULL_HANDLE){
			vkDestroyShaderModule(device.get(), _vertexModule, allocationCallbacks);
			_vertexModule = VK_NULL_HANDLE;
		}
	}

	void FullscreenQuad::createFragmentModule(){
		auto code = readFile(std::filesystem::current_path() / "shaders/scene/fullscreenQuad/shader.glsl.frag.spv");
		createShaderModule(code, _fragmentModule);
	}

	void FullscreenQuad::createVertexModule(){
		auto code = readFile(std::filesystem::current_path() / "shaders/scene/fullscreenQuad/shader.glsl.vert.spv");
		createShaderModule(code, _vertexModule);
	}

	void FullscreenQuad::createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(device.get(), &createInfo, allocationCallbacks, &shaderModule) != VK_SUCCESS){
			spdlog::error("Failed to create shader module");
			throw std::runtime_error("failed to create shader module");
		}
	}
}