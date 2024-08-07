#include <Raindrop/Graphics/ModelLayoutConfig.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics{
	ModelLayoutConfig::MeshProfile::MeshProfile(const VertexLayout& layout) noexcept : 
		_required{0},
		_except{0},
		_layout{layout}
	{}

	ModelLayoutConfig::MeshProfile& ModelLayoutConfig::MeshProfile::require(const UsageFlags& flags) noexcept{
		_required |= flags;
		return *this;
	}

	ModelLayoutConfig::MeshProfile& ModelLayoutConfig::MeshProfile::except(const UsageFlags& flags) noexcept{
		_except |= flags;
		return *this;
	}

	const VertexLayout& ModelLayoutConfig::MeshProfile::getLayout() const noexcept{
		return _layout;
	}

	const ModelLayoutConfig::UsageFlags& ModelLayoutConfig::MeshProfile::getRequired() const noexcept{
		return _required;
	}

	const ModelLayoutConfig::UsageFlags& ModelLayoutConfig::MeshProfile::getExcept() const noexcept{
		return _except;
	}

	bool ModelLayoutConfig::MeshProfile::test(const UsageFlags& flags) const noexcept{
		// ensure that at least all the required flags are here and the expect ones are not
		return (flags & _required == _required) && (flags & _except == 0);
	}

	ModelLayoutConfig::MeshProfile& ModelLayoutConfig::addLayout(const VertexLayout& layout){
		return _profiles.emplace_back(layout);
	}

	const ModelLayoutConfig::MeshProfile* ModelLayoutConfig::find(const UsageFlags& flags) const noexcept{
		for (const auto& profile : _profiles){
			if (profile.test(flags)) return &profile;
		}
		return nullptr;
	}
}