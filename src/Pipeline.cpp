#include <Raindrop/Pipeline.hpp>
#include <Raindrop_internal/Graphics/PipelineLayout.hpp>
#include <Raindrop_internal/Pipeline.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/RenderPass.hpp>
#include <Raindrop_internal/RenderPass.hpp>

#include <fstream>

#define LOGGER _impl->context->getInternalContext()->getLogger()
#define INFO _impl->info
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()
#define VERTEX_ATTRIBUTE_INFO reinterpret_cast<VkVertexInputAttributeDescription*>(_data)
#define VERTEX_BINDING_INFO reinterpret_cast<Raindrop::Internal::Graphics::GraphicsPipelineConfigInfo::Binding*>(_data)
#define VIEWPORT_INFO reinterpret_cast<VkViewport*>(_data)
#define SCISSOR_INFO reinterpret_cast<VkRect2D*>(_data)
#define ATTACHMENT_INFO reinterpret_cast<VkPipelineColorBlendAttachmentState*>(_data)
#define STENCIL_OP_STATE_INFO reinterpret_cast<VkStencilOpState*>(_data)

namespace Raindrop{

	//--------------------------------------------------------------------
	//-----------------         Pipeline layout          -----------------
	//--------------------------------------------------------------------

	Pipeline::Layout Pipeline::Layout::Create(Context& context){
		return Layout(context);
	}

	Pipeline::Layout::Layout(Context& context){
		_impl = new Impl(context);
	}

	Pipeline::Layout::~Layout(){
		delete _impl;
		_impl = nullptr;
	}

	Pipeline::Layout::Layout(const Layout& other) : _impl{nullptr}{
		_impl = new Impl(*other._impl->context);
		*_impl = *other._impl;
	}

	Pipeline::Layout& Pipeline::Layout::operator=(const Layout& other){
		*_impl = *other._impl;
		return *this;
	}

	void Pipeline::Layout::initialize(){
		LOGGER->info("Initializing pipeline layout...");
		_impl->layout = std::make_shared<Internal::Graphics::PipelineLayout>(GRAPHICS_CONTEXT, _impl->info);
		LOGGER->info("Pipeline layout initialized with success !");
	}

	void Pipeline::Layout::release(){
		_impl->layout.reset();
	}

	void Pipeline::Layout::setFlags(const Flags& flags){
		INFO.flags = static_cast<VkPipelineLayoutCreateFlags>(flags.get());
	}

	Pipeline::Layout::Flags Pipeline::Layout::getFlags() const noexcept{
		return static_cast<Flags::Bitset>(INFO.flags);
	}

	bool Pipeline::Layout::isInitialized() const noexcept{
		return _impl->layout != nullptr;
	}

	void* Pipeline::Layout::getNativeHandle() const{
		return static_cast<void*>(_impl->layout->get());
	}

	Pipeline::Layout::Impl* Pipeline::Layout::getImpl() const noexcept{
		return _impl;
	}

	GUID Pipeline::Layout::getGUID() const noexcept{
		// TODO
	}


	//--------------------------------------------------------------------
	//-----------------    Vertex binding attribute      -----------------
	//--------------------------------------------------------------------


	//! IF OFFSET IS EQUAL TO 'UINT32_MAX'. IT IS CALCULATED DURING PIPELINE INTIALIZATION

	Pipeline::VertexBinding::VertexAttribute::VertexAttribute(VertexBinding& owner, void* data) noexcept : _owner{owner}, _data{data}{
		assert(data);
		VERTEX_ATTRIBUTE_INFO->binding = reinterpret_cast<VkVertexInputBindingDescription*>(owner._data)->binding;
		VERTEX_ATTRIBUTE_INFO->offset = UINT32_MAX;
	}

	Pipeline::VertexBinding::VertexAttribute& Pipeline::VertexBinding::VertexAttribute::setLocation(const std::size_t& location) noexcept{
		VERTEX_ATTRIBUTE_INFO->location = static_cast<uint32_t>(location);
		return *this;
	}

