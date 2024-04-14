#ifndef __RAINDROP_EXCEPTIONS_VULKAN_EXCEPTIONS_HPP__
#define __RAINDROP_EXCEPTIONS_VULKAN_EXCEPTIONS_HPP__

#include "common.hpp"
#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>

namespace Raindrop::Exceptions{
	enum class VulkanOperationType{
		NONE,
		CREATION,
		DESTRUCTION,
		BINDING,
		MODIFICATION,
		MAPPING,
		UNMAPPING,
		COPYING,
		TRANSFERT,
		VALIDATION,
		SYNCHRONIZATION,
		QUERYING,
		USAGE,
		SUBMIT,
		BEGIN,
		END,
		WAIT,
		RESET
	};

	template<typename T>
	constexpr VkObjectType deduceVkObjectType(){
		return VK_OBJECT_TYPE_UNKNOWN;
	}

	#define DEDUCE_TYPE(VulkanType, objectType) template<> constexpr VkObjectType deduceVkObjectType<VulkanType>(){return VK_OBJECT_TYPE_##objectType;}

	DEDUCE_TYPE(VkBuffer, BUFFER);
	DEDUCE_TYPE(VkImage, IMAGE);
	DEDUCE_TYPE(VkImageView, IMAGE_VIEW);
	DEDUCE_TYPE(VkDescriptorSet, DESCRIPTOR_SET);
	DEDUCE_TYPE(VkPipeline, PIPELINE);
	DEDUCE_TYPE(VkRenderPass, RENDER_PASS);
	DEDUCE_TYPE(VkFramebuffer, FRAMEBUFFER);
	DEDUCE_TYPE(VkShaderModule, SHADER_MODULE);
	DEDUCE_TYPE(VkSemaphore, SEMAPHORE);
	DEDUCE_TYPE(VkFence, FENCE);
	DEDUCE_TYPE(VkEvent, EVENT);
	DEDUCE_TYPE(VkQueryPool, QUERY_POOL);
	DEDUCE_TYPE(VkDeviceMemory, DEVICE_MEMORY);
	DEDUCE_TYPE(VkPipelineLayout, PIPELINE_LAYOUT);
	DEDUCE_TYPE(VkDescriptorSetLayout, DESCRIPTOR_SET_LAYOUT);
	DEDUCE_TYPE(VkSampler, SAMPLER);
	DEDUCE_TYPE(VkSwapchainKHR, SWAPCHAIN_KHR);
	DEDUCE_TYPE(VkCommandBuffer, COMMAND_BUFFER);
	DEDUCE_TYPE(VkCommandPool, COMMAND_POOL);
	DEDUCE_TYPE(VkInstance, INSTANCE);
	DEDUCE_TYPE(VkDevice, DEVICE);
	DEDUCE_TYPE(VkPhysicalDevice, PHYSICAL_DEVICE);

	class VulkanException : public std::exception{
		public:
			VulkanException(const VkResult& result, std::string reason) : _result{result}, _reason{reason}{}
			VulkanException(const VulkanException& other) : _result{other._result}, _reason{other._reason}{}

			virtual const char* what() const noexcept override{
				return _reason.c_str();
			}

			inline constexpr VkResult result() const noexcept{
				return _result;
			}

		protected:
			VkResult _result;
			std::string _reason;
	};

	// Thrown when initializing the renderer, logical / physical device, instance, ...
	class VulkanIntializationException : public VulkanException{
		public:
			using VulkanException::VulkanException;
			using VulkanException::what;
			using VulkanException::result;
	};

	// Thrown when the creation of a vulkan resource failes
	class VulkanResourceCreationException : public VulkanException{
		public:
			VulkanResourceCreationException(const VkResult& result, std::string reason, const VkObjectType& type) : VulkanException(result, reason), _type{type}{}
			
			using VulkanException::what;
			using VulkanException::result;

			inline constexpr VkObjectType resourceType() const noexcept{
				return _type;
			}

		private:
			VkObjectType _type;
	};

	// Thrown when an operation applied on a vulkan resource failes
	class VulkanResourceOperationException : public VulkanException{
		public:
			VulkanResourceOperationException(const VkResult& result, std::string reason, const VulkanOperationType& operation, const VkObjectType& type) : VulkanException(result, reason), _operation{operation}, _type{type}{}
			using VulkanException::what;
			using VulkanException::result;

			inline constexpr VulkanOperationType operationType() const noexcept{
				return _operation;
			}

			inline constexpr VkObjectType resourceType() const noexcept{
				return _type;
			}

		private:
			VulkanOperationType _operation;
			VkObjectType _type;
	};

	void checkVkCreation(VkResult result, const std::string& failureReason, VkObjectType resourceType, std::shared_ptr<spdlog::logger> logger = spdlog::default_logger());

	template<typename T>
	inline void checkVkCreation(VkResult result, const std::string& failureReason, std::shared_ptr<spdlog::logger> logger = spdlog::default_logger()){
		checkVkCreation(result, failureReason, deduceVkObjectType<T>(), logger);
	}

	void checkVkOperation(VkResult result, const std::string& failureReason, VulkanOperationType operationType, VkObjectType resourceType, std::shared_ptr<spdlog::logger> logger = spdlog::default_logger());

	template<typename T>
	inline void checkVkOperation(VkResult result, const std::string& failureReason, VulkanOperationType operationType, std::shared_ptr<spdlog::logger> logger = spdlog::default_logger()){
		checkVkOperation(result, failureReason, operationType, deduceVkObjectType<T>(), logger);
	}
}

#endif