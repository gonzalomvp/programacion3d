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

	// Load Mesh
	MeshPtr columnMesh = Mesh::load("data/column.msh.xml");

	// Create Model
	ModelPtr model = Model::create(columnMesh);
	model->setScale(glm::vec3(0.01f));
	world->addEntity(model);

	// Set ambient color
	world->setAmbient(glm::vec3(0.2f));

	// Create lights
	LightPtr pointLight = Light::create(Light::POINT);
	pointLight->setPosition(model->getPosition() + glm::vec3(0.0f, 7.0f, 0.0f));
	pointLight->setColor(glm::vec3(1.0f));
	pointLight->setLinearAttenuation(0.2f);
	world->addEntity(pointLight);

	// Create Emitters
	MaterialPtr smokeMat = Material::create(Texture::load("data/smoke.png"), nullptr);
	smokeMat->setBlendMode(Material::ALPHA);
	smokeMat->setLighting(false);
	smokeMat->setCulling(false);
	smokeMat->setDepthWrite(false);

	EmitterPtr smokeEmitter = Emitter::create(smokeMat);
	smokeEmitter->setPosition(pointLight->getPosition());
	smokeEmitter->setRateRange(5.0f, 10.0f);
	smokeEmitter->setLifetimeRange(1.0f, 5.0f);
	smokeEmitter->setVelocityRange(glm::vec3(-0.1f, 1.0f, -0.1f), glm::vec3(0.1f, 4.0f, 0.1f));
	smokeEmitter->setSpinVelocityRange(30.0f, 60.0f);
	smokeEmitter->setScaleRange(0.05f, 0.1f);
	smokeEmitter->emit(true);
	world->addEntity(smokeEmitter);

	MaterialPtr flameMat = Material::create(Texture::load("data/flame.png"), nullptr);
	flameMat->setBlendMode(Material::ADD);
	flameMat->setLighting(false);
	flameMat->setCulling(false);
	flameMat->setDepthWrite(false);

	EmitterPtr flameEmitter = Emitter::create(flameMat);
	flameEmitter->setPosition(pointLight->getPosition());
	flameEmitter->setRateRange(10.0f, 25.0f);
	flameEmitter->setLifetimeRange(0.5f, 0.5f);
	flameEmitter->setVelocityRange(glm::vec3(-1.0f, 5.0f, -1.0f), glm::vec3(1.0f, 10.0f, 1.0f));
	flameEmitter->setSpinVelocityRange(0.0f, 0.0f);
	flameEmitter->setScaleRange(0.025f, 0.1f);
	flameEmitter->emit(true);
	world->addEntity(flameEmitter);

	// Create Camera
	CameraPtr camera = Camera::create();
	camera->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->setCallback(CameraData::rotateCamera);
	camera->setUserData(std::make_shared<CameraData>(model->getPosition() + glm::vec3(0.0f, 20.0f, 0.0f), pointLight->getPosition(), 20.f, 25.0f));
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

