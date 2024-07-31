#include <Raindrop/Graphics/DescriptorPool.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

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
}