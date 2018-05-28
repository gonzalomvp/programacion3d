#ifdef _MSC_VER
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "common.h"
#include "../lib/glfw/glfw3.h"
#include "buffer.h"
#include "shader.h"
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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

	ShaderPtr shader = Shader::createShader(readString("data/vertex.glsl"), readString("data/fragment.glsl"));
	
	if (!shader) {
		std::cout << Shader::getError() << std::endl;
		glfwTerminate();
		return -1;
	}

	// use program and get locations
	shader->use();

	// initialize opengl states
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);

	// define triangle vertex
	std::vector<Vertex> vertices = {
		Vertex( 0,     0.5f, 0, 1, 0, 0),
		Vertex(-0.5f, -0.5f, 0, 0, 1, 0),
		Vertex( 0.5f, -0.5f, 0, 0, 0, 1)
	};

	// define triangle indexes
	std::vector<uint16_t> indexes = {
		0, 1, 2
	};

	// create buffer with vertex and indexes
	BufferPtr buffer = Buffer::createBuffer(vertices, indexes);

	// calculate matrices which does not change in each tick
	glm::vec3 eye(0.0f, 0.0f, 6.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(eye, center, up);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));

	// main loop
	double lastTime = glfwGetTime();
	float angle = 0.0f;
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

		// logic

		// calculate projection matrix with the screen size of each tick
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		// calculate rotation
		angle += deltaTime * 32;
		if (angle >= 360)
		{
			angle -= 360;
		}
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));

		// calculate model matrix of each triangle
		for (size_t i = 0; i < 9; i++) {
			glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f * (static_cast<float>(i % 3) - 1), 0, -3.0f * static_cast<float> (i / 3)));
			glm::mat4 model = translate * rotate * scale;
			glm::mat4 MVP = projection * view * model;
			shader->setMatrix(shader->getLocation("MVP"), MVP);
			buffer->draw(*shader);
		}

		// refresh screen
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// shutdown
	//glDeleteProgram(program);
	//glDeleteBuffers(1, &vertexBuffer);
	glfwTerminate();
}
