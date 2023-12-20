#include <Raindrop/Graphics/Shaders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>
#include <Raindrop/Graphics/Shaders/Shader.hpp>
#include <Raindrop/Graphics/Utils/StringToVulkan.hpp>
#include <Raindrop/Graphics/RenderPass/RenderPass.hpp>
#include <Raindrop/Graphics/Buffers/VertexLayout.hpp>
#include <Raindrop/Core/Engine.hpp>

namespace Raindrop::Graphics::Shaders{
	GraphicsPipelineBuilder::GraphicsPipelineBuilder(Context& context) : _context{context}{
		_renderPass = VK_NULL_HANDLE;
		_subpass = 0;

		_vertexInputStateCreateInfo = {};
		_viewportStateCreateInfo = {};
		_rasterizationStateCreateInfo = {};
		_multisampleStateCreateInfo = {};
		_inputAssemblyStateCreateInfo = {};
		_colorBlendStateCreateInfo = {};
		_depthStencilStateCreateInfo = {};
		_tessellationStateCreateInfo = {};
		_dynamicStateCreateInfo = {};
		_vertexLayout = _context.graphics().buffers().vertexdLayoutRegistry().get("default");
		_flags = 0;
	}

	GraphicsPipelineBuilder::~GraphicsPipelineBuilder(){}

	void GraphicsPipelineBuilder::loadFromNode(const YAML::Node& node){
		YAML::Mark mark = node.Mark();
		_context.logger().trace("Loading graphics pipeline at line {}...", mark.line);

		_name = node["name"].as<std::string>();
		YAML::decodeVkPipelineCreateFlags(node["flags"], _flags);

		for (const auto& stage : node["stages"]){
			loadStage(stage);
		}

		loadVertexInputStateCreateInfo(node["inputStateCreateInfo"]);
		loadInputAssemblyStateCreateInfo(node["inputAssemblyStateCreateInfo"]);
		loadTessellationStateCreateInfo(node["tesselationStateCreateInfo"]);
		loadViewportStateCreateInfo(node["viewportStateCreateInfo"]);
		loadRasterizationStateCreateInfo(node["rasterizationStateCreateInfo"]);
		loadMultisampleStateCreateInfo(node["multisampleStateCreateInfo"]);
		loadDepthStencilStateCreateInfo(node["depthStencilStateCreateInfo"]);
		loadColorBlendStateCreateInfo(node["colorBlendStateCreateInfo"]);
		loadDynamicStateCreateInfo(node["dynamicStateCreateInfo"]);

		{
			const YAML::Node& renderPassNode = node["renderPass"];
			try{
				_renderPass = _context.graphics().renderPass().renderPassManager().get(renderPassNode.as<std::string>());
			} catch (const std::exception &e){
				_context.logger().warn("Cannot find \"{}\" render pass at line {}", renderPassNode.as<std::string>("None"), renderPassNode.Mark().line);
			}
		}

		{
			const YAML::Node& layoutNode = node["layout"];
			try{
				_layout = _context.pipelineLayoutManager().get(layoutNode.as<std::string>());
			} catch (const std::exception& e){
				_context.logger().warn("Cannot find \"{}\" layout at line {}", layoutNode.as<std::string>("None"), layoutNode.Mark().line);
			}
		}
	}

	void GraphicsPipelineBuilder::loadStage(const YAML::Node& node){
		YAML::Mark mark = node.Mark();
		auto& registry = _context.graphics().core().registry();

		VkPipelineShaderStageCreateInfo info{};
		std::shared_ptr<Shader> shader;
		
		try{
			shader = _context.graphics().core().loadOrGet<Shader>(registry.get<std::string>(node["shader"].as<std::string>()));
		} catch (const std::exception &e){
			_context.logger().warn("Failed to find the shader \"{}\" line {}", node.as<std::string>("None"), mark.line);
			throw std::runtime_error("Failed to find the shader");
		}

		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		YAML::decodeVkPipelineShaderStageCreateFlags(node["flags"], info.flags);
		info.stage = node["stage"].as<VkShaderStageFlagBits>(shader->stage());
		info.module = shader->get();
		info.pSpecializationInfo = getSpecializationInfo(node["specialization"]);

		std::string name = node["name"].as<std::string>("main");
		_names.push_back(name);
		info.pName = _names.back().c_str();

		_stages.push_back(info);
		_shaders.push_back(shader);
	}

