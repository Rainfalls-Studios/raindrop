#pragma once

#include "UUID.hpp"

namespace Raindrop::Core{
	class Object{
		friend class UUIDRegistry;
		public:
			virtual ~Object() = default;

			inline const UUID& getUUID() const noexcept{
				return _uuid;
			}

		private:
			UUID _uuid;
	};
}