#ifndef __RAINDROP_EXCEPTIONS_MEMORY_EXCEPTIONS_HPP__
#define __RAINDROP_EXCEPTIONS_MEMORY_EXCEPTIONS_HPP__

#include "common.hpp"

namespace Raindrop::Exceptions{
	class DoubleFreeException : public std::exception{
		public:
   			DoubleFreeException(const std::string& resourceName = "") : _resourceName(resourceName) {}

			virtual const char* what() const noexcept override{
				return ("Attempt to free an already freed resource" + (_resourceName.empty() ? "" : (" - " + _resourceName))).c_str();
			}

			const std::string& resourceName() const noexcept{
				return _resourceName;
			}
		
		protected:
			std::string _resourceName;
	};
}

#endif