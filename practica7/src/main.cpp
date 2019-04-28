#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup /NODEFAULTLIB:MSVCRT")
//#pragma comment(linker, "/ENTRY:mainCRTStartup /NODEFAULTLIB:MSVCRT")
#endif

#include "ugine3d.h"
#include "../lib/glfw/glfw3.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CAMERA_MOVE_SPEED 5.0f
#define CAMERA_ROT_SPEED  0.2f

struct CameraData;
typedef std::shared_ptr<CameraData> CameraDataPtr;

struct LightData;
typedef std::shared_ptr<LightData> LightDataPtr;

struct CameraData {
	const glm::vec3 pivot;
	const glm::vec3 lookAtPos;
	float angularSpeed;
	float distance;

	CameraData(const glm::vec3& _pivot, const glm::vec3& _lookAtPos, float _angularSpeed, float _distance) : pivot(_pivot), lookAtPos(_lookAtPos), angularSpeed(_angularSpeed), distance(_distance) {}

	static void rotateCamera(Entity& entity, float deltaTime) {
		CameraDataPtr data = std::static_pointer_cast<CameraData>(entity.getUserData());
		float angle = atan2(entity.getPosition().z, entity.getPosition().x);
		angle += glm::radians(data->angularSpeed) * deltaTime;
		entity.setPosition(data->pivot + glm::vec3(cosf(angle), 0.0f, sinf(angle)) * data->distance);

		glm::mat4 lookAtMatrix = glm::lookAt(entity.getPosition(), data->lookAtPos, glm::vec3(0.0f, 1.0f, 0.0f));
		entity.setQuaternion(glm::quat(glm::transpose(lookAtMatrix)));
	}
};

struct LightData {
	const EntityPtr refEntity;

	LightData(const EntityPtr _refEntity) : refEntity(_refEntity) {}

	static void moveLight(Entity& entity, float deltaTime) {
		LightDataPtr data = std::static_pointer_cast<LightData>(entity.getUserData());
		entity.setPosition(data->refEntity->getPosition());
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
	//glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if ( !init() ) {
		std::cout << "could not initialize opengl extensions" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Create World
	WorldPtr world = World::create();

	// Create skybox
	ShaderPtr skyboxShader = Shader::create(readString("data/skybox_vertex.glsl"), readString("data/skybox_fragment.glsl"));
	MeshPtr skyboxMesh = Mesh::load("data/skybox.msh.xml", skyboxShader);
	ModelPtr skyboxModel = Model::create(skyboxMesh);
	world->addEntity(skyboxModel);

	// Create teapot
	MeshPtr teapotMesh = Mesh::load("data/teapot_reflect.msh.xml");
	ModelPtr teapotModel = Model::create(teapotMesh);
	teapotModel->setScale(glm::vec3(1.0f));
	teapotModel->setEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	teapotModel->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	world->addEntity(teapotModel);

	// Create monkey
	MeshPtr suzanneMesh = Mesh::load("data/suzanne_refract.msh.xml");
	ModelPtr suzanneModel = Model::create(suzanneMesh);
	suzanneModel->setScale(glm::vec3(1.0f));
	suzanneModel->setEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	suzanneModel->setPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
	world->addEntity(suzanneModel);

	// Create wall cube
	MeshPtr cubeMesh = Mesh::load("data/cube.msh.xml");
	ModelPtr cubeModel = Model::create(cubeMesh);
	cubeModel->setScale(glm::vec3(1.0f));
	cubeModel->setEuler(glm::vec3(0.0f, 0.0f, 90.0f));
	cubeModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	world->addEntity(cubeModel);

	// Create Camera
	CameraPtr camera = Camera::create();
	camera->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
	camera->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->setCallback(CameraData::rotateCamera);
	camera->setUserData(std::make_shared<CameraData>(cubeModel->getPosition(), cubeModel->getPosition(), -30.f, 10.0f));
	world->addEntity(camera);

	// Set ambient color
	world->setAmbient(glm::vec3(0.2f));

	// Create point light moving with camera
	LightPtr pointLight = Light::create(Light::POINT);
	pointLight->setPosition(camera->getPosition());
	pointLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	pointLight->setLinearAttenuation(0.01f);
	pointLight->setCallback(LightData::moveLight);
	pointLight->setUserData(std::make_shared<LightData>(camera));
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

