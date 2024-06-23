#include <Raindrop/Pipeline.hpp>
#include <Raindrop_internal/Graphics/PipelineLayout.hpp>
#include <Raindrop_internal/Pipeline.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/Engine.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/RenderPass.hpp>
#include <Raindrop_internal/RenderPass.hpp>
#include <Raindrop_internal/Vertex.hpp>
#include <Raindrop/CommandBuffer.hpp>
#include <Raindrop_internal/CommandBuffer.hpp>
#include <Raindrop_internal/Operators.hpp>
#include <Raindrop_internal/Graphics/GraphicsPipeline.hpp>
#include <Raindrop_internal/Format.hpp>

#include <fstream>

#define LOGGER _impl->context->getLogger()
#define GRAPHICS_CONTEXT _impl->context->getEngine().getContext()

namespace Raindrop{

	VkStencilOpState StencilOpStateToVulkan(const Pipeline::StencilOpState& state){
		return VkStencilOpState{
			.failOp = stencilOpToVulkan(state.getFailOp()),
			.passOp = stencilOpToVulkan(state.getPassOp()),
			.depthFailOp = stencilOpToVulkan(state.getDepthFailOp()),
			.compareOp = toVulkan(state.getCompareOp()),
			.compareMask = state.getCompareMask(),
			.writeMask = state.getWriteMask(),
			.reference = state.getReference()
		};
	}
	
	//--------------------------------------------------------------------
	//-----------------         Pipeline layout          -----------------
	//--------------------------------------------------------------------

	Pipeline::Layout Pipeline::Layout::Create(Context& context){
		return Layout(context);
	}

	Pipeline::Layout::Layout(Context& context){
		_impl = std::make_unique<Impl>();
		_impl->context = context.getInternalContext();
	}

	Pipeline::Layout::~Layout(){
		_impl.reset();
	}

	void Pipeline::Layout::initialize(){
		LOGGER->info("Initializing pipeline layout...");

		Internal::Graphics::PipelineLayoutConfigInfo info;
		info.flags = pipelineLayoutFlagsToVulkan(_impl->flags);

		if (_impl->pushConstantDescription.has_value()){
			info.pushConstant = VkPushConstantRange{
				.stageFlags = pipelineStageFlagsToVulkan(_impl->pushConstantDescription->stage),
				.offset = 0,
				.size = static_cast<uint32_t>(_impl->pushConstantDescription->size)
			};
		}

		_impl->internal = std::make_shared<Internal::Graphics::PipelineLayout>(GRAPHICS_CONTEXT, info);
		LOGGER->info("Pipeline layout initialized with success !");
	}

	void Pipeline::Layout::release(){
		auto context =_impl->context;

		_impl = std::make_unique<Impl>();
		_impl->context = context;
	}

	Pipeline::Layout& Pipeline::Layout::setFlags(const Flags& flags){
		_impl->flags = flags;
		return *this;
	}

	const Pipeline::Layout::Flags& Pipeline::Layout::getFlags() const noexcept{
		return _impl->flags;
	}

	bool Pipeline::Layout::isInitialized() const noexcept{
		return _impl->internal != nullptr;
	}

	void* Pipeline::Layout::getNativeHandle() const{
		return _impl->internal != nullptr ? static_cast<void*>(_impl->internal->get()) : nullptr;
	}

	Pipeline::Layout::Impl* Pipeline::Layout::getImpl() const noexcept{
		return _impl.get();
	}

	GUID Pipeline::Layout::getGUID() const noexcept{
		// TODO
	}

	Pipeline::Layout& Pipeline::Layout::setPushConstant(const Stage& stage, const std::size_t& size){
		_impl->pushConstantDescription = PushConstantDescription{
			.stage = stage,
			.size = size
		};
		return *this;
	}

	//--------------------------------------------------------------------
	//-----------------             Viewport             -----------------
	//--------------------------------------------------------------------

	Pipeline::Viewport::Viewport(){}

