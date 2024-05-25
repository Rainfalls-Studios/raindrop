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
#include <Raindrop_internal/Vertex.hpp>

#include <fstream>

#define LOGGER _impl->context->getInternalContext()->getLogger()
#define LAYOUT_INFO _impl->info
#define SHADER_INFO _impl->info
#define INFO _impl->info.internal
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()
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
		LAYOUT_INFO.flags = static_cast<VkPipelineLayoutCreateFlags>(flags.get());
	}

	Pipeline::Layout::Flags Pipeline::Layout::getFlags() const noexcept{
		return static_cast<Flags::Bitset>(LAYOUT_INFO.flags);
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
	//-----------------             Viewport             -----------------
	//--------------------------------------------------------------------

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
		SHADER_INFO.code = code;
		_impl->module = std::make_shared<Internal::Graphics::Shader>(_impl->context.getInternalContext()->getRenderer().getContext(), SHADER_INFO);
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
		if (_impl){
			delete _impl;
			_impl = nullptr;
		}
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


		std::vector<VkRect2D> scissors(_impl->info.scissors.size());
		std::vector<VkViewport> viewports(_impl->info.viewports.size());
		std::vector<VkPipelineColorBlendAttachmentState> attachments(_impl->info.attachments.size());
		std::vector<VkVertexInputAttributeDescription> attributes;
		std::vector<VkVertexInputBindingDescription> bindings;
		std::vector<VkDynamicState> dynamicStates(_impl->info.dynamicStates.size());

		
		// copy scissors
		for (std::size_t i=0; i<scissors.size(); i++){
			const auto& dst = _impl->info.scissors[i];
			scissors[i] = VkRect2D{
				.offset = {
					.x = dst.getX(),
					.y = dst.getY()
				},
				.extent = {
					.width = dst.getWidth(),
					.height = dst.getHeight()
				}
			};
		}
		
		// copy viewports
		for (std::size_t i=0; i<viewports.size(); i++){
			const auto& dst = _impl->info.viewports[i];
			viewports[i] = VkViewport{
				.x = dst.getX(),
				.y = dst.getY(),
				.width = dst.getWidth(),
				.height = dst.getHeight(),
				.minDepth = dst.getMinDepth(),
				.maxDepth = dst.getMaxDepth()
			};
		}
		
		// copy attachment descriptions
		for (std::size_t i=0; i<attachments.size(); i++){
			const auto& dst = _impl->info.attachments[i];
			attachments[i] = VkPipelineColorBlendAttachmentState{
				.blendEnable = toVulkan(dst.isBlendingEnable()),
				.srcColorBlendFactor = toVulkan(dst.getSrcColorBlendFactor()),
				.dstColorBlendFactor = toVulkan(dst.getDstColorBlendFactor()),
				.colorBlendOp = toVulkan(dst.getColorBlendOp()),
				.srcAlphaBlendFactor = toVulkan(dst.getSrcAlphaBlendFactor()),
				.dstAlphaBlendFactor = toVulkan(dst.getDstAlphaBlendFactor()),
				.alphaBlendOp = toVulkan(dst.getAlphaBlendOp()),
				.colorWriteMask = toVulkan(dst.getWriteMask())
			};
		}

		uint32_t bindingId = 0;
		for (const auto& binding : _impl->info.bindings){
			bindings.push_back(VkVertexInputBindingDescription{
				.binding = bindingId,
				.stride = static_cast<uint32_t>(binding.first.getStride()),
				.inputRate = toVulkan(binding.second)
			});

			uint32_t location = 0;
			const auto& layout = binding.first;
			for (const auto& attribute : layout.getAttributes()){
				attributes.push_back(VkVertexInputAttributeDescription{
					.location = location,
					.binding = bindingId,
					.format = static_cast<VkFormat>(attribute.format.get()),
					.offset = static_cast<uint32_t>(attribute.offset)
				});

				location++;
			}

			bindingId++;
		}

		for (std::size_t i=0; i<dynamicStates.size(); i++){
			dynamicStates[i] = toVulkan(_impl->info.dynamicStates[i]);
		}

		for (const auto& stage : _impl->info.stages){
			auto shader = stage.shader->getImpl()->module;
			_impl->info.internal.shaders.push_back(shader);

			_impl->info.internal.stages.push_back(VkPipelineShaderStageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = toVulkan(stage.flags),
				.stage = toVulkan(stage.stage),
				.module = shader->get(),
				.pName = stage.entryPoint.c_str()
			});
		}

		_impl->info.internal.vertexInfo.pVertexAttributeDescriptions = attributes.data();
		_impl->info.internal.vertexInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());

		_impl->info.internal.vertexInfo.pVertexBindingDescriptions = bindings.data();
		_impl->info.internal.vertexInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());

		_impl->info.internal.colorBlendInfo.pAttachments = attachments.data();
		_impl->info.internal.colorBlendInfo.attachmentCount = static_cast<uint32_t>(attachments.size());

		_impl->info.internal.viewportInfo.viewportCount = static_cast<uint32_t>(viewports.size());
		_impl->info.internal.viewportInfo.pViewports = viewports.data();

		_impl->info.internal.viewportInfo.scissorCount = static_cast<uint32_t>(scissors.size());
		_impl->info.internal.viewportInfo.pScissors = scissors.data();

		_impl->info.internal.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		_impl->info.internal.dynamicStateInfo.pDynamicStates = dynamicStates.data();


		_impl->pipeline = std::make_shared<Internal::Graphics::GraphicsPipeline>(GRAPHICS_CONTEXT, _impl->info.internal);
		LOGGER->info("Pipeline initialized with success !");
	}

	void Pipeline::release(){
		if (_impl){
			*_impl = Impl(*_impl->context);
		}
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
		_impl->info.internal.flags = static_cast<VkPipelineCreateFlags>(flags.get());
		return *this;
	}

	Pipeline& Pipeline::setRenderPass(const RenderPass& renderPass){
		_impl->info.internal.renderPass = renderPass.getImpl()->renderPass;
		return *this;
	}

	Pipeline& Pipeline::setLayout(const Layout& layout){
		_impl->info.internal.pipelineLayout = layout.getImpl()->layout;
	}


	Pipeline::ColorAttachment& Pipeline::addColorAttachment(){
		_impl->info.attachments.push_back({});
		return _impl->info.attachments.back();
	}

	Vertex::Layout& Pipeline::addVertexBinding(const Vertex::InputRate& inputRate){
		_impl->info.bindings.push_back({Vertex::Layout(), inputRate});
		return _impl->info.bindings.back().first;
	}

	Pipeline::Viewport& Pipeline::addViewport(){
		_impl->info.viewports.push_back({});
		return _impl->info.viewports.back();
	}

	Pipeline::Scissor& Pipeline::addScissor(){
		_impl->info.scissors.push_back({});
		return _impl->info.scissors.back();
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

	Pipeline& Pipeline::setLineWidth(const float& width){
		INFO.rasterizationInfo.lineWidth = width;
		return *this;
	}

	Pipeline& Pipeline::setSampleCount(const SampleCount::Bits& count){
		INFO.multisampleInfo.rasterizationSamples = toVulkan(count);
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

	Pipeline& Pipeline::setBlendLogicOperation(const LogicOperator& op){
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

	Pipeline& Pipeline::setDepthCompareOp(const CompareOperator& op){
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
		_impl->info.dynamicStates.push_back(state);
		return *this;
	}

	Pipeline& Pipeline::addDynamicStates(const std::initializer_list<DynamicState>& states){
		for (auto& state : states){
			_impl->info.dynamicStates.push_back(state);
		}
		return *this;
	}

	Pipeline& Pipeline::setTellesationPatchControlPoints(const std::uint32_t& count){
		INFO.tessellationInfo.patchControlPoints = count;
		return *this;
	}

	Pipeline& Pipeline::addStage(const std::shared_ptr<Shader>& shader, const Shader::Stage& stage, const char* entryPoint, const Shader::Flags& flags){
		_impl->info.stages.push_back({shader, entryPoint, stage, flags});
		return *this;
	}

	Pipeline& Pipeline::setFrontStencilOpertions(const StencilOpState& operations){
		INFO.depthStencilInfo.front = toVulkan(operations);
		return *this;
	}

	Pipeline& Pipeline::setBackStencilOpertions(const StencilOpState& operations){
		INFO.depthStencilInfo.back = toVulkan(operations);
		return *this;
	}
}