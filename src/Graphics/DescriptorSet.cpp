#include <Raindrop/Graphics/DescriptorSet.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics{
	DescriptorSet::DescriptorSet() noexcept : 
		_context{nullptr},
		_set{VK_NULL_HANDLE}
	{}

	DescriptorSet::DescriptorSet(Context& context, const VkDescriptorSet& set) noexcept : 
		_context{&context},
		_set{set}
	{}

	DescriptorSet::~DescriptorSet(){
		_context = nullptr;
		_set = VK_NULL_HANDLE;
	}

	const VkDescriptorSet& DescriptorSet::get() const noexcept{
		return _set;
	}

}