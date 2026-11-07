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
void UpdatePlanets(double deltaTime);

glm::vec3 SphericalToCartesian(float radius, float theta, float phi)
{
    float x = radius * glm::sin(theta) * glm::cos(phi);
    float y = radius * glm::cos(theta);
    float z = radius * glm::sin(theta) * glm::sin(phi);

    return glm::vec3(x, y, z);
}

constexpr double G = 39.47841760435743; // AU^3 / (solar mass * year^2)
constexpr double SIMULATION_DAYS_PER_SECOND = 30.0;
constexpr double DAYS_PER_YEAR = 365.25;

struct Planets {
    float radius;
    glm::dvec3 position;
    glm::vec3 color;
    double mass;
    glm::dvec3 velocity{0.0};
    glm::dvec3 acceleration{0.0};
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
        // Visual radius, position (AU), color, mass (solar masses), velocity (AU/year), acceleration

        // Sun
        {0.20f, {0.0f, 0.0f, 0.0f},
        {1.00f, 0.75f, 0.15f},
        1.0, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

        // Mercury
        {0.035f, {0.387f, 0.0f, 0.0f},
        {0.65f, 0.65f, 0.65f},
        1.660e-7, {0.0f, 0.0f, 10.08f}, {0.0f, 0.0f, 0.0f}},

        // Venus
        {0.045f, {0.723f, 0.0f, 0.0f},
        {0.90f, 0.70f, 0.50f},
        2.448e-6, {0.0f, 0.0f, 7.38f}, {0.0f, 0.0f, 0.0f}},

        // Earth
        {0.05f, {1.000f, 0.0f, 0.0f},
        {0.20f, 0.50f, 1.00f},
        3.003e-6, {0.0f, 0.0f, 6.28f}, {0.0f, 0.0f, 0.0f}},

        // Mars
        {0.04f, {1.524f, 0.0f, 0.0f},
        {0.90f, 0.35f, 0.20f},
        3.227e-7, {0.0f, 0.0f, 5.09f}, {0.0f, 0.0f, 0.0f}},

        // Jupiter
        {0.12f, {5.203f, 0.0f, 0.0f},
        {0.80f, 0.65f, 0.50f},
        9.545e-4, {0.0f, 0.0f, 2.75f}, {0.0f, 0.0f, 0.0f}},

        // Saturn
        {0.10f, {9.537f, 0.0f, 0.0f},
        {0.85f, 0.75f, 0.55f},
        2.858e-4, {0.0f, 0.0f, 2.03f}, {0.0f, 0.0f, 0.0f}},

        // Uranus
        {0.075f, {19.191f, 0.0f, 0.0f},
        {0.50f, 0.80f, 0.90f},
        4.366e-5, {0.0f, 0.0f, 1.43f}, {0.0f, 0.0f, 0.0f}},

        // Neptune
        {0.075f, {30.069f, 0.0f, 0.0f},
        {0.25f, 0.40f, 0.90f},
        5.151e-5, {0.0f, 0.0f, 1.14f}, {0.0f, 0.0f, 0.0f}}
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
        double simulationDeltaTime =
            deltaTime * SIMULATION_DAYS_PER_SECOND / DAYS_PER_YEAR;
        constexpr int SUBSTEPS = 10;

        double subDeltaTime = simulationDeltaTime / SUBSTEPS;

        for (int i = 0; i < SUBSTEPS; i++)
        {
            UpdatePlanets(subDeltaTime);
        }
        for (const Planets& planet : planets)
        {
            glm::mat4 model(1.0f);

            model = glm::translate(
                model,
                glm::vec3(planet.position)
            );

            model = glm::scale(
                model,
                glm::vec3(planet.radius)
            );

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

void UpdatePlanets(double deltaTime)
{
    for (auto& planet : planets)
    {
        planet.acceleration = glm::dvec3(0.0);
    }
    for (size_t i = 0; i < planets.size(); i++)
    {
        for (size_t j = i + 1; j < planets.size(); j++)
        {
            glm::dvec3 direction =
                planets[j].position - planets[i].position;

            double distance = glm::length(direction);

            if (distance < 1e-10)
                continue;

            glm::dvec3 unitDirection = direction / distance;

            double force =
                G * planets[i].mass * planets[j].mass /
                (distance * distance);

            glm::dvec3 forceVector = unitDirection * force;

            planets[i].acceleration +=
                forceVector / planets[i].mass;

            planets[j].acceleration -=
                forceVector / planets[j].mass;
        }
    }
    for (auto& planet : planets)
    {
        planet.velocity += planet.acceleration * deltaTime;
        planet.position += planet.velocity * deltaTime;
    }
}
