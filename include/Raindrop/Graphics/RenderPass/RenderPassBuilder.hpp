#ifndef __RAINDROP_GRAPHICS_RENDER_PASS_RENDER_PASS_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASS_RENDER_PASS_BUILDER_HPP__

#include <Raindrop/Graphics/RenderPass/common.hpp>

namespace Raindrop::Graphics::RenderPass{
	class RenderPassBuilder{
		public:
			struct SubpassData{
				std::vector<uint32_t> preserveAttachments;
				std::vector<VkAttachmentReference> inputAttachments;
				std::vector<VkAttachmentReference> resolveAttachments;
				std::vector<VkAttachmentReference> colorAttachments;
				VkAttachmentReference depthAttachment;
			};

			RenderPassBuilder(Context& context);
			~RenderPassBuilder();

			/**
			 * @brief Loads render pass builder data from a yaml node
			 * @throw std::exception if invalid node format
			 * @param node the node to load the data from
			 */
			void loadFromNode(const YAML::Node& node);

			/**
			 * @brief Gves the attachments as a read-only array
			 * @return const VkAttachmentDescription* 
			 */
			const VkAttachmentDescription* attachments() const;

			/**
			 * @brief Gives the attachments as a read-write array
			 * @return VkAttachmentDescription* 
			 */
			VkAttachmentDescription* attachments();

			/**
			 * @brief Gives a specific attachment description as a read-only reference
			 * @param id the index of the attachment
			 * @return const VkAttachmentDescription& 
			 */
			const VkAttachmentDescription& attachment(const std::size_t& id) const;

			/**
			 * @brief Gives a specific attachment description as a read-write reference
			 * @param id the index of the attachment
			 * @return VkAttachmentDescription& 
			 */
			VkAttachmentDescription& attachment(const std::size_t& id);

			/**
			 * @brief Sets the count of attachments in the render pass builder
			 * @warning This could results in build errors if not used correctly
			 * @param count the count of attachment
			 */
			void setAttachmentCount(const std::size_t& count);

			/**
			 * @brief Gives the count of attachments in the render pass builder
			 * @return std::size_t 
			 */
			std::size_t attachmentCount() const;

			/**
			 * @brief Gives the subpasses as a read-only array
			 * @return const VkSubpassDescription* 
			 */
			const VkSubpassDescription* subpasses() const;

			/**
			 * @brief Gives the subpasses as a read-write array
			 * @return VkSubpassDescription* 
			 */
			VkSubpassDescription* subpasses();

			/**
			 * @brief Gives a read-only reference to a specific index in the subpasses array
			 * @param id the index of the subpass in the array
			 * @return const VkSubpassDescription& 
			 */
			const VkSubpassDescription& subpass(const std::size_t& id) const;

			/**
			 * @brief Gives a read-write reference to a specific index in the subpasses array
			 * @param id teh index of the subpass in the array
			 * @return VkSubpassDescription& 
			 */
			VkSubpassDescription& subpass(const std::size_t& id);

			/**
			 * @brief Sets the count of subpasses in the render pass builder
			 * @warning This could results in build errors if not used correctly
			 * @param count the new count of subpasses
			 */
			void setSubpassCount(const std::size_t& count);

			/**
			 * @brief Gives the number of subpasses in the render pass builder
			 * @return std::size_t 
			 */
			std::size_t subpassCount() const;

			/**
			 * @brief Gives a read-write reference to a specific subpass data in the subpasses array
			 * @note This is the most preferable way of setting up custom subpass data.
			 * @attention Do not forget to call updateSubpass(<id>) to update the given informations
			 * @param id the index of the subpass whose data is given
			 * @return SubpassData& 
			 */
			SubpassData& subpassData(const std::size_t& id);

			/**
			 * @brief Gives a read-only reference to a specific subpass data in the subpasses array
			 * @param id the index of the subpass whose data is given
			 * @return const SubpassData& 
			 */
			const SubpassData& subpassData(const std::size_t& id) const;

			/**
			 * @brief Update the given subpass from it's data
			 * @param id the subpass to update
			 */
			void updateSubpass(const std::size_t& id);

			/**
			 * @brief Gives the dependencies as a read-only array
			 * @return const VkSubpassDependency* 
			 */
			const VkSubpassDependency* dependencies() const;

			/**
			 * @brief Gives the dependencies as a read-write array
			 * @return VkSubpassDependency* 
			 */
			VkSubpassDependency* dependencies();

			/**
			 * @brief Gives a read-only reference to a specific dependency in the dependencies array
			 * @param id the index of the dependency in the array
			 * @return const VkSubpassDependency& 
			 */
			const VkSubpassDependency& dependency(const std::size_t& id) const;

			/**
			 * @brief Gives a read-write reference to a specific dependency in the dependencies array
			 * @param id the index of the dependency in the array
			 * @return const VkSubpassDependency& 
			 */
			VkSubpassDependency& dependency(const std::size_t& id);

			/**
			 * @brief Sets the count of dependencies in the render pass builder
			 * @warning This could results in build errors if not used correctly
			 * @param count the new count of dependencies
			 */
			void setDependencyCount(const std::size_t& count);

			/**
			 * @brief Gives the count of dependencies in the render pass builder
			 * @return std::size_t 
			 */
			std::size_t dependencyCount() const;

			/**
			 * @brief Sets the render pass builder build flags
			 * @param flags the build flags
			 */
			void setFlags(const VkRenderPassCreateFlags& flags);

			/**
			 * @brief Gives the build flags as read-only reference
			 * @return const VkRenderPassCreateFlags& 
			 */
			const VkRenderPassCreateFlags& flags() const;

			/**
			 * @brief Gives the build flags as a read-write reference
			 * @return VkRenderPassCreateFlags 
			 */
			VkRenderPassCreateFlags& flags();

			/**
			 * @brief Sets the name of the render pass when builded. It is used in the render pass manager
			 * @param name the new name of the potential render pass
			 */
			void setName(const std::string& name);

			/**
			 * @brief Gives the name of the render pass when builded as a read-write reference. It is used in the render pass manager
			 * @return std::string& 
			 */
			std::string& name();

			/**
			 * @brief Gives the name of the render pass when builded as a read-only reference. It is used in the render pass manager
			 * @return const std::string& 
			 */
			const std::string& name() const;

		private:
			Context& _context;

			std::vector<VkAttachmentDescription> _attachmentDescriptions;
			std::vector<VkSubpassDescription> _subpasses;
			std::vector<SubpassData> _subpassDatas;
			std::vector<VkSubpassDependency> _dependencies;
			VkRenderPassCreateFlags _flags;
			std::string _name;

			std::unordered_map<std::string, std::size_t> _attachmentToId;
			std::unordered_map<std::string, std::size_t> _subpassToId;

			void loadAttachment(const YAML::Node& node);
			void loadSubpass(const YAML::Node& node);
			void loadDependency(const YAML::Node& node);

			VkAttachmentReference loadAttachmentReference(const YAML::Node& node);
			void loadAttachmentRef(std::vector<VkAttachmentReference>& storage, const YAML::Node& node);
			void loadPreserveAttachments(std::vector<uint32_t>& storage, const YAML::Node& node);

			uint32_t findSubpass(const YAML::Node& node);
	};
}

#endif