	Pipeline::Viewport& Pipeline::Viewport::setX(const float& x) noexcept{
		_x = x;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setY(const float& y) noexcept{
		_y = y;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setWidth(const float& width) noexcept{
		_width = width;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setHeight(const float& height) noexcept{
		_height = height;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setMinDepth(const float& minDepth) noexcept{
		_minDepth = minDepth;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setMaxDepth(const float& maxDepth) noexcept{
		_maxDepth = maxDepth;
		return *this;
	}


	const float& Pipeline::Viewport::getX() const noexcept{
		return _x;
	}

	const float& Pipeline::Viewport::getY() const noexcept{
		return _y;
	}

	const float& Pipeline::Viewport::getWidth() const noexcept{
		return _width;
	}

	const float& Pipeline::Viewport::getHeight() const noexcept{
		return _height;
	}

	const float& Pipeline::Viewport::getMinDepth() const noexcept{
		return _minDepth;
	}

	const float& Pipeline::Viewport::getMaxDepth() const noexcept{
		return _maxDepth;
	}

	//--------------------------------------------------------------------
	//-----------------             Scissor              -----------------
	//--------------------------------------------------------------------

	Pipeline::Scissor::Scissor(){}

	Pipeline::Scissor& Pipeline::Scissor::setX(const std::int32_t& x) noexcept{
		_x = static_cast<uint32_t>(x);
		return *this;
	}

	Pipeline::Scissor& Pipeline::Scissor::setY(const std::int32_t& y) noexcept{
		_y = static_cast<uint32_t>(y);
		return *this;
	}

	Pipeline::Scissor& Pipeline::Scissor::setWidth(const std::uint32_t& width) noexcept{
		_width = static_cast<uint32_t>(width);
		return *this;
	}

	Pipeline::Scissor& Pipeline::Scissor::setHeight(const std::uint32_t& height) noexcept{
		_height = static_cast<uint32_t>(height);
		return *this;
	}

	const std::int32_t& Pipeline::Scissor::getX() const noexcept{
		return _x;
	}

	const std::int32_t& Pipeline::Scissor::getY() const noexcept{
		return _y;
	}

	const std::uint32_t& Pipeline::Scissor::getWidth() const noexcept{
		return _width;
	}

	const std::uint32_t& Pipeline::Scissor::getHeight() const noexcept{
		return _height;
	}

	//--------------------------------------------------------------------
	//-----------------         Color attachments        -----------------
	//--------------------------------------------------------------------


	Pipeline::ColorAttachment::ColorAttachment() noexcept : 
		_blendEnabled{false},
		_srcColorBlendFactor{Color::BlendFactor::SRC_COLOR},
		_dstColorBlendFactor{Color::BlendFactor::ZERO},
		_colorBlendOp{Color::BlendOperation::ADD},
		_srcAlphaBlendFactor{Color::BlendFactor::ONE},
		_dstAlphaBlendFactor{Color::BlendFactor::ZERO},
		_alphaBlendOp{Color::BlendOperation::ADD},
		_writeMask{Color::Components::RGBA}
	{}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::enableBlending(const bool& enable) noexcept{
		_blendEnabled = enable;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setSrcColorBlendFactor(const Color::BlendFactor& srcColorBlendFactor) noexcept{
		_srcColorBlendFactor = srcColorBlendFactor;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setDstColorBlendFactor(const Color::BlendFactor& dstColorBlendFactor) noexcept{
		_dstColorBlendFactor = dstColorBlendFactor;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setColorBlendOp(const Color::BlendOperation& colorBlendOp) noexcept{
		_colorBlendOp = colorBlendOp;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setSrcAlphaBlendFactor(const Color::BlendFactor& srcAlphaBlendFactor) noexcept{
		_srcAlphaBlendFactor = srcAlphaBlendFactor;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setDstAlphaBlendFactor(const Color::BlendFactor& dstAlphaBlendFactor) noexcept{
		_dstAlphaBlendFactor = dstAlphaBlendFactor;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setAlphaBlendOp(const Color::BlendOperation& alphaBlendOp) noexcept{
		_alphaBlendOp = alphaBlendOp;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setWriteMask(const Color::Components& writeMask) noexcept{
		_writeMask = writeMask;
		return *this;
	}

	const bool& Pipeline::ColorAttachment::isBlendingEnable() const noexcept{
		return _blendEnabled;
	}

	const Color::BlendFactor& Pipeline::ColorAttachment::getSrcColorBlendFactor() const noexcept{
		return _srcColorBlendFactor;
	}

	const Color::BlendFactor& Pipeline::ColorAttachment::getDstColorBlendFactor() const noexcept{
		return _dstColorBlendFactor;
	}

	const Color::BlendOperation& Pipeline::ColorAttachment::getColorBlendOp() const noexcept{
		return _colorBlendOp;
	}

	const Color::BlendFactor& Pipeline::ColorAttachment::getSrcAlphaBlendFactor() const noexcept{
		return _srcAlphaBlendFactor;
	}

	const Color::BlendFactor& Pipeline::ColorAttachment::getDstAlphaBlendFactor() const noexcept{
		return _dstAlphaBlendFactor;
	}

	const Color::BlendOperation& Pipeline::ColorAttachment::getAlphaBlendOp() const noexcept{
		return _alphaBlendOp;
	}

	const Color::Components& Pipeline::ColorAttachment::getWriteMask() const noexcept{
		return _writeMask;
	}
	
	//--------------------------------------------------------------------
	//-----------------        Stencil Op state          -----------------
	//--------------------------------------------------------------------

	Pipeline::StencilOpState::StencilOpState() : 
		_failOp{StencilOperation::KEEP},
		_passOp{StencilOperation::KEEP},
		_depthFailOp{StencilOperation::KEEP},
		_compareOp{CompareOperator::NEVER},
		_compareMask{0},
		_writeMask{0},
		_reference{0}
	{}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setFailOp(const StencilOperation& failOp) noexcept{
		_failOp = failOp;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setPassOp(const StencilOperation& passOp) noexcept{
		_passOp = passOp;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setDepthFailOp(const StencilOperation& depthFailOp) noexcept{
		_depthFailOp = depthFailOp;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setCompareOp(const CompareOperator& compareOp) noexcept{
		_compareOp = compareOp;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setCompareMask(const std::uint32_t& compareMask) noexcept{
		_compareMask = compareMask;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setWriteMask(const std::uint32_t& writeMask) noexcept{
		_writeMask = writeMask;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setReference(const std::uint32_t& reference) noexcept{
		_reference = reference;
		return *this;
	}

	const Pipeline::StencilOperation& Pipeline::StencilOpState::getFailOp() const noexcept{
		return _failOp;
	}

	const Pipeline::StencilOperation& Pipeline::StencilOpState::getPassOp() const noexcept{
		return _passOp;
	}

	const Pipeline::StencilOperation& Pipeline::StencilOpState::getDepthFailOp() const noexcept{
		return _depthFailOp;
	}

	const CompareOperator& Pipeline::StencilOpState::getCompareOp() const noexcept{
		return _compareOp;
	}

	const std::uint32_t& Pipeline::StencilOpState::getCompareMask() const noexcept{
		return _compareMask;
	}

	const std::uint32_t& Pipeline::StencilOpState::getWriteMask() const noexcept{
		return _writeMask;
	}

	const std::uint32_t& Pipeline::StencilOpState::getReference() const noexcept{
		return _reference;
	}


	//--------------------------------------------------------------------
	//-----------------        Pipeline shader           -----------------
	//--------------------------------------------------------------------


	Pipeline::Shader Pipeline::Shader::Create(Context& context, const Path& path){
		return Shader(context, path);
	}

	std::shared_ptr<Pipeline::Shader> Pipeline::Shader::Load(Context& context, const Path& path){
		return Asset::Load<Pipeline::Shader>(context, "Shader", path);
	}

	std::vector<char> readFile(const Path& filepath) {
		std::ifstream file{filepath, std::ios::ate | std::ios::binary};

		if (!file.is_open()) {
			spdlog::warn("Failed to open \"{}\" shader file", filepath.string());
			throw std::runtime_error("failed to open file");
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	void* Pipeline::Shader::getNativeHandle() const{
		return _impl->internal != nullptr ? static_cast<void*>(_impl->internal->get()) : nullptr;
	}

	Pipeline::Shader::Impl* Pipeline::Shader::getImpl() const noexcept{
		return _impl.get();
	}

	bool Pipeline::Shader::isInitialized() const noexcept{
		return _impl->internal != nullptr;
	}

	Pipeline::Shader::Shader(Context& context, const Path& path){
		_impl = std::make_unique<Impl>();
		_impl->context = context.getInternalContext();

		Internal::Graphics::ShaderConfigInfo info;
		info.code = readFile(path);

		_impl->internal = std::make_shared<Internal::Graphics::Shader>(GRAPHICS_CONTEXT, info);
	}
	
	Pipeline::Shader::~Shader(){
		_impl.reset();
	}

	//--------------------------------------------------------------------
	//-----------------            Pipeline              -----------------
	//--------------------------------------------------------------------

	Pipeline Pipeline::Create(Context& context){
		return Pipeline(context);
	}

	Pipeline::Pipeline(Context& context){
		_impl = std::make_unique<Impl>();
		_impl->context = context.getInternalContext();
	}

	Pipeline::~Pipeline(){
		_impl.reset();
	}

	struct NativeTranslation{
		Internal::Graphics::GraphicsPipelineConfigInfo info;

		std::vector<VkRect2D> scissors;
		std::vector<VkViewport> viewports;
		std::vector<VkPipelineColorBlendAttachmentState> attachments;
		std::vector<VkVertexInputAttributeDescription> attributes;
		std::vector<VkVertexInputBindingDescription> bindings;
		std::vector<VkDynamicState> dynamicStates;

		NativeTranslation() :
			info(),
			scissors{},
			viewports{},
			attachments{},
			attributes{},
			bindings{},
			dynamicStates{}
		{
			// Internal::Graphics::GraphicsPipelineConfigInfo::defaultInfo(info);
		}
	};

	void Pipeline::initialize(){
		LOGGER->info("Initializing pipeline...");

		NativeTranslation translation;

		// copy scissors
		translation.scissors.resize(_impl->scissors.size());
		for (std::size_t i=0; i<_impl->scissors.size(); i++){

			const auto& src = _impl->scissors[i];
			translation.scissors[i] = VkRect2D{
				.offset = {
					.x = src.getX(),
					.y = src.getY()
				},
				.extent = {
					.width = src.getWidth(),
					.height = src.getHeight()
				}
			};
		}
		
		// copy viewports
		translation.viewports.resize(_impl->viewports.size());
		for (std::size_t i=0; i<_impl->viewports.size(); i++){

			const auto& src = _impl->viewports[i];
			translation.viewports[i] = VkViewport{
				.x = src.getX(),
				.y = src.getY(),
				.width = src.getWidth(),
				.height = src.getHeight(),
				.minDepth = src.getMinDepth(),
				.maxDepth = src.getMaxDepth()
			};
		}
		
		// copy attachment descriptions
		translation.attachments.resize(_impl->attachments.size());
		for (std::size_t i=0; i<_impl->attachments.size(); i++){

			const auto& src = _impl->attachments[i];
			translation.attachments[i] = VkPipelineColorBlendAttachmentState{
				.blendEnable = toVulkan(src.isBlendingEnable()),
				.srcColorBlendFactor = BlendFactorToVulkan(src.getSrcColorBlendFactor()),
				.dstColorBlendFactor = BlendFactorToVulkan(src.getDstColorBlendFactor()),
				.colorBlendOp = BlendOperationToVulkan(src.getColorBlendOp()),
				.srcAlphaBlendFactor = BlendFactorToVulkan(src.getSrcAlphaBlendFactor()),
				.dstAlphaBlendFactor = BlendFactorToVulkan(src.getDstAlphaBlendFactor()),
				.alphaBlendOp = BlendOperationToVulkan(src.getAlphaBlendOp()),
				.colorWriteMask = ColorComponentsFlagsToVulkan(src.getWriteMask())
			};
		}

		uint32_t bindingId = 0;
		for (const auto& binding : _impl->bindings){

			translation.bindings.push_back(VkVertexInputBindingDescription{
				.binding = bindingId,
				.stride = static_cast<uint32_t>(binding.first.getStride()),
				.inputRate = VertexInputRateToVulkan(binding.second)
			});

			uint32_t location = 0;
			const auto& layout = binding.first;
			for (const auto& attribute : layout.getAttributes()){

				translation.attributes.push_back(VkVertexInputAttributeDescription{
					.location = location,
					.binding = bindingId,
					.format = FormatToVulkan(attribute.format.get()),
					.offset = static_cast<uint32_t>(attribute.offset)
				});

				location++;
			}

			bindingId++;
		}

		translation.dynamicStates.resize(_impl->dynamicStates.size());
		for (std::size_t i=0; i<_impl->dynamicStates.size(); i++){
			translation.dynamicStates[i] = dynamicStateToVulkan(_impl->dynamicStates[i]);
		}

		translation.info = Internal::Graphics::GraphicsPipelineConfigInfo{
			.flags = PipelineCreateFlagsToVulkan(_impl->flags),
			.vertexInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.vertexBindingDescriptionCount = static_cast<uint32_t>(translation.bindings.size()),
				.pVertexBindingDescriptions = translation.bindings.data(),
				.vertexAttributeDescriptionCount = static_cast<uint32_t>(translation.attributes.size()),
				.pVertexAttributeDescriptions = translation.attributes.data()
			},
			.viewportInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.viewportCount = static_cast<uint32_t>(translation.viewports.size()),
				.pViewports = translation.viewports.data(),
				.scissorCount = static_cast<uint32_t>(translation.scissors.size()),
				.pScissors = translation.scissors.data()
			},
			.inputAssemblyInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.topology = primitiveTopologyToVulkan(_impl->primitiveTopology),
				.primitiveRestartEnable = toVulkan(_impl->primitiveRestartEnable)
			},
			.rasterizationInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.depthClampEnable = toVulkan(_impl->depthClampEnable),
				.rasterizerDiscardEnable = toVulkan(_impl->rasterizationDiscardEnable),
				.polygonMode = polygonModeToVulkan(_impl->polygonMode),
				.cullMode = cullModeFlagsToVulkan(_impl->cullMode),
				.frontFace = frontFaceToVulkan(_impl->frontFace),
				.depthBiasEnable = toVulkan(_impl->depthBiasEnable),
				.depthBiasConstantFactor = _impl->depthBiasConstantFactor,
				.depthBiasClamp = _impl->depthBiasClamp,
				.depthBiasSlopeFactor = _impl->depthBiasSlopeFactor,
				.lineWidth = _impl->lineWidth
			},
			.multisampleInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.rasterizationSamples = sampleCountFlagsBitsToVulkan(_impl->sampleCount),
				.sampleShadingEnable = toVulkan(_impl->sampleShadingEnable),
				.minSampleShading = _impl->minSampleShading,
				.pSampleMask = nullptr,
				.alphaToCoverageEnable = toVulkan(_impl->alphaToCoverageEnable),
				.alphaToOneEnable = toVulkan(_impl->alphaToOneEnable)
			},
			.colorBlendInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.logicOpEnable = toVulkan(_impl->blendLogicOpertionEnable),
				.logicOp = toVulkan(_impl->blendLogicOperation),
				.attachmentCount = static_cast<uint32_t>(translation.attachments.size()),
				.pAttachments = translation.attachments.data(),
				.blendConstants = {
					_impl->blendConstants[0],
					_impl->blendConstants[1],
					_impl->blendConstants[2],
					_impl->blendConstants[3]
				}
			},
			.depthStencilInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.depthTestEnable = toVulkan(_impl->depthTestEnable),
				.depthWriteEnable = toVulkan(_impl->depthWriteEnable),
				.depthCompareOp = toVulkan(_impl->depthCompareOperation),
				.depthBoundsTestEnable = toVulkan(_impl->depthBoundsTestEnable),
				.stencilTestEnable = toVulkan(_impl->stencilTestEnable),
				.front = StencilOpStateToVulkan(_impl->frontStencilOpertions),
				.back = StencilOpStateToVulkan(_impl->backStencilOpertions),
				.minDepthBounds = _impl->minDepthBounds,
				.maxDepthBounds = _impl->maxDepthBounds
			},
			.dynamicStateInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.dynamicStateCount = static_cast<uint32_t>(translation.dynamicStates.size()),
				.pDynamicStates = translation.dynamicStates.data()
			},
			.tessellationInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.patchControlPoints = static_cast<uint32_t>(_impl->tessellationPatchControlPoints)
			},
			.subpass = static_cast<uint32_t>(_impl->subpass),
			.pNext = nullptr
		};

		for (const auto& stage : _impl->stages){
			auto shader = stage.shader->getImpl()->internal;

			translation.info.shaders.push_back(shader);
			translation.info.stages.push_back(VkPipelineShaderStageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = pipelineShaderStageCreateFlagsToVulkan(stage.flags),
				.stage = shaderStageFlagsBits(stage.stage),
				.module = shader->get(),
				.pName = stage.entryPoint.c_str(),
				.pSpecializationInfo = nullptr
			});
		}

		translation.info.pipelineLayout = _impl->layout->getImpl()->internal;
		translation.info.renderPass = _impl->renderPass->getImpl()->internal;

		_impl->internal = std::make_shared<Internal::Graphics::GraphicsPipeline>(GRAPHICS_CONTEXT, translation.info);
		LOGGER->info("Pipeline initialized with success !");
	}

	void Pipeline::release(){
		auto context = _impl->context;

		_impl = std::make_unique<Impl>();
		_impl->context = context;
	}

	bool Pipeline::isInitialized() const noexcept{
		return _impl->internal != nullptr;
	}
	
	void* Pipeline::getNativeHandle() const{
		return _impl->internal != nullptr ? static_cast<void*>(_impl->internal->get()) : nullptr;
	}
	
	Pipeline::Impl* Pipeline::getImpl() const noexcept{
		return _impl.get();
	}
	
	GUID Pipeline::getGUID() const noexcept{

	}

	Pipeline& Pipeline::setFlags(const Flags& flags){
		_impl->flags = flags;
		return *this;
	}

	Pipeline& Pipeline::setRenderPass(const RenderPass& renderPass){
		_impl->renderPass = &renderPass;
		return *this;
	}

	Pipeline& Pipeline::setLayout(const Layout& layout){
		_impl->layout = &layout;
		return *this;
	}

	Pipeline& Pipeline::setSubpass(const uint32_t& subpass){
		_impl->subpass = subpass;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::addColorAttachment(){
		return _impl->attachments.emplace_back();
	}

	Vertex::Layout& Pipeline::addVertexBinding(const Vertex::InputRate& inputRate){
		return _impl->bindings.emplace_back(Vertex::Layout(), inputRate).first;
	}

	Pipeline::Viewport& Pipeline::addViewport(){
		return _impl->viewports.emplace_back();
	}

	Pipeline::Scissor& Pipeline::addScissor(){
		return _impl->scissors.emplace_back();
	}

	Pipeline& Pipeline::setPrimitiveTopology(const Topology& topology){
		_impl->primitiveTopology = topology;
		return *this;
	}

	Pipeline& Pipeline::enablePrimitiveRestart(const bool& enable){
		_impl->primitiveRestartEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::enableDepthClamp(const bool& enable){
		_impl->depthClampEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::enableRasterizerDiscard(const bool& enable){
		_impl->rasterizationDiscardEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::setPolygonMode(const PolygonMode& mode){
		_impl->polygonMode = mode;
		return *this;
	}

	Pipeline& Pipeline::setCullMode(const CullMode& mode){
		_impl->cullMode = mode;
		return *this;
	}

	Pipeline& Pipeline::setFrontFace(const FrontFace& frontFace){
		_impl->frontFace = frontFace;
		return *this;
	}

	Pipeline& Pipeline::enableDepthBias(const bool& enable){
		_impl->depthBiasEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::setDepthBiasConstant(const float& constant){
		_impl->depthBiasConstantFactor = constant;
		return *this;
	}

	Pipeline& Pipeline::setDepthBiasSlope(const float& slope){
		_impl->depthBiasSlopeFactor = slope;
		return *this;
	}

	Pipeline& Pipeline::setDepthBiasClamp(const float& clamp){
		_impl->depthBiasClamp = clamp;
		return *this;
	}

	Pipeline& Pipeline::setLineWidth(const float& width){
		_impl->lineWidth = width;
		return *this;
	}

	Pipeline& Pipeline::setSampleCount(const SampleCount::Bits& count){
		_impl->sampleCount = count;
		return *this;
	}

	Pipeline& Pipeline::enableSampleShading(const bool& enable){
		_impl->sampleShadingEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::setMinimumSampleShading(const float& min){
		_impl->minSampleShading = min;
		return *this;
	}

	Pipeline& Pipeline::enableAlphaToCoverage(const bool& enable){
		_impl->alphaToCoverageEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::enableAlphaToOne(const bool& enable){
		_impl->alphaToOneEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::setColorBlendFlags(const ColorBlendFlags& flags){
		_impl->colorBlendFlags = flags;
		return *this;
	}

	Pipeline& Pipeline::enableBlendLogicOperation(const bool& enable){
		_impl->blendLogicOpertionEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::setBlendLogicOperation(const LogicOperator& op){
		_impl->blendLogicOperation = op;
		return *this;
	}

	Pipeline& Pipeline::setBlendConstant(const Color::Components::Bits& component, const float& constant){
		std::size_t id=0;
		switch (component){
			case Color::Components::RED: id = 0; break;
			case Color::Components::GREEN: id = 1; break;
			case Color::Components::BLUE: id = 2; break;
			case Color::Components::ALPHA: id = 3; break;
		}

		_impl->blendConstants[id] = constant;
		return *this;
	}

	Pipeline& Pipeline::enableDepthTest(const bool& enable){
		_impl->depthTestEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::enableDepthWrite(const bool& enable){
		_impl->depthWriteEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::setDepthCompareOp(const CompareOperator& op){
		_impl->depthCompareOperation = op;
		return *this;
	}

	Pipeline& Pipeline::enableDepthBoundsTest(const bool& enable){
		_impl->depthBoundsTestEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::enableStencilTest(const bool& enable){
		_impl->stencilTestEnable = enable;
		return *this;
	}

	Pipeline& Pipeline::setMaxDepthBounds(const float& max){
		_impl->maxDepthBounds = max;
		return *this;
	}

	Pipeline& Pipeline::setMinDepthBounds(const float& min){
		_impl->minDepthBounds = min;
		return *this;
	}

	Pipeline& Pipeline::addDynamicState(const DynamicState& state){
		_impl->dynamicStates.push_back(state);
		return *this;
	}

	Pipeline& Pipeline::addDynamicStates(const std::initializer_list<DynamicState>& states){
		for (auto& state : states){
			_impl->dynamicStates.push_back(state);
		}
		return *this;
	}

	Pipeline& Pipeline::setTessellationPatchControlPoints(const std::uint32_t& count){
		_impl->tessellationPatchControlPoints = count;
		return *this;
	}

	Pipeline& Pipeline::addStage(const std::shared_ptr<Shader>& shader, const Shader::Stage::Bits& stage, const char* entryPoint, const Shader::Flags& flags){
		_impl->stages.push_back({shader, entryPoint, stage, flags});
		return *this;
	}

	Pipeline& Pipeline::setFrontStencilOpertions(const StencilOpState& operations){
		_impl->frontStencilOpertions = operations;
		return *this;
	}

	Pipeline& Pipeline::setBackStencilOpertions(const StencilOpState& operations){
		_impl->backStencilOpertions = operations;
		return *this;
	}
	
	void Pipeline::bind(CommandBuffer& commandBuffer){
		_impl->internal->bind(commandBuffer.getImpl()->commandBuffer);
	}
}