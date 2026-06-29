#include <iostream>
#include "../supernova.hpp"

Shaders shader;
Mesh mesh;

Game game(800, 600, "Game");

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


	shader.CreateShaders();
	
	mesh.Init();


	game.SkyColor = Color(0.5f, 0.5f, 0.5f, 1.0f);


	game.Update(&update);
	
	shader.UnloadShaders();

	game.Terminate();	
	
	return 0;
}