	Pipeline::VertexBinding::VertexAttribute& Pipeline::VertexBinding::VertexAttribute::setOffset(const std::size_t& offset) noexcept{
		VERTEX_ATTRIBUTE_INFO->offset = static_cast<uint32_t>(offset);
		return *this;
	}

	Pipeline::VertexBinding::VertexAttribute& Pipeline::VertexBinding::VertexAttribute::setFormat(const Format& format) noexcept{
		VERTEX_ATTRIBUTE_INFO->format = static_cast<VkFormat>(format.get());
		return *this;
	}

	//--------------------------------------------------------------------
	//-----------------             Viewport             -----------------
	//--------------------------------------------------------------------

	Pipeline::Viewport::Viewport(void* data) noexcept : _data{data}{}

	Pipeline::Viewport& Pipeline::Viewport::setX(const float& x) noexcept{
		VIEWPORT_INFO->x = x;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setY(const float& y) noexcept{
		VIEWPORT_INFO->y = y;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setWidth(const float& width) noexcept{
		VIEWPORT_INFO->width = width;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setHeight(const float& height) noexcept{
		VIEWPORT_INFO->height = height;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setMinDepth(const float& minDepth) noexcept{
		VIEWPORT_INFO->minDepth = minDepth;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setMaxDepth(const float& maxDepth) noexcept{
		VIEWPORT_INFO->maxDepth = maxDepth;
		return *this;
	}

	//--------------------------------------------------------------------
	//-----------------             Scissor              -----------------
	//--------------------------------------------------------------------

	Pipeline::Scissor::Scissor(void* data) noexcept : _data{data}{}

	Pipeline::Scissor& Pipeline::Scissor::setX(const std::size_t& x) noexcept{
		SCISSOR_INFO->offset.x = static_cast<uint32_t>(x);
		return *this;
	}

	Pipeline::Scissor& Pipeline::Scissor::setY(const std::size_t& y) noexcept{
		SCISSOR_INFO->offset.y = static_cast<uint32_t>(y);
		return *this;
	}

	Pipeline::Scissor& Pipeline::Scissor::setWidth(const std::size_t& width) noexcept{
		SCISSOR_INFO->extent.width = static_cast<uint32_t>(width);
		return *this;
	}

	Pipeline::Scissor& Pipeline::Scissor::setHeight(const std::size_t& height) noexcept{
		SCISSOR_INFO->extent.height = static_cast<uint32_t>(height);
		return *this;
	}

	//--------------------------------------------------------------------
	//-----------------          Vertex binding          -----------------
	//--------------------------------------------------------------------

	//! IF STRIDE IS EQUAL TO 'UINT32_MAX'. IT IS CALCULATED DURING PIPELINE INTIALIZATION

	Pipeline::VertexBinding::VertexBinding(Pipeline& owner, void* data) noexcept : _owner{owner}, _data{data}{
		setBinding(owner._impl->info.bindings.size()-1);
		setStride(UINT32_MAX);
	}

	Pipeline::VertexBinding& Pipeline::VertexBinding::setBinding(const std::size_t& binding) noexcept{
		VERTEX_BINDING_INFO->description.binding = static_cast<uint32_t>(binding);
		return *this;
	}

	Pipeline::VertexBinding& Pipeline::VertexBinding::setStride(const std::size_t stride) noexcept{
		VERTEX_BINDING_INFO->description.stride = static_cast<uint32_t>(stride);
		return *this;
	}

	Pipeline::VertexBinding& Pipeline::VertexBinding::setInputRate(const InputRate& rate) noexcept{
		VERTEX_BINDING_INFO->description.inputRate = static_cast<VkVertexInputRate>(rate);
		return *this;
	}
	
	Pipeline::VertexBinding::VertexAttribute Pipeline::VertexBinding::addAttribute(){
		VERTEX_BINDING_INFO->vertices.push_back({});
		return VertexAttribute(*this, &VERTEX_BINDING_INFO->vertices[VERTEX_BINDING_INFO->vertices.size() - 1]);
	}

	//--------------------------------------------------------------------
	//-----------------         Color attachments        -----------------
	//--------------------------------------------------------------------

	Pipeline::ColorAttachment::ColorAttachment(void* data) noexcept : _data{data}{}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::enableBlending(const bool& enable) noexcept{
		ATTACHMENT_INFO->blendEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setSrcColorBlendFactor(const Color::BlendFactor& srcColorBlendFactor) noexcept{
		ATTACHMENT_INFO->srcColorBlendFactor = static_cast<VkBlendFactor>(srcColorBlendFactor);
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setDstColorBlendFactor(const Color::BlendFactor& dstColorBlendFactor) noexcept{
		ATTACHMENT_INFO->dstColorBlendFactor = static_cast<VkBlendFactor>(dstColorBlendFactor);
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setColorBlendOp(const Color::BlendOperation& colorBlendOp) noexcept{
		ATTACHMENT_INFO->colorBlendOp = static_cast<VkBlendOp>(colorBlendOp);
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setSrcAlphaBlendFactor(const Color::BlendFactor& srcAlphaBlendFactor) noexcept{
		ATTACHMENT_INFO->srcAlphaBlendFactor = static_cast<VkBlendFactor>(srcAlphaBlendFactor);
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setDstAlphaBlendFactor(const Color::BlendFactor& dstAlphaBlendFactor) noexcept{
		ATTACHMENT_INFO->dstAlphaBlendFactor = static_cast<VkBlendFactor>(dstAlphaBlendFactor);
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setAlphaBlendOp(const Color::BlendOperation& alphaBlendOp) noexcept{
		ATTACHMENT_INFO->alphaBlendOp = static_cast<VkBlendOp>(alphaBlendOp);
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setWriteMask(const Color::Components& writeMask) noexcept{
		ATTACHMENT_INFO->colorWriteMask = static_cast<VkColorComponentFlags>(writeMask.get());
		return *this;
	}

	//--------------------------------------------------------------------
	//-----------------        Stencil Op state          -----------------
	//--------------------------------------------------------------------

	Pipeline::StencilOpState::StencilOpState(void* data) noexcept : _data{data}{}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setFailOp(const StencilOp& failOp) noexcept{
		STENCIL_OP_STATE_INFO->failOp = static_cast<VkStencilOp>(failOp);
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setPassOp(const StencilOp& passOp) noexcept{
		STENCIL_OP_STATE_INFO->passOp = static_cast<VkStencilOp>(passOp);
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setDepthFailOp(const StencilOp& depthFailOp) noexcept{
		STENCIL_OP_STATE_INFO->depthFailOp = static_cast<VkStencilOp>(depthFailOp);
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setCompareOp(const CompareOp& compareOp) noexcept{
		STENCIL_OP_STATE_INFO->compareOp = static_cast<VkCompareOp>(compareOp);
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setCompareMask(const std::uint32_t& compareMask) noexcept{
		STENCIL_OP_STATE_INFO->compareMask = compareMask;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setWriteMask(const std::uint32_t& writeMask) noexcept{
		STENCIL_OP_STATE_INFO->writeMask = writeMask;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setReference(const std::size_t& reference) noexcept{
		STENCIL_OP_STATE_INFO->reference = reference;
		return *this;
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
		return static_cast<void*>(_impl->module->get());
	}

	Pipeline::Shader::Impl* Pipeline::Shader::getImpl() const noexcept{
		return _impl;
	}

	bool Pipeline::Shader::isInitialized() const noexcept{
		return _impl->module != VK_NULL_HANDLE;
	}

	Pipeline::Shader::Shader(Context& context, const Path& path){
		_impl = new Impl(context);

		std::vector<char> code = readFile(path);
		INFO.code = code;
		_impl->module = std::make_shared<Internal::Graphics::Shader>(_impl->context.getInternalContext()->getRenderer().getContext(), INFO);
	}
	
	Pipeline::Shader::~Shader(){
		delete _impl;
	}

	//--------------------------------------------------------------------
	//-----------------            Pipeline              -----------------
	//--------------------------------------------------------------------

	Pipeline Pipeline::Create(Context& context){
		return Pipeline(context);
	}

	Pipeline::Pipeline(Context& context){
		_impl = new Impl(context);
	}

	Pipeline::~Pipeline(){
		delete _impl;
		_impl = nullptr;
	}

	Pipeline::Pipeline(const Pipeline& other){
		_impl = new Impl(*other._impl->context);
		*_impl = *other._impl;
	}

	Pipeline& Pipeline::operator=(const Pipeline& other){
		*_impl = *other._impl;
		return *this;
	}
	
	void Pipeline::initialize(){
		LOGGER->info("Initializing pipeline...");
		
		for (std::size_t i=0; i<INFO.bindings.size(); i++){
			
		}


		_impl->pipeline = std::make_shared<Internal::Graphics::GraphicsPipeline>(GRAPHICS_CONTEXT, _impl->info);
		LOGGER->info("Pipeline initialized with success !");
	}

	void Pipeline::release(){
		_impl->pipeline.reset();
		INFO.renderPass.reset();
		INFO.pipelineLayout.reset();
		INFO.shaders.clear();
	}

	bool Pipeline::isInitialized() const noexcept{
		return _impl->pipeline != nullptr;
	}
	
	void* Pipeline::getNativeHandle() const{
		return static_cast<void*>(_impl->pipeline->get());
	}
	
	Pipeline::Impl* Pipeline::getImpl() const noexcept{
		return _impl;
	}
	
	GUID Pipeline::getGUID() const noexcept{

	}

	Pipeline& Pipeline::setFlags(const Flags& flags){
		INFO.flags = static_cast<VkPipelineCreateFlags>(flags.get());
		return *this;
	}

	Pipeline& Pipeline::setRenderPass(const RenderPass& renderPass){
		INFO.renderPass = renderPass.getImpl()->renderPass;
		return *this;
	}

	Pipeline::ColorAttachment Pipeline::addColorAttachment(){
		INFO.colorAttachments.push_back({});
		return ColorAttachment(&INFO.colorAttachments.back());
	}

	Pipeline::VertexBinding Pipeline::addVertexBinding(){
		INFO.bindings.push_back({});
		return VertexBinding(*this, &INFO.bindings.back());
	}

	Pipeline::Viewport Pipeline::addViewport(){
		INFO.viewports.push_back({});
		return Viewport(&INFO.viewports.back());
	}

	Pipeline::Scissor Pipeline::addScissor(){
		INFO.scissors.push_back({});
		return Scissor(&INFO.scissors.back());
	}

	Pipeline& Pipeline::setPrimitiveTopology(const Topology& topology){
		INFO.inputAssemblyInfo.topology = static_cast<VkPrimitiveTopology>(topology);
		return *this;
	}

	Pipeline& Pipeline::enablePrimitiveRestart(const bool& enable){
		INFO.inputAssemblyInfo.primitiveRestartEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::enableDepthClamp(const bool& enable){
		INFO.rasterizationInfo.depthClampEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::enableRasterizerDiscard(const bool& enable){
		INFO.rasterizationInfo.rasterizerDiscardEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::setPolygonMode(const PolygonMode& mode){
		INFO.rasterizationInfo.polygonMode = static_cast<VkPolygonMode>(mode);
		return *this;
	}

	Pipeline& Pipeline::setCullMode(const CullMode& mode){
		INFO.rasterizationInfo.cullMode = static_cast<VkCullModeFlags>(mode.get());
		return *this;
	}

	Pipeline& Pipeline::setFrontFace(const FrontFace& frontFace){
		INFO.rasterizationInfo.frontFace = static_cast<VkFrontFace>(frontFace);
		return *this;
	}

	Pipeline& Pipeline::enableDepthBias(const bool& enable){
		INFO.rasterizationInfo.depthBiasEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::setDepthBiasConstant(const float& constant){
		INFO.rasterizationInfo.depthBiasConstantFactor = constant;
		return *this;
	}

	Pipeline& Pipeline::setDepthBiasSlope(const float& slope){
		INFO.rasterizationInfo.depthBiasSlopeFactor = slope;
		return *this;
	}

	Pipeline& Pipeline::setDepthBiasClamp(const float& clamp){
		INFO.rasterizationInfo.depthBiasClamp = clamp;
		return *this;
	}

	Pipeline& Pipeline::setLinceWidth(const float& width){
		INFO.rasterizationInfo.lineWidth = width;
		return *this;
	}

	Pipeline& Pipeline::setSampleCount(const SampleCount& count){
		INFO.multisampleInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>(count);
		return *this;
	}

	Pipeline& Pipeline::enableSampleShading(const bool& enable){
		INFO.multisampleInfo.sampleShadingEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::setMinimumSampleShading(const float& min){
		INFO.multisampleInfo.minSampleShading = min;
		return *this;
	}

	Pipeline& Pipeline::enableAlphaToCoverage(const bool& enable){
		INFO.multisampleInfo.alphaToCoverageEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::enableAlphaToOne(const bool& enable){
		INFO.multisampleInfo.alphaToOneEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::setColorBlendFlags(const ColorBlendFlags& flags){
		INFO.colorBlendInfo.flags = static_cast<VkPipelineColorBlendStateCreateFlags>(flags.get());
		return *this;
	}

	Pipeline& Pipeline::enableBlendLogicOperation(const bool& enable){
		INFO.colorBlendInfo.logicOpEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::setBlendLogicOperation(const LogicOp& op){
		INFO.colorBlendInfo.logicOp = static_cast<VkLogicOp>(op);
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

		INFO.colorBlendInfo.blendConstants[id] = constant;
		return *this;
	}

	Pipeline& Pipeline::enableDepthTest(const bool& enable){
		INFO.depthStencilInfo.depthTestEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::enableDepthWrite(const bool& enable){
		INFO.depthStencilInfo.depthWriteEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::setDepthCompareOp(const CompareOp& op){
		INFO.depthStencilInfo.depthCompareOp = static_cast<VkCompareOp>(op);
		return *this;
	}

	Pipeline& Pipeline::enableDepthBoundsTest(const bool& enable){
		INFO.depthStencilInfo.depthBoundsTestEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::enableStencilTest(const bool& enable){
		INFO.depthStencilInfo.stencilTestEnable = static_cast<VkBool32>(enable);
		return *this;
	}

	Pipeline& Pipeline::setMaxDepthBounds(const float& max){
		INFO.depthStencilInfo.maxDepthBounds = max;
		return *this;
	}

	Pipeline& Pipeline::setMinDepthBounds(const float& min){
		INFO.depthStencilInfo.maxDepthBounds = min;
		return *this;
	}

	Pipeline& Pipeline::addDynamicState(const DynamicState& state){
		INFO.dynamicStateEnables.push_back(static_cast<VkDynamicState>(state));
		return *this;
	}

	Pipeline& Pipeline::setDynamicStates(const std::vector<DynamicState>& states){
		INFO.dynamicStateEnables.clear();
		std::transform(
			states.begin(),
			states.end(),
			std::back_inserter(INFO.dynamicStateEnables),
			[](const DynamicState& state) -> VkDynamicState {
				return static_cast<VkDynamicState>(state);
			}
		);
		return *this;
	}

	Pipeline& Pipeline::setTellesationPatchControlPoints(const std::size_t& count){
		INFO.tessellationInfo.patchControlPoints = static_cast<uint32_t>(count);
		return *this;
	}

	Pipeline& Pipeline::addStage(const std::shared_ptr<Shader>& shader, const Shader::Stage& stage, const char* entryPoint){
		INFO.shaders.push_back(
			{
				.shader = shader->getImpl()->module,
				.stage = static_cast<VkShaderStageFlagBits>(stage),
				.entryPoint = entryPoint
			}
		);
	}
}