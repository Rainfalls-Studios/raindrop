#ifndef __RAINDROP_EXCEPTIONS_FILSYSTEM_EXCEPTIONS_HPP__
#define __RAINDROP_EXCEPTIONS_FILSYSTEM_EXCEPTIONS_HPP__

#include "pch.pch"

namespace Raindrop::Exceptions{
	class FilesystemException : public std::exception{
		public:
			FilesystemException(const Path& path, std::string reason, const std::error_code& code) : _reason{reason}, _path{path}, _code{code}{}
			FilesystemException(const FilesystemException& other) : _reason{other._reason}, _path{other._path}{}
			FilesystemException() noexcept = default;

			inline FilesystemException& operator=(const FilesystemException& other){
				_reason = other._reason,
				_path = other._path;
				return *this;
			}

			virtual const char* what() const noexcept override{
				return _reason.c_str();
			}

			inline constexpr const Path& path() const noexcept{
				return _path;
			}

			inline constexpr const std::error_code& code() const noexcept{
				return _code;
			}
		
		protected:
			std::string _reason;
			Path _path;
			std::error_code _code;
	};
	
	#define USING_FILESYSTEM_EXCEPTION using FilesystemException::FilesystemException; using FilesystemException::operator=; using FilesystemException::what; using FilesystemException::path;

	// As the name suggest, should be thrown when a path does not exists / has not be found
	class PathNotFoundException : public FilesystemException{
		public:
			USING_FILESYSTEM_EXCEPTION;
	};

	// Specilized varient of PathNotFoundException for files
	class FileNotFoundException : public PathNotFoundException{
		public:
			using PathNotFoundException::PathNotFoundException;
			using PathNotFoundException::operator=;
			using PathNotFoundException::what;
			using PathNotFoundException::path;
	};

	class FileAccessDeniedException : public FilesystemException{
		public:
			USING_FILESYSTEM_EXCEPTION;

			inline std::filesystem::perms permisions() const{
				try{
					return std::filesystem::status(_path).permissions();
				} catch (const std::filesystem::filesystem_error& e){
					throw FileNotFoundException(_path, e.what(), e.code());
				}
			}
	};

	class FileReadException : public FilesystemException{
		public:
			FileReadException(const Path& path, std::string reason, const std::error_code& code, std::size_t offset, std::size_t size) : FilesystemException(path, reason, code), _offset{offset}, _size{size}{}

			using FilesystemException::operator=;
			using FilesystemException::what;
			using FilesystemException::path;

			// The offset within the file when the exception occured
			inline constexpr std::size_t offset() const noexcept{
				return _offset;
			}

			// The size of the data being read when the exception occured
			inline constexpr std::size_t size() const noexcept{
				return _size;
			}
		
		private:
			std::size_t _offset;
			std::size_t _size;
	};

	class FileWriteException : public FilesystemException{
		public:
			FileWriteException(const Path& path, std::string reason, const std::error_code& code, std::size_t offset, std::size_t size) : FilesystemException(path, reason, code), _offset{offset}, _size{size}{}

			using FilesystemException::operator=;
			using FilesystemException::what;
			using FilesystemException::path;

			// The offset within the file when the exception occured
			inline constexpr std::size_t offset() const noexcept{
				return _offset;
			}

			// The size of the data being read when the exception occured
			inline constexpr std::size_t size() const noexcept{
				return _size;
			}
		
		private:
			std::size_t _offset;
			std::size_t _size;

	};

	class FileFormatException : public FilesystemException{
		public:
			FileFormatException(const Path& path, std::string reason,  const std::error_code& code, std::string format) : FilesystemException(path, reason, code), _format{format}{}

			using FilesystemException::operator=;
			using FilesystemException::what;
			using FilesystemException::path;

			inline constexpr const std::string& format() const noexcept{
				return _format;
			}
		
		private:
			std::string _format;
	};
}

#endif