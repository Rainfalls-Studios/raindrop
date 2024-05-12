#include <Raindrop/Pipeline.hpp>
#include <Raindrop_internal/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop_internal/Pipeline.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

#include <fstream>

#define LOGGER _impl->context->getInternalContext()->getLogger()
#define INFO _impl->info
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()

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
		_impl->layout = std::make_shared<Internal::Graphics::Pipelines::PipelineLayout>(GRAPHICS_CONTEXT, _impl->info);
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


	Pipeline::VertexBinding::VertexAttribute::VertexAttribute() noexcept :
		_location{0},
		_offset{0},
		_format{Format::UNDEFINED}
	{}

	Pipeline::VertexBinding::VertexAttribute::VertexAttribute(const std::size_t& location, const std::size_t& offset, const Format& format) noexcept : 
		_location{location},
		_offset{offset},
		_format{format}
	{}

	void Pipeline::VertexBinding::VertexAttribute::setLocation(const std::size_t& location) noexcept{
		_location = location;
	}

	void Pipeline::VertexBinding::VertexAttribute::setOffset(const std::size_t& offset) noexcept{
		_offset = offset;
	}

	void Pipeline::VertexBinding::VertexAttribute::setFormat(const Format& format) noexcept{
		_format = format;
	}

	const std::size_t& Pipeline::VertexBinding::VertexAttribute::getLocation() const noexcept{
		return _location;
	}

	const std::size_t& Pipeline::VertexBinding::VertexAttribute::getOffset() const noexcept{
		return _offset;
	}

	const Format& Pipeline::VertexBinding::VertexAttribute::getFormat() const noexcept{
		return _format;
	}

	//--------------------------------------------------------------------
	//-----------------             Viewport             -----------------
	//--------------------------------------------------------------------

	Pipeline::Viewport::Viewport() noexcept : 
		_x{0.f},
		_y{0.f},
		_width{255.f},
		_height{255.f},
		_minDepth{0.f},
		_maxDepth{1.f}
	{}

	void Pipeline::Viewport::setX(const float& x) noexcept{
		_x = x;
	}

	void Pipeline::Viewport::setY(const float& y) noexcept{
		_y = y;
	}

	void Pipeline::Viewport::setWidth(const float& width) noexcept{
		_width = width;
	}

	void Pipeline::Viewport::setHeight(const float& height) noexcept{
		_height = height;
	}

	void Pipeline::Viewport::setMinDepth(const float& minDepth) noexcept{
		_minDepth = minDepth;
	}

	void Pipeline::Viewport::setMaxDepth(const float& maxDepth) noexcept{
		_maxDepth = maxDepth;
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

	Pipeline::Scissor::Scissor() noexcept : 
		_x{0},
		_y{0},
		_width{255},
		_height{255}
	{}
	
	void Pipeline::Scissor::setX(const std::size_t& x) noexcept{
		_x = x;
	}

	void Pipeline::Scissor::setY(const std::size_t& y) noexcept{
		_y = y;
	}

	void Pipeline::Scissor::setWidth(const std::size_t& width) noexcept{
		_width = width;
	}

	void Pipeline::Scissor::setHeight(const std::size_t& height) noexcept{
		_height = height;
	}

	const std::size_t& Pipeline::Scissor::getX() const noexcept{
		return _x;
	}

	const std::size_t& Pipeline::Scissor::getY() const noexcept{
		return _y;
	}

	const std::size_t& Pipeline::Scissor::getWidth() const noexcept{
		return _width;
	}

	const std::size_t& Pipeline::Scissor::getHeight() const noexcept{
		return _height;
	}

	//--------------------------------------------------------------------
	//-----------------          Vertex binding          -----------------
	//--------------------------------------------------------------------

	Pipeline::VertexBinding::VertexBinding() noexcept : 
		_attributes{},
		_binding{0},
		_stride{0}
	{}

	Pipeline::VertexBinding::VertexBinding(const std::size_t& binding, const std::initializer_list<VertexAttribute>& attributes) : 
		_binding{binding},
		_attributes(attributes.begin(), attributes.end())
	{
		checkLocations();
	}

	void Pipeline::VertexBinding::setBinding(const std::size_t& binding) noexcept{
		_binding = binding;
	}

	void Pipeline::VertexBinding::setStride(const std::size_t stride) noexcept{
		_stride = stride;
	}

	void Pipeline::VertexBinding::setInputRate(const InputRate& rate) noexcept{
		_inputRate = rate;
	}
	
	void Pipeline::VertexBinding::addAttribute(const VertexAttribute& attribute){
		_attributes.push_back(attribute);
		checkLocations();
	}
	
	void Pipeline::VertexBinding::setAttributes(const std::initializer_list<VertexAttribute>& attributes){
		_attributes.assign(attributes.begin(), attributes.end());
		checkLocations();
	}
	
	const std::size_t& Pipeline::VertexBinding::getBinding() const noexcept{
		return _binding;
	}

	const std::size_t& Pipeline::VertexBinding::getStride() const noexcept{
		return _stride;
	}

	const Pipeline::InputRate& Pipeline::VertexBinding::getInputRate() const noexcept{
		return _inputRate;
	}

	const std::vector<Pipeline::VertexBinding::VertexAttribute>& Pipeline::VertexBinding::getAttributes() const noexcept{
		return _attributes;
	}

	void Pipeline::VertexBinding::checkLocations(){
		if (_attributes.size() <= 1) return;

		std::set<std::size_t> usedLocations;
		for (auto& attribute : _attributes){
			auto pair = usedLocations.emplace(attribute.getLocation());

			// if pair.second if false, it means that the location has not actualy been emplaced, thus, this location is already used
			if (!pair.second){
				throw std::runtime_error("A location is used twice !");
			}
		}
	}

	void Pipeline::VertexBinding::deduceStride(){
		_stride = 0;
		for (auto& attribute : _attributes){
			_stride += attribute.getFormat().getSize();
		}
	}


	//--------------------------------------------------------------------
	//-----------------         Color attachments        -----------------
	//--------------------------------------------------------------------

	Pipeline::ColorAttachment::ColorAttachment() noexcept : 
		_blendEnable{false},
		_srcColorBlendFactor{Color::BlendFactor::ONE},
		_dstColorBlendFactor{Color::BlendFactor::ONE},
		_colorBlendOp{Color::BlendOperation::ADD},
		_srcAlphaBlendFactor{Color::BlendFactor::ONE},
		_dstAlphaBlendFactor{Color::BlendFactor::ONE},
		_alphaBlendOp{Color::BlendOperation::ADD},
		_writeMask{0xFFFFFFFF}
	{}

	void Pipeline::ColorAttachment::enableBlending(const bool& enable) noexcept{
		_blendEnable = enable;
	}

	void Pipeline::ColorAttachment::setSrcColorBlendFactor(const Color::BlendFactor& srcColorBlendFactor) noexcept{
		_srcColorBlendFactor = srcColorBlendFactor;
	}

	void Pipeline::ColorAttachment::setDstColorBlendFactor(const Color::BlendFactor& dstColorBlendFactor) noexcept{
		_dstColorBlendFactor = dstColorBlendFactor;
	}

	void Pipeline::ColorAttachment::setColorBlendOp(const Color::BlendOperation& colorBlendOp) noexcept{
		_colorBlendOp = colorBlendOp;
	}

	void Pipeline::ColorAttachment::setSrcAlphaBlendFactor(const Color::BlendFactor& srcAlphaBlendFactor) noexcept{
		_srcAlphaBlendFactor = srcAlphaBlendFactor;
	}

	void Pipeline::ColorAttachment::setDstAlphaBlendFactor(const Color::BlendFactor& dstAlphaBlendFactor) noexcept{
		_dstAlphaBlendFactor = dstAlphaBlendFactor;
	}

	void Pipeline::ColorAttachment::setAlphaBlendOp(const Color::BlendOperation& alphaBlendOp) noexcept{
		_alphaBlendOp = alphaBlendOp;
	}

	void Pipeline::ColorAttachment::setWriteMask(const Color::Components& writeMask) noexcept{
		_writeMask = writeMask;
	}


	bool Pipeline::ColorAttachment::isBlendingEnabled() const noexcept{
		return _blendEnable;
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

	Pipeline::StencilOpState::StencilOpState() noexcept : 
		_failOp{StencilOp::KEEP},
		_passOp{StencilOp::KEEP},
		_depthFailOp{StencilOp::KEEP},
		_compareOp{CompareOp::ALWAYS},
		_comparMask{0xFFFFFFFF},
		_writeMask{0x0FFFFFFFF},
		_reference{0}
	{}

	void Pipeline::StencilOpState::setFailOp(const StencilOp& failOp) noexcept{
		_failOp = failOp;
	}

	void Pipeline::StencilOpState::setPassOp(const StencilOp& passOp) noexcept{
		_passOp = passOp;
	}

	void Pipeline::StencilOpState::setDepthFailOp(const StencilOp& depthFailOp) noexcept{
		_depthFailOp = depthFailOp;
	}

	void Pipeline::StencilOpState::setCompareOp(const CompareOp& compareOp) noexcept{
		_compareOp = compareOp;
	}

	void Pipeline::StencilOpState::setComparMask(const std::uint32_t& comparMask) noexcept{
		_comparMask = comparMask;
	}

	void Pipeline::StencilOpState::setWriteMask(const std::uint32_t& writeMask) noexcept{
		_writeMask = writeMask;
	}

	void Pipeline::StencilOpState::setReference(const std::size_t& reference) noexcept{
		_reference = reference;
	}

	const Pipeline::StencilOp& Pipeline::StencilOpState::getFailOp() const noexcept{
		return _failOp;
	}

	const Pipeline::StencilOp& Pipeline::StencilOpState::getPassOp() const noexcept{
		return _passOp;
	}

	const Pipeline::StencilOp& Pipeline::StencilOpState::getDepthFailOp() const noexcept{
		return _depthFailOp;
	}

	const Pipeline::CompareOp& Pipeline::StencilOpState::getCompareOp() const noexcept{
		return _compareOp;
	}

	const std::uint32_t& Pipeline::StencilOpState::getComparMask() const noexcept{
		return _comparMask;
	}

	const std::uint32_t& Pipeline::StencilOpState::getWriteMask() const noexcept{
		return _writeMask;
	}

	const std::size_t& Pipeline::StencilOpState::getReference() const noexcept{
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
		INFO.code = code;
		_impl->module = std::make_shared<Internal::Graphics::Shader>(_impl->context.getInternalContext()->getRenderer().getContext(), INFO);
	}
	
	Pipeline::Shader::~Shader(){
		delete _impl;
	}

	//--------------------------------------------------------------------
	//-----------------            Pipeline              -----------------
	//--------------------------------------------------------------------

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
		_impl->pipeline = std::make_shared<Internal::Graphics::Pipelines::GraphicsPipeline>(GRAPHICS_CONTEXT, _impl->info);
		LOGGER->info("Pipeline initialized with success !");
	}

	void Pipeline::release(){
		_impl->pipeline.reset();
		INFO.renderPass.reset();
		INFO.pipelineLayout.reset();
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

	void Pipeline::setFlags(const Flags& flags){
		INFO.flags = static_cast<VkPipelineCreateFlags>(flags.get());
	}

	void Pipeline::addVertexBinding(const VertexBinding& binding){
		VkVertexInputBindingDescription bindingDescription{
			.binding = static_cast<uint32_t>(binding.getBinding()),
			.stride = static_cast<uint32_t>(binding.getStride()),
			.inputRate = static_cast<VkVertexInputRate>(binding.getInputRate())
		};

		INFO.bindings.push_back(bindingDescription);

		for (const auto& vertex : binding.getAttributes()){
			VkVertexInputAttributeDescription attributeDescription{
				.location = static_cast<uint32_t>(vertex.getLocation()),
				.binding = static_cast<uint32_t>(binding.getBinding()),
				.format = static_cast<VkFormat>(vertex.getFormat().get()),
				.offset = static_cast<uint32_t>(vertex.getOffset())
			};

			INFO.vertices.push_back(attributeDescription);
		}
	}

	void Pipeline::setVertexBindings(const std::vector<VertexBinding>& bindings){
		INFO.bindings.clear();
		INFO.vertices.clear();

		for (const auto& binding : bindings){
			VkVertexInputBindingDescription bindingDescription{
				.binding = static_cast<uint32_t>(binding.getBinding()),
				.stride = static_cast<uint32_t>(binding.getStride()),
				.inputRate = static_cast<VkVertexInputRate>(binding.getInputRate())
			};

			INFO.bindings.push_back(bindingDescription);

			for (const auto& vertex : binding.getAttributes()){
				VkVertexInputAttributeDescription attributeDescription{
					.location = static_cast<uint32_t>(vertex.getLocation()),
					.binding = static_cast<uint32_t>(binding.getBinding()),
					.format = static_cast<VkFormat>(vertex.getFormat().get()),
					.offset = static_cast<uint32_t>(vertex.getOffset())
				};

				INFO.vertices.push_back(attributeDescription);
			}
		}
	}

	void Pipeline::addViewport(const Viewport& viewport){
		INFO.viewports.push_back(VkViewport{
			.x = viewport.getX(),
			.y = viewport.getY(),
			.width = viewport.getWidth(),
			.height = viewport.getHeight(),
			.minDepth = viewport.getMinDepth(),
			.maxDepth = viewport.getMaxDepth()
		});
	}

	void Pipeline::setViewports(const std::vector<Viewport>& viewports){
		INFO.viewports.clear();
		std::transform(
			viewports.begin(),
			viewports.end(),
			std::back_inserter(INFO.viewports),
			[](const Viewport &viewport) -> VkViewport {
				return VkViewport{
					.x = viewport.getX(),
					.y = viewport.getY(),
					.width = viewport.getWidth(),
					.height = viewport.getHeight(),
					.minDepth = viewport.getMinDepth(),
					.maxDepth = viewport.getMaxDepth()
				};
			}
		);	
	}

	void Pipeline::addScissor(const Scissor& scissor){
		INFO.scissors.push_back(VkRect2D{
			.offset = VkOffset2D{
				.x = static_cast<std::int32_t>(scissor.getX()),
				.y = static_cast<std::int32_t>(scissor.getY()),
			},
			.extent = VkExtent2D{
				.width = static_cast<uint32_t>(scissor.getWidth()),
				.height = static_cast<uint32_t>(scissor.getHeight())
			}
		});
	}

	void Pipeline::setScissors(const std::vector<Scissor>& scissors){
		INFO.scissors.clear();
		std::transform(
			scissors.begin(),
			scissors.end(),
			std::back_inserter(INFO.scissors),
			[](const Scissor &scissor) -> VkRect2D {
				return VkRect2D{
					.offset = VkOffset2D{
						.x = static_cast<std::int32_t>(scissor.getX()),
						.y = static_cast<std::int32_t>(scissor.getY()),
					},
					.extent = VkExtent2D{
						.width = static_cast<uint32_t>(scissor.getWidth()),
						.height = static_cast<uint32_t>(scissor.getHeight())
					}
				};
			}
		);	
	}

	void Pipeline::setPrimitiveTopology(const Topology& topology){
		INFO.inputAssemblyInfo.topology = static_cast<VkPrimitiveTopology>(topology);
	}

	void Pipeline::enablePrimitiveRestart(const bool& enable){
		INFO.inputAssemblyInfo.primitiveRestartEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::enableDepthClamp(const bool& enable){
		INFO.rasterizationInfo.depthClampEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::enableRasterizerDiscard(const bool& enable){
		INFO.rasterizationInfo.rasterizerDiscardEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::setPolygonMode(const PolygonMode& mode){
		INFO.rasterizationInfo.polygonMode = static_cast<VkPolygonMode>(mode);
	}

	void Pipeline::setCullMode(const CullMode& mode){
		INFO.rasterizationInfo.cullMode = static_cast<VkCullModeFlags>(mode.get());
	}

	void Pipeline::setFrontFace(const FrontFace& frontFace){
		INFO.rasterizationInfo.frontFace = static_cast<VkFrontFace>(frontFace);
	}

	void Pipeline::enableDepthBias(const bool& enable){
		INFO.rasterizationInfo.depthBiasEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::setDepthBiasConstant(const float& constant){
		INFO.rasterizationInfo.depthBiasConstantFactor = constant;
	}

	void Pipeline::setDepthBiasSlope(const float& slope){
		INFO.rasterizationInfo.depthBiasSlopeFactor = slope;
	}

	void Pipeline::setDepthBiasClamp(const float& clamp){
		INFO.rasterizationInfo.depthBiasClamp = clamp;
	}

	void Pipeline::setLinceWidth(const float& width){
		INFO.rasterizationInfo.lineWidth = width;
	}

	void Pipeline::setSampleCount(const SampleCount& count){
		INFO.multisampleInfo.rasterizationSamples = static_cast<VkSampleCountFlagBits>(count);
	}

	void Pipeline::enableSampleShading(const bool& enable){
		INFO.multisampleInfo.sampleShadingEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::setMinimumSampleShading(const float& min){
		INFO.multisampleInfo.minSampleShading = min;
	}

	void Pipeline::enableAlphaToCoverage(const bool& enable){
		INFO.multisampleInfo.alphaToCoverageEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::enableAlphaToOne(const bool& enable){
		INFO.multisampleInfo.alphaToOneEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::setColorBlendFlags(const ColorBlendFlags& flags){
		INFO.colorBlendInfo.flags = static_cast<VkPipelineColorBlendStateCreateFlags>(flags.get());
	}

	void Pipeline::enableBlendLogicOperation(const bool& enable){
		INFO.colorBlendInfo.logicOpEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::setBlendLogicOperation(const LogicOp& op){
		INFO.colorBlendInfo.logicOp = static_cast<VkLogicOp>(op);
	}

	void Pipeline::addColorAttachment(const ColorAttachment& attachment){
		INFO.colorAttachments.push_back(VkPipelineColorBlendAttachmentState{
				.blendEnable = static_cast<VkBool32>(attachment.isBlendingEnabled()),
				.srcColorBlendFactor = static_cast<VkBlendFactor>(attachment.getSrcColorBlendFactor()),
				.dstColorBlendFactor = static_cast<VkBlendFactor>(attachment.getDstColorBlendFactor()),
				.colorBlendOp = static_cast<VkBlendOp>(attachment.getColorBlendOp()),
				.srcAlphaBlendFactor = static_cast<VkBlendFactor>(attachment.getSrcAlphaBlendFactor()),
				.dstAlphaBlendFactor = static_cast<VkBlendFactor>(attachment.getDstAlphaBlendFactor()),
				.alphaBlendOp = static_cast<VkBlendOp>(attachment.getAlphaBlendOp()),
				.colorWriteMask = attachment.getWriteMask()
			}
		);
	}

	void Pipeline::setColorAttachments(const std::vector<ColorAttachment>& attachments){
		INFO.colorAttachments.clear();
		std::transform(
			attachments.begin(),
			attachments.end(),
			std::back_inserter(INFO.colorAttachments),
			[](const ColorAttachment &attachment) -> VkPipelineColorBlendAttachmentState {
				return VkPipelineColorBlendAttachmentState{
					.blendEnable = static_cast<VkBool32>(attachment.isBlendingEnabled()),
					.srcColorBlendFactor = static_cast<VkBlendFactor>(attachment.getSrcColorBlendFactor()),
					.dstColorBlendFactor = static_cast<VkBlendFactor>(attachment.getDstColorBlendFactor()),
					.colorBlendOp = static_cast<VkBlendOp>(attachment.getColorBlendOp()),
					.srcAlphaBlendFactor = static_cast<VkBlendFactor>(attachment.getSrcAlphaBlendFactor()),
					.dstAlphaBlendFactor = static_cast<VkBlendFactor>(attachment.getDstAlphaBlendFactor()),
					.alphaBlendOp = static_cast<VkBlendOp>(attachment.getAlphaBlendOp()),
					.colorWriteMask = attachment.getWriteMask()
				};
			}
		);	
	}

	void Pipeline::setBlendConstant(const Color::Components::Bits& component, const float& constant){
		std::size_t id=0;
		switch (component){
			case Color::Components::RED: id = 0; break;
			case Color::Components::GREEN: id = 1; break;
			case Color::Components::BLUE: id = 2; break;
			case Color::Components::ALPHA: id = 3; break;
		}

		INFO.colorBlendInfo.blendConstants[id] = constant;
	}

	void Pipeline::enableDepthTest(const bool& enable){
		INFO.depthStencilInfo.depthTestEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::enableDepthWrite(const bool& enable){
		INFO.depthStencilInfo.depthWriteEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::setDepthCompareOp(const CompareOp& op){
		INFO.depthStencilInfo.depthCompareOp = static_cast<VkCompareOp>(op);
	}

	void Pipeline::enableDepthBoundsTest(const bool& enable){
		INFO.depthStencilInfo.depthBoundsTestEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::enableStencilTest(const bool& enable){
		INFO.depthStencilInfo.stencilTestEnable = static_cast<VkBool32>(enable);
	}

	void Pipeline::setFrontStencilOpertions(const StencilOpState& operations){
		INFO.depthStencilInfo.front = VkStencilOpState{
			.failOp = static_cast<VkStencilOp>(operations.getFailOp()),
			.passOp = static_cast<VkStencilOp>(operations.getPassOp()),
			.depthFailOp = static_cast<VkStencilOp>(operations.getDepthFailOp()),
			.compareOp = static_cast<VkCompareOp>(operations.getCompareOp()),
			.compareMask = operations.getComparMask(),
			.writeMask = operations.getWriteMask(),
			.reference = static_cast<uint32_t>(operations.getReference())
		};
	}

	void Pipeline::setBackStencilOpertions(const StencilOpState& operations){
		INFO.depthStencilInfo.back = VkStencilOpState{
			.failOp = static_cast<VkStencilOp>(operations.getFailOp()),
			.passOp = static_cast<VkStencilOp>(operations.getPassOp()),
			.depthFailOp = static_cast<VkStencilOp>(operations.getDepthFailOp()),
			.compareOp = static_cast<VkCompareOp>(operations.getCompareOp()),
			.compareMask = operations.getComparMask(),
			.writeMask = operations.getWriteMask(),
			.reference = static_cast<uint32_t>(operations.getReference())
		};
	}

	void Pipeline::setMaxDepthBounds(const float& max){
		INFO.depthStencilInfo.maxDepthBounds = max;
	}

	void Pipeline::setMinDepthBounds(const float& min){
		INFO.depthStencilInfo.maxDepthBounds = min;
	}

	void Pipeline::addDynamicState(const DynamicState& state){
		INFO.dynamicStateEnables.push_back(static_cast<VkDynamicState>(state));
	}

	void Pipeline::setDynamicStates(const std::vector<DynamicState>& states){
		INFO.dynamicStateEnables.clear();
		std::transform(
			states.begin(),
			states.end(),
			std::back_inserter(INFO.dynamicStateEnables),
			[](const DynamicState& state) -> VkDynamicState {
				return static_cast<VkDynamicState>(state);
			}
		);
	}

	void Pipeline::setTellesationPatchControlPoints(const std::size_t& count){
		INFO.tessellationInfo.patchControlPoints = static_cast<uint32_t>(count);
	}
}