#ifndef __RAINDROP_GUID_HPP__
#define __RAINDROP_GUID_HPP__

#include "common.hpp"

namespace Raindrop{
	class GUID{
		public:
			static GUID Generate(Context& context) noexcept;

			template<typename T>
			static inline void CreateStorage(Context& context);

			template<typename T>
			static inline GUID Store(Context& context, const T& t);

			template<typename T>
			static inline void Remove(Context& context, const GUID& guid);

			template<typename T>
			static inline T& Get(Context& context, const GUID& guid);


			GUID(const GUID& other) noexcept;
			GUID& operator=(const GUID& other) noexcept;
			

		private:
			std::uint64_t _high;
			std::uint64_t _low;

			static void _CreateStorage(Context& context, const std::size_t& typeSize, const std::type_index& typeIndex);
			static GUID _Store(Context& context, const std::type_index& typeId, const void* data);
			static void _Remove(Context& context, const std::type_index& typeId, const GUID& guid);
			static void* _Get(Context& context, const std::type_index& typeId, const GUID& guid);
	};
}

#include "Inline/GUID.inl"

#endif