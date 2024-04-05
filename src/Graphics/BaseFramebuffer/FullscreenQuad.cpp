#include <Raindrop/Graphics/BaseFramebuffer/FullscreenQuad.hpp>
#include <Raindrop/Graphics/BaseFramebuffer/Context.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Pipelines/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop/Graphics/Pipelines/LayoutRegistry.hpp>

#include <spdlog/spdlog.h>
#include <fstream>

namespace Raindrop::Graphics::BaseFramebuffer{
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
		_layoutID{Pipelines::INVALID_LAYOUT_ID},
		_pipeline{nullptr},
		_fragmentModule{VK_NULL_HANDLE},
		_vertexModule{VK_NULL_HANDLE}
	{
		spdlog::info("Constructing fullscreen quad baseframebuffer pipeline");

		createPipelineLayout();
		createShaderModules();
		createPipeline();
	}

	VkPipelineLayout FullscreenQuad::getLayout(){
		return _context.renderer.pipelineLayoutRegistry.get(_layoutID)->get();
	}
	
	FullscreenQuad::~FullscreenQuad(){
		spdlog::info("Destroying fullscreen quad baseframebuffer pipeline");

		_pipeline = nullptr;
		destroyShaderModules();
		destroyPipelineLayout();
	}

	void FullscreenQuad::render(VkCommandBuffer commandBuffer){
		_pipeline->bind(commandBuffer);

		VkDescriptorSet set = _context.set.set();
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, getLayout(), 0, 1, &set, 0, nullptr);

		vkCmdDraw(commandBuffer, 6, 1, 0, 0);
	}

	void FullscreenQuad::createPipelineLayout(){
		Pipelines::PipelineLayoutConfigInfo info{};
		info.setLayouts.push_back(_context.set.layout());

		_layoutID = _context.renderer.pipelineLayoutRegistry.create(info);
	}

	void FullscreenQuad::destroyPipelineLayout(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;
		_context.renderer.pipelineLayoutRegistry.destroy(_layoutID);
	}

	void FullscreenQuad::createPipeline(){
		Pipelines::GraphicsPipelineConfigInfo info;
		Pipelines::GraphicsPipelineConfigInfo::defaultInfo(info);

		info.renderPass = _context.renderer.swapchain.renderPass();
		info.pipelineLayoutID = _layoutID;
		
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
		auto code = readFile((std::filesystem::current_path() / "shaders/scene/fullscreenQuad/shader.glsl.frag.spv").string());
		createShaderModule(code, _fragmentModule);
	}

	void FullscreenQuad::createVertexModule(){
		auto code = readFile((std::filesystem::current_path() / "shaders/scene/fullscreenQuad/shader.glsl.vert.spv").string());
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