#include <Raindrop/Graphics/DescriptorPool.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Graphics/DescriptorSet.hpp>
#include <Raindrop/Graphics/DescriptorSetLayout.hpp>

namespace Raindrop::Graphics{
	DescriptorPool::BuildInfo::BuildInfo() : 
		info{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.maxSets = 0,
			.poolSizeCount = 0,
			.pPoolSizes = nullptr
		},
		sizes{}
	{}

	DescriptorPool::DescriptorPool() noexcept : 
		_context{nullptr},
		_pool{VK_NULL_HANDLE},
		_info{}
	{}

	DescriptorPool::~DescriptorPool(){
		release();
	}

	DescriptorPool::DescriptorPool(DescriptorPool&& other) : 
		_context{nullptr},
		_pool{VK_NULL_HANDLE},
		_info{}
	{
		swap(*this, other);
	}

	DescriptorPool& DescriptorPool::operator=(DescriptorPool&& other){
		swap(*this, other);
		return *this;
	}

	void swap(DescriptorPool& A, DescriptorPool& B){
		std::swap(A._context, B._context);
		std::swap(A._pool, B._pool);
		std::swap(A._info, B._info);
	}

	void DescriptorPool::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();
	}

	void DescriptorPool::initialize(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to initialized a non prepared descriptor pool");
			throw std::runtime_error("The descriptor pool has not been prepared !");
		}

		VkDescriptorPoolCreateInfo& info = _info->info;
		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		_context->logger->trace("Creating descriptor pool...");

		Exceptions::checkVkCreation<VkDescriptorPool>(
			vkCreateDescriptorPool(device.get(), &info, allocationCallbacks, &_pool),
			"Failed to create descriptor pool",
			_context->logger
		);

		_info.reset();
	}

	void DescriptorPool::release(){
		if (!_context) return;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		if (_pool){
			_context->logger->trace("Destroying descriptor pool...");
			vkDestroyDescriptorPool(device.get(), _pool, allocationCallbacks);
			_pool = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	DescriptorPool& DescriptorPool::setFlags(const VkDescriptorPoolCreateFlags& flags){
		getInfo().info.flags = flags;
		return *this;
	}

	DescriptorPool& DescriptorPool::setMaxSets(const std::uint32_t& max){
		getInfo().info.maxSets = max;
		return *this;
	}

	DescriptorPool& DescriptorPool::allocate(const VkDescriptorType& type, const std::uint32_t& count){
		auto& info = getInfo();

		info.sizes.emplace_back(VkDescriptorPoolSize{type, count});

		info.info.pPoolSizes = info.sizes.data();
		info.info.poolSizeCount = static_cast<uint32_t>(info.sizes.size());

		return *this;
	}

	const VkDescriptorPool& DescriptorPool::get() const noexcept{
		return _pool;
	}

	DescriptorPool::BuildInfo& DescriptorPool::getInfo(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to access a non prepared descriptor pool");
			throw std::runtime_error("The descriptor pool has not been prepared !");
		}

		return *_info;
	}

	std::vector<DescriptorSet> DescriptorPool::allocate(const std::vector<DescriptorSetLayout>& layouts){
		checkInitialize();

		std::vector<VkDescriptorSetLayout> vkLayouts;
		std::transform(
			layouts.begin(),
			layouts.end(),
			std::back_inserter(vkLayouts),
			[](const DescriptorSetLayout& layout) -> VkDescriptorSetLayout {
				return layout.get();
			}
		);

		VkDescriptorSetAllocateInfo info{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.pNext = nullptr,
			.descriptorPool = _pool,
			.descriptorSetCount = static_cast<uint32_t>(layouts.size()),
			.pSetLayouts = vkLayouts.data()
		};

		auto& device = _context->getDevice();
		std::vector<VkDescriptorSet> vkSets(layouts.size());

		Exceptions::checkVkOperation<VkDescriptorPool>(
			vkAllocateDescriptorSets(device.get(), &info, vkSets.data()),
			"Failed to allocate descriptor sets",
			Exceptions::VulkanOperationType::ALLOCATION,
			_context->logger
		);

		std::vector<DescriptorSet> sets;

		std::transform(
			vkSets.begin(),
			vkSets.end(),
			std::back_inserter(sets),
			[this](const VkDescriptorSet& set) -> DescriptorSet {
				return DescriptorSet(*_context, set);
			}
		);

		return std::move(sets);
	}

	void DescriptorPool::checkInitialize(){
		if (!_context || !_info){
			(_context == nullptr ? spdlog::default_logger() : _context->logger)->warn("Attempt to access a non initialized descriptor pool");
			throw std::runtime_error("The descriptor pool has not been initialized !");
		}
	}

	void DescriptorPool::reset(const VkDescriptorPoolResetFlags& flags){
		checkInitialize();

		auto& device = _context->getDevice();

		Exceptions::checkVkOperation<VkDescriptorPool>(
			vkResetDescriptorPool(device.get(), _pool, flags),
			"Failed to reset descriptor pool",
			Exceptions::VulkanOperationType::RESET,
			_context->logger
		);
	}

	void DescriptorPool::free(const std::vector<DescriptorSet>& sets){
		checkInitialize();

		auto& device = _context->getDevice();

		std::vector<VkDescriptorSet> vkSets;
		vkSets.reserve(sets.size());

		std::transform(
			sets.begin(),
			sets.end(),
			std::back_inserter(vkSets),
			[](const DescriptorSet& set) -> VkDescriptorSet {
				return set.get();
			}
		);

		Exceptions::checkVkOperation<VkDescriptorPool>(
			vkFreeDescriptorSets(device.get(), _pool, static_cast<uint32_t>(sets.size()), vkSets.data()),
			"Failed to free descriptor sets",
			Exceptions::VulkanOperationType::FREE,
			_context->logger
		);
	}
}