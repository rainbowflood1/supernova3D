#ifndef SUPERNOVA
#define SUPERNOVA


#include <string>
#include <functional>
#include <fstream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Shaders {
public:
	std::string vertex_shader_source = "#version 440 core\nlayout (location = 0) in vec3 aPos;\nvoid main() {\n\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);\n}";
	std::string fragment_shader_source = "#version 440 core\nout vec3 col;\nvoid main() {\n\tcol = vec3(1, 1, 1)\n}";

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	
	void ShaderSourceFiles(std::string vertex_shader_file, std::string fragment_shader_file) {
		std::ifstream vshaderfile(vertex_shader_file);
		std::ifstream fshaderfile(fragment_shader_file);
		
		// Reset all of the shaders to ""
		this->vertex_shader_source = "";
		this->fragment_shader_source = "";

		std::string vshader_line;
		std::string fshader_line;


		// std::getline may not include the newline into the shader so add back the newline at the end
		while (std::getline(vshaderfile, vshader_line)) {
			this->vertex_shader_source = this->vertex_shader_source + vshader_line + "\n";
		}

		while (std::getline(fshaderfile, fshader_line)) {
			this->fragment_shader_source = this->fragment_shader_source + fshader_line + "\n";
		}
		



		vshaderfile.close();
		fshaderfile.close();
	}

	void UnloadShaders() {
		glDeleteShader(this->vshader);
		glDeleteShader(this->fshader);
		glDeleteProgram(this->program);
	}

	
	
};

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

	void Terminate() {
		glfwDestroyWindow(this->window);
		glfwTerminate();

	}
private:
	GLFWwindow* window;
};


#endif
