#ifndef SUPERNOVA
#define SUPERNOVA


#include <string>
#include <functional>
#include <fstream>
#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define TRIANGLES GL_TRIANGLES


class Shaders {
public:
	std::string vertex_shader_source = "#version 440 core\nlayout (location = 0) in vec3 aPos;\nvoid main() {\n\tgl_Position = vec4(aPos.x, aPos.y, aPos.z, 1);\n}";
	std::string fragment_shader_source = "#version 440 core\nout vec3 col;\nvoid main() {\n\tcol = vec3(1, 1, 1);\n}";

	GLuint vshader;
	GLuint fshader;
	GLuint program;

	void CreateShaders() {
		// Because it is an std::string, it will be converted to a const char* by using std::string::c_str()
		const char* vertex_shader_const_char = this->vertex_shader_source.c_str();
		const char* fragment_shader_const_char = this->fragment_shader_source.c_str();
		
		// Create Shaders
		this->vshader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(this->vshader, 1, &vertex_shader_const_char, NULL);
		glCompileShader(this->vshader);

		this->fshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(this->fshader, 1, &fragment_shader_const_char, NULL);
		glCompileShader(this->fshader);

		// Attach shaders to the program
		
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vshader);
		glAttachShader(this->program, this->fshader);
		glLinkProgram(this->program);
	}
	
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

	void UseShaders() {
		glUseProgram(this->program);
	}

	void UnloadShaders() {
		glDeleteShader(this->vshader);
		glDeleteShader(this->fshader);
		glDeleteProgram(this->program);
	}

	
	
};

class Color {
public:
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;
	float a = 0.0;

	Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};

class Vector3 {
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	Vector3(GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Mesh {
public:
	int mode = TRIANGLES;
	std::vector<Vector3> Verts;
	std::vector<int> Ind;
	void Init() {
		size_t points_size = this->Verts.size()*sizeof(Vector3);	

		GLfloat points[points_size];
		
		for (int i = 0; i < points_size; i++) {
			points[i] = this->VecToFloat().data()[i];
		}	


		size_t indices_size = this->Ind.size();

		int indices[indices_size];

		for (int i = 0; i < indices_size; i++) {
			indices[i] = this->Ind[i];
		}


		// Initalize buffers
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}

	void Render() {
		glBindVertexArray(this->VAO);
		glDrawElements(this->mode, this->Ind.size(), GL_UNSIGNED_INT, 0);
	}
private:
	GLuint VBO, VAO, EBO;

	std::vector<GLfloat> VecToFloat() {
		size_t points_size = this->Verts.size()*sizeof(Vector3);	
		std::vector<GLfloat> vector_points;

		for (Vector3 vec : this->Verts) {
			vector_points.push_back(vec.x);
			vector_points.push_back(vec.y);
			vector_points.push_back(vec.z);
		}

		return vector_points;
	}
};

class Game {
public:
	// Window
	int WindowWidth;
	int WindowHeight;
	std::string WindowName;
	// Render
	Color SkyColor = Color();

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

		glViewport(0, 0, this->WindowWidth, this->WindowHeight);

		while (!glfwWindowShouldClose(this->window)) {
			dt = glfwGetTime()-last_time;
			glClearColor(this->SkyColor.r, this->SkyColor.g, this->SkyColor.b, this->SkyColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			update_function(dt);
			
			
			glfwPollEvents();
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
