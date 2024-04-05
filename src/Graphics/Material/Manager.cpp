#include <Raindrop/Graphics/Materials/Manager.hpp>
#include <Raindrop/Graphics/Materials/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Materials{
	Manager::Manager(::Raindrop::Graphics::Context& context) : _context{nullptr}{
		spdlog::info("Constructing graphics matetial manager ...");
		_context = new Context(context);

		Material placeholder(context);
		placeholder.properties = {
			.diffuseColor = glm::vec4(0.f),
			.specularColor = glm::vec4(0.f),
			.shininess = 0.f,
		};

		assert(registerMaterial(placeholder) == PLACEHOLDER_MATERIAL_ID && "Failed to create the placeholder material");
	}

	Manager::~Manager(){
		if (_context != nullptr){
			{
				std::size_t total = _context->registry.size();
				std::size_t freeSize = _context->registry.freeSize();
				spdlog::info("Destroying graphics material manager ... ({} material(s) out of {})", total - freeSize, total);
			}
			delete _context;
			_context = nullptr;
		}
	}

	MaterialID Manager::registerMaterial(const Material& material){
		MaterialID ID = _context->registry.registerMaterial();
		VkDescriptorSet set = _context->pool.getSet(static_cast<std::size_t>(ID));

		_context->registry.updateMaterial(ID, Registry::MaterialPair(material, set));
		updateMaterial(ID, material);
		return ID;
	}

	void Manager::destroyMaterial(const MaterialID& ID){
		_context->registry.unregisterMaterial(ID);
	}

	Material& Manager::getMaterial(const MaterialID& ID){
		const auto& pair = _context->registry.get(ID);
		if (pair){
			return pair->material;
		} else {
			return getMaterial(PLACEHOLDER_MATERIAL_ID);
		}
	}

	VkDescriptorSet Manager::getDescriptorSet(const MaterialID& ID){
		const auto& pair = _context->registry.get(ID);
		if (pair){
			return pair->set;
		} else {
			return getDescriptorSet(PLACEHOLDER_MATERIAL_ID);
		}
	}

	void Manager::updateMaterial(const MaterialID& ID){
		updateMaterial(ID, getMaterial(ID));
	}

	void Manager::updateMaterial(const MaterialID& ID, const Material& material){
		auto& device = _context->renderer.device;
		_context->buffer.write(ID, material.properties);

		auto set = getDescriptorSet(ID);

		VkDescriptorImageInfo imageInfos[] = {
			material.textures.getDiffuse().info()
		};

		VkDescriptorBufferInfo bufferInfo[] = {
			_context->buffer.info(ID)
		};

		VkWriteDescriptorSet writes[] = {
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = set,
				.dstBinding = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.pBufferInfo = bufferInfo
			},
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstSet = set,
				.dstBinding = 1,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.pImageInfo = imageInfos
			}
		};

		vkUpdateDescriptorSets(
			device.get(),
			sizeof(writes) / sizeof(writes[0]),
			writes,
			0,
			nullptr
		);
	}

	SetLayout& Manager::layout() const{
		return _context->layout;
	}
}