#include "headers/Camera.h"

Camera::Camera(int width, int height, glm::vec3 position, GLFWwindow* window)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, ShaderProgram& shader, const char* uniform)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(Position, Position + front, Up);
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window)
{
    // Keyboard Movement Handling
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += speed * front;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= speed * front;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= speed * glm::normalize(glm::cross(front, Up));

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += speed * glm::normalize(glm::cross(front, Up));

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Position += speed * Up;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        Position -= speed * Up;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = 0.4f;
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        speed = 0.1f;

    // Mouse Look Handling
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstMouse)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstMouse = false;
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        glm::vec3 newFront = glm::normalize(glm::rotate(front, glm::radians(-rotX), glm::normalize(glm::cross(front, Up))));

        // Decouple vertical snapping boundaries
        if (abs(glm::angle(newFront, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            front = newFront;
        }

        front = glm::normalize(glm::rotate(front, glm::radians(-rotY), Up));

        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;
    }
}

float Camera::getPositionX() {
    return Position.x;
}

float Camera::getPositionZ() {
    return Position.z;
}

glm::vec3 Camera::getCameraPos(){
    return Position;
}

glm::vec3 Camera::getCameraDirection(){
    return front;
}
