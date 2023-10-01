#include <Raindrop/Core/Core.hpp>
#include <Raindrop/Core/Context.hpp>

namespace Raindrop::Core{
	Core::Core(){
		_context = std::make_unique<Context>();
		_context->logger.info("Initializing Core...");
		_context->logger.info("Core initialized without any critical error");
	}

	Core::~Core(){
		_context->logger.info("Terminating Core...");
		_context->logger.info("Core terminated without any critical error");
		_context.reset();
	}
}