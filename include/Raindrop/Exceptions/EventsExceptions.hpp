#ifndef __RAINDROP_EXCEPTIONS_EVENTS_EXCEPTIONS_HPP__
#define __RAINDROP_EXCEPTIONS_EVENTS_EXCEPTIONS_HPP__

#include "pch.pch"

namespace Raindrop::Exceptions{
	class EventException : public std::exception{
		public:
			EventException(const std::string& eventName, const std::string& reason) : _eventName{eventName}, _reason{reason}{}

			virtual const char* what() const noexcept override{
				return ("Event exception : " + _reason + " : event (" + _eventName + ")").c_str();
			}

			inline constexpr const std::string& eventName() const noexcept{
				return _eventName;
			}

			inline constexpr const std::string& reason() const noexcept{
				return _reason;
			}

		private:
			std::string _eventName;
			std::string _reason;
	};

	class EventNameAlreadyUsed : public EventException{
		public:
			EventNameAlreadyUsed(const std::string& eventName) : EventException(eventName, "The name " + eventName + "is already used"){};
	};
}

#endif