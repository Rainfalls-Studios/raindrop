#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vk_enum_string_helper.h>

namespace Raindrop::Exceptions{
	void checkVkCreation(VkResult result, const std::string& failureReason, VkObjectType objectType){
		if (result != VK_SUCCESS){
			throw VulkanResourceCreationException(result, failureReason, objectType);
		}
	}

	void checkVkOperation(VkResult result, const std::string& failureReason, VulkanOperationType operationType, VkObjectType objectType){
		if (result != VK_SUCCESS){
			throw VulkanResourceOperationException(result, failureReason, operationType, objectType);
		}
	}
}