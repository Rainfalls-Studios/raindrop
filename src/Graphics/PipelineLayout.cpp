#include <Raindrop/Graphics/PipelineLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	PipelineLayout::PushConstantRange::PushConstantRange(VkPushConstantRange& range) noexcept : _range{range}{}

	PipelineLayout::PushConstantRange& PipelineLayout::PushConstantRange::setStage(const VkShaderStageFlags& stage) noexcept{
		_range.stageFlags = stage;
		return *this;
	}

	PipelineLayout::PushConstantRange& PipelineLayout::PushConstantRange::setOffset(const std::uint32_t& offset) noexcept{
		_range.offset = offset;
		return *this;
	}

	PipelineLayout::PushConstantRange& PipelineLayout::PushConstantRange::setSize(const std::uint32_t& size) noexcept{
		_range.size = size;
		return *this;
	}



	PipelineLayout::PipelineLayout() noexcept : 
		_context{nullptr},
		_layout{VK_NULL_HANDLE},
		_info{}
	{}

	PipelineLayout::~PipelineLayout(){
		release();
	}

	PipelineLayout::PipelineLayout(PipelineLayout&& other) : 
		_context{nullptr},
		_layout{VK_NULL_HANDLE},
		_info{}
	{
		swap(*this, other);
	}

	PipelineLayout& PipelineLayout::operator=(PipelineLayout&& other){
		swap(*this, other);
		return *this;
	}

	void swap(PipelineLayout& A, PipelineLayout& B){
		std::swap(A._context, B._context);
		std::swap(A._layout, B._layout);
		std::swap(A._info, B._info);
	}

	void PipelineLayout::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();
	}

	void PipelineLayout::initialize(){
		if (!_context){
			spdlog::warn("Attempt to initialize an unprepared pipeline layout");
			throw std::runtime_error("Attempt to initialize an unprepared pipeline layout");
		}

		if (!_info){
			spdlog::warn("Attempt to initialize an already initialized pipeline layout");
			throw std::runtime_error("The pipeline layout has already been initialied");
		}

		spdlog::trace("Creating new pipeline layout");

		VkPipelineLayoutCreateInfo info{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = _info->flags,
			.setLayoutCount = static_cast<uint32_t>(_info->layouts.size()),
			.pSetLayouts = _info->layouts.data(),
			.pushConstantRangeCount = static_cast<uint32_t>(_info->pushConstants.size()),
			.pPushConstantRanges = _info->pushConstants.data()
		};

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkPipelineLayout>(
			vkCreatePipelineLayout(device.get(), &info, allocationCallbacks, &_layout),
			"Failed to create pipeline layout",
			_context->logger
		);

		_info.reset();
	}

	void PipelineLayout::release(){
		if (!_context) return;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		if (_layout){
			vkDestroyPipelineLayout(device.get(), _layout, allocationCallbacks);
			_layout = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	PipelineLayout& PipelineLayout::setFlags(const VkPipelineLayoutCreateFlags& flags) noexcept{
		getInfo().flags = flags;
		return *this;
	}

	PipelineLayout& PipelineLayout::setSetLayouts(const std::vector<DescriptorSetLayout>& setLayouts){
		auto& layouts = getInfo().layouts;

		layouts.clear();
		std::transform(
			setLayouts.begin(),
			setLayouts.end(),
			std::back_inserter(layouts),
			[](const DescriptorSetLayout& set) -> VkDescriptorSetLayout {
				return set.get();
			}
		);

		return *this;
	}

	PipelineLayout::PushConstantRange PipelineLayout::addPushConstant(){
		VkPushConstantRange& pushConstants = getInfo().pushConstants.emplace_back();
		return std::move(PushConstantRange(pushConstants));
	}

	const VkPipelineLayout& PipelineLayout::get() const noexcept{
		return _layout;
	}

	PipelineLayout::BuildInfo& PipelineLayout::getInfo(){
		if (!_context){
			spdlog::warn("Attempt to change build info of an unprepared pipeline layout");
			throw std::runtime_error("The pipeline layout has not been prepared");
		}

		if (!_info){
			_context->logger->warn("Attempt to change build info of an unprepared pipeline layout");
			throw std::runtime_error("The pipeline layout has not been prepared");
		}

		return *_info;
	}
}