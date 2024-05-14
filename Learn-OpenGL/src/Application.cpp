#include <iostream>
#include <chrono>
#include <ctime>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Renderer.h"
#include "Debugger.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

constexpr int WIDTH = 1920, HEIGHT = 960;
//constexpr int WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;
constexpr float PI = 3.14159265359;

// Function prototypes
static void doMovement();
static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = WIDTH / 2.0;
float lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Objects
struct Object
{
	Texture* m_Texture;
	int m_TextureSlot;
	bool m_UseTexture;
	float m_TexOpacity;
	glm::vec4 m_Color;
	glm::mat4 m_ScaleMat;
	glm::mat4 m_RotateMat;
	glm::mat4 m_TranslateMat;
	glm::vec3 m_RotationDirection;
	float m_RotationSpeed;
	
	Object(Texture* texture, int textureSlot, bool useTexture, float texOpacity, glm::vec4 color, glm::mat4 scaleMat, glm::mat4 rotateMat,
		glm::mat4 translateMat, glm::vec3 rotationDirection, float rotationSpeed) : m_Texture(texture), m_TextureSlot(textureSlot),
		m_UseTexture(useTexture), m_TexOpacity(texOpacity), m_Color(color), m_ScaleMat(scaleMat), m_RotateMat(rotateMat), 
		m_TranslateMat(translateMat), m_RotationDirection(rotationDirection), m_RotationSpeed(rotationSpeed) {};
};

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
		unsigned int n = 24;
		unsigned int dataPerVertex = 5;
		float vertices[] = {
			-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,

			-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,

			-1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,

			 1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
								
			-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,

			-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, 0.0f, 0.0f
		};

		unsigned int m = 36;
		unsigned int elements[] = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20
		};
		
		// Vertex Buffer
		// -------------
		VertexBuffer vbo(n * dataPerVertex * sizeof(float), vertices);

		// Vertex Attributes and Layout
		// ----------------------------
		VertexAttribute position(3, GL_FLOAT, GL_FALSE);
		VertexAttribute textureCoord(2, GL_FLOAT, GL_FALSE);
		VertexBufferLayout layout;
		layout.addAttributes({ position, textureCoord });
		
		// Vertex Array
		// ------------
		VertexArray vao;
		vao.addBuffer(vbo, layout); 

		// Element/Index Buffer
		// --------------------
		ElementBuffer ebo(m, elements);

		// Unbind
		// ------
		vao.unbind();
		vbo.unbind();
		ebo.unbind();
		
		// Basic shader
		// ------------
		Shader shader("res/shaders/Basic.shader");

		// Renderer
		// --------
		Renderer renderer;

		// Space box
		// ---------
		Object space
		(
			new Texture("res/textures/space.jpg"),
			0,
			true,
			0.5f,
			glm::vec4(1.0f),
			glm::scale(glm::mat4(1.0f), glm::vec3(400.0f)),
			glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f)),
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)),
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.1f
		);

		// Earth
		// -----
		Object earth
		(
			new Texture("res/textures/earth.jpg"),
			1,
			true,
			1.0f,
			glm::vec4(1.0f),
			glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)),
			glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f)),
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)),
			glm::vec3(1.0f, 0.5f, 0.0f),
			1.0f
		);

		Object objects[] = { space, earth };

		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			doMovement();

			renderer.clear();
			
			glm::mat4 projectionMat = glm::perspective(camera.getZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
			glm::mat4 viewMat = camera.GetViewMatrix();

			for (Object& object : objects)
			{
				object.m_Texture->bind(object.m_TextureSlot);
				Material material(shader, object.m_ScaleMat, object.m_RotateMat, object.m_TranslateMat, object.m_RotationSpeed, object.m_RotationDirection, viewMat, projectionMat);
				if (object.m_UseTexture)
					material.setMaterialTexture(object.m_TextureSlot, object.m_TexOpacity);
				else
					material.setMaterialColor(object.m_Color);
				renderer.draw(vao, ebo, shader);
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