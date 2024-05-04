#ifndef __RAINDROP_INLINE_GUID_HPP__
#define __RAINDROP_INLINE_GUID_HPP__

#include "../GUID.hpp"

namespace Raindrop{
	template<typename T>
	inline void GUID::CreateStorage(Context& context){
		_CreateStorage(context, sizeof(T), typeid(T));
	}

	template<typename T>
	inline GUID GUID::Store(Context& context, const T& t){
		return _Store(context, typeid(T), static_cast<const void*>(&t));
	}

	template<typename T>
	inline void GUID::Remove(Context& context, const GUID& guid){
		_Remove(context, typeid(T), guid);
	}

	template<typename T>
	inline T& GUID::Get(Context& context, const GUID& guid){
		return _Get(context, typeid(T), guid);
	}
}

#endif