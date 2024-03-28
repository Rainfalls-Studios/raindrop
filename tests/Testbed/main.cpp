#include <iostream>
#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Components/Transformation.hpp>
#include <Raindrop/Components/Model.hpp>

#include "config.h"

void testbed(){
	namespace RD = Raindrop;
	namespace fs = std::filesystem;

	using fs::current_path;

	current_path(PATH);
	RD::Raindrop engine;

	auto entity = RD::Entity::create(engine.scene());

	entity.emplace<RD::Components::Model>().model = engine.assetManager().get<RD::Renderer::Model::Model>("Model", current_path() / "models/Sponza/Sponza.gltf");
	entity.emplace<RD::Components::Transformation>();

	engine.run();
}

int main(){
	std::cout << "==========================================" << std::endl;
	std::cout << "=                 TESTBED                =" << std::endl;
	std::cout << "==========================================" << std::endl;
	std::cout << std::endl;
	
	testbed();

	std::cout << std::endl;
	std::cout << "==========================================" << std::endl;
	std::cout << "=                 TESTBED                =" << std::endl;
	std::cout << "==========================================" << std::endl;

	return 0;
}