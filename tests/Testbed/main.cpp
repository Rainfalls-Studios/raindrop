#include <Raindrop/Raindrop.hpp>
#include <spdlog/spdlog.h>

class Testbed : public Raindrop::Engine{
	public:
		Testbed() : Engine(){
			Engine::initialize(INIT_EVERYTHING);
		}

		void run(){
			auto& windowContext = getGraphicsWindowContext();
		}

	private:

};

int main(){
	Testbed testbed;

	return EXIT_SUCCESS;
}