	VkSpecializationInfo* GraphicsPipelineBuilder::getSpecializationInfo(const YAML::Node& node){
		return nullptr;
		// TODO
	}

	void GraphicsPipelineBuilder::loadVertexInputStateCreateInfo(const YAML::Node& node){
		_vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		if (auto layout = node["layout"]){
			try{
				_vertexLayout = _context.graphics().buffers().vertexdLayoutRegistry().get(layout.as<std::string>());
			} catch (const std::exception &e){
				_context.logger().error("Failed to get \"layout\" graphics pipeline inpiut state create info node value at line {}", layout.Mark().line);
				throw std::runtime_error("Failed to find \"layout\" node");
			}

			if (!_vertexLayout){
				_context.logger().error("The layout is note defined");
				throw std::runtime_error("Undefined layout");
			}

			_vertexInputStateCreateInfo.pVertexAttributeDescriptions = _vertexLayout->attributeDescriptions();
			_vertexInputStateCreateInfo.vertexAttributeDescriptionCount = _vertexLayout->attributeCount();

			_vertexInputStateCreateInfo.pVertexBindingDescriptions = _vertexLayout->bindingDescriptions();
			_vertexInputStateCreateInfo.vertexBindingDescriptionCount = _vertexLayout->bindingCount();

		} else {
			for (const auto& attribute : node["vertexAttributeDescriptions"]){
				loadVertexAttribute(attribute);
			}

			for (const auto& binding : node["vertexBindingDescriptions"]){
				loadVertexBinding(binding);
			}

			_vertexInputStateCreateInfo.pVertexAttributeDescriptions = _vertexAttributes.data();
			_vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(_vertexAttributes.size());

			_vertexInputStateCreateInfo.pVertexBindingDescriptions = _vertexBindings.data();
			_vertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(_vertexBindings.size());
		}
	}

	void GraphicsPipelineBuilder::loadVertexAttribute(const YAML::Node& node){
		VkVertexInputBindingDescription binding;

		binding.binding = node["binding"].as<uint32_t>();
		binding.inputRate = node["inputRate"].as<VkVertexInputRate>();
		binding.stride = node["stride"].as<uint32_t>();

		_vertexBindings.push_back(binding);
	}

	void GraphicsPipelineBuilder::loadVertexBinding(const YAML::Node& node){
		VkVertexInputAttributeDescription attribute;

		attribute.location = node["location"].as<uint32_t>();
		attribute.binding = node["binding"].as<uint32_t>();
		attribute.format = node["format"].as<VkFormat>();
		attribute.offset = node["offset"].as<uint32_t>();

		_vertexAttributes.push_back(attribute);
	}

