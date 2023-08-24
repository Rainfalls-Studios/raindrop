#include <Raindrop/Raindrop.hpp>

static std::filesystem::path RESOURCES = RESOURCES_PATH;

int main(int argc, char **argv){
	START_EASYLOGGINGPP(argc, argv);

	Raindrop::Raindrop engine;
	auto& scene = engine.scene();

	auto entity = scene.createEntity();
	
	engine.run();
	return 0;
}