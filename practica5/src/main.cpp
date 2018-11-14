#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup /NODEFAULTLIB:MSVCRT")
#endif

#include "ugine3d.h"
#include "../lib/glfw/glfw3.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CAMERA_MOVE_SPEED 5.0f
#define CAMERA_ROT_SPEED  0.2f

struct CameraData;
typedef std::shared_ptr<CameraData> CameraDataPtr;

struct CameraData {
	GLFWwindow* win;
	glm::dvec2 mouseCursorPrev;

	CameraData(GLFWwindow* _win, glm::dvec2 _mouseCursor) : win(_win), mouseCursorPrev(_mouseCursor){}

	static void update(Entity& entity, float deltaTime){
		CameraDataPtr data = std::static_pointer_cast<CameraData>(entity.getUserData());
		glm::dvec2 mouseCursor;
		glfwGetCursorPos(data->win, &mouseCursor.x, &mouseCursor.y);
		glm::dvec2 speedMouse = data->mouseCursorPrev - mouseCursor;
		data->mouseCursorPrev = mouseCursor;

		glm::vec3 cameraRotation = entity.getEuler() + glm::vec3(CAMERA_ROT_SPEED * static_cast<float>(speedMouse.y), CAMERA_ROT_SPEED * static_cast<float>(speedMouse.x), 0.0f);
		cameraRotation = glm::vec3(glm::clamp(cameraRotation.x, -80.0f, 80.0f), cameraRotation.y, cameraRotation.z);
		entity.setEuler(cameraRotation);

		glm::vec3 moveDirection;
		if (glfwGetKey(data->win, GLFW_KEY_W)) {
			moveDirection += glm::vec3(0.0f, 0.0f, -1.0f);
		}
		if (glfwGetKey(data->win, GLFW_KEY_S)) {
			moveDirection += glm::vec3(0.0f, 0.0f, 1.0f);
		}
		if (glfwGetKey(data->win, GLFW_KEY_D)) {
			moveDirection += glm::vec3(1.0f, 0.0f, 0.0f);
		}
		if (glfwGetKey(data->win, GLFW_KEY_A)) {
			moveDirection += glm::vec3(-1.0f, 0.0f, 0.0f);
		}
		if (glm::length(moveDirection) != 0) {
			moveDirection = glm::normalize(moveDirection) * CAMERA_MOVE_SPEED * deltaTime;
			entity.move(moveDirection);
		}
	}
};

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
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if ( !init() ) {
		std::cout << "could not initialize opengl extensions" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Create World
	WorldPtr world = World::create();

	// Load Mesh
	MeshPtr bunnyMesh = Mesh::load("data/bunny.msh.xml");
	//MeshPtr townMesh = Mesh::load("data/asian_town.msh.xml");

	// Create Model
	ModelPtr model = Model::create(bunnyMesh);
	model->setScale(glm::vec3(10.0f));
	model->setEuler(glm::vec3(90.0f, 0.0f, 0.0f));
	world->addEntity(model);

	// Create Camera
	CameraPtr camera = Camera::create();
	camera->setPosition(glm::vec3(0.0f, 2.0f, 3.0f));
	camera->setEuler(glm::vec3(-20.0f, 5.0f, 0.0f));
	camera->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->setCallback(CameraData::update);
	glm::dvec2 mouseCursor;
	glfwGetCursorPos(win, &mouseCursor.x, &mouseCursor.y);
	camera->setUserData(std::make_shared<CameraData>(win, mouseCursor));
	world->addEntity(camera);

	// Create lights
	world->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));

	LightPtr directionalLight = Light::create();
	directionalLight->setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
	directionalLight->setType(Light::DIRECTIONAL);
	directionalLight->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	world->addEntity(directionalLight);

	LightPtr pointLight = Light::create();
	pointLight->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	pointLight->setType(Light::POINT);
	pointLight->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pointLight->setLinearAttenuation(0.2f);
	world->addEntity(pointLight);

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

