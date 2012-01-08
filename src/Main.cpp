#include <cstdlib>
#include <Engine.hpp>

int main() {
	if( terra::Engine::Get().Initialize() ) {
		terra::Engine::Get().Run();
	}
	return EXIT_SUCCESS;
}
