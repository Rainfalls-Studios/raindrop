#include <Raindrop/Core/Context.hpp>

namespace Raindrop::Core{
	Context::Context() : 
		rootDirectory{std::filesystem::current_path()},
		dataDirectory{rootDirectory / ".data"},
		config{dataDirectory / "config.ini"}
	{
		registry["DataDirectory"] = dataDirectory;
		registry["RootDirectory"] = dataDirectory;
	}	
}