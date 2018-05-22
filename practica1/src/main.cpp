#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "common.h"
#include "../lib/glfw/glfw3.h"
#include <fstream>
#include <iostream>
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct Vertex {
	float x, y;
	float r, g, b;

	Vertex(float x, float y, float r, float g, float b)
	:
		x(x), y(y),
		r(r), g(g), b(b)
	{}
};

std::string readString(const std::string& filename) {
	std::ifstream ifs(filename.c_str(), std::ios_base::binary | std::ios_base::ate);
	std::vector<char> chars(ifs.tellg());
	ifs.seekg(0);
	ifs.read(chars.data(), chars.size());
	return std::string(chars.data(), chars.size());
}

int main() {
	// init glfw
	if ( !glfwInit() ) {
		std::cout << "could not initialize glfw" << std::endl;
		return -1;
	}

	// create window
	//glfwWindowHint(GLFW_RESIZABLE, false);
	glfwWindowHint(GLFW_SAMPLES, 8);
	GLFWwindow* win = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "", nullptr, nullptr);
	if (!win) {
		std::cout << "could not create opengl window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);

	// initialize opengl extensions
	if ( glewInit() != GLEW_OK ) {
		std::cout << "could not initialize opengl extensions" << std::endl;
		glfwTerminate();
		return -1;
	}

	// load shaders code
	std::string vertexShaderSource = readString("data/vertex.glsl");
	std::string fragmentShaderSource = readString("data/fragment.glsl");

	// create vertex shader
	int retCode;
	const char* cVertexShaderSource = vertexShaderSource.c_str();
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cVertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		char errorLog[1024];
		glGetShaderInfoLog(vertexShader, sizeof(errorLog), nullptr, errorLog);
		std::cout
			<< "vertex shader could not be compiled:"
			<< std::endl
			<< errorLog
			<< std::endl;
		glDeleteShader(vertexShader);
		glfwTerminate();
		return -1;
	}

	// create fragment shader
	const char* cFragmentShaderSource = fragmentShaderSource.c_str();
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cFragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		char errorLog[1024];
		glGetShaderInfoLog(fragmentShader, sizeof(errorLog), nullptr, errorLog);
		std::cout
			<< "fragment shader could not be compiled:"
			<< std::endl
			<< errorLog
			<< std::endl;
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glfwTerminate();
		return -1;
	}

	// create and link program
	uint32_t program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(program, GL_LINK_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		char errorLog[1024];
		glGetProgramInfoLog(program, sizeof(errorLog), nullptr, errorLog);
		std::cout
			<< "program could not be linked:"
			<< std::endl
			<< errorLog
			<< std::endl;
		glDeleteProgram(program);
		glfwTerminate();
		return -1;
	}

	// use program and get locations
	int vposLoc = glGetAttribLocation(program, "vpos");
	int vcolorLoc = glGetAttribLocation(program, "vcolor");
	glUseProgram(program);

	// initialize opengl states
	glEnable(GL_SCISSOR_TEST);
	//glEnableClientState(GL_VERTEX_ARRAY); // ***
	//glEnableClientState(GL_COLOR_ARRAY); // ***
	glEnableVertexAttribArray(vposLoc);
	glEnableVertexAttribArray(vcolorLoc);

	// define triangle
	std::vector<Vertex> vertices = {
		Vertex( 0,  1,  1, 0, 0),
		Vertex(-1, -1,  0, 1, 0),
		Vertex( 1, -1,  0, 0, 1)
	};

	// store triangle in vram
	uint32_t vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// describe data for vertex buffer objects
	//glVertexPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, x))); // ***
	//glColorPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, r))); // ***
	glVertexAttribPointer(vposLoc, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, x)));
	glVertexAttribPointer(vcolorLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, r)));

	// describe data for vertex arrays
	//glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].x); // ***
	//glColorPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].r); // ***

	// main loop
	double lastTime = glfwGetTime();
	while ( !glfwWindowShouldClose(win) && !glfwGetKey(win, GLFW_KEY_ESCAPE) ) {
		// get delta time
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		// get window size
		int screenWidth, screenHeight;
		glfwGetWindowSize(win, &screenWidth, &screenHeight);

		// setup viewport
		glViewport(0, 0, screenWidth, screenHeight);
		glScissor(0, 0, screenWidth, screenHeight);

		// clear screen
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw with vertex arrays & vbos
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		
		// draw with glBegin ... glEnd
		/*glBegin(GL_TRIANGLES);
			glColor4f(1, 0, 0, 1);
			glVertex2f(0, 1);
			glColor4f(0, 1, 0, 1);
			glVertex2f(-1, -1);
			glColor4f(0, 0, 1, 1);
			glVertex2f(1, -1);
		glEnd();*/

		// refresh screen
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// shutdown
	glDeleteProgram(program);
	glDeleteBuffers(1, &vertexBuffer);
	glfwTerminate();
}
