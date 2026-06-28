#ifndef SUPERNOVA
#define SUPERNOVA


#include <string>
#include <functional>

#include <glad/gl.h>
#include <GLFW/glfw3.h>


class Game {
public:
	// Window
	int WindowWidth;
	int WindowHeight;
	std::string WindowName;
	Game(int WindowWidth, int WindowHeight, std::string WindowName) {
		this->WindowWidth = WindowWidth;
		this->WindowHeight = WindowHeight;
		this->WindowName = WindowName;
		// Initalize GLFW
		glfwInit();

		// Tell GLFW what version of OpenGL we are using (4.6)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		this->window = glfwCreateWindow(this->WindowWidth, this->WindowHeight, this->WindowName.c_str(), nullptr, nullptr);

		if (!this->window) {
			std::cerr << "ERROR! WINDOW CREATION FAILED!";
		}

		glfwMakeContextCurrent(window);

		gladLoadGL(glfwGetProcAddress);
	}

	void Update(std::function<void(double)> update_function) {
		double last_time = 0.0;
		double dt = 0.0;

		while (!glfwWindowShouldClose(this->window)) {
			dt = glfwGetTime()-last_time;
			glfwPollEvents();
			glViewport(0, 0, this->WindowWidth, this->WindowHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glClearColor(0, 0.1, 1, 1);
			update_function(dt);

			
			
			glfwSwapBuffers(this->window);
			last_time = glfwGetTime();
		}
	}

	~Game() {
		glfwDestroyWindow(this->window);
		glfwTerminate();

	}
private:
	GLFWwindow* window;
};


#endif
