#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup /NODEFAULTLIB:MSVCRT")
#endif

#include "common.h"
#include "../lib/glfw/glfw3.h"
#include "buffer.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "state.h"
#include "world.h"
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

bool init();

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

	if ( !init() ) {
		std::cout << "could not initialize opengl extensions" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Create World
	WorldPtr world = std::make_shared<World>();

	// define triangle vertices
	std::vector<Vertex> vertices = {
		Vertex( glm::vec3( 0,     0.5f, 0),  glm::vec3(1, 0, 0)),
		Vertex( glm::vec3(-0.5f, -0.5f, 0),  glm::vec3(0, 1, 0)),
		Vertex( glm::vec3( 0.5f, -0.5f, 0),  glm::vec3(0, 0, 1))
	};

	// define triangle indexes
	std::vector<uint16_t> indexes = {
		0, 1, 2
	};

	// create buffer with vertices and indexes
	BufferPtr buffer = Buffer::createBuffer(vertices, indexes);
	if (!buffer) {
		std::cout << "could not create Buffer" << std::endl;
		glfwTerminate();
		return -1;
	}

	MeshPtr triangleMesh = std::make_shared<Mesh>();
	triangleMesh->addBuffer(buffer);

	for (size_t i = 0; i < 9; i++) {
		ModelPtr model = std::make_shared<Model>(triangleMesh);
		model->setPosition(glm::vec3(3.0f * (static_cast<float>(i % 3) - 1), 0, -3.0f * static_cast<float> (i / 3)));
		model->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		model->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
		world->addEntity(model);
	}

	CameraPtr camera = std::make_shared<Camera>();
	camera->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	camera->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));
	world->addEntity(camera);

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

		// calculate projection matrix with the screen size in each tick
		camera->setProjection(glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f));
		camera->setViewport(glm::ivec4(0, 0, screenWidth, screenHeight));
		// calculate rotation
		angle += 32 * deltaTime;
		if (angle >= 360) {
			angle -= 360;
		}

		for (size_t i = 0; i < world->getNumEntities(); i++) {
			CameraPtr camera = std::dynamic_pointer_cast<Camera>(world->getEntity(i));
			if (!camera) {
				world->getEntity(i)->setRotation(glm::vec3(0.0f, angle, 0.0f));
			}
		}
		
		world->update(deltaTime);
		world->draw();

		// refresh screen
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// shutdown
	glfwTerminate();
}

bool init() {
	bool ret = false;

	// initialize opengl extensions and opengl states
	if (glewInit() == GLEW_OK) {
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);
		ret = true;
	}

	// create default shader
	std::shared_ptr<Shader> shader = Shader::createShader(readString("data/vertex.glsl"), readString("data/fragment.glsl"));
	if (shader) {
		State::defaultShader = shader;
		ret = true;
	}

	return ret;
}