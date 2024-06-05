#ifndef __RAINDROP_BUFFER_HPP__
#define __RAINDROP_BUFFER_HPP__

#include "common.hpp"

namespace Raindrop{
	class Buffer{
		public:
			struct Impl;

			static Buffer Create(Context& context);

			Buffer(Context& context);
			~Buffer();

			Buffer(const Buffer& other);
			Buffer& operator=(const Buffer& other);

			bool isInitialized() const noexcept;
			void* getNativeHandle() const;
			Impl* getImpl() const noexcept;
			GUID getGUID() const noexcept;

			void allocate()

		private:
			Impl* _impl;
	};

	inline static Buffer CreateBuffer(Context& context){
		return Buffer::Create(context);
	}

}

#endif