#ifndef __RAINDROP_GRAPHICS_PIPELINE_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_PIPELINE_LAYOUT_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class PipelineLayout{
		public:
			class PushConstantRange{
				public:
					PushConstantRange(VkPushConstantRange& range) noexcept;

					PushConstantRange& setStage(const VkShaderStageFlags& stage) noexcept;
					PushConstantRange& setOffset(const std::uint32_t& offset) noexcept;
					PushConstantRange& setSize(const std::uint32_t& size) noexcept;

					template<typename T>
					PushConstantRange& set() noexcept{
						return setSize(sizeof(T));
					}

				private:
					VkPushConstantRange& _range;
			};

			PipelineLayout() noexcept;
			~PipelineLayout();

			PipelineLayout(const PipelineLayout&) = delete;
			PipelineLayout& operator=(const PipelineLayout&) = delete;

			PipelineLayout(PipelineLayout&& other);
			PipelineLayout& operator=(PipelineLayout&& other);

			friend void swap(PipelineLayout& A, PipelineLayout& B);

			PipelineLayout& prepare(Context& context);
			void initialize();
			void release();

			PipelineLayout& setFlags(const VkPipelineLayoutCreateFlags& flags) noexcept;
			PipelineLayout& setSetLayouts(const std::vector<DescriptorSetLayout>& setLayouts);
			PushConstantRange addPushConstant();

			const VkPipelineLayout& get() const noexcept;

		private:
			Context* _context;
			VkPipelineLayout _layout;

			struct BuildInfo{
				VkPipelineLayoutCreateFlags flags;
				std::vector<VkDescriptorSetLayout> layouts;
				std::vector<VkPushConstantRange> pushConstants;
			};

			std::unique_ptr<BuildInfo> _info;
			BuildInfo& getInfo();

	};
}

#endif