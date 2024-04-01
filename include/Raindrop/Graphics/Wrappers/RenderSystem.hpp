#ifndef __RAINDROP_GRAPHICS_WRAPPERS_RENDER_SYSTEM_HPP__
#define __RAINDROP_GRAPHICS_WRAPPERS_RENDER_SYSTEM_HPP__

#include "common.hpp"
#include "../RenderSystems/common.hpp"

namespace Raindrop::Graphics::Wrappers{
	template<typename T = RenderSystems::RenderSystem>
	class RenderSystemWrapper{
		public:
			using RenderSystemID = RenderSystems::RenderSystemID;
			using RenderSystem = RenderSystems::RenderSystem;

			static_assert(std::is_base_of_v<RenderSystem, T>, "Cannot create a render system wrapper from a class that is not derived from Raindrop::Graphics::RenderSystem (Raindrop::Graphics::RenderSystems::RenderSystem)");

			RenderSystemWrapper(Context& context) : _context{&context}, _ID{RenderSystems::INVALID_RENDER_SYSTEM_ID}{}
			RenderSystemWrapper(Context& context, const RenderSystemID& ID) : _context{&context}, _ID{ID}{}
			RenderSystemWrapper(const RenderSystemWrapper& other) : _context{other._context}, _ID{other._ID}{}
			~RenderSystemWrapper() = default;


			RenderSystemWrapper& operator=(const RenderSystemWrapper& other){
				_context = other._context;
				_ID = other._ID;
				return *this;
			}

			RenderSystemWrapper& operator=(const RenderSystemID& ID){
				_ID = ID;
				return *this;
			}

			T& get(){
				return static_cast<T&>(get_raw(*_context, _ID));
			}

			const T& get() const{
				return static_cast<const T&>(get_raw(*_context, _ID));
			}

			RenderSystemID ID() const{
				return _ID;
			}

			RenderSystem* operator->(){
				return &get();
			}

			const RenderSystem* operator->() const{
				return &get();
			}

		private:
			Context* _context;
			RenderSystemID _ID;

			static RenderSystem& get_raw(Context& context, const RenderSystemID& ID);
	};
}

#endif