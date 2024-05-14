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

#include "SpaceMaterial.h"
#include "RockMaterial.h"
#include "EarthMaterial.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

constexpr int WIDTH = 1920, HEIGHT = 960;
int SCREEN_WIDTH, SCREEN_HEIGHT;
constexpr float PI = 3.14159265359;

// Function prototypes
static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
static void doMovement();

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
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

	window = glfwCreateWindow(WIDTH, HEIGHT, "SPACE WAR", NULL, NULL);
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
		//unsigned int n = 64;
		//float radius = 1.0f; // radius of the circle
		//float angleStep = 2 * PI / n; // angle step (in radians) between each point
		//float *vertices = new float[5 * (n + 1)]; // 3 for position, 2 for texture coordinates
		//vertices[0] = 0.0f; // center x
		//vertices[1] = 0.0f; // center y
		//vertices[2] = 0.0f; // center z
		//vertices[3] = 2.0f;
		//vertices[4] = 2.0f;
		//for (int i = 1; i <= n; i++)
		//{
		//	float angle = (i - 1) * angleStep;
		//	vertices[5 * i] = cos(angle) * radius + vertices[0]; // positions[0] = center[0]
		//	vertices[5 * i + 1] = sin(angle) * radius + vertices[1]; // positions[1] = center[1]
		//	vertices[5 * i + 2] = 0.0f; // positions[2] = center[2]
		//	// Texture coordinates
		//	vertices[5 * i + 3] = (cos(angle) + 1.0f) / 2.0f; // x = (cos(angle) + 1) / 2
		//	vertices[5 * i + 4] = (sin(angle) + 1.0f) / 2.0f; // y = (sin(angle) + 1) / 2
		//}
		//// number of elements = number of triangles * 3
		//unsigned int m = n * 3; // number of elements (triangles)
		//unsigned int *elements = new unsigned int[m]; // elements of the triangles
		//for (int i = 0; i < n; i++)
		//{
		//	elements[3 * i] = 0; // center
		//	elements[3 * i + 1] = i + 1; // current point
		//	elements[3 * i + 2] = (i + 1) % n + 1; // next point
		//}

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

		// Projection Matrix
		// -----------------
		glm::mat4 projectionMat = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

		// View Matrix
		// -----------
		glm::mat4 viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		// Textures
		// --------
		Texture spaceTex("res/textures/space.jpg");
		Texture earthTex("res/textures/earth.jpg");
		Texture rockTex("res/textures/rock.png");

		while (!glfwWindowShouldClose(window))
		{
			renderer.clear();
			
			// Update the view matrix by setting x, y
			// --------------------------------------
			{
				//viewMat[3][0] += x;
				//viewMat[3][2] += z;
				/*viewMat = glm::rotate(viewMat, 0.001f, glm::vec3(0.0f, 1.0f, 0.0f));*/
			}

			spaceTex.bind();
			SpaceMaterial spaceMaterial(shader, viewMat, projectionMat, 1.0f, 1.0f, 0, 1);
			renderer.draw(vao, ebo, shader);

			SpaceMaterial spaceMaterial2(shader, viewMat, projectionMat, 0.0f, 0.1f, 0, 0);
			renderer.draw(vao, ebo, shader);

			rockTex.bind(1);
			RockMaterial rockMaterial(shader, viewMat, projectionMat, 1);
			renderer.draw(vao, ebo, shader);

			earthTex.bind(2);
			EarthMaterial earthMaterial(shader, viewMat, projectionMat, 2);
			renderer.draw(vao, ebo, shader);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
{

}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
}

void doMovement()
{

}