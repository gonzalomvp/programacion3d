#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup /NODEFAULTLIB:MSVCRT")
#endif

#include "ugine3d.h"
#include "../lib/glfw/glfw3.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct CubeData;
typedef std::shared_ptr<CubeData> CubeDataPtr;

struct CubeData {
	float angle;
	float rotationSpeed;

	CubeData(float initialAngle, float rotationSpeed) : angle(initialAngle), rotationSpeed(rotationSpeed) {}

	static void update(Entity& entity, float deltaTime){
		CubeDataPtr data = std::static_pointer_cast<CubeData>(entity.getUserData());
		data->angle += data->rotationSpeed * deltaTime;
		if (data->angle >= 360) {
			data->angle -= 360;
		}
		entity.setEuler(glm::vec3(0.0f, data->angle, 0.0f));
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

	if ( !init() ) {
		std::cout << "could not initialize opengl extensions" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Create World
	WorldPtr world = World::create();

	// Create Cube Mesh
	MeshPtr cubeMesh = Mesh::create();

	// define front faces vertices
	std::vector<Vertex> vertices = {
		Vertex( glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(1), glm::vec2(0, 0)),
		Vertex( glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 1)),

		Vertex( glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3(1), glm::vec2(0, 0)),
		Vertex( glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec3(1), glm::vec2(1, 1)),

		Vertex( glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 0)),
		Vertex( glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(1), glm::vec2(1, 1)),

		Vertex( glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 0)),
		Vertex( glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 1))
	};

	// define front faces vertices
	std::vector<uint16_t> indexes;
	for (uint16_t i = 0; i < 24; ++i) {
		indexes.push_back(i);
	}

	// Add front faces buffer to the cube mesh
	cubeMesh->addBuffer(Buffer::create(vertices, indexes), Material::create(Texture::load("data/front.png")));

	// define top and bottom faces vertices
	vertices.clear();
	vertices = {
		Vertex( glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(1), glm::vec2(0, 0)),
		Vertex( glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec3(1), glm::vec2(1, 1)),

		Vertex( glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(1), glm::vec2(0, 0)),
		Vertex( glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(1), glm::vec2(0, 1)),
		Vertex( glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3(1), glm::vec2(1, 0)),
		Vertex( glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3(1), glm::vec2(1, 1)),
	};

	// define top and bottom faces vertices
	indexes.clear();
	for (uint16_t i = 0; i < 12; ++i) {
		indexes.push_back(i);
	}

	// Add top and bottom faces buffer to the cube mesh
	cubeMesh->addBuffer(Buffer::create(vertices, indexes), Material::create(Texture::load("data/top.png")));

	MeshPtr townMesh = Mesh::load("data/asian_town.msh.xml");

	ModelPtr model = Model::create(townMesh);
	model->setPosition(glm::vec3());
	model->setScale(glm::vec3(100.0f));
	//model->setCallback(CubeData::update);
	//model->setUserData(std::make_shared<CubeData>(0.0f, 32.0f));
	world->addEntity(model);

	CameraPtr camera = Camera::create();
	camera->setPosition(glm::vec3(0.0f, 1.0f, 3.0f));
	camera->setEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->setClearColor(glm::vec3(0.0f, 0.0f, 0.5f));
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

