#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <spdlog/spdlog.h>
#include <vulkan/vk_enum_string_helper.h>

namespace Raindrop::Exceptions{
	void checkVkCreation(VkResult result, const std::string& failureReason, VkObjectType objectType, std::shared_ptr<spdlog::logger> logger){
		if (result != VK_SUCCESS){
			logger->error("failed to create {} : {} | reason : {}", string_VkObjectType(objectType), string_VkResult(result), failureReason);
			throw VulkanResourceCreationException(result, failureReason, objectType);
		}
	}

	void checkVkOperation(VkResult result, const std::string& failureReason, VulkanOperationType operationType, VkObjectType objectType, std::shared_ptr<spdlog::logger> logger){
		if (result != VK_SUCCESS){
			logger->error("failed to create {} : {} | reason : {}", string_VkObjectType(objectType), string_VkResult(result), failureReason);
			throw VulkanResourceOperationException(result, failureReason, operationType, objectType);
		}
	}
}