#ifndef __RAINDROP_ASSETS_CONTEXT_HPP__
#define __RAINDROP_ASSETS_CONTEXT_HPP__

#include "pch.pch"
#include "Manager.hpp"
#include "types.hpp"

namespace Raindrop::Assets{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;
		Raindrop::Context* raindrop;

		Manager manager;
		
		void initialize(Raindrop::Context& raindrop);
		void createLogger();
	};
}

#endif