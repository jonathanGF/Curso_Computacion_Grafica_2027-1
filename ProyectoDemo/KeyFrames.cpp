// Previo 12   Gonzalez Fernandez Jonathan Uriel
// 02/11/2026    420051871
// FI UNAM Grupo 14

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

// Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Prototipos de Keyframes
void saveFrame(void);
void resetElements(void);
void interpolation(void);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.5f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Base
float rotBall = 0.0f;

// Variables manipulables del modelo
float dogPosX = 0.0f, dogPosY = 0.0f, dogPosZ = 0.0f;
float rotDog = 0.0f;       // Giro en eje Y
float rotDogPitch = 0.0f;  // Inclinación del tronco en eje X

// Articulaciones independientes
float head = 0.0f;
float tail = 0.0f;
float FLegLeft = 0.0f;
float FLegRight = 0.0f;
float RLegLeft = 0.0f;
float RLegRight = 0.0f;

// =========================================================================
// KeyFrames
// =========================================================================
#define MAX_FRAMES 15
int i_max_steps = 90;
int i_curr_steps = 0;

typedef struct _frame {
    // Transformaciones del cuerpo
    float dogPosX;
    float dogPosY;
    float dogPosZ;
    float rotDog;
    float rotDogPitch;

    float incX;
    float incY;
    float incZ;
    float rotDogInc;
    float rotDogPitchInc;

    // Cabeza y Cola
    float head;
    float headInc;
    float tail;
    float tailInc;

    // Cuatro patas independientes
    float FLegLeft;
    float FLegLeftInc;
    float FLegRight;
    float FLegRightInc;
    float RLegLeft;
    float RLegLeftInc;
    float RLegRight;
    float RLegRightInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
    if (FrameIndex < MAX_FRAMES)
    {
        printf("Frame guardado en Index: %d\n", FrameIndex);

        KeyFrame[FrameIndex].dogPosX = dogPosX;
        KeyFrame[FrameIndex].dogPosY = dogPosY;
        KeyFrame[FrameIndex].dogPosZ = dogPosZ;
        KeyFrame[FrameIndex].rotDog = rotDog;
        KeyFrame[FrameIndex].rotDogPitch = rotDogPitch;

        KeyFrame[FrameIndex].head = head;
        KeyFrame[FrameIndex].tail = tail;

        KeyFrame[FrameIndex].FLegLeft = FLegLeft;
        KeyFrame[FrameIndex].FLegRight = FLegRight;
        KeyFrame[FrameIndex].RLegLeft = RLegLeft;
        KeyFrame[FrameIndex].RLegRight = RLegRight;

        FrameIndex++;
    }
    else
    {
        printf("Limite maximo de frames alcanzado (%d)\n", MAX_FRAMES);
    }
}

void resetElements(void)
{
    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    rotDog = KeyFrame[0].rotDog;
    rotDogPitch = KeyFrame[0].rotDogPitch;

    head = KeyFrame[0].head;
    tail = KeyFrame[0].tail;

    FLegLeft = KeyFrame[0].FLegLeft;
    FLegRight = KeyFrame[0].FLegRight;
    RLegLeft = KeyFrame[0].RLegLeft;
    RLegRight = KeyFrame[0].RLegRight;
}

