#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup /NODEFAULTLIB:MSVCRT")
//#pragma comment(linker, "/ENTRY:mainCRTStartup /NODEFAULTLIB:MSVCRT")
#endif

#include "ugine3d.h"
#include "../lib/glfw/glfw3.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

static void moveLight(Entity& entity, float deltaTime) {
	float pitch = glm::radians(-30.0f);
	float yaw = atan2f(entity.getPosition().x, entity.getPosition().z) + glm::radians(32.0f) * deltaTime;
	float roll = 0.0f;
	glm::quat rotation = glm::quat(glm::vec3(pitch, yaw, roll));
	entity.setPosition(rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
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
	//glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if ( !init() ) {
		std::cout << "could not initialize opengl extensions" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Create World
	WorldPtr world = World::create();
	world->setShadows(true);
	world->setDepthOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);

	// Create scene
	MeshPtr sceneMesh = Mesh::load("data/scene.msh.xml");
	ModelPtr sceneModel = Model::create(sceneMesh);
	sceneModel->setScale(glm::vec3(1.0f));
	sceneModel->setEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	sceneModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	world->addEntity(sceneModel);

	// Create Camera
	CameraPtr camera = Camera::create();
	camera->setPosition(glm::vec3(0.0f, 8.0f, -12.0f));
	camera->setEuler(glm::vec3(-30.0f, 180.0f, 0.0f));
	camera->setClearColor(glm::vec3(0.15f, 0.15f, 0.15f));
	world->addEntity(camera);

	// Set ambient color
	world->setAmbient(glm::vec3(0.5f));

	// Create point light moving with camera
	LightPtr directionalLight = Light::create(Light::DIRECTIONAL);
	directionalLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	directionalLight->setCallback(moveLight);
	world->addEntity(directionalLight);

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
		camera->setProjection(glm::perspective(glm::radians(60.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f));
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

