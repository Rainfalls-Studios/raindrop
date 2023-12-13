#include <Raindrop/Graphics/Shaders/PipelineLayoutBuilder.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>
#include <Raindrop/Graphics/Utils/StringToVulkan.hpp>

namespace Raindrop::Graphics::Shaders{
	PipelineLayoutBuilder::PipelineLayoutBuilder(Context& context) : _context{context}{
		_flags = 0;
	}

	PipelineLayoutBuilder::~PipelineLayoutBuilder(){}

	void PipelineLayoutBuilder::loadFromNode(const YAML::Node& node){
		_context.logger().info("Loading pipeline builder ...");

		_name = node["name"].as<std::string>();
		YAML::decodeVkPipelineLayoutCreateFlags(node["flags"], _flags);

		for (const auto& descriptorSetNode : node["descriptorSets"]){
			loadDescriptorSet(descriptorSetNode);
		}

		for (const auto& pushConstantNode : node["pushConstans"]){
			loadPushConstant(pushConstantNode);
		}
	}

	void PipelineLayoutBuilder::setName(const std::string& name){
		_name = name;
	}

	const std::string& PipelineLayoutBuilder::name() const{
		return _name;
	}

	const VkPushConstantRange* PipelineLayoutBuilder::pushConstants() const{
		return _pushConstants.data();
	}

	uint32_t PipelineLayoutBuilder::pushConstantCount() const{
		return static_cast<uint32_t>(_pushConstants.size());
	}

	const VkDescriptorSetLayout* PipelineLayoutBuilder::setLayouts() const{
		return nullptr;
	}

	uint32_t PipelineLayoutBuilder::setLayoutCount() const{
		return 0;
	}

	void PipelineLayoutBuilder::loadDescriptorSet(const YAML::Node& node){

	}

	void PipelineLayoutBuilder::loadPushConstant(const YAML::Node& node){
		VkPushConstantRange range;

		range.offset = node["offset"].as<uint32_t>();
		range.size = node["size"].as<uint32_t>();
		
		YAML::decodeVkShaderStageFlags(node["stages"], range.stageFlags);

		_pushConstants.push_back(range);
	}

	void PipelineLayoutBuilder::setFlags(const VkPipelineLayoutCreateFlags& flags){
		_flags = flags;
	}

	const VkPipelineLayoutCreateFlags& PipelineLayoutBuilder::flags() const{
		return _flags;
	}
}