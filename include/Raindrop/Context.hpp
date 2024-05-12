#ifndef __RAINDROP_CONTEXT_HPP__
#define __RAINDROP_CONTEXT_HPP__

#include "common.hpp"

namespace Raindrop{
	namespace Internal{
		class Context;
	}

	class Context{
		public:
			using Handle = void*;

			static Context Create();

			~Context();
			Context(Handle handle);

			Context(const Context& other) = delete;
			Context& operator=(const Context& other) = delete;

			void initialize();
			void release();

			bool isInitialized() const noexcept;
			Handle getHandle() const noexcept;

			Internal::Context* getInternalContext();

			void start();
			void stop();

		private:
			Context();

			Handle _handle;
	};

	inline Context CreateContext(){
		return Context::Create();
	}

	inline void Start(Context& context){
		context.start();
	}
}

#endif