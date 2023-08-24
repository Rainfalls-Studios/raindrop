#include <Raindrop/Graphics/SceneFramebuffer.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/builders/DescriptorPoolBuilder.hpp>
#include <Raindrop/Graphics/builders/DescriptorSetLayoutBuilder.hpp>
#include <Raindrop/Graphics/DescriptorPool.hpp>
#include <Raindrop/Graphics/DescriptorSetLayout.hpp>
#include <Raindrop/Graphics/builders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/Shader.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>

#include "configurations/Scene.hpp"

namespace Raindrop::Graphics{
	SceneFramebuffer::SceneFramebuffer(GraphicsContext& context, uint32_t width, uint32_t height) :
			_context{context},
			_descriptorSet{VK_NULL_HANDLE}{
		
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.SceneFramebuffer");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.SceneFramebuffer") << "Creating world framebuffer...";

		checkDefaultFormats();
		allocDescriptorSet();
		resize(width, height);

		_context.gRegistry["SceneFramebuffer"] = this;

		CLOG(INFO, "Engine.Graphics.SceneFramebuffer") << "Created world framebuffer with success !";
	}

	void SceneFramebuffer::resize(uint32_t width, uint32_t height){
		
		_width = width;
		_height = height;

		createAttachments();
		createFramebuffer();
		updateDescriptorSet();
	}

	SceneFramebuffer::~SceneFramebuffer(){
		if (_framebuffer) vkDestroyFramebuffer(_context.device.get(), _framebuffer, _context.allocationCallbacks);
		
		for (auto &a : _attachments){
			if (a.imageView) vkDestroyImageView(_context.device.get(), a.imageView, _context.allocationCallbacks);
			if (a.image) vkDestroyImage(_context.device.get(), a.image, _context.allocationCallbacks);
			if (a.memory) vkFreeMemory(_context.device.get(), a.memory, _context.allocationCallbacks);
			if (a.sampler) vkDestroySampler(_context.device.get(), a.sampler, _context.allocationCallbacks);
		}
	}

	void SceneFramebuffer::createAttachments(){
		_attachments.resize(attachments.size());

		for (int i=0; i<attachments.size(); i++){
			auto& attachment = _attachments[i];
			VkImageCreateInfo imageInfo{};

			imageInfo = attachments[i].image;
			imageInfo.extent.width = _width;
			imageInfo.extent.height = _height;

			uint32_t familyIndices[] = {_context.graphics.familyIndex};

			imageInfo.pQueueFamilyIndices = familyIndices;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageInfo.queueFamilyIndexCount = sizeof(familyIndices) / sizeof(uint32_t);
			
			if (vkCreateImage(_context.device.get(), &imageInfo, _context.allocationCallbacks, &attachment.image) != VK_SUCCESS){
				CLOG(ERROR, "Engine.Graphics.SceneFramebuffer") << "Failed to create world framebuffer attachment (" << i << ") image";
				throw std::runtime_error("Failed to create world framebuffer attachment image");
			}

			VkMemoryRequirements requirements;
			vkGetImageMemoryRequirements(_context.device.get(), attachment.image, &requirements);

			VkMemoryAllocateInfo allocationInfo{};
			allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocationInfo.memoryTypeIndex = _context.device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			allocationInfo.allocationSize = requirements.size;
			

			if (vkAllocateMemory(_context.device.get(), &allocationInfo, _context.allocationCallbacks, &attachment.memory) != VK_SUCCESS){
				CLOG(ERROR, "Engine.Graphics.SceneFramebuffer") << "Failed to allocate world framebuffer attachment memory";
				throw std::runtime_error("Failed to allocate world framebuffer attachment memory");
			}

			if (vkBindImageMemory(_context.device.get(), attachment.image, attachment.memory, 0) != VK_SUCCESS){
				CLOG(ERROR, "Engine.graphics.SceneFramebuffer") << "Failed to bind world framebuffer attachment image memory";
				throw std::runtime_error("Failed to bind world framebuffer attachment image memory");
			}

			VkImageViewCreateInfo imageViewInfo{};
			imageViewInfo = attachments[i].imageView;
			imageViewInfo.image = attachment.image;

			if (vkCreateImageView(_context.device.get(), &imageViewInfo, _context.allocationCallbacks, &attachment.imageView) != VK_SUCCESS){
				CLOG(ERROR, "Engine.Graphics.SceneFramebuffer") << "Failed to create world framebuffer attachment (" << i << ") image view";
				throw std::runtime_error("Failed to create world framebuffer attachment image view");
			}

			VkSamplerCreateInfo samplerInfo{};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.anisotropyEnable = VK_FALSE;
			samplerInfo.maxAnisotropy = 1.0f;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 0.0f;
			samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;

			if (vkCreateSampler(_context.device.get(), &samplerInfo, _context.allocationCallbacks, &attachment.sampler) != VK_SUCCESS){
				CLOG(ERROR, "Engine.Graphics.SceneFramebuffer") << "Failed to create world framebuffer attachment (" << i << ") sampler";
				throw std::runtime_error("Failed to create sampler");
			}
		}
	}

	void SceneFramebuffer::createFramebuffer(){
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		info.width = _width;
		info.height = _height;
		info.layers = 1;

		std::vector<VkImageView> imageViews(attachments.size());
		for (int i=0; i<attachments.size(); i++){
			imageViews[i] = _attachments[i].imageView;
		}

		info.pAttachments = imageViews.data();
		info.attachmentCount = imageViews.size();
		info.renderPass = _context.renderPasses.scene.get();
		
		if (vkCreateFramebuffer(_context.device.get(), &info, _context.allocationCallbacks, &_framebuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.SceneFramebuffer") << "Failed to create world framebuffer";
			throw std::runtime_error("Failed to create world framebuffer");
		}
	}

	void SceneFramebuffer::checkDefaultFormats(){
		for (auto &a : attachments){

			VkFormat bestCase = VK_FORMAT_UNDEFINED;

			if (bestCase == VK_FORMAT_UNDEFINED){
				for (auto &f : a.formats){
					VkFormatProperties properties;
					vkGetPhysicalDeviceFormatProperties(_context.device.getPhysicalDevice(), f, &properties);

					if (properties.optimalTilingFeatures & a.requiredFeatures){
						a.image.tiling = VK_IMAGE_TILING_OPTIMAL;
						bestCase = f;
						break;
					} else if (properties.linearTilingFeatures & a.requiredFeatures){
						a.image.tiling = VK_IMAGE_TILING_LINEAR;
						bestCase = f;
						break;
					}
				}
			}
			
			for (auto &f : a.formats){
				VkFormatProperties properties;
				vkGetPhysicalDeviceFormatProperties(_context.device.getPhysicalDevice(), f, &properties);

				if (a.image.tiling == VK_IMAGE_TILING_OPTIMAL){
					if (properties.optimalTilingFeatures & a.requiredFeatures){
						bestCase = f;
						break;
					}
				} else if (a.image.tiling == VK_IMAGE_TILING_LINEAR){
					if (properties.linearTilingFeatures & a.requiredFeatures){
						bestCase = f;
						break;
					}
				}
			}

			if (bestCase == VK_FORMAT_UNDEFINED){
				CLOG(ERROR, "Engine.Graphics.SceneFramebuffer") << "failed to find a format for world framebuffer attachment";
				throw std::runtime_error("failed to find a format for world framebuffer attachment");
			}

			a.imageView.format = bestCase;
			a.description.format = bestCase;
			a.image.format = bestCase;
		}
	}
	
	bool SceneFramebuffer::beginRenderPass(VkCommandBuffer commandBuffer){
		if (_width == 0 || _height == 0) return false;

		VkRenderPassBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.framebuffer = _framebuffer;
		info.renderPass = _context.renderPasses.scene.get();
		info.renderArea = VkRect2D{0, 0, _width, _height};

		info.clearValueCount = static_cast<uint32_t>(attachments.size());
		std::vector<VkClearValue> clear(attachments.size());

		for (int i=0; i<attachments.size(); i++){
			clear[i] = attachments[i].clear;
		}

		info.pClearValues = clear.data();

		std::vector<VkImageMemoryBarrier> layoutTransitionBarriers(attachments.size());
		for (int i=0; i<layoutTransitionBarriers.size(); i++){
			auto& layoutTransitionBarrier = layoutTransitionBarriers[i];

			layoutTransitionBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			if (i == 0){
				layoutTransitionBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				layoutTransitionBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				layoutTransitionBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			} else {
				layoutTransitionBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				layoutTransitionBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				layoutTransitionBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			}

			layoutTransitionBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			layoutTransitionBarrier.newLayout = attachments[i].description.initialLayout;
			layoutTransitionBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			layoutTransitionBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			layoutTransitionBarrier.image = _attachments[i].image;
			layoutTransitionBarrier.subresourceRange.baseMipLevel = 0;
			layoutTransitionBarrier.subresourceRange.levelCount = 1;
			layoutTransitionBarrier.subresourceRange.baseArrayLayer = 0;
			layoutTransitionBarrier.subresourceRange.layerCount = 1;
		}

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			0, 0, nullptr, 0, nullptr, layoutTransitionBarriers.size(), layoutTransitionBarriers.data());

		vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		return true;
	}

	void SceneFramebuffer::endRenderPass(VkCommandBuffer commandBuffer){
		vkCmdEndRenderPass(commandBuffer);
	}
	
	SceneFramebuffer::Attachment& SceneFramebuffer::getAttachment(uint32_t id){
		return _attachments[id];
	}

	std::vector<SceneFramebuffer::Attachment>& SceneFramebuffer::getAttachments(){
		return _attachments;
	}

	VkRenderPass SceneFramebuffer::renderPass() const{
		return _context.renderPasses.scene.get();
	}

	uint32_t SceneFramebuffer::attachmentCount() const{
		return _attachments.size() - 1;
	}

	VkDescriptorImageInfo SceneFramebuffer::getAttachmentInfo(uint32_t attachment) const{
		VkDescriptorImageInfo info;
		info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		info.imageView = _attachments[attachment].imageView;
		info.sampler = _attachments[attachment].sampler;
		return info;
	}

	float SceneFramebuffer::aspectRatio() const{
		return static_cast<float>(_width) / static_cast<float>(_height);
	}

	uint32_t SceneFramebuffer::width() const{
		return _width;
	}

	uint32_t SceneFramebuffer::height() const{
		return _height;
	}
	
	void SceneFramebuffer::allocDescriptorSet(){
		freeDescriptorSet();

		auto device = _context.device.get();
		auto pool = _context.descriptorPool.get();

		VkDescriptorSetLayout layout = _context.layouts.scene.get();

		VkDescriptorSetAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorSetCount = 1;
		info.descriptorPool = pool;
		info.pSetLayouts = &layout;
		
		if (vkAllocateDescriptorSets(device, &info, &_descriptorSet) != VK_SUCCESS){
			throw std::runtime_error("failed to allocate scene descriptor set");
		}
	}

	void SceneFramebuffer::updateDescriptorSet(){
		std::vector<VkDescriptorImageInfo> images(attachments.size());
		for (int i=0; i<images.size(); i++){
			auto& image = images[i];
			image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			image.imageView = _attachments[i].imageView;
			image.sampler = _attachments[i].sampler;
		}

		VkWriteDescriptorSet write = {};
		
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.dstBinding = 0;
		write.pImageInfo = images.data();
		write.descriptorCount = static_cast<uint32_t>(images.size());
		write.dstSet = _descriptorSet;

		vkUpdateDescriptorSets(_context.device.get(), 1, &write, 0, nullptr);
	}

	void SceneFramebuffer::freeDescriptorSet(){
		auto device = _context.device.get();
		auto pool = _context.descriptorPool.get();

		if (_descriptorSet != VK_NULL_HANDLE) vkFreeDescriptorSets(device, pool, 1, &_descriptorSet);
		_descriptorSet = VK_NULL_HANDLE;
	}

	VkDescriptorSet SceneFramebuffer::descriptorSet() const{
		return _descriptorSet;
	}
}