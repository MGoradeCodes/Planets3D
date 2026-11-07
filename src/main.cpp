#include <iostream>
#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
#include "headers/Mesh.h"
#include "headers/ShaderClass.h"
#include "headers/Camera.h"
#include "headers/Texture.h"
#include "headers/stb.h"


std::vector<float> Draw(int res);

glm::vec3 SphericalToCartesian(float radius, float theta, float phi);

void setWindowIcon(GLFWwindow* window, const char* filePath);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void UpdatePlanets(float deltaTime);

glm::vec3 SphericalToCartesian(float radius, float theta, float phi)
{
    float x = radius * glm::sin(theta) * glm::cos(phi);
    float y = radius * glm::cos(theta);
    float z = radius * glm::sin(theta) * glm::sin(phi);

    return glm::vec3(x, y, z);
}

struct Planets{
    float radius;
    glm::vec3 position;
    glm::vec3 color;
    float speed;
    float currAngle;
    float orbitRad;
};

std::vector<Planets> planets;

glm::vec3 color;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Minecraft In OpenGL!", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    ShaderProgram shaderProgram("shaders/default.vert", "shaders/default.frag");
    Texture texture("textures/grass.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    
    glEnable(GL_DEPTH_TEST);

    Camera camera(800, 600, glm::vec3(0.0f, 0.0f, 4.0f), window);
    glfwSetWindowUserPointer(window, &camera);
    glfwSwapInterval(1);

    double prevTime = 0.0;
    double currTime = 0.0;
    double diff = 0.0;
    unsigned int counter = 0;

    planets = {
        {5.00f, { 0.00f, 0.0f, 0.0f}, {1.00f, 0.75f, 0.15f}, 0.00f, 0.2f, 0.0f},
        {0.25f, { 7.50f, 0.0f, 0.0f}, {0.65f, 0.65f, 0.65f}, 3.0f, 1.2f, 10.0f},
        {0.60f, { 9.50f, 0.0f, 0.0f}, {0.90f, 0.70f, 0.50f}, 2.2f, 0.8f, 15.0f},
        {0.30f, {12.00f, 0.0f, 0.0f}, {0.20f, 0.50f, 1.00f}, 1.5f, 1.4f, 21.0f},
        {0.34f, {14.50f, 0.0f, 0.0f}, {0.90f, 0.35f, 0.20f}, 1.0f, 2.3f, 28.0f},
        {2.20f, {19.00f, 0.0f, 0.0f}, {0.80f, 0.65f, 0.50f}, 0.6f, 3.4f, 37.0f},
        {1.80f, {25.00f, 0.0f, 0.0f}, {0.85f, 0.75f, 0.55f}, 0.4f, 5.0f, 48.0f},
        {1.00f, {31.00f, 0.0f, 0.0f}, {0.50f, 0.80f, 0.90f}, 0.2f, 1.5f, 60.0f},
        {0.95f, {36.00f, 0.0f, 0.0f}, {0.25f, 0.40f, 0.90f}, 0.1f, 3.5f, 72.0f}
    };
    std::vector<float> vertices = Draw(24);
    std::vector<unsigned int> indices;
    
    size_t totalVertices = vertices.size() / 6; 
    for (unsigned int i = 0; i < totalVertices; ++i) {
        indices.push_back(i);
    }

    Mesh sphere(vertices, indices); 
    glfwSwapInterval(1);
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        camera.Inputs(window);
        

        float cameraX = camera.getPositionX();
        float cameraZ = camera.getPositionZ();
        currTime = glfwGetTime();
        diff = currTime - prevTime;
        ++counter;

        if (diff >= 1.0 / 30.0) {
            const float fps = (1.0f / diff) * static_cast<float>(counter);
            std::string fpsText = std::to_string(fps);
            std::string newTitle = std::string("Simulating 3D Planets - FPS : ") + fpsText + 
                                " X: " + std::to_string(cameraX) + 
                                " Z: " + std::to_string(cameraZ);
            glfwSetWindowTitle(window, newTitle.c_str());
            prevTime = currTime;
            counter = 0;
        }
        

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
        texture.Bind();
        UpdatePlanets(deltaTime);
        for (const Planets& planet : planets)
        {
            glm::mat4 model(1.0f);

            model = glm::translate(model, planet.position);
            model = glm::scale(model, glm::vec3(planet.radius));
            shaderProgram.SetMat4("model", model);
            shaderProgram.SetVec3("objectColor", planet.color);
            sphere.Draw();
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    
    sphere.Delete();
    texture.Delete();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    auto* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera && width > 0 && height > 0)
    {
        camera->width  = width;
        camera->height = height;
    }
}

void setWindowIcon(GLFWwindow* window, const char* filePath) {
    GLFWimage images;
    int channels;
    stbi_set_flip_vertically_on_load(false); 
    images.pixels = stbi_load(filePath, &images.width, &images.height, &channels, 4); 
    if (images.pixels != nullptr) {
        glfwSetWindowIcon(window, 1, &images);
        stbi_image_free(images.pixels);
    } 
}

std::vector<float> Draw(int res)
{
    std::vector<float> vertices;

    float resF = static_cast<float>(res);
    for (int i = 0; i < res; i++)
    {
        float theta = (static_cast<float>(i) / resF) * glm::pi<float>();
        float theta1 = (static_cast<float>(i + 1) / resF) * glm::pi<float>();

        for (int j = 0; j < res; j++)
        {
            float phi = (static_cast<float>(j) / resF) * (2.0f * glm::pi<float>());
            float phi1 = (static_cast<float>(j + 1) / resF) * (2.0f * glm::pi<float>());

            glm::vec3 v1 = SphericalToCartesian(1.0f, theta, phi);
            glm::vec3 v2 = SphericalToCartesian(1.0f, theta, phi1);
            glm::vec3 v3 = SphericalToCartesian(1.0f, theta1, phi);
            glm::vec3 v4 = SphericalToCartesian(1.0f, theta1, phi1);

            glm::vec3 n1 = glm::normalize(v1);
            glm::vec3 n2 = glm::normalize(v2);
            glm::vec3 n3 = glm::normalize(v3);
            glm::vec3 n4 = glm::normalize(v4);

            vertices.insert(vertices.end(), {
                v1.x, v1.y, v1.z, n1.x, n1.y, n1.z,
                v2.x, v2.y, v2.z, n2.x, n2.y, n2.z,
                v3.x, v3.y, v3.z, n3.x, n3.y, n3.z
            });

            vertices.insert(vertices.end(), {
                v2.x, v2.y, v2.z, n2.x, n2.y, n2.z,
                v4.x, v4.y, v4.z, n4.x, n4.y, n4.z,
                v3.x, v3.y, v3.z, n3.x, n3.y, n3.z
            });
            
        }
    }
    
    return vertices;
}

void UpdatePlanets(float deltaTime) {
    glm::vec3 SunPos = planets[0].position;
    for (int i = 1; i < planets.size(); ++i) {
        auto& planet = planets[i];
        
        planet.currAngle += (planet.speed * deltaTime);
        if (planet.currAngle > glm::two_pi<float>()) {
            planet.currAngle -= glm::two_pi<float>();
        }
        
        planet.position.x = glm::cos(planet.currAngle) * planet.orbitRad + SunPos.x;
        planet.position.y = SunPos.y;
        planet.position.z = glm::sin(planet.currAngle) * planet.orbitRad + SunPos.z;
    }
}


