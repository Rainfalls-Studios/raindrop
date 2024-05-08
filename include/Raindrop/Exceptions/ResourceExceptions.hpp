#ifndef __RAINDROP_EXCEPTIONS_RESOURCE_EXCEPTIONS_HPP__
#define __RAINDROP_EXCEPTIONS_RESOURCE_EXCEPTIONS_HPP__

#include "common.hpp"
#include <Raindrop/Asset.hpp>

// TODO: implement asset ID reference

namespace Raindrop::Exceptions{
	// resource exception super class
	class ResourceException : public std::exception{
		public:
			ResourceException(std::string type, std::string reason) : _type{type}, _reason{reason}{}
			ResourceException(const ResourceException& other) : _reason{other._reason}, _type{other._type}{}

			constexpr inline const std::string& type() const noexcept{
				return _type;
			}

			constexpr inline const std::string& reason() const noexcept{
				return _reason;
			}

			virtual const char* what() const noexcept override{
				return _reason.c_str();
			}

		protected:
			std::string _type;
			std::string _reason;
	};

	// Should be used when a resource loading failes
	class ResourceLoadException : public ResourceException{
		public:
			ResourceLoadException(const Path& path, std::string type, std::string reason) : ResourceException(type, reason), _path{path}{}

			using ResourceException::type;
			using ResourceException::reason;
			using ResourceException::what;

			constexpr inline const Path& path() const noexcept{
				return _path;
			}

		private:
			const Path& _path;
	};

	// Should be used when a resource couldn't be found
	class ResourceNotFoundException : public ResourceException{
		public:
			ResourceNotFoundException(const Path& path, std::string type, std::string reason) : ResourceException(type, reason), _path{path}{}

			using ResourceException::type;
			using ResourceException::reason;
			using ResourceException::what;

			constexpr inline const Path& path() const noexcept{
				return _path;
			}

		private:
			const Path& _path;
	};

	// Should be used when the release of a resource failes
	class ResourceReleaseException : public ResourceException{
		public:
			using ResourceException::ResourceException;
			using ResourceException::type;
			using ResourceException::reason;
			using ResourceException::what;
	};
}

#endif