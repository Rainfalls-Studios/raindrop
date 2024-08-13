#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/RenderPass.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Graphics/VertexLayout.hpp>
#include <Raindrop/Graphics/CommandBuffer.hpp>
#include <Raindrop/Graphics/PipelineLayout.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Graphics{
	std::shared_ptr<GraphicsPipeline> GraphicsPipeline::create(Raindrop::Context& context){
		return context.registry.emplace<GraphicsPipeline>();
	}

	// =========================== VIEWPORT ========================

	GraphicsPipeline::Viewport::Viewport(VkViewport& viewport) noexcept : _viewport{&viewport}{}

	GraphicsPipeline::Viewport::Viewport(Viewport&& other) noexcept : _viewport{nullptr}{
		swap(*this, other);
	}

	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::operator=(Viewport&& other) noexcept{
		swap(*this, other);
		return *this;
	}

	void swap(GraphicsPipeline::Viewport& A, GraphicsPipeline::Viewport& B) noexcept{
		std::swap(A._viewport, B._viewport);
	}


	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::setX(const float& x) noexcept{
		_viewport->x = x;
		return *this;
	}

	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::setY(const float& y) noexcept{
		_viewport->y = y;
		return *this;
	}

	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::setPosition(const float& x, const float& y) noexcept{
		setX(x);
		setY(y);
		return *this;
	}

	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::setWidth(const float& width) noexcept{
		_viewport->width = width;
		return *this;
	}

	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::setHeight(const float& height) noexcept{
		_viewport->height = height;
		return *this;
	}

	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::setSize(const float& width, const float& height) noexcept{
		setWidth(width);
		setHeight(height);
		return *this;
	}

	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::setMinDepth(const float& minDepth) noexcept{
		_viewport->minDepth = minDepth;
		return *this;
	}

	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::setMaxDepth(const float& maxDepth) noexcept{
		_viewport->maxDepth = maxDepth;
		return *this;
	}

	GraphicsPipeline::Viewport& GraphicsPipeline::Viewport::setDepthBounds(const float& min, const float& max) noexcept{
		setMinDepth(min);
		setMaxDepth(max);
		return *this;
	}

	VkViewport& GraphicsPipeline::Viewport::get() noexcept{
		return *_viewport;
	}


	// =========================== SCISSOR ========================


	GraphicsPipeline::Scissor::Scissor(VkRect2D& rect) noexcept : _rect{&rect}{}

	GraphicsPipeline::Scissor::Scissor(Scissor&& other) noexcept : _rect{nullptr}{
		swap(*this, other);
	}

	GraphicsPipeline::Scissor& GraphicsPipeline::Scissor::operator=(Scissor&& other) noexcept{
		swap(*this, other);
		return *this;
	}

	void swap(GraphicsPipeline::Scissor& A, GraphicsPipeline::Scissor& B) noexcept{
		std::swap(A._rect, B._rect);
	}

	GraphicsPipeline::Scissor& GraphicsPipeline::Scissor::setX(const std::int32_t& x) noexcept{
		_rect->offset.x = x;
		return *this;
	}

	GraphicsPipeline::Scissor& GraphicsPipeline::Scissor::setY(const std::int32_t& y) noexcept{
		_rect->offset.y = y;
		return *this;
	}

	GraphicsPipeline::Scissor& GraphicsPipeline::Scissor::setPosition(const std::int32_t& x, const std::int32_t& y) noexcept{
		setX(x);
		setY(y);
		return *this;
	}

	GraphicsPipeline::Scissor& GraphicsPipeline::Scissor::setWidth(const std::uint32_t& width) noexcept{
		_rect->extent.width = width;
		return *this;
	}

	GraphicsPipeline::Scissor& GraphicsPipeline::Scissor::setHeight(const std::uint32_t& height) noexcept{
		_rect->extent.height = height;
		return *this;
	}

	GraphicsPipeline::Scissor& GraphicsPipeline::Scissor::setSize(const std::uint32_t& width, const std::uint32_t& height) noexcept{
		setWidth(width);
		setHeight(height);
		return *this;
	}

	VkRect2D& GraphicsPipeline::Scissor::get() noexcept{
		return *_rect;
	}


	// =========================== COLOR ATTACHMENT ========================


	GraphicsPipeline::ColorAttachment::ColorAttachment(VkPipelineColorBlendAttachmentState& description) noexcept : _description{description} {}

	GraphicsPipeline::ColorAttachment& GraphicsPipeline::ColorAttachment::enableBlending(const bool& enable) noexcept{
		_description.blendEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::ColorAttachment& GraphicsPipeline::ColorAttachment::setSrcColorBlendFactor(const VkBlendFactor& srcColorBlendFactor) noexcept{
		_description.srcColorBlendFactor = srcColorBlendFactor;
		return *this;
	}

	GraphicsPipeline::ColorAttachment& GraphicsPipeline::ColorAttachment::setDstColorBlendFactor(const VkBlendFactor& dstColorBlendFactor) noexcept{
		_description.dstColorBlendFactor = dstColorBlendFactor;
		return *this;
	}

	GraphicsPipeline::ColorAttachment& GraphicsPipeline::ColorAttachment::setColorBlendOp(const VkBlendOp& colorBlendOp) noexcept{
		_description.colorBlendOp = colorBlendOp;
		return *this;
	}

	GraphicsPipeline::ColorAttachment& GraphicsPipeline::ColorAttachment::setSrcAlphaBlendFactor(const VkBlendFactor& srcAlphaBlendFactor) noexcept{
		_description.srcAlphaBlendFactor = srcAlphaBlendFactor;
		return *this;
	}

	GraphicsPipeline::ColorAttachment& GraphicsPipeline::ColorAttachment::setDstAlphaBlendFactor(const VkBlendFactor& dstAlphaBlendFactor) noexcept{
		_description.dstAlphaBlendFactor = dstAlphaBlendFactor;
		return *this;
	}

	GraphicsPipeline::ColorAttachment& GraphicsPipeline::ColorAttachment::setAlphaBlendOp(const VkBlendOp& alphaBlendOp) noexcept{
		_description.alphaBlendOp = alphaBlendOp;
		return *this;
	}

	GraphicsPipeline::ColorAttachment& GraphicsPipeline::ColorAttachment::setWriteMask(const VkColorComponentFlags& writeMask) noexcept{
		_description.colorWriteMask = writeMask;
		return *this;
	}

	VkPipelineColorBlendAttachmentState& GraphicsPipeline::ColorAttachment::get() noexcept{
		return _description;
	}

	// =========================== STENCIL OP STATE ========================

	GraphicsPipeline::StencilOpState::StencilOpState(VkStencilOpState& state) noexcept : _state{state}{}


	GraphicsPipeline::StencilOpState& GraphicsPipeline::StencilOpState::setFailOp(const VkStencilOp& failOp) noexcept{
		_state.failOp = failOp;
		return *this;
	}

	GraphicsPipeline::StencilOpState& GraphicsPipeline::StencilOpState::setPassOp(const VkStencilOp& passOp) noexcept{
		_state.passOp = passOp;
		return *this;
	}

	GraphicsPipeline::StencilOpState& GraphicsPipeline::StencilOpState::setDepthFailOp(const VkStencilOp& depthFailOp) noexcept{
		_state.depthFailOp = depthFailOp;
		return *this;
	}

	GraphicsPipeline::StencilOpState& GraphicsPipeline::StencilOpState::setCompareOp(const VkCompareOp& compareOp) noexcept{
		_state.compareOp = compareOp;
		return *this;
	}

	GraphicsPipeline::StencilOpState& GraphicsPipeline::StencilOpState::setCompareMask(const std::uint32_t& compareMask) noexcept{
		_state.compareMask = compareMask;
		return *this;
	}

	GraphicsPipeline::StencilOpState& GraphicsPipeline::StencilOpState::setWriteMask(const std::uint32_t& writeMask) noexcept{
		_state.writeMask = writeMask;
		return *this;
	}

	GraphicsPipeline::StencilOpState& GraphicsPipeline::StencilOpState::setReference(const std::uint32_t& reference) noexcept{
		_state.reference = reference;
		return *this;
	}

	VkStencilOpState& GraphicsPipeline::StencilOpState::get() noexcept{
		return _state;
	}

	// =========================== VERTEX ASSEMBLY STATE ========================

	GraphicsPipeline::VertexInputState::VertexInputState() noexcept : 
		_info{nullptr},
		_data{nullptr}
	{}

	GraphicsPipeline::VertexInputState::VertexInputState(VkPipelineVertexInputStateCreateInfo& info, BuildInfo::VertexInputData& data) noexcept : 
		_info{&info},
		_data{&data}
	{}

	GraphicsPipeline::VertexInputState& GraphicsPipeline::VertexInputState::setFlags(const VkPipelineVertexInputStateCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	GraphicsPipeline::VertexInputState& GraphicsPipeline::VertexInputState::addBindingDescriptions(const std::initializer_list<VkVertexInputBindingDescription>& descriptions){
		auto& info = getInfo();
		auto& data = getData();

		data.vertexBindingDescriptions.insert(
			data.vertexBindingDescriptions.end(),
			descriptions
		);

		info.pVertexBindingDescriptions = data.vertexBindingDescriptions.data();
		info.vertexBindingDescriptionCount = static_cast<uint32_t>(data.vertexBindingDescriptions.size());

		return *this;
	}

	GraphicsPipeline::VertexInputState& GraphicsPipeline::VertexInputState::addAttributeDescriptions(const std::initializer_list<VkVertexInputAttributeDescription>& descriptions){
		auto& info = getInfo();
		auto& data = getData();

		data.vertexAttributeDescription.insert(
			data.vertexAttributeDescription.end(),
			descriptions
		);

		info.pVertexAttributeDescriptions = data.vertexAttributeDescription.data();
		info.vertexAttributeDescriptionCount = static_cast<uint32_t>(data.vertexAttributeDescription.size());

		return *this;
	}

	GraphicsPipeline::VertexInputState& GraphicsPipeline::VertexInputState::setLayout(const VertexLayout& layout){
		auto& info = getInfo();
		auto& data = getData();

		data.vertexAttributeDescription = layout.getAttributeDescriptions();
		data.vertexBindingDescriptions = layout.getBindingDescriptions();

		info.pVertexAttributeDescriptions = data.vertexAttributeDescription.data();
		info.vertexAttributeDescriptionCount = static_cast<uint32_t>(data.vertexAttributeDescription.size());

		info.pVertexBindingDescriptions = data.vertexBindingDescriptions.data();
		info.vertexBindingDescriptionCount = static_cast<uint32_t>(data.vertexBindingDescriptions.size());
	
		return *this;
	}

	VkPipelineVertexInputStateCreateInfo& GraphicsPipeline::VertexInputState::get(){
		return getInfo();
	}

	VkPipelineVertexInputStateCreateInfo& GraphicsPipeline::VertexInputState::getInfo(){
		if (!_info) throw std::runtime_error("Invalid vertex input state");
		return *_info;
	}

	GraphicsPipeline::BuildInfo::VertexInputData& GraphicsPipeline::VertexInputState::getData(){
		if (!_data) throw std::runtime_error("Invalid vertex input state");
		return *_data;
	}


	// =========================== INPUT ASSEMBLY STATE ========================

	GraphicsPipeline::InputAssemblyState::InputAssemblyState() noexcept : _info{nullptr}{}
	GraphicsPipeline::InputAssemblyState::InputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& info) noexcept : _info{&info}{}

	GraphicsPipeline::InputAssemblyState& GraphicsPipeline::InputAssemblyState::setFlags(const VkPipelineInputAssemblyStateCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	GraphicsPipeline::InputAssemblyState& GraphicsPipeline::InputAssemblyState::setPrimitiveTopology(const VkPrimitiveTopology& topology){
		getInfo().topology = topology;
		return *this;
	}

	GraphicsPipeline::InputAssemblyState& GraphicsPipeline::InputAssemblyState::enablePrimitiveRestart(const bool& enable){
		getInfo().primitiveRestartEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	VkPipelineInputAssemblyStateCreateInfo& GraphicsPipeline::InputAssemblyState::get(){
		return getInfo();
	}

	VkPipelineInputAssemblyStateCreateInfo& GraphicsPipeline::InputAssemblyState::getInfo(){
		if (!_info) throw std::runtime_error("Invalid input assembly state");
		return *_info;
	}


	// =========================== TESSELLATION STATE ========================

	GraphicsPipeline::TessellationState::TessellationState() noexcept : _info{nullptr}{}
	GraphicsPipeline::TessellationState::TessellationState(VkPipelineTessellationStateCreateInfo& info) noexcept : _info{&info}{}
					
	GraphicsPipeline::TessellationState& GraphicsPipeline::TessellationState::setFlags(const VkPipelineTessellationStateCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	GraphicsPipeline::TessellationState& GraphicsPipeline::TessellationState::setPatchControlPoints(const std::uint32_t& points){
		getInfo().patchControlPoints = points;
		return *this;
	}

	VkPipelineTessellationStateCreateInfo& GraphicsPipeline::TessellationState::get(){
		return getInfo();
	}

	VkPipelineTessellationStateCreateInfo& GraphicsPipeline::TessellationState::getInfo(){
		if (!_info) throw std::runtime_error("Invalid tessellation state");
		return *_info;
	}


	// =========================== VIEWPORT STATE ========================

	GraphicsPipeline::ViewportState::ViewportState() noexcept : 
		_info{nullptr},
		_data{nullptr}
	{}

	GraphicsPipeline::ViewportState::ViewportState(VkPipelineViewportStateCreateInfo& info, BuildInfo::ViewportData& data) noexcept : 
		_info{&info},
		_data{&data}
	{}

	GraphicsPipeline::ViewportState& GraphicsPipeline::ViewportState::setFlags(const VkPipelineViewportStateCreateFlags& flags) noexcept{
		getInfo().flags = flags;
		return *this;
	}
	
	GraphicsPipeline::Viewport GraphicsPipeline::ViewportState::addViewport(){
		auto& info = getInfo();
		auto& data = getData();

		VkViewport& viewport = data.viewports.emplace_back();

		info.viewportCount = static_cast<uint32_t>(data.viewports.size());
		info.pViewports = data.viewports.data();

		return std::move(Viewport(viewport));
	}

	GraphicsPipeline::Scissor GraphicsPipeline::ViewportState::addScissor(){
		auto& info = getInfo();
		auto& data = getData();

		VkRect2D& scissor = data.scissors.emplace_back();

		info.scissorCount = static_cast<uint32_t>(data.scissors.size());
		info.pScissors = data.scissors.data();

		return std::move(Scissor(scissor));
	}

	VkPipelineViewportStateCreateInfo& GraphicsPipeline::ViewportState::get(){
		return getInfo();
	}
	
	VkPipelineViewportStateCreateInfo& GraphicsPipeline::ViewportState::getInfo(){
		if (!_info) throw std::runtime_error("Invalid viewport state");
		return *_info;
	}

	GraphicsPipeline::BuildInfo::ViewportData& GraphicsPipeline::ViewportState::getData(){
		if (!_data) throw std::runtime_error("Invalid viewport state");
		return *_data;
	}


	// =========================== RASTERIZATION STATE ========================

	GraphicsPipeline::RasterizationState::RasterizationState() noexcept : _info{nullptr}{}
	GraphicsPipeline::RasterizationState::RasterizationState(VkPipelineRasterizationStateCreateInfo& info) noexcept : _info{&info}{}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::setFlags(const VkPipelineRasterizationStateCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::enableDepthClamp(const bool& enable){
		getInfo().depthClampEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::enableRasterizerDiscard(const bool& enable){
		getInfo().rasterizerDiscardEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::setPolygoneMode(const VkPolygonMode& mode){
		getInfo().polygonMode = mode;
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::setCullMode(const VkCullModeFlags& mode){
		getInfo().cullMode = mode;
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::setFrontFace(const VkFrontFace& frontFace){
		getInfo().frontFace = frontFace;
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::enableDepthBias(const bool& enable){
		getInfo().depthBiasEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::setDepthBiasConstant(const float& factor){
		getInfo().depthBiasConstantFactor = factor;
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::setDepthBiasSlop(const float& factor){
		getInfo().depthBiasSlopeFactor = factor;
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::setDepthBiasClamp(const float& clamp){
		getInfo().depthBiasClamp = clamp;
		return *this;
	}

	GraphicsPipeline::RasterizationState& GraphicsPipeline::RasterizationState::setLineWidth(const float& width){
		getInfo().lineWidth = width;
		return *this;
	}

	VkPipelineRasterizationStateCreateInfo& GraphicsPipeline::RasterizationState::get(){
		return getInfo();
	}

	VkPipelineRasterizationStateCreateInfo& GraphicsPipeline::RasterizationState::getInfo(){
		if (!_info) throw std::runtime_error("Invalid rasterization state");
		return *_info;
	}


	// =========================== MULTISAMPLE STATE ========================

	GraphicsPipeline::MultisampleState::MultisampleState() noexcept :
		_info{nullptr},
		_data{nullptr}
	{}

	GraphicsPipeline::MultisampleState::MultisampleState(VkPipelineMultisampleStateCreateInfo& info, BuildInfo::MultisampleData& data) noexcept :
		_info{&info},
		_data{&data}
	{}

	GraphicsPipeline::MultisampleState& GraphicsPipeline::MultisampleState::setFlags(VkPipelineMultisampleStateCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	GraphicsPipeline::MultisampleState& GraphicsPipeline::MultisampleState::setRasterizationSamples(const VkSampleCountFlagBits& samples){
		getInfo().rasterizationSamples = samples;
		return *this;
	}

	GraphicsPipeline::MultisampleState& GraphicsPipeline::MultisampleState::enableSampleShading(const bool& enable){
		getInfo().sampleShadingEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::MultisampleState& GraphicsPipeline::MultisampleState::setMinSampleShading(const float& min){
		getInfo().minSampleShading = min;
		return *this;
	}

	GraphicsPipeline::MultisampleState& GraphicsPipeline::MultisampleState::setSampleMask(const VkSampleMask mask[BuildInfo::MultisampleData::SAMPLE_MASK_SIZE]){
		auto& data = getData();

		std::memcpy(data.mask, &mask, BuildInfo::MultisampleData::SAMPLE_MASK_SIZE * sizeof(VkSampleMask));
		getInfo().pSampleMask = data.mask;
		return *this;
	}

	GraphicsPipeline::MultisampleState& GraphicsPipeline::MultisampleState::enableAlphaToCoverage(const bool& enable){
		getInfo().alphaToCoverageEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::MultisampleState& GraphicsPipeline::MultisampleState::enableAlphaToOne(const bool& enable){
		getInfo().alphaToOneEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	VkPipelineMultisampleStateCreateInfo& GraphicsPipeline::MultisampleState::get(){
		return getInfo();
	}


	VkPipelineMultisampleStateCreateInfo& GraphicsPipeline::MultisampleState::getInfo(){
		if (!_info) throw std::runtime_error("Invalid multisample state");
		return *_info;
	}

	GraphicsPipeline::BuildInfo::MultisampleData& GraphicsPipeline::MultisampleState::getData(){
		if (!_data) throw std::runtime_error("Invalid multisample state");
		return *_data;
	}

	// =========================== DEPTH STENCIL STATE ========================

	GraphicsPipeline::DepthStencilState::DepthStencilState() noexcept : _info{nullptr}{}
	GraphicsPipeline::DepthStencilState::DepthStencilState(VkPipelineDepthStencilStateCreateInfo& info) noexcept : _info{&info}{}

	GraphicsPipeline::DepthStencilState& GraphicsPipeline::DepthStencilState::setFlags(const VkPipelineDepthStencilStateCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	GraphicsPipeline::DepthStencilState& GraphicsPipeline::DepthStencilState::enableDepthTest(const bool& enable){
		getInfo().depthTestEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::DepthStencilState& GraphicsPipeline::DepthStencilState::enableDepthWrite(const bool& enable){
		getInfo().depthWriteEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::DepthStencilState& GraphicsPipeline::DepthStencilState::setDepthCompare(const VkCompareOp& operation){
		getInfo().depthCompareOp = operation;
		return *this;
	}

	GraphicsPipeline::DepthStencilState& GraphicsPipeline::DepthStencilState::enableDepthBoundsTest(const bool& enable){
		getInfo().depthBoundsTestEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::DepthStencilState& GraphicsPipeline::DepthStencilState::enableStencilTest(const bool& enable){
		getInfo().stencilTestEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::StencilOpState GraphicsPipeline::DepthStencilState::setFrontStencilOperations(){
		return std::move(StencilOpState(getInfo().front));
	}

	GraphicsPipeline::StencilOpState GraphicsPipeline::DepthStencilState::setBackStencilOperations(){
		return std::move(StencilOpState(getInfo().front));
	}

	GraphicsPipeline::DepthStencilState& GraphicsPipeline::DepthStencilState::setMinDepthBounds(const float& min){
		getInfo().minDepthBounds = min;
		return *this;
	}

	GraphicsPipeline::DepthStencilState& GraphicsPipeline::DepthStencilState::setMaxDepthBounds(const float& max){
		getInfo().maxDepthBounds = max;
		return *this;
	}

	GraphicsPipeline::DepthStencilState& GraphicsPipeline::DepthStencilState::setDepthBounds(const float& min, const float& max){
		setMinDepthBounds(min);
		setMaxDepthBounds(max);
		return *this;
	}

	VkPipelineDepthStencilStateCreateInfo& GraphicsPipeline::DepthStencilState::get(){
		return getInfo();
	}

	VkPipelineDepthStencilStateCreateInfo& GraphicsPipeline::DepthStencilState::getInfo(){
		if (!_info) throw std::runtime_error("Invalid depth stencil state");
		return *_info;
	}


	// =========================== COLOR BLEND STATE ========================

	GraphicsPipeline::ColorBlendState::ColorBlendState() noexcept : 
		_info{nullptr},
		_data{nullptr}
	{}

	GraphicsPipeline::ColorBlendState::ColorBlendState(VkPipelineColorBlendStateCreateInfo& info, BuildInfo::ColorBlendData& data) noexcept :
		_info{&info},
		_data{&data}
	{}

	GraphicsPipeline::ColorBlendState& GraphicsPipeline::ColorBlendState::setFlags(const VkPipelineColorBlendStateCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}
	
	GraphicsPipeline::ColorBlendState& GraphicsPipeline::ColorBlendState::enableLogicOperation(const bool& enable){
		getInfo().logicOpEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	GraphicsPipeline::ColorBlendState& GraphicsPipeline::ColorBlendState::setLogicOperation(const VkLogicOp& operation){
		getInfo().logicOp = operation;
		return *this;
	}

	GraphicsPipeline::ColorBlendState& GraphicsPipeline::ColorBlendState::setBlendConstants(const glm::vec4& constants){
		auto& blendConstants = getInfo().blendConstants;
		blendConstants[0] = constants.r;
		blendConstants[1] = constants.g;
		blendConstants[2] = constants.b;
		blendConstants[3] = constants.a;
		return *this;
	}

	GraphicsPipeline::ColorAttachment GraphicsPipeline::ColorBlendState::addColorAttachment(){
		static constexpr VkPipelineColorBlendAttachmentState DEFAULT{
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};

		auto& info = getInfo();
		auto& data = getData();

		auto& attachment = data.attachments.emplace_back(DEFAULT);

		info.attachmentCount = static_cast<uint32_t>(data.attachments.size());
		info.pAttachments = data.attachments.data();

		return ColorAttachment(attachment);
	}

	VkPipelineColorBlendStateCreateInfo& GraphicsPipeline::ColorBlendState::get(){
		return getInfo();
	}

	VkPipelineColorBlendStateCreateInfo& GraphicsPipeline::ColorBlendState::getInfo(){
		if (!_info) throw std::runtime_error("Invalid color blend state");
		return *_info;
	}

	GraphicsPipeline::BuildInfo::ColorBlendData& GraphicsPipeline::ColorBlendState::getData(){
		if (!_data) throw std::runtime_error("Invalid color blend state");
		return *_data;
	}

	// =========================== DYNAMIC STATE ========================

	GraphicsPipeline::DynamicState::DynamicState() noexcept : 
		_info{nullptr},
		_data{nullptr}
	{}

	GraphicsPipeline::DynamicState::DynamicState(VkPipelineDynamicStateCreateInfo& info, BuildInfo::DynamicStateData& data) noexcept :
		_info{&info},
		_data{&data}
	{}

	GraphicsPipeline::DynamicState& GraphicsPipeline::DynamicState::setFlags(const VkPipelineDynamicStateCreateFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	GraphicsPipeline::DynamicState& GraphicsPipeline::DynamicState::addDynamicStates(const std::initializer_list<VkDynamicState>& dynamicStates){
		auto& data = getData();
		auto& info = getInfo();

		data.dynamicStates.insert(
			data.dynamicStates.end(),
			dynamicStates
		);

		info.dynamicStateCount = static_cast<uint32_t>(data.dynamicStates.size());
		info.pDynamicStates = data.dynamicStates.data();

		return *this;
	}

	VkPipelineDynamicStateCreateInfo& GraphicsPipeline::DynamicState::get(){
		return getInfo();
	}

	VkPipelineDynamicStateCreateInfo& GraphicsPipeline::DynamicState::getInfo(){
		if (!_info) throw std::runtime_error("Invalid dynamic state");
		return *_info;
	}

	GraphicsPipeline::BuildInfo::DynamicStateData& GraphicsPipeline::DynamicState::getData(){
		if (!_data) throw std::runtime_error("Invalid dynamic state");
		return *_data;
	}

	// =========================== SHADER STAGE ========================

	GraphicsPipeline::ShaderStage::ShaderStage() noexcept : 
		_info{nullptr},
		_data{nullptr}
	{}

	GraphicsPipeline::ShaderStage::ShaderStage(VkPipelineShaderStageCreateInfo& info, BuildInfo::ShaderStageData& data) noexcept :	
		_info{&info},
		_data{&data}
	{}

	GraphicsPipeline::ShaderStage& GraphicsPipeline::ShaderStage::setFlags(const VkPipelineStageFlags& flags){
		getInfo().flags = flags;
		return *this;
	}

	GraphicsPipeline::ShaderStage& GraphicsPipeline::ShaderStage::setStage(const VkShaderStageFlagBits& stage){
		getInfo().stage = stage;
		return *this;
	}

	GraphicsPipeline::ShaderStage& GraphicsPipeline::ShaderStage::setModule(const VkShaderModule& module){
		getInfo().module = module;
		return *this;
	}

	GraphicsPipeline::ShaderStage& GraphicsPipeline::ShaderStage::setEntryPoint(const std::string& entryPoint){
		auto& data = getData();
		auto& info = getInfo();

		auto& str = data.entryPoints.emplace_back(entryPoint);
		info.pName = str.c_str();

		return *this;
	}

	GraphicsPipeline::ShaderStage& GraphicsPipeline::ShaderStage::addSpecialization(const VkSpecializationInfo& specialization){
		auto& data = getData();
		auto& info = getInfo();

		info.pSpecializationInfo = &data.specializations.emplace_back(specialization);

		return *this;
	}

	VkPipelineShaderStageCreateInfo& GraphicsPipeline::ShaderStage::get(){
		return getInfo();
	}

	VkPipelineShaderStageCreateInfo& GraphicsPipeline::ShaderStage::getInfo(){
		if (!_info) throw std::runtime_error("Invalid shader stage");
		return *_info;
	}

	GraphicsPipeline::BuildInfo::ShaderStageData& GraphicsPipeline::ShaderStage::getData(){
		if (!_data) throw std::runtime_error("Invalid dynamic state");
		return *_data;
	}

	// =========================== GRAPHICS PIPELINE ========================


	GraphicsPipeline::GraphicsPipeline() noexcept : 
		_context{nullptr},
		_pipeline{VK_NULL_HANDLE},
		_info{}
	{}

	GraphicsPipeline::~GraphicsPipeline(){
		release();
	}


	GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& other) : 
		_context{nullptr},
		_pipeline{VK_NULL_HANDLE},
		_info{}
	{
		swap(*this, other);
	}

	GraphicsPipeline& GraphicsPipeline::operator=(GraphicsPipeline&& other){
		swap(*this, other);
		return *this;
	}

	void swap(GraphicsPipeline& A, GraphicsPipeline& B){
		std::swap(A._context, B._context);
		std::swap(A._pipeline, B._pipeline);
		std::swap(A._info, B._info);
	}

	void GraphicsPipeline::prepare(Context& context){
		_context = &context;

		_info = std::make_unique<BuildInfo>();

		// forcefully initialize the building with pipeline states
		getVertexInputState();
		getInputAssemplyState();
		getTessellationState();
		getViewportState();
		getRasterizationState();
		getMultisampleState();
		getDepthStencilState();
		getColorBlendState();
		getDynamicState();
	}

	void GraphicsPipeline::initialize(){
		if (!_context){
			spdlog::warn("Attempt to initialize a non prepared graphics pipeline");
			throw std::runtime_error("The pipeline has not been prepared for initialization");
		}

		if (!_info){
			spdlog::warn("Attempt to initialize an already initialized graphics pipeline");
			throw std::runtime_error("The pipeline has already been initialied");
		}

		VkGraphicsPipelineCreateInfo info{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = nullptr,
			.flags = _info->flags,
			.stageCount = static_cast<uint32_t>(_info->stages.size()),
			.pStages = _info->stages.data(),
			.pVertexInputState = _info->vertexInputState.has_value() ? &_info->vertexInputState.value() : nullptr,
			.pInputAssemblyState = _info->inputAssemblyState.has_value() ? &_info->inputAssemblyState.value() : nullptr,
			.pTessellationState = _info->tessellationState.has_value() ? &_info->tessellationState.value() : nullptr,
			.pViewportState = _info->viewportState.has_value() ? &_info->viewportState.value() : nullptr,
			.pRasterizationState = _info->rasterizationState.has_value() ? &_info->rasterizationState.value() : nullptr,
			.pMultisampleState = _info->multisampleState.has_value() ? &_info->multisampleState.value() : nullptr,
			.pDepthStencilState = _info->depthStencilState.has_value() ? &_info->depthStencilState.value() : nullptr,
			.pColorBlendState = _info->colorBlendState.has_value() ? &_info->colorBlendState.value() : nullptr,
			.pDynamicState = _info->dynamicState.has_value() ? &_info->dynamicState.value() : nullptr,
			.layout = _info->layout->get(),
			.renderPass = _info->renderPass->get(),
			.subpass = _info->subpass,
			.basePipelineHandle = VK_NULL_HANDLE, // TODO : batched pipeline creation
			.basePipelineIndex = -1
		};

		auto& device = _context->getDevice().get();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkPipeline>(
			vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &info, nullptr, &_pipeline),
			"Failed to create graphics pipeline",
			_context->logger
		);

		_info.reset();
	}

	void GraphicsPipeline::release(){
		if (!_context) return;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		if (_pipeline){
			vkDestroyPipeline(device.get(), _pipeline, allocationCallbacks);
			_pipeline = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	GraphicsPipeline::BuildInfo& GraphicsPipeline::checkBuild(){
		if (!_context){
			spdlog::warn("Attempt to change build info of an unprepared graphics pipeline");
			throw std::runtime_error("The graphics pipeline has not been prepared");
		}
		
		if (!_info){
			_context->logger->warn("Attempt to change build info of an unprepared graphics pipeline");
			throw std::runtime_error("The graphics pipeline has not been prepared");
		}
		return *_info;
	}

	GraphicsPipeline& GraphicsPipeline::setFlags(const VkPipelineCreateFlags& flags){
		checkBuild().flags = flags;
		return *this;
	}

	GraphicsPipeline& GraphicsPipeline::setRenderPass(const RenderPass& renderPass){
		checkBuild().renderPass = &renderPass;
		return *this;
	}

	GraphicsPipeline& GraphicsPipeline::setLayout(const PipelineLayout& layout){
		checkBuild().layout = &layout;
		return *this;
	}

	GraphicsPipeline& GraphicsPipeline::setSubpass(const std::uint32_t& subpass){
		checkBuild().subpass = subpass;
		return *this;
	}

	GraphicsPipeline::ShaderStage GraphicsPipeline::addStage(){
		static constexpr VkPipelineShaderStageCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.stage = VkShaderStageFlagBits(0),
			.module = VK_NULL_HANDLE,
			.pName = nullptr,
			.pSpecializationInfo = nullptr
		};

		auto& info = checkBuild();

		auto& stage = info.stages.emplace_back(DEFAULT);
		return std::move(ShaderStage(stage, info.shaderStageData));
	}

	GraphicsPipeline::VertexInputState GraphicsPipeline::getVertexInputState(const bool& disable){
		static constexpr VkPipelineVertexInputStateCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.vertexBindingDescriptionCount = 0,
			.pVertexBindingDescriptions = nullptr,
			.vertexAttributeDescriptionCount = 0,
			.pVertexAttributeDescriptions = nullptr
		};

		auto& info = checkBuild();

		if (disable){
			info.vertexInputState.reset();
			return VertexInputState();
		}
		return VertexInputState(info.vertexInputState.has_value() ? info.vertexInputState.value() : info.vertexInputState.emplace(DEFAULT), info.vertexInputData);
	}

	GraphicsPipeline::InputAssemblyState GraphicsPipeline::getInputAssemplyState(const bool& disable){
		static constexpr VkPipelineInputAssemblyStateCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
			.primitiveRestartEnable = VK_FALSE
		};

		auto& info = checkBuild();

		if (disable){
			info.inputAssemblyState.reset();
			return InputAssemblyState();
		}
		return InputAssemblyState(info.inputAssemblyState.has_value() ? info.inputAssemblyState.value() : info.inputAssemblyState.emplace(DEFAULT));
	}

	GraphicsPipeline::TessellationState GraphicsPipeline::getTessellationState(const bool& disable){
		static constexpr VkPipelineTessellationStateCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.patchControlPoints = 0
		};

		auto& info = checkBuild();

		if (disable){
			info.tessellationState.reset();
			return TessellationState();
		}
		return TessellationState(info.tessellationState.has_value() ? info.tessellationState.value() : info.tessellationState.emplace(DEFAULT));
	}

	GraphicsPipeline::ViewportState GraphicsPipeline::getViewportState(const bool& disable){
		static constexpr VkPipelineViewportStateCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.viewportCount = 0,
			.pViewports = nullptr,
			.scissorCount = 0,
			.pScissors = nullptr
		};

		auto& info = checkBuild();

		if (disable){
			info.viewportState.reset();
			return ViewportState();
		}
		return ViewportState(info.viewportState.has_value() ? info.viewportState.value() : info.viewportState.emplace(DEFAULT), info.viewportData);
	}

	GraphicsPipeline::RasterizationState GraphicsPipeline::getRasterizationState(const bool& disable){
		static constexpr VkPipelineRasterizationStateCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_NONE,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.f,
			.depthBiasClamp = 0.f,
			.depthBiasSlopeFactor = 0.f,
			.lineWidth = 1.f
		};

		auto& info = checkBuild();

		if (disable){
			info.rasterizationState.reset();
			return RasterizationState();
		}
		return RasterizationState(info.rasterizationState.has_value() ? info.rasterizationState.value() : info.rasterizationState.emplace(DEFAULT));
	}

	GraphicsPipeline::MultisampleState GraphicsPipeline::getMultisampleState(const bool& disable){
		static constexpr VkPipelineMultisampleStateCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 0.f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE
		};

		auto& info = checkBuild();

		if (disable){
			info.multisampleState.reset();
			return MultisampleState();
		}
		return MultisampleState(info.multisampleState.has_value() ? info.multisampleState.value() : info.multisampleState.emplace(DEFAULT), info.multisampleData);
	}

	GraphicsPipeline::DepthStencilState GraphicsPipeline::getDepthStencilState(const bool& disable){
		static constexpr VkStencilOpState DEFAULT_STENCIL{
			.failOp = VK_STENCIL_OP_KEEP,
			.passOp = VK_STENCIL_OP_KEEP,
			.depthFailOp = VK_STENCIL_OP_KEEP,
			.compareOp = VK_COMPARE_OP_NEVER,
			.compareMask = 0x0,
			.writeMask = 0x0,
			.reference = 0
		};

		static constexpr VkPipelineDepthStencilStateCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.depthTestEnable = VK_TRUE,
			.depthWriteEnable = VK_TRUE,
			.depthCompareOp = VK_COMPARE_OP_LESS,
			.stencilTestEnable = VK_FALSE,
			.front = DEFAULT_STENCIL,
			.back = DEFAULT_STENCIL,
			.minDepthBounds = 0.f,
			.maxDepthBounds = 1.f
		};

		auto& info = checkBuild();

		if (disable){
			info.depthStencilState.reset();
			return DepthStencilState();
		}
		return DepthStencilState(info.depthStencilState.has_value() ? info.depthStencilState.value() : info.depthStencilState.emplace(DEFAULT));
	}

	GraphicsPipeline::ColorBlendState GraphicsPipeline::getColorBlendState(const bool& disable){
		static constexpr VkPipelineColorBlendStateCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_SET,
			.attachmentCount = 0,
			.pAttachments = nullptr,
			.blendConstants{1.f, 1.f, 1.f, 1.f}
		};

		auto& info = checkBuild();

		if (disable){
			info.colorBlendState.reset();
			return ColorBlendState();
		}
		return ColorBlendState(info.colorBlendState.has_value() ? info.colorBlendState.value() : info.colorBlendState.emplace(DEFAULT), info.colorBlendData);
	}

	GraphicsPipeline::DynamicState GraphicsPipeline::getDynamicState(const bool& disable){
		static constexpr VkPipelineDynamicStateCreateInfo DEFAULT{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.dynamicStateCount = 0,
			.pDynamicStates = nullptr
		};

		auto& info = checkBuild();

		if (disable){
			info.dynamicState.reset();
			return DynamicState();
		}
		return DynamicState(info.dynamicState.has_value() ? info.dynamicState.value() : info.dynamicState.emplace(DEFAULT), info.dynamicStateData);
	}

	void GraphicsPipeline::bind(const CommandBuffer& cmd){
		vkCmdBindPipeline(cmd.get(), VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
	}

}