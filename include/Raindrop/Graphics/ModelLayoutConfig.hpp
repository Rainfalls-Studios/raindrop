#ifndef __RAINDROP__GRAPHICS_MODEL_LAYOUT_CONFIG_HPP__
#define __RAINDROP__GRAPHICS_MODEL_LAYOUT_CONFIG_HPP__

#include "types.hpp"
#include "pch.pch"
#include "VertexLayout.hpp"

namespace Raindrop::Graphics{
	class ModelLayoutConfig{
		public:
			using Usage = VertexLayout::AttributeUsage;
			using UsageFlags = VertexLayout::UsageFlags;

			class MeshProfile{
				public:
					MeshProfile(const VertexLayout& layout) noexcept;
					~MeshProfile() = default;

					MeshProfile& require(const UsageFlags& flags) noexcept;
					MeshProfile& except(const UsageFlags& flags) noexcept;

					const VertexLayout& getLayout() const noexcept;
					const UsageFlags& getRequired() const noexcept;
					const UsageFlags& getExcept() const noexcept;

					bool test(const UsageFlags& flags) const noexcept;

				private:
					UsageFlags _required;
					UsageFlags _except;
					VertexLayout _layout;
			};

			MeshProfile& addLayout(const VertexLayout& layout);

			const MeshProfile* find(const UsageFlags& flags) const noexcept;

		private:
			std::list<MeshProfile> _profiles;
	};
}

#endif