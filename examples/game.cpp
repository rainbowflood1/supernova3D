#include <iostream>
#include "../supernova.hpp"





void update(double dt) {
	//std::cout << dt << std::endl;
}

int main() {
	Game game(800, 600, "Game");
	Shaders shader;
	
	shader.ShaderSourceFiles("./examples/vertex_shader.glsl", "./examples/fragment_shader.glsl");
	
	game.Update(&update);
	
	shader.UnloadShaders();
	std::cout << shader.vertex_shader_source << std::endl;
	return 0;
}
