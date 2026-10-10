// Practica 8   Gonzalez Fernandez Jonathan Uriel
// 10/10/2026    420051871

// Std. Includes
#include <string>
#include <iostream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 - Dia y Noche - Jonathan Gonzalez", nullptr, nullptr);
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Shaders
    Shader unlitShader("Shader/modelLoading.vs", "Shader/modelLoading.frag");     // Para Sol y Luna (emisivos)
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");           // Para objetos con iluminacion Phong

    // Carga de modelos
    Model dog((char*)"Models/RedDog.obj");
    Model rocket((char*)"Models/Nave2.obj");
    Model rover((char*)"Models/SpaceRover.obj");
    Model antena((char*)"Models/SatelliteDish.obj");
    Model luna((char*)"Models/PUSHILIN_moon.obj");
    Model sun((char*)"Models/sun.obj");

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // =========================================================================
        // 1. Calculo orbital del Sol y la Luna (Centro en el origen / Perro)
        // =========================================================================
        float orbitSpeed = 0.4f;
        float orbitAngle = currentFrame * orbitSpeed;
        float orbitRadius = 4.0f;

        // Posicion del Sol y de la Luna (opuestas a 180 grados en el plano vertical XY)
        glm::vec3 sunPos(orbitRadius * cos(orbitAngle), orbitRadius * sin(orbitAngle), 0.7f);
        glm::vec3 moonPos(-orbitRadius * cos(orbitAngle), -orbitRadius * sin(orbitAngle), 0.7f);

        // =========================================================================
        // 2. Transicion de color de fondo (Dia / Noche) segun la altura del sol
        // =========================================================================
        float dayFactor = glm::clamp((sunPos.y + 2.0f) / (orbitRadius + 2.0f), 0.0f, 1.0f);
        glm::vec3 daySky(0.35f, 0.55f, 0.85f);
        glm::vec3 nightSky(0.02f, 0.02f, 0.08f);
        glm::vec3 currentSky = glm::mix(nightSky, daySky, dayFactor);

        glClearColor(currentSky.r, currentSky.g, currentSky.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        
        // 3. Renderizado de la escena con Iluminacion 
        lightingShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // --- Parametros de la Luz 1: Sol (Calida / Amarilla) ---
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), sunPos.x, sunPos.y, sunPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.15f * dayFactor, 0.15f * dayFactor, 0.10f * dayFactor);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 0.95f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 0.95f, 0.8f);

        // --- Parametros de la Luz 2: Luna (Fria / Azulada) ---
        float nightFactor = 1.0f - dayFactor;
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), moonPos.x, moonPos.y, moonPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.03f * nightFactor, 0.03f * nightFactor, 0.08f * nightFactor);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.25f, 0.35f, 0.65f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.3f, 0.4f, 0.7f);

        // --- Propiedades del Material (Neutro para preservar texturas) ---
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.4f, 0.4f, 0.4f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // --- 1. Perro (retrasado a Z = -1.5f para recibir la luz frontalmente) ---
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // --- 2. Cohete (retrasado a Z = -3.5f) ---
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -3.5f));
        model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        rocket.Draw(lightingShader);

        // --- 3. Rover (retrasado a Z = -2.5f) ---
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.5f, -0.5f, -2.5f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        rover.Draw(lightingShader);

        // --- 4. Antena (retrasada a Z = -7.5f) ---
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -7.5f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        antena.Draw(lightingShader);

        // =========================================================================
        // 4. Renderizado de los emisores (Sol y Luna sin sombras propias)
        // =========================================================================
        unlitShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(unlitShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(unlitShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glUniform1i(glGetUniformLocation(unlitShader.Program, "useUniformColor"), 1);
        glUniform3f(glGetUniformLocation(unlitShader.Program, "customColor"), 1.0f, 0.85f, 0.15f);
        // Dibujar Sol
        model = glm::mat4(1.0f);
        model = glm::translate(model, sunPos);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Ajustar escala segun el .obj descargado
        glUniformMatrix4fv(glGetUniformLocation(unlitShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        sun.Draw(unlitShader);

        glUniform1i(glGetUniformLocation(unlitShader.Program, "useUniformColor"), 0);
        // Dibujar Luna
        model = glm::mat4(1.0f);
        model = glm::translate(model, moonPos);
        model = glm::rotate(model, glm::radians((GLfloat)currentFrame * 15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(unlitShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        luna.Draw(unlitShader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
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