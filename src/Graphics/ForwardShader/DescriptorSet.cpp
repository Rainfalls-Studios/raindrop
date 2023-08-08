#include <Raindrop/Graphics/ForwardShader/DescriptorSet.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/builders/DescriptorSetLayoutBuilder.hpp>
#include <Raindrop/Graphics/DescriptorSetLayout.hpp>
#include <Raindrop/Graphics/WorldFramebuffer.hpp>

namespace Raindrop::Graphics::ForwardShader{
	DescriptorSet::DescriptorSet(GraphicsContext& context) : 
		_context{context},
		_setLayout{VK_NULL_HANDLE},
		_set{VK_NULL_HANDLE}{
		
		createSetLayout();
		createSet();
	}

	DescriptorSet::~DescriptorSet(){
		auto device = _context.device.get();
		_setLayout.reset();

		if (_set) vkFreeDescriptorSets(device, _context.pool, 1, &_set);
	}
	
	void DescriptorSet::createSetLayout(){
		Builders::DescriptorSetLayoutBuilder builder;

		for (int i=0; i<3; i++){
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = i;
			binding.descriptorCount = 1;
			binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			binding.pImmutableSamplers = nullptr;
			builder.pushBinding(binding);
		}

		_setLayout = builder.build(_context);
	}

	void DescriptorSet::createSet(){
		VkDescriptorSetAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorSetCount = 1;
		info.descriptorPool = _context.pool;
		
		VkDescriptorSetLayout layout = _setLayout->get();
		info.pSetLayouts = &layout;
		
		if (vkAllocateDescriptorSets(_context.device.get(), &info, &_set) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader descriptor set";
			throw std::runtime_error("Failed to create forward shader descriptor set");
		}
	}

	VkDescriptorSet DescriptorSet::set() const{
		return _set;
	}

	void DescriptorSet::update(WorldFramebuffer& worldFramebuffer){
		VkDescriptorImageInfo infos[3];
		VkWriteDescriptorSet writes[3];

		infos[0] = worldFramebuffer.getAttachmentInfo(1);
		infos[1] = worldFramebuffer.getAttachmentInfo(3);
		infos[2] = worldFramebuffer.getAttachmentInfo(4); 
		
		for (int i=0; i<3; i++){
			writes[i] = {};
			writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writes[i].dstBinding = i;
			writes[i].pImageInfo = &infos[i];
			writes[i].descriptorCount = 1;
			writes[i].dstSet = _set;
		}

		vkUpdateDescriptorSets(_context.device.get(), 3, writes, 0, nullptr);	
	}

	VkDescriptorSetLayout DescriptorSet::layout() const{
		return _setLayout->get();
	}
}