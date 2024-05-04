#include <Raindrop/GUID.hpp>

namespace Raindrop{
	GUID GUID::Generate(Context& context) noexcept{

	}

	void GUID::_CreateStorage(Context& context, const std::size_t& typeSize, const std::type_index& typeIndex){

	}

	GUID GUID::_Store(Context& context, const std::type_index& typeId, const void* data){

	}

	void GUID::_Remove(Context& context, const std::type_index& typeId, const GUID& guid){

	}

	void* GUID::_Get(Context& context, const std::type_index& typeId, const GUID& guid){

	}

	GUID::GUID(const GUID& other) noexcept : _high{other._high}, _low{other._low}{}

	GUID& GUID::operator=(const GUID& other) noexcept{
		_high = other._high;
		_low = other._low;
		return *this;
	}

}