	void GraphicsPipelineBuilder::loadInputAssemblyStateCreateInfo(const YAML::Node& node){
		_inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		_inputAssemblyStateCreateInfo.primitiveRestartEnable = static_cast<VkBool32>(node["primitiveRestartEnable"].as<bool>(false));
		_inputAssemblyStateCreateInfo.topology = node["topology"].as<VkPrimitiveTopology>(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	}

	void GraphicsPipelineBuilder::loadTessellationStateCreateInfo(const YAML::Node& node){
		_tessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		_tessellationStateCreateInfo.patchControlPoints = node["patchControlPoints"].as<uint32_t>(0);
	}

	void GraphicsPipelineBuilder::loadViewportStateCreateInfo(const YAML::Node& node){
		YAML::Mark mark = node.Mark();

		_viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		
		for (const auto& viewport : node["viewports"]){
			loadViewport(viewport);
		}

		for (const auto& scissor : node["scissors"]){
			loadScissor(scissor);
		}

		if (_viewports.size() != _scissors.size()){
			_context.logger().error("The count of viewports and scissors must match ({} != {}) at line", _viewports.size(), _scissors.size(), mark.line);
			throw std::runtime_error("The count of viewports and scissors does not match");
		}

		_viewportStateCreateInfo.pScissors = _scissors.data();
		_viewportStateCreateInfo.scissorCount = static_cast<uint32_t>(_scissors.size());

		_viewportStateCreateInfo.pViewports = _viewports.data();
		_viewportStateCreateInfo.viewportCount = static_cast<uint32_t>(_viewports.size());
	}

	void GraphicsPipelineBuilder::loadViewport(const YAML::Node& node){
		VkViewport viewport;

		viewport.x = node["x"].as<uint32_t>(0);
		viewport.y = node["y"].as<uint32_t>(0);
		viewport.width = node["width"].as<uint32_t>(1080);
		viewport.height = node["height"].as<uint32_t>(720);
		viewport.minDepth = node["minDepth"].as<float>(0.0);
		viewport.maxDepth = node["maxDepth"].as<float>(1.0);

		_viewports.push_back(viewport);
	}

	void GraphicsPipelineBuilder::loadScissor(const YAML::Node& node){
		VkRect2D scissor;

		scissor.extent.width = node["extentWidth"].as<uint32_t>(1080);
		scissor.extent.height = node["extentHeight"].as<uint32_t>(720);
		scissor.offset.x = node["offsetX"].as<uint32_t>(0);
		scissor.offset.y = node["offsetY"].as<uint32_t>(0);

		_scissors.push_back(scissor);
	}

	void GraphicsPipelineBuilder::loadRasterizationStateCreateInfo(const YAML::Node& node){
		_rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

		_rasterizationStateCreateInfo.depthClampEnable = static_cast<VkBool32>(node["depthClampEnable"].as<bool>(false));
		_rasterizationStateCreateInfo.rasterizerDiscardEnable = static_cast<VkBool32>(node["rasterizerDiscardEnable"].as<bool>(false));
		_rasterizationStateCreateInfo.polygonMode = node["polygonMode"].as<VkPolygonMode>(VK_POLYGON_MODE_FILL);
		YAML::decodeVkCullModeFlags(node["cullMode"], _rasterizationStateCreateInfo.cullMode);
		_rasterizationStateCreateInfo.frontFace = node["frontFace"].as<VkFrontFace>(VK_FRONT_FACE_CLOCKWISE);
		_rasterizationStateCreateInfo.depthBiasEnable = static_cast<VkBool32>(node["depthBiasEnable"].as<bool>(true));
		_rasterizationStateCreateInfo.depthBiasConstantFactor = node["depthBiasConstantFactor"].as<float>(0.0);
		_rasterizationStateCreateInfo.depthBiasClamp = node["depthBiasClamp"].as<float>(0.0);
		_rasterizationStateCreateInfo.depthBiasSlopeFactor = node["depthBiasSlopFactor"].as<float>(0.0);
		_rasterizationStateCreateInfo.lineWidth = node["lineWidth"].as<float>(1.0);
	}

	void GraphicsPipelineBuilder::loadMultisampleStateCreateInfo(const YAML::Node& node){
		_multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		_multisampleStateCreateInfo.rasterizationSamples = node["rasterizationSamples"].as<VkSampleCountFlagBits>(VK_SAMPLE_COUNT_1_BIT);
		_multisampleStateCreateInfo.sampleShadingEnable = static_cast<VkBool32>(node["sampleShadingEnable"].as<bool>(false));
		_multisampleStateCreateInfo.minSampleShading = node["minSampleShading"].as<float>(1.0);

		const auto& maskNode = node["sampleMask"];
		if (maskNode.IsNull()){
			_multisampleStateCreateInfo.pSampleMask = nullptr;
		} else {
			std::bitset<256> set(maskNode.as<std::string>());
			
			static constexpr std::size_t maskBitCount = sizeof(VkSampleMask) * 8;
			static constexpr std::size_t maskCount = set.size() / (maskBitCount);

			_sampleMasks = std::make_unique<VkSampleMask[]>(maskCount);

			for (std::size_t i=0; i<maskCount; i++){
				_sampleMasks[i] = (set & std::bitset<256>(~VkSampleMask(0))).to_ulong();
				set << maskBitCount;
			}
			_multisampleStateCreateInfo.pSampleMask = _sampleMasks.get();
		}

		_multisampleStateCreateInfo.pSampleMask = nullptr;
		_multisampleStateCreateInfo.alphaToCoverageEnable = static_cast<VkBool32>(node["alphaToCoverageEnable"].as<bool>(false));
		_multisampleStateCreateInfo.alphaToOneEnable = static_cast<VkBool32>(node["alphaToOneEnable"].as<bool>(false));
	}

	void GraphicsPipelineBuilder::loadDepthStencilStateCreateInfo(const YAML::Node& node){
		_depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		YAML::decodeVkPipelineDepthStencilStateCreateFlags(node["flags"], _depthStencilStateCreateInfo.flags);
		_depthStencilStateCreateInfo.depthTestEnable = static_cast<VkBool32>(node["depthTestEnable"].as<bool>(true));
		_depthStencilStateCreateInfo.depthWriteEnable = static_cast<VkBool32>(node["depthWriteEnable"].as<bool>(true));
		_depthStencilStateCreateInfo.depthCompareOp = node["depthCompareOp"].as<VkCompareOp>(VK_COMPARE_OP_LESS);
		_depthStencilStateCreateInfo.depthBoundsTestEnable = static_cast<VkBool32>(node["depthBoundsTestEnable"].as<bool>(false));
		_depthStencilStateCreateInfo.stencilTestEnable = static_cast<VkBool32>(node["stencilTestEnable"].as<bool>(false));

		loadStencilOpState(node["front"], _depthStencilStateCreateInfo.front);
		loadStencilOpState(node["back"], _depthStencilStateCreateInfo.back);

		_depthStencilStateCreateInfo.minDepthBounds = node["minDepthBounds"].as<float>(0.0);
		_depthStencilStateCreateInfo.maxDepthBounds = node["maxDepthBounds"].as<float>(1.0);
	}

	void GraphicsPipelineBuilder::loadStencilOpState(const YAML::Node& node, VkStencilOpState& state){
		if (!node) return;
		state.failOp = node["failOp"].as<VkStencilOp>();
		state.passOp = node["passOp"].as<VkStencilOp>();
		state.depthFailOp = node["depthFailOp"].as<VkStencilOp>();
		state.compareOp = node["compareOp"].as<VkCompareOp>();
		state.compareMask = node["compareMask"].as<uint32_t>();
		state.writeMask = node["writeMask"].as<uint32_t>();
		state.reference = node["reference"].as<uint32_t>();
	}

	void GraphicsPipelineBuilder::loadColorBlendStateCreateInfo(const YAML::Node& node){
		_colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		YAML::decodeVkPipelineColorBlendStateCreateFlags(node["flags"], _colorBlendStateCreateInfo.flags);
		_colorBlendStateCreateInfo.logicOpEnable = static_cast<VkBool32>(node["logicOpEnable"].as<bool>(false));
		_colorBlendStateCreateInfo.logicOp = node["logicOp"].as<VkLogicOp>(VK_LOGIC_OP_COPY);

		loadBlendConstants(node["blendConstants"]);

		for (const auto& attachment : node["attachments"]){
			loadColorAttachment(attachment);
		}

		_colorBlendStateCreateInfo.pAttachments = _colorAttachments.data();
		_colorBlendStateCreateInfo.attachmentCount = static_cast<uint32_t>(_colorAttachments.size());
	}

	void GraphicsPipelineBuilder::loadBlendConstants(const YAML::Node& node){
		auto& constants = _colorBlendStateCreateInfo.blendConstants;

		constants[0] = node["R"].as<float>(1.0);
		constants[1] = node["G"].as<float>(1.0);
		constants[2] = node["B"].as<float>(1.0);
		constants[3] = node["A"].as<float>(1.0);
	}

	void GraphicsPipelineBuilder::loadColorAttachment(const YAML::Node& node){
		VkPipelineColorBlendAttachmentState attachment{};

		attachment.blendEnable = static_cast<uint32_t>(node["blendEnable"].as<bool>(true));
		attachment.srcColorBlendFactor = node["srcColorBlendFactor"].as<VkBlendFactor>(VK_BLEND_FACTOR_SRC_ALPHA);
		attachment.dstColorBlendFactor = node["dstColorBlendFactor"].as<VkBlendFactor>(VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
		attachment.colorBlendOp = node["colorBlendOp"].as<VkBlendOp>(VK_BLEND_OP_ADD);
		attachment.srcAlphaBlendFactor = node["srcAlphaBlendFactor"].as<VkBlendFactor>(VK_BLEND_FACTOR_ONE);
		attachment.dstAlphaBlendFactor = node["dstAlphaBlendFactor"].as<VkBlendFactor>(VK_BLEND_FACTOR_ZERO);
		attachment.alphaBlendOp = node["alphaBlendOp"].as<VkBlendOp>(VK_BLEND_OP_ADD);

		YAML::decodeVkColorComponentFlags(node["colorWriteMask"], attachment.colorWriteMask);

		_colorAttachments.push_back(attachment);
	}

	void GraphicsPipelineBuilder::loadDynamicStateCreateInfo(const YAML::Node& node){
		YAML::Mark mark = node.Mark();
		_dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		_dynamicStateCreateInfo.flags = 0;
		_dynamicStateCreateInfo.pNext = nullptr;
		
		for (const auto& dynamicStateNode : node["dynamicStates"]){
			VkDynamicState dynamicState;
			try{
				dynamicState = dynamicStateNode.as<VkDynamicState>();
			} catch (const std::exception &e){
				_context.logger().warn("Failed to recognize \"{}\" pipeline dynamic state at line {}", dynamicStateNode.as<std::string>("None"), mark.line);
				continue;
			}
			_dynamicStates.push_back(dynamicState);
		}

		_dynamicStateCreateInfo.pDynamicStates = _dynamicStates.data();
		_dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(_dynamicStates.size());
	}

	const VkPipelineVertexInputStateCreateInfo& GraphicsPipelineBuilder::vertexInputStateCreateInfo() const{
		return _vertexInputStateCreateInfo;
	}

	const VkPipelineViewportStateCreateInfo& GraphicsPipelineBuilder::viewportStateCreateInfo() const{
		return _viewportStateCreateInfo;
	}

	const VkPipelineRasterizationStateCreateInfo& GraphicsPipelineBuilder::rasterizationStateCreateInfo() const{
		return _rasterizationStateCreateInfo;
	}

	const VkPipelineMultisampleStateCreateInfo& GraphicsPipelineBuilder::multisampleStateCreateInfo() const{
		return _multisampleStateCreateInfo;
	}

	const VkPipelineInputAssemblyStateCreateInfo& GraphicsPipelineBuilder::inputAssemblyStateCreateInfo() const{
		return _inputAssemblyStateCreateInfo;
	}

	const VkPipelineColorBlendStateCreateInfo& GraphicsPipelineBuilder::colorBlendStateCreateInfo() const{
		return _colorBlendStateCreateInfo;
	}

	const VkPipelineDepthStencilStateCreateInfo& GraphicsPipelineBuilder::depthStencilStateCreateInfo() const{
		return _depthStencilStateCreateInfo;
	}

	const VkPipelineTessellationStateCreateInfo& GraphicsPipelineBuilder::tessellationStateCreateInfo() const{
		return _tessellationStateCreateInfo;
	}

	const std::string& GraphicsPipelineBuilder::name() const{
		return _name;
	}
	const VkPipelineShaderStageCreateInfo* GraphicsPipelineBuilder::stages() const{
		return _stages.data();
	}

	std::size_t GraphicsPipelineBuilder::stageCount() const{
		return _stages.size();
	}

	const VkPipelineDynamicStateCreateInfo& GraphicsPipelineBuilder::dynamicStateCreateInfo() const{
		return _dynamicStateCreateInfo;
	}

	const std::shared_ptr<PipelineLayout>& GraphicsPipelineBuilder::layout() const{
		return _layout;
	}

	const std::shared_ptr<RenderPass::RenderPass>& GraphicsPipelineBuilder::renderPass() const{
		return _renderPass;
	}

	uint32_t GraphicsPipelineBuilder::subpass() const{
		return _subpass;
	}

	const VkPipelineCreateFlags& GraphicsPipelineBuilder::flags() const{
		return _flags;
	}

	const std::list<std::shared_ptr<Shader>>& GraphicsPipelineBuilder::shaders() const{
		return _shaders;
	}

	const std::shared_ptr<Buffers::VertexLayout> GraphicsPipelineBuilder::vertexLayout() const{
		return _vertexLayout;
	}
}