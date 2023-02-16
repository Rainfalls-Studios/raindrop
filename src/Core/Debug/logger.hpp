#ifndef __RAINDROP_CORE_DEBUG_LOGGER_HPP__
#define __RAINDROP_CORE_DEBUG_LOGGER_HPP__

#include "../core.hpp"
#include "../common.hpp"

#include <cstdio>

namespace Raindrop::Core::Debug::Log{
	enum class Severity : uint8{
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		CRITICAL,
	};

	enum class Cause : uint8{
		SYSTEM,
		MEMORY,
		API,
		ARITMETIC,
		ASSERT,
		THREAD,
		IO,
		OTHER,
	};

	const char* severityToStr(Severity severity);
	const char* causeToStr(Cause cause);

	struct Log{
		Log(){}

		Severity severity;
		Cause cause;

		char message[1048];
		const char* file;
		const char* function;
		const char* condition;
	};

	class LogModule{
		friend class Logger;
		public:
			LogModule() = default;
			virtual ~LogModule() = default;

			virtual void callback(const Log &msg) = 0;
		
		protected:

			/**
			 * @brief push the module into the logger, cannot be pushed mutiple times
			 * 
			 */
			void push();

			/**
			 * @brief pop the mofule from the logger, cannot be poped if the module is not pushed
			 * 
			 */
			void pop();

		private:
			LogModule* next;
	};

	// output message
	void log(const Log &msg);
}

namespace Raindrop::Core::Debug{
	void inline log(const Log::Log &msg){
		Log::log(msg);	
	}
}

#define RAINDROP_log(_severity, _cause, ...) {								\
	::Raindrop::Core::Debug::Log::Log __log__;								\
	__log__.severity = Raindrop::Core::Debug::Log::Severity::_severity;		\
	__log__.cause = Raindrop::Core::Debug::Log::Cause::_cause;				\
	__log__.function = __PRETTY_FUNCTION__;									\
	__log__.file = __FILE__;												\
	__log__.condition = "none";												\
	sprintf(__log__.message, __VA_ARGS__);									\
	Raindrop::Core::Debug::log(__log__);									\
}

#define RAINDROP_cond_log(_cond, _severity, _cause, ...) {					\
	if (_cond){																\
		::Raindrop::Core::Debug::Log::Log __log__;							\
		__log__.severity = Raindrop::Core::Debug::Log::Severity::_severity;	\
		__log__.cause = Raindrop::Core::Debug::Log::Cause::_cause;			\
		__log__.function = __PRETTY_FUNCTION__;								\
		__log__.file = __FILE__;											\
		__log__.condition = #_cond;											\
		sprintf(__log__.message, __VA_ARGS__);								\
		Raindrop::Core::Debug::log(__log__);								\
	}																		\
}

#endif