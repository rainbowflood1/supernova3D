#include <iostream>
#include "../supernova.hpp"

Shaders shader;
Mesh mesh;

void update(double dt) {
	shader.UseShaders();
	mesh.Render();
	//std::cout << dt << std::endl;
}

int main() {
	mesh.Verts = {
		Vector3(-0.5, 0, 0),
		Vector3(0.5, 0, 0),
		Vector3(0, 0.5, 0),
		Vector3(0.5, 0.5, 0)
	};
	
	mesh.Ind = {
		0, 1, 2,
		3, 2, 1
	};

	Game game(800, 600, "Game");

	shader.CreateShaders();
	
	mesh.Init();
	
	game.Update(&update);
	
	shader.UnloadShaders();

	game.Terminate();	
	
	return 0;
}
