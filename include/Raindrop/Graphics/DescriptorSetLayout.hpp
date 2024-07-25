#ifndef __RAINDROP_GRAPHICS_DESCRIPTOR_SET_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_DESCRIPTOR_SET_LAYOUT_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class DescriptorSetLayout{
		public:
			class Binding{
				public:
					Binding(VkDescriptorSetLayoutBinding& info, std::vector<VkSampler>& immutableSampler) noexcept;

					Binding& setBinding(const std::uint32_t& binding) noexcept;
					Binding& setDescriptorType(const VkDescriptorType& type) noexcept;
					Binding& setDescriptorCount(const std::uint32_t &count) noexcept;
					Binding& setShaderStage(const VkShaderStageFlags& stages) noexcept;
					Binding& setImmutableSamplers(const std::vector<VkSampler>& samplers);

					VkDescriptorSetLayoutBinding& get() noexcept;

				private:
					VkDescriptorSetLayoutBinding& _info;
					std::vector<VkSampler>& _immutableSampler;
			};

			DescriptorSetLayout() noexcept;
			~DescriptorSetLayout();

			void prepare(Context& context);
			void initialize();
			void release();

			DescriptorSetLayout& setFlags(const VkDescriptorSetLayoutCreateFlags& flags) noexcept;
			Binding addBinding();

		private:
			Context* _context;
			VkDescriptorSetLayout _layout;

			struct BuildInfo{
				VkDescriptorSetLayoutCreateFlags flags;
				std::vector<VkDescriptorSetLayoutBinding> bindings;
				std::vector<VkSampler> immutableSamplers;
			};

			std::unique_ptr<BuildInfo> _info;

			BuildInfo& getInfo();
	};
}

#endif