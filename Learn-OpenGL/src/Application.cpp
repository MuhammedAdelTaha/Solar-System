#include <iostream>
#include <chrono>
#include <ctime>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Debugger.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexBufferLayout.h"

#include "ColorMaterial.h"
#include "TextureMaterial.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shapes.h"

const char* ColorShaderPath = "res/shaders/Color.shader";
const char* TextureShaderPath = "res/shaders/Texture.shader";

const char* SpaceTexturePath = "res/textures/space.jpg";
const char* SunTexturePath = "res/textures/sun.jpg";
const char* MercuryTexturePath = "res/textures/mercury.jpg";
const char* VenusTexturePath = "res/textures/venus.jpg";
const char* EarthTexturePath = "res/textures/earth.jpg";
const char* MarsTexturePath = "res/textures/mars.jpg";
const char* JupiterTexturePath = "res/textures/jupiter.jpg";
const char* SaturnTexturePath = "res/textures/saturn.jpg";
const char* UranusTexturePath = "res/textures/uranus.jpg";
const char* NeptuneTexturePath = "res/textures/neptune.jpg";

constexpr int WIDTH = 1920, HEIGHT = 960;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
static void doMovement();
static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3000.0f));
float lastX = WIDTH / 2.0;
float lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Space War", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	cout << "Screen width: " << SCREEN_WIDTH << " Screen height: " << SCREEN_HEIGHT << endl;

	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Used to sync the rendering loop with the screen frame rate
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glCall(glEnable(GL_DEPTH_TEST));

	glCall(glEnable(GL_BLEND));
	glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	
	{
		Shape cube = getCube();
		vector<float> cubeVertices = cube.vertices;
		vector<unsigned int> cubeElements = cube.elements;
		unsigned int n = cubeVertices.size();
		unsigned int m = cubeElements.size();
		
		float* cubeVerticesArr = &cubeVertices[0];
		unsigned int* cubeElementsArr = &cubeElements[0];

		VertexBuffer cubeVBO(n * sizeof(float), cubeVerticesArr);

		VertexAttribute position(3, GL_FLOAT, GL_FALSE);
		VertexAttribute textureCoord(2, GL_FLOAT, GL_FALSE);
		VertexAttribute normal(3, GL_FLOAT, GL_FALSE);
		VertexBufferLayout cubeLayout;
		cubeLayout.addAttributes({ position, textureCoord, normal });
		
		VertexArray cubeVAO;
		cubeVAO.addBuffer(cubeVBO, cubeLayout); 

		ElementBuffer cubeEBO(m, cubeElementsArr);

		cubeVAO.unbind();
		cubeVBO.unbind();
		cubeEBO.unbind();

		Shape sphere = getSphere();
		vector<float> sphereVertices = sphere.vertices;
		vector<unsigned int> sphereElements = sphere.elements;
		n = sphereVertices.size();
		m = sphereElements.size();

		float* sphereVerticesArr = &sphereVertices[0];
		unsigned int* sphereElementsArr = &sphereElements[0];

		VertexBuffer sphereVBO(n * sizeof(float), sphereVerticesArr);

		VertexBufferLayout sphereLayout;
		sphereLayout.addAttributes({ position, textureCoord, normal });

		VertexArray sphereVAO;
		sphereVAO.addBuffer(sphereVBO, sphereLayout);

		ElementBuffer sphereEBO(m, sphereElementsArr);

		sphereVAO.unbind();
		sphereVBO.unbind();
		sphereEBO.unbind();

		// Renderer
		// --------
		Renderer renderer;

		// Shaders
		// -------
		Shader colorShader(ColorShaderPath);
		Shader textureShader(TextureShaderPath);

		// Textures
		// --------
		Texture spaceTexture(SpaceTexturePath);
		Texture sunTexture(SunTexturePath);
		Texture mercuryTexture(MercuryTexturePath);
		Texture venusTexture(VenusTexturePath);
		Texture earthTexture(EarthTexturePath);
		Texture marsTexture(MarsTexturePath);
		Texture jupiterTexture(JupiterTexturePath);
		Texture saturnTexture(SaturnTexturePath);
		Texture uranusTexture(UranusTexturePath);
		Texture neptuneTexture(NeptuneTexturePath);

		// Sun Light
		// ---------
		ColorMaterial* sunLight	= new ColorMaterial
		(
			/*Shader*/					colorShader,
			/*Translate Vector*/		glm::vec3(0.0f),
			/*Scale Vector*/			glm::vec3(1.0f),
			/*Rotation Direction*/		glm::vec3(0.5f, 1.0f, 0.5f),
			/*Rotation Speed*/			0.1f,
			/*Color*/					glm::vec4(1.0f, 0.8f, 0.2f, 1.0f)
		);

		// Space box
		// ---------
		TextureMaterial* space = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f),
			/*Scale Vector*/			glm::vec3(40000.0f),
			/*Rotation Direction*/		glm::vec3(0.5f, 1.0f, 0.5f),
			/*Rotation Speed*/			0.01f,
			/*Texture*/					spaceTexture,
			/*Texture Slot*/			0,
			/*Texture Opacity*/			0.5f
		);

		// sun
		// ---
		TextureMaterial* sun = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f),
			/*Scale Vector*/			glm::vec3(200.0f),
			/*Rotation Direction*/		glm::vec3(0.0f, 1.0f, 0.0f),
			/*Rotation Speed*/			0.1f,
			/*Texture*/					sunTexture,
			/*Texture Slot*/			1,
			/*Texture Opacity*/			1.0f
		);
		
		// Mercury
		// -------
		TextureMaterial* mercury = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f, 0.0f, 400.0f),
			/*Scale Vector*/			glm::vec3(30.0f),
			/*Rotation Direction*/		glm::vec3(0.0f, 1.0f, 0.0f),
			/*Rotation Speed*/			0.2f,
			/*Texture*/					mercuryTexture,
			/*Texture Slot*/			2,
			/*Texture Opacity*/			1.0f
		);

		// Venus
		// -----
		TextureMaterial* venus = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f, 0.0f, 600.0f),
			/*Scale Vector*/			glm::vec3(50.0f),
			/*Rotation Direction*/		glm::vec3(0.0f, 1.0f, 0.0f),
			/*Rotation Speed*/			0.28f,
			/*Texture*/					venusTexture,
			/*Texture Slot*/			3,
			/*Texture Opacity*/			1.0f
		);

		// Earth
		// -----
		TextureMaterial* earth = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f, 0.0f, 800.0f),
			/*Scale Vector*/			glm::vec3(60.0f),
			/*Rotation Direction*/		glm::vec3(0.0f, 1.0f, 0.0f),
			/*Rotation Speed*/			0.35f,
			/*Texture*/					earthTexture,
			/*Texture Slot*/			4,
			/*Texture Opacity*/			1.0f
		);

		// Mars
		// ----
		TextureMaterial* mars = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f, 0.0f, 1000.0f),
			/*Scale Vector*/			glm::vec3(40.0f),
			/*Rotation Direction*/		glm::vec3(0.0f, 1.0f, 0.0f),
			/*Rotation Speed*/			0.42f,
			/*Texture*/					marsTexture,
			/*Texture Slot*/			5,
			/*Texture Opacity*/			1.0f
		);

		// Jupiter
		// -------
		TextureMaterial* jupiter = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f, 0.0f, -1500.0f),
			/*Scale Vector*/			glm::vec3(100.0f),
			/*Rotation Direction*/		glm::vec3(0.0f, 1.0f, 0.0f),
			/*Rotation Speed*/			0.5f,
			/*Texture*/					jupiterTexture,
			/*Texture Slot*/			6,
			/*Texture Opacity*/			1.0f
		);

		// Saturn
		// ------
		TextureMaterial* saturn = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f, 0.0f, -1800.0f),
			/*Scale Vector*/			glm::vec3(90.0f),
			/*Rotation Direction*/		glm::vec3(0.0f, 1.0f, 0.0f),
			/*Rotation Speed*/			0.65f,
			/*Texture*/					saturnTexture,
			/*Texture Slot*/			7,
			/*Texture Opacity*/			1.0f
		);

		// Uranus
		// ------
		TextureMaterial* uranus = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f, 0.0f, -2100.0f),
			/*Scale Vector*/			glm::vec3(70.0f),
			/*Rotation Direction*/		glm::vec3(0.0f, 1.0f, 0.0f),
			/*Rotation Speed*/			0.7f,
			/*Texture*/					uranusTexture,
			/*Texture Slot*/			8,
			/*Texture Opacity*/			1.0f
		);

		// Neptune
		// -------
		TextureMaterial* neptune = new TextureMaterial
		(
			/*Shader*/					textureShader,
			/*Translate Vector*/		glm::vec3(0.0f, 0.0f, -2300.0f),
			/*Scale Vector*/			glm::vec3(60.0f),
			/*Rotation Direction*/		glm::vec3(0.0f, 1.0f, 0.0f),
			/*Rotation Speed*/			1.0f,
			/*Texture*/					neptuneTexture,
			/*Texture Slot*/			9,
			/*Texture Opacity*/			1.0f
		);

		Material* materials[] = { sunLight, sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune };

		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			doMovement();

			renderer.clear();
			
			glm::mat4 projectionMat = glm::perspective(camera.getZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100000.0f);
			glm::mat4 viewMat = camera.GetViewMatrix();

			{
				space->setColorTexture();
				space->updateModelMat(deltaTime);
				space->updateViewMat(viewMat);
				space->updateProjectionMat(projectionMat);
				renderer.draw(cubeVAO, cubeEBO, space->getShaderId());
			}

			for (Material* material : materials)
			{
				material->setColorTexture();
				material->updateModelMat(deltaTime);
				material->updateViewMat(viewMat);
				material->updateProjectionMat(projectionMat);

				renderer.draw(sphereVAO, sphereEBO, material->getShaderId());
			}

			glfwSwapBuffers(window);
		}
	}

	glfwTerminate();
	return 0;
}

void doMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}