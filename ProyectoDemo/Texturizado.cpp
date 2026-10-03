// Práctica 7: Texturizado de Cubo (Dado)
// Gonzalez Fernandez Jonathan Uriel - 420051871
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica7_Dado_Jonathan_Gonzalez", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// =========================================================================
	// 36 VÉRTICES DEL CUBO (Cada cara mapea una región de cuboMapa.png)
	// =========================================================================
	GLfloat vertices[] =
	{
		// Positions            // Colors              // Texture Coords (UV)

		// 1. CARA FRONTAL (+Z) -> DADO 4 (U: 0.25 a 0.50, V: 0.75 a 1.00)
		-0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.75f,
		 0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.75f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 1.00f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 1.00f,
		-0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 1.00f,
		-0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.75f,

		// 2. CARA TRASERA (-Z) -> DADO 3 (U: 0.25 a 0.50, V: 0.25 a 0.50)
		-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.25f,
		 0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.25f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.50f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.50f,
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.50f,
		-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.25f,

		// 3. CARA SUPERIOR (+Y) -> DADO 1 (U: 0.25 a 0.50, V: 0.50 a 0.75)
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.75f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.75f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.50f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.50f,
		-0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.50f,
		-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.75f,

		// 4. CARA INFERIOR (-Y) -> DADO 6 (U: 0.25 a 0.50, V: 0.00 a 0.25)
		-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.00f,
		 0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.00f,
		 0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.25f,
		 0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.25f,
		-0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.25f,
		-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.00f,

		// 5. CARA DERECHA (+X) -> DADO 2 (U: 0.50 a 0.75, V: 0.00 a 0.25)
		 0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.00f,
		 0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.75f, 0.00f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.75f, 0.25f,
		 0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.75f, 0.25f,
		 0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.25f,
		 0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.50f, 0.00f,

		 // 6. CARA IZQUIERDA (-X) -> DADO 5 (U: 0.00 a 0.25, V: 0.00 a 0.25)
		 -0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.00f, 0.00f,
		 -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.00f,
		 -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.25f,
		 -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,      0.25f, 0.25f,
		 -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.00f, 0.25f,
		 -0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,      0.00f, 0.00f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// =========================================================================
	// CARGA DE LA TEXTURA DEL DADO (cuboMapa.png)
	// =========================================================================
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int textureWidth, textureHeight, nrChannels;
	unsigned char* image = stbi_load("images/cuboMapa.png", &textureWidth, &textureHeight, &nrChannels, 4);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lampShader.Use();

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

		
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.8f, glm::vec3(0.5f, 1.0f, 0.2f));

		GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Dibujar el cubo completo (36 vértices)
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}

void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
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