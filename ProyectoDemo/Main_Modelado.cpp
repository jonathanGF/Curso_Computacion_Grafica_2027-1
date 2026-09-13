#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Practica 4	Gonzalez Fernandez Jonathan Uriel 
// Fecha de entrega: 12/09/2026		420051871
// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

// ==========================================
// Paleta de colores (Pollito Voxel)
// ==========================================
const glm::vec3 COLOR_CUERPO = glm::vec3(1.0f, 0.65f, 0.30f); // Naranja base
const glm::vec3 COLOR_CRESTA = glm::vec3(0.9f, 0.15f, 0.10f); // Rojo
const glm::vec3 COLOR_PICO = glm::vec3(1.0f, 0.75f, 0.00f); // Amarillo
const glm::vec3 COLOR_OJOS = glm::vec3(0.05f, 0.05f, 0.05f); // Negro
const glm::vec3 COLOR_MEJILLAS = glm::vec3(1.0f, 0.35f, 0.10f); // Naranja rojizo
const glm::vec3 COLOR_PECHO = glm::vec3(1.0f, 1.0f, 1.0f);   // Blanco
const glm::vec3 COLOR_PATAS = glm::vec3(1.0f, 0.80f, 0.00f); // Amarillo patas

// ==========================================
// Función auxiliar para dibujar cubos
// ==========================================
void DrawCube(glm::vec3 pos, glm::vec3 scale, glm::vec3 color, GLint modelLoc, GLint colorLoc) {
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -6.0f;
float rot = 0.0f;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "P4_Jonathan_Gonzalez", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Front
		 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Back
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Right
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, // Left
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // Bottom
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f, // Top
		 0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.2f, 0.5f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f, 0.5f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window)) {
		Inputs(window);
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint colorLoc = glGetUniformLocation(ourShader.Program, "objectColor");

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);

		// 1. Cuerpo principal
		DrawCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.8f, 2.0f, 1.8f), COLOR_CUERPO, modelLoc, colorLoc);

		// ==========================================
		// 2. Cresta roja superior (3 bloques)
		// ==========================================
		// Bloque trasero (alto)
		DrawCube(glm::vec3(0.0f, 1.1f, -0.1f), glm::vec3(0.3f, 0.35f, 0.6f), COLOR_CRESTA, modelLoc, colorLoc);

		// Bloque delantero (bajo)
		DrawCube(glm::vec3(0.0f, 1.0f, 0.3f), glm::vec3(0.3f, 0.2f, 0.3f), COLOR_CRESTA, modelLoc, colorLoc);
		DrawCube(glm::vec3(0.0f, 1.0f, -0.5f), glm::vec3(0.3f, 0.2f, 0.3f), COLOR_CRESTA, modelLoc, colorLoc);

		// ==========================================
		// 3. PICO (Amarillo central sobresaliente)
		// ==========================================
		DrawCube(glm::vec3(0.0f, 0.0f, 0.98f), glm::vec3(0.38f, 0.28f, 0.25f), COLOR_PICO, modelLoc, colorLoc);

		// ==========================================
		// 4. Cara
		// ==========================================
		// Ojo Izquierdo
		DrawCube(glm::vec3(-0.45f, 0.18f, 0.91f), glm::vec3(0.30f, 0.10f, 0.05f), COLOR_OJOS, modelLoc, colorLoc);
		// Ojo Derecho
		DrawCube(glm::vec3(0.45f, 0.18f, 0.91f), glm::vec3(0.30f, 0.10f, 0.05f), COLOR_OJOS, modelLoc, colorLoc);
		// Ceja Derecha
		DrawCube(glm::vec3(0.45f, 0.5f, 0.91f), glm::vec3(0.12f, 0.05f, 0.05f), COLOR_OJOS, modelLoc, colorLoc);
		//Ceja Izquierda
		DrawCube(glm::vec3(-0.45f, 0.5f, 0.91f), glm::vec3(0.12f, 0.05f, 0.05f), COLOR_OJOS, modelLoc, colorLoc);
		//mejillas izquierda 
		DrawCube(glm::vec3(-0.79f, -0.03f, 0.783f), glm::vec3(0.24f, 0.24f, 0.24f), COLOR_MEJILLAS, modelLoc, colorLoc);
		//Mejilla Derecha
		DrawCube(glm::vec3(0.79f, -0.03f, 0.783f), glm::vec3(0.24f, 0.24f, 0.24f), COLOR_MEJILLAS, modelLoc, colorLoc);
		//Pecho Blanco 
		DrawCube(glm::vec3(0.0f, -0.8f, 0.783f), glm::vec3(0.24f, 0.96f, 0.24f), COLOR_PECHO, modelLoc, colorLoc);
		DrawCube(glm::vec3(0.0f, -0.86f, 0.783f), glm::vec3(0.72f, 0.66f, 0.24f), COLOR_PECHO, modelLoc, colorLoc);
		//Patas
		DrawCube(glm::vec3(0.35f, -0.9f, 0.98f), glm::vec3(0.48f, 0.18f, 0.25f), COLOR_PATAS, modelLoc, colorLoc);
		DrawCube(glm::vec3(-0.35f, -0.9f, 0.98f), glm::vec3(0.48f, 0.18f, 0.25f), COLOR_PATAS, modelLoc, colorLoc);
		//Alas
		DrawCube(glm::vec3(0.95f, -0.05f, 0.0f), glm::vec3(0.16f, 0.75f, 0.55f), COLOR_PATAS, modelLoc, colorLoc);
		DrawCube(glm::vec3(-0.95f, -0.05f, 0.0f), glm::vec3(0.16f, 0.75f, 0.55f), COLOR_PATAS, modelLoc, colorLoc);
		DrawCube(glm::vec3(-0.95f, -0.55f, -0.13f), glm::vec3(0.16f, 0.35f, 0.275f), COLOR_PATAS, modelLoc, colorLoc);
		DrawCube(glm::vec3(0.95f, -0.55f, -0.13f), glm::vec3(0.16f, 0.35f, 0.275f), COLOR_PATAS, modelLoc, colorLoc);
	


		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.4f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.4f;
}