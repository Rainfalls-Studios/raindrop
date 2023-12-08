#include <Raindrop/Graphics/RenderPass/RenderPassBuilder.hpp>
#include <Raindrop/Graphics/RenderPass/Context.hpp>
#include <Raindrop/Core/Engine.hpp>

namespace Raindrop::Graphics::RenderPass{
	RenderPassBuilder::RenderPassBuilder(Context& context) : _context{context}{}
	RenderPassBuilder::~RenderPassBuilder(){}

	void RenderPassBuilder::loadFromNode(const YAML::Node& node){
		auto& registry = _context.graphics().core().registry();

		_name = node["name"].as<std::string>();
		YAML::decodeVkRenderPassCreateFlags(node["flags"], _flags);

		for (const auto& attachment : node["attachments"]){
			loadAttachment(attachment);
		}

		for (const auto& subpass : node["subpasses"]){
			loadSubpass(subpass);
		}

		for (const auto& dependency : node["dependencies"]){
			loadDependency(dependency);
		}
	}

	const VkAttachmentDescription* RenderPassBuilder::attachments() const{
		return _attachmentDescriptions.data();
	}

	uint32_t RenderPassBuilder::attachmentCount() const{
		return _attachmentDescriptions.size();
	}

	const VkSubpassDescription* RenderPassBuilder::subpasses() const{
		return _subpasses.data();
	}

	uint32_t RenderPassBuilder::subpassCount() const{
		return _subpasses.size();
	}

	const VkSubpassDependency* RenderPassBuilder::dependencies() const{
		return _dependencies.data();
	}

	uint32_t RenderPassBuilder::dependencyCount() const{
		return _dependencies.size();
	}

	VkRenderPassCreateFlags RenderPassBuilder::flags() const{
		return _flags;
	}

	const std::string& RenderPassBuilder::name() const{
		return _name;
	}

	void RenderPassBuilder::loadAttachment(const YAML::Node& node){
		VkAttachmentDescription description{};

		std::string name = node["name"].as<std::string>();
		YAML::decodeVkAttachmentDescriptionFlags(node["flags"], description.flags);
		description.format = _context.graphics().formats().registry().get(node["format"].as<std::string>());
		description.samples = node["sampleCount"].as<VkSampleCountFlagBits>();
		description.loadOp = node["loadOp"].as<VkAttachmentLoadOp>();
		description.storeOp = node["storeOp"].as<VkAttachmentStoreOp>();
		description.stencilLoadOp = node["stencilLoadOp"].as<VkAttachmentLoadOp>();
		description.stencilStoreOp = node["stencilLoadOp"].as<VkAttachmentStoreOp>();
		description.initialLayout = node["initialLayout"].as<VkImageLayout>();
		description.finalLayout = node["finalLayout"].as<VkImageLayout>();

		_attachmentToId[name] = _attachmentDescriptions.size();
		_attachmentDescriptions.push_back(description);
	}

	void RenderPassBuilder::loadSubpass(const YAML::Node& node){
		YAML::Mark mark = node.Mark();

		// TO AVOID USELESS COPY
		_subpasses.push_back({});
		VkSubpassDescription& description = _subpasses.back();

		_subpassDatas.push_back({});
		SubpassData& data = _subpassDatas.back();

		std::string name = node["name"].as<std::string>();

		_subpassToId[name] = _subpasses.size() - 1;

		// description.flags = 
		YAML::decodeVkSubpassDescriptionFlags(node["flags"], description.flags);
		description.pipelineBindPoint = node["pipelineBindPoint"].as<VkPipelineBindPoint>();
		// description.

		loadAttachmentRef(data.inputAttachments, node["inputAttachments"]);
		loadAttachmentRef(data.resolveAttachments, node["resolveAttachments"]);
		loadAttachmentRef(data.colorAttachments, node["colorAttachments"]);
		loadPreserveAttachments(data.preserveAttachments, node["preserveAttachments"]);
		data.depthAttachment = loadAttachmentReference(node["depthAttachment"]);

		if (!data.resolveAttachments.empty()){
			if (data.resolveAttachments.size() != data.colorAttachments.size()){
				_context.logger().warn("Threre should be the same ammount of resolve attachment as there is color attachments or none ({} != {}) at line {}", data.resolveAttachments.size(), data.colorAttachments.size(), mark.line);
				throw std::runtime_error("Invalid resolve attachment count");
			}
		}

		description.pDepthStencilAttachment = &data.depthAttachment;

		description.inputAttachmentCount = static_cast<uint32_t>(data.inputAttachments.size());
		description.pInputAttachments = data.inputAttachments.data();

		description.colorAttachmentCount = static_cast<uint32_t>(data.colorAttachments.size());
		description.pColorAttachments = data.colorAttachments.data();

		description.preserveAttachmentCount = static_cast<uint32_t>(data.preserveAttachments.size());
		description.pPreserveAttachments = data.preserveAttachments.data();

		// IT IS THE SAME SIZE AS COLOR ATTACHMENTS
		description.pResolveAttachments = data.resolveAttachments.data();
	}

