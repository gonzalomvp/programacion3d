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
	//glEnableClientState(GL_VERTEX_ARRAY); // ***
	//glEnableClientState(GL_COLOR_ARRAY); // ***
	//glEnableVertexAttribArray(vposLoc);
	//glEnableVertexAttribArray(vcolorLoc);

	// define triangle
	std::vector<Vertex> vertices = {
		Vertex( 0,     0.5f, 0, 1, 0, 0),
		Vertex(-0.5f, -0.5f, 0, 0, 1, 0),
		Vertex( 0.5f, -0.5f, 0, 0, 0, 1)
	};

	std::vector<uint16_t> indexes = {
		0, 1, 2
	};

	BufferPtr buffer = Buffer::createBuffer(vertices, indexes);

	// store triangle in vram
	//uint32_t vertexBuffer;
	//glGenBuffers(1, &vertexBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// describe data for vertex buffer objects
	//shader->setupAttribs();

	//glVertexPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, x))); // ***
	//glColorPointer(3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, r))); // ***
	//glVertexAttribPointer(vposLoc, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, x)));
	//glVertexAttribPointer(vcolorLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, r)));

	// describe data for vertex arrays
	//glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].x); // ***
	//glColorPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].r); // ***

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


		//matriz
		angle += deltaTime * 32;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::vec3 eye(0.0f, 0.0f, 6.0f);
		glm::vec3 center(0.0f, 0.0f, 0.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::mat4 view = glm::lookAt(eye, center, up);
		//glm::mat4 model = glm::mat4(1.0f);

		int i = shader->getLocation("MVP");
		

		// pinta primera fila
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		glm::mat4 MVP = projection * view * model;
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-3, 0, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		MVP = projection * view * model;
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		MVP = projection * view * model;
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);

		// pinta segunda fila
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		MVP = projection * view * model;
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-3, 0, -3)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		MVP = projection * view * model;
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, -3)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		MVP = projection * view * model;
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);

		// pinta tercera fila
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		MVP = projection * view * model;
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-3, 0, -6)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		MVP = projection * view * model;
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, -6)) * glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));
		MVP = projection * view * model;
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);


		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		
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
	//glDeleteProgram(program);
	//glDeleteBuffers(1, &vertexBuffer);
	glfwTerminate();
}
