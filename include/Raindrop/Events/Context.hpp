#ifndef __RAINDROP_EVENTS_CONTEXT_HPP__
#define __RAINDROP_EVENTS_CONTEXT_HPP__

#include "types.hpp"
#include "pch.pch"
#include "Manager.hpp"
#include "Inputs.hpp"

namespace Raindrop::Events{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;
		Raindrop::Context* raindrop;

		Manager manager;
		Inputs inputs;

		void initialize(Raindrop::Context& raindrop);
		void createLogger();
	};
}

#endif