void interpolation(void)
{
    KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
    KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
    KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
    KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
    KeyFrame[playIndex].rotDogPitchInc = (KeyFrame[playIndex + 1].rotDogPitch - KeyFrame[playIndex].rotDogPitch) / i_max_steps;

    KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
    KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;

    KeyFrame[playIndex].FLegLeftInc = (KeyFrame[playIndex + 1].FLegLeft - KeyFrame[playIndex].FLegLeft) / i_max_steps;
    KeyFrame[playIndex].FLegRightInc = (KeyFrame[playIndex + 1].FLegRight - KeyFrame[playIndex].FLegRight) / i_max_steps;
    KeyFrame[playIndex].RLegLeftInc = (KeyFrame[playIndex + 1].RLegLeft - KeyFrame[playIndex].RLegLeft) / i_max_steps;
    KeyFrame[playIndex].RLegRightInc = (KeyFrame[playIndex + 1].RLegRight - KeyFrame[playIndex].RLegRight) / i_max_steps;
}

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Previo 12 - Keyframes Animacion Completa", nullptr, nullptr);

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

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Models
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // Inicializar estructura de Keyframes
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        KeyFrame[i].dogPosX = 0;
        KeyFrame[i].dogPosY = 0;
        KeyFrame[i].dogPosZ = 0;
        KeyFrame[i].incX = 0;
        KeyFrame[i].incY = 0;
        KeyFrame[i].incZ = 0;
        KeyFrame[i].rotDog = 0;
        KeyFrame[i].rotDogInc = 0;
        KeyFrame[i].rotDogPitch = 0;
        KeyFrame[i].rotDogPitchInc = 0;

        KeyFrame[i].head = 0;
        KeyFrame[i].headInc = 0;
        KeyFrame[i].tail = 0;
        KeyFrame[i].tailInc = 0;

        KeyFrame[i].FLegLeft = 0;
        KeyFrame[i].FLegLeftInc = 0;
        KeyFrame[i].FLegRight = 0;
        KeyFrame[i].FLegRightInc = 0;
        KeyFrame[i].RLegLeft = 0;
        KeyFrame[i].RLegLeftInc = 0;
        KeyFrame[i].RLegRight = 0;
        KeyFrame[i].RLegRightInc = 0;
    }

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        Animation();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        glm::mat4 modelTemp = glm::mat4(1.0f);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        glm::mat4 view = camera.GetViewMatrix();

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);

        // Piso
        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // Jerarquía del Perro
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        // Cuerpo (Nodo padre: traslación, rotación en Y y cabeceo/pitch en X)
        model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
        model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotDogPitch), glm::vec3(1.0f, 0.0f, 0.0f));
        modelTemp = model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // Cabeza
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Cola
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Pata delantera izquierda
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegLeft), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Pata delantera derecha (Saludadora)
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegRight), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Pata trasera izquierda (eje 1.0f)
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(RLegLeft), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Pata trasera derecha (eje unificado 1.0f para girar en el mismo sentido)
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(RLegRight), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // Pelota
        model = glm::mat4(1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        // Lámpara
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Manipulación manual de articulaciones y cámara
void DoMovement()
{
    // Cámara
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    // Traslación y rotación lateral del cuerpo
    if (keys[GLFW_KEY_H]) dogPosZ += 0.01f;
    if (keys[GLFW_KEY_Y]) dogPosZ -= 0.01f;
    if (keys[GLFW_KEY_G]) dogPosX -= 0.01f;
    if (keys[GLFW_KEY_J]) dogPosX += 0.01f;
    if (keys[GLFW_KEY_2]) rotDog += 1.0f;
    if (keys[GLFW_KEY_3]) rotDog -= 1.0f;

    // Altura en Y del cuerpo (para bajar al perro al sentarse)
    if (keys[GLFW_KEY_R]) dogPosY += 0.005f;
    if (keys[GLFW_KEY_F]) dogPosY -= 0.005f;

    // Inclinación del tronco (Pitch) para postura natural de sentado
    if (keys[GLFW_KEY_V]) rotDogPitch += 1.0f; // Inclina hacia atrás / levanta pecho
    if (keys[GLFW_KEY_B]) rotDogPitch -= 1.0f; // Inclina hacia adelante

    // Cabeza
    if (keys[GLFW_KEY_O]) head += 1.0f;
    if (keys[GLFW_KEY_P]) head -= 1.0f;

    // Cola
    if (keys[GLFW_KEY_6]) tail += 1.0f;
    if (keys[GLFW_KEY_7]) tail -= 1.0f;

    // Pata delantera derecha (Saludar)
    if (keys[GLFW_KEY_8]) FLegRight += 1.0f;
    if (keys[GLFW_KEY_9]) FLegRight -= 1.0f;

    // Pata delantera izquierda
    if (keys[GLFW_KEY_U]) FLegLeft += 1.0f;
    if (keys[GLFW_KEY_I]) FLegLeft -= 1.0f;

    // Patas traseras (Sentarse)
    if (keys[GLFW_KEY_Z]) { RLegLeft += 1.0f; RLegRight += 1.0f; }
    if (keys[GLFW_KEY_X]) { RLegLeft -= 1.0f; RLegRight -= 1.0f; }

    // Luces
    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
}

// Callback de eventos de teclado únicos
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }

    // Tecla K: Guardar fotograma clave
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        saveFrame();
    }

    // Tecla L: Reproducir / Pausar animación por Keyframes
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (!play && (FrameIndex > 1))
        {
            resetElements();
            interpolation();
            play = true;
            playIndex = 0;
            i_curr_steps = 0;
            printf("Reproduciendo animacion de Keyframes...\n");
        }
        else
        {
            play = false;
        }
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        active = !active;
        if (active)
            Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
        else
            Light1 = glm::vec3(0.0f);
    }
}

// Reproducción e interpolación lineal de todas las partes
void Animation()
{
    if (play)
    {
        if (i_curr_steps >= i_max_steps)
        {
            playIndex++;
            if (playIndex > FrameIndex - 2)
            {
                printf("Termino la animacion completa.\n");
                playIndex = 0;
                play = false;
            }
            else
            {
                i_curr_steps = 0;
                interpolation();
            }
        }
        else
        {
            dogPosX += KeyFrame[playIndex].incX;
            dogPosY += KeyFrame[playIndex].incY;
            dogPosZ += KeyFrame[playIndex].incZ;
            rotDog += KeyFrame[playIndex].rotDogInc;
            rotDogPitch += KeyFrame[playIndex].rotDogPitchInc;

            head += KeyFrame[playIndex].headInc;
            tail += KeyFrame[playIndex].tailInc;

            FLegLeft += KeyFrame[playIndex].FLegLeftInc;
            FLegRight += KeyFrame[playIndex].FLegRightInc;
            RLegLeft += KeyFrame[playIndex].RLegLeftInc;
            RLegRight += KeyFrame[playIndex].RLegRightInc;

            i_curr_steps++;
        }
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