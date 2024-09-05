#pragma once

#include "Raindrop/Core/Object.hpp"

namespace Raindrop::Core::Assets{
	class Asset : public Object{
		public:
			virtual ~Asset() = default;
	};
}