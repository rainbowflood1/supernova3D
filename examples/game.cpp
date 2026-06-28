#include <iostream>
#include "../supernova.hpp"

Game game(800, 600, "Game");

void update(float dt) {
	std::cout << "Hello world!" << std::endl;
}

int main() {
	game.Update(&update);
	return 0;
}
