#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup /NODEFAULTLIB:MSVCRT")
#endif

#include "ugine3d.h"
#include "../lib/glfw/glfw3.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void rotateTriangle(Entity& entity, float deltaTime) {
	// with quaternions
	entity.setRotation(entity.getRotation() * glm::quat(glm::radians(glm::vec3(0.0f, 32 * deltaTime, 0.0f))));
	// with euler angles
	//entity.setRotation(glm::vec3(entity.getRotation().x, entity.getRotation().y + 32.0f * deltaTime, entity.getRotation().z));
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

	if ( !init() ) {
		std::cout << "could not initialize opengl extensions" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Create World
	WorldPtr world = World::createWorld();

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

	MeshPtr triangleMesh = Mesh::createMesh();
	triangleMesh->addBuffer(buffer);

	for (size_t i = 0; i < 9; i++) {
		ModelPtr model = Model::createModel(triangleMesh);
		model->setPosition(glm::vec3(3.0f * (static_cast<float>(i % 3) - 1), 0, -3.0f * static_cast<float> (i / 3)));
		model->setCallback(rotateTriangle);
		world->addEntity(model);
	}

	CameraPtr camera = Camera::createCamera();
	camera->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
	camera->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));
	world->addEntity(camera);

	// main loop
	double lastTime = glfwGetTime();
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
		
		world->update(deltaTime);
		world->draw();

		// refresh screen
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	// shutdown
	glfwTerminate();
}

