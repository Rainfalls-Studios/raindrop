#ifndef __RAINDROP_OBJECT_HPP__
#define __RAINDROP_OBJECT_HPP__

#include "UUID.hpp"

namespace Raindrop{
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

#endif