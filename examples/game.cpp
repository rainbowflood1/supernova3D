#include <iostream>
#include "../supernova.hpp"

Game game(800, 600, "Game");

void update(double dt) {
	std::cout << dt << std::endl;
}

int main() {
	game.Update(&update);
	return 0;
}
