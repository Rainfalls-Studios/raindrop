#include <Prograd/common.hpp>

namespace Prograd{
	::Raindrop::Context*& raindropContext() noexcept{
		static ::Raindrop::Context* context;
		return context;
	}
}