	void RenderPassBuilder::loadPreserveAttachments(std::vector<uint32_t>& storage, const YAML::Node& node){
		YAML::Mark mark = node.Mark();

		for (const auto& child : node){
			std::string attachment = child.as<std::string>();

			auto it = _attachmentToId.find(attachment);
			if (it == _attachmentToId.end()){
				_context.logger().warn("Cannot find \"{}\" attachment at line {}", attachment, mark.line);
				throw std::runtime_error("Cannot find attachment");
			}

			storage.push_back(it->second);
		}
	}

	VkAttachmentReference RenderPassBuilder::loadAttachmentReference(const YAML::Node& node){
		YAML::Mark mark = node.Mark();

		VkAttachmentReference ref;
		{
			std::string base = node["base"].as<std::string>();
			auto it = _attachmentToId.find(base);
			if (it == _attachmentToId.end()){
				_context.logger().warn("Cannot find \"{}\" attachment at line {}", base, mark.line);
				throw std::runtime_error("Cannot find attachment");
			}

			ref.attachment = static_cast<uint32_t>(it->second);
		}

		ref.layout = node["layout"].as<VkImageLayout>(VK_IMAGE_LAYOUT_UNDEFINED);
		if (ref.layout == VK_IMAGE_LAYOUT_UNDEFINED){
			_context.logger().warn("Cannot support \"{}\" as image layout at line {}", Utils::StringToVulkan::VkImageLayoutToStr(ref.layout), mark.line);
			throw std::runtime_error("Undefined image layout");
		}

		return ref;
	}

	void RenderPassBuilder::loadAttachmentRef(std::vector<VkAttachmentReference>& storage, const YAML::Node& node){
		for (const auto& child : node){
			auto mark = child.Mark();
			try{
				auto ref = loadAttachmentReference(child);
				storage.push_back(ref);
			} catch (const std::exception& e){
				_context.logger().warn("Failed to load subpass attachment at line {}", mark.line);
				throw std::runtime_error("Failed to load subpass attachment");
			}
		}
	}

	void RenderPassBuilder::loadDependency(const YAML::Node& node){
		_dependencies.push_back({});
		VkSubpassDependency& dependency = _dependencies.back();

		dependency.srcSubpass = findSubpass(node["srcSubpass"]);
		dependency.dstSubpass = findSubpass(node["dstSubpass"]);

		YAML::decodeVkPipelineStageFlags(node["srcStageMask"], dependency.srcStageMask);
		YAML::decodeVkPipelineStageFlags(node["dstStageMask"], dependency.dstStageMask);

		YAML::decodeVkAccessFlags(node["srcAccessMask"], dependency.srcAccessMask);
		YAML::decodeVkAccessFlags(node["dstAccessMask"], dependency.dstAccessMask);

		YAML::decodeVkDependencyFlags(node["flags"], dependency.dependencyFlags);
	}

	uint32_t RenderPassBuilder::findSubpass(const YAML::Node& node){
		YAML::Mark mark = node.Mark();
		if (node.IsNull()){
			return VK_SUBPASS_EXTERNAL;
		}

		std::string name = node.as<std::string>();
		auto it = _subpassToId.find(name);
		if (it == _subpassToId.end()){
			_context.logger().warn("Failed to find \"{}\" subpass at line {}", name, mark.line);
			throw std::runtime_error("Failed to gind subpass");
		}

		return static_cast<uint32_t>(it->second);	
	}
}