#include <Raindrop/Graphics/DescriptorSetLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Graphics{
	std::shared_ptr<DescriptorSetLayout> DescriptorSetLayout::create(Raindrop::Context& context){
		return context.registry.emplace<DescriptorSetLayout>();
	}

	DescriptorSetLayout::Binding::Binding(VkDescriptorSetLayoutBinding& info, std::vector<VkSampler>& immutableSampler) noexcept : 
		_info{info},
		_immutableSampler{immutableSampler}
	{}

	DescriptorSetLayout::Binding& DescriptorSetLayout::Binding::setBinding(const std::uint32_t& binding) noexcept{
		_info.binding = binding;
		return *this;
	}

	DescriptorSetLayout::Binding& DescriptorSetLayout::Binding::setDescriptorType(const VkDescriptorType& type) noexcept{
		_info.descriptorType = type;
		return *this;
	}

	DescriptorSetLayout::Binding& DescriptorSetLayout::Binding::setDescriptorCount(const std::uint32_t &count) noexcept{
		_info.descriptorCount = count;
		return *this;
	}

	DescriptorSetLayout::Binding& DescriptorSetLayout::Binding::setShaderStage(const VkShaderStageFlags& stages) noexcept{
		_info.stageFlags = stages;
		return *this;
	}

	DescriptorSetLayout::Binding& DescriptorSetLayout::Binding::setImmutableSamplers(const std::vector<VkSampler>& samplers){
		const std::size_t index = _immutableSampler.size() - 1;

		_immutableSampler.insert(
			_immutableSampler.end(),
			samplers.begin(),
			samplers.end()
		);

		_info.pImmutableSamplers = _immutableSampler.data() + index;
		return *this;
	}

	VkDescriptorSetLayoutBinding& DescriptorSetLayout::Binding::get() noexcept{
		return _info;
	}

	// =========================== DESCRIPTOR SET LAYOUT ========================

	DescriptorSetLayout::DescriptorSetLayout() noexcept : 
		_context{nullptr},
		_layout{VK_NULL_HANDLE},
		_info{}
	{}

	DescriptorSetLayout::~DescriptorSetLayout(){
		release();
	}

	DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout&& other) : 
		_context{nullptr},
		_layout{VK_NULL_HANDLE},
		_info{}
	{
		swap(*this, other);
	}

	DescriptorSetLayout& DescriptorSetLayout::operator=(DescriptorSetLayout&& other){
		swap(*this, other);
		return *this;
	}

	void swap(DescriptorSetLayout& A, DescriptorSetLayout& B){
		std::swap(A._context, B._context);
		std::swap(A._layout, B._layout);
		std::swap(A._info, B._info);
	}

	void DescriptorSetLayout::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();
	}

	void DescriptorSetLayout::initialize(){
		if (!_context){
			spdlog::warn("Attempt to initialize a non prepared descriptor set layout");
			throw std::runtime_error("The descriptor set layout has not been prepared");
		}

		if (!_info){
			_context->logger->warn("Attempt to initialize an already initialized descriptor set");
			throw std::runtime_error("The descriptor set layout has already been initialized");
		}

		_context->logger->trace("Creating new descriptor set layout...");

		VkDescriptorSetLayoutCreateInfo info{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = _info->flags,
			.bindingCount = static_cast<uint32_t>(_info->bindings.size()),
			.pBindings =_info->bindings.data()
		};

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkDescriptorSetLayout>(
			vkCreateDescriptorSetLayout(device.get(), &info, allocationCallbacks, &_layout),
			"Failed to create descriptro set layout",
			_context->logger
		);

		_info.reset();
	}

	void DescriptorSetLayout::release(){
		if (!_context) return;


		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		if (_layout){
			vkDestroyDescriptorSetLayout(device.get(), _layout, allocationCallbacks);
			_layout = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	DescriptorSetLayout& DescriptorSetLayout::setFlags(const VkDescriptorSetLayoutCreateFlags& flags) noexcept{
		getInfo().flags = flags;
		return *this;
	}

	DescriptorSetLayout::Binding DescriptorSetLayout::addBinding(){
		auto& info = getInfo();

		auto& descriptorSetLayout = info.bindings.emplace_back();
		descriptorSetLayout.binding = static_cast<uint32_t>(info.bindings.size() - 1);

		return std::move(Binding(descriptorSetLayout, info.immutableSamplers));
	}

	DescriptorSetLayout::BuildInfo& DescriptorSetLayout::getInfo(){
		if (!_info){
			_context->logger->warn("Attempt to change build info of an unprepared descriptor set layout");
			throw std::runtime_error("The descritpro set layout has not been prepared");
		}
		return *_info;
	}


	const VkDescriptorSetLayout& DescriptorSetLayout::get() const noexcept{
		return _layout;
	}

	VkDescriptorSetLayout& DescriptorSetLayout::get() noexcept{
		return _layout;
	}
}