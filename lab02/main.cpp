#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "loadObj.hpp"

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cerr << "GLFW failed\n";
        return -1;
    }

    // Configuración de la ventana GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Creación de la ventana GLFW
    window = glfwCreateWindow(600, 600, "OpenGL OBJ", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "GLFW window failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW failed\n";
        glfwTerminate();
        return -1;
    }

    // Configuración de OpenGL
    glClearColor(0.2, 0.2, 0.5, 0.0);

    std::vector<glm::vec3> bunnyVertices;
    if (!loadObject("Bunny.obj", bunnyVertices))
    {
        glfwTerminate();
        return -1;
    }

    std::vector<glm::vec3> dragonVertices;
    if (!loadObject("Dragon.obj", dragonVertices))
    {
        glfwTerminate();
        return -1;
    }

    // Creación y configuración del Vertex Buffer Object (VBO)
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, bunnyVertices.size() * sizeof(glm::vec3), &bunnyVertices[0], GL_STATIC_DRAW);

    // Creación y configuración del Vertex Buffer Object (VBO) para el dragon
    GLuint vboDragon;
    glGenBuffers(1, &vboDragon);
    glBindBuffer(GL_ARRAY_BUFFER, vboDragon);
    glBufferData(GL_ARRAY_BUFFER, dragonVertices.size() * sizeof(glm::vec3), &dragonVertices[0], GL_STATIC_DRAW);

    // Creación y configuración del Vertex Array Object (VAO)
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    // Definición de los shaders de vértices y fragmentos
    const char *vertex_shader =
        "#version 330 core\n"
        "in vec3 vp;"
        "uniform mat4 projectionMatrix;"
        "uniform mat4 mvMatrix;"
        "void main(){"
        //"gl_Position = vec4(vp, 1.0);"
        "gl_Position = projectionMatrix * mvMatrix * vec4(vp, 1.0);"
        "}";

    const char *fragment_shader =
        "#version 330 core\n"
        "out vec4 frag_color;"
        "void main(){"
        "frag_color = vec4(0.5, 0.0, 0.0, 1.0);"
        "}";

    // Compilación y enlace de los shaders
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fs);
    glAttachShader(shaderProgram, vs);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    // Gravity Euler
    auto gravity = glm::vec3(0.0f, -0.98f, 0.0f);
    float h = 0.0001f;
    float t = 0.0f;

    std::vector<glm::vec3> velocities(bunnyVertices.size(), glm::vec3(0.0f));

    // Perspective Matrix
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    auto aspect = (float)width / (float)height;
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.f), aspect, .1f, 100.0f);

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    float angle = 0.0f;
    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        for (unsigned int i = 0; i < velocities.size(); i++)
        {
            velocities[i] += gravity * h;
            bunnyVertices[i] += velocities[i] * t;
        }
        t += h;

        // Cam
        auto viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.f, -5.0f));
        auto rotateMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.5f, 0.0f));
        angle += 0.01f;
        viewMatrix = viewMatrix * rotateMatrix;

        // dragon
        auto modelMatrixObj1 = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        auto mvMatrix = viewMatrix * modelMatrixObj1;
        GLuint mvMatrixLocation = glGetUniformLocation(shaderProgram, "mvMatrix");
        glUniformMatrix4fv(mvMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));

        glBufferData(GL_ARRAY_BUFFER, 3 * dragonVertices.size() * sizeof(float), &dragonVertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, dragonVertices.size() * 3);

        // bunny
        auto modelMatrixObj2 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 1.0f));
        mvMatrix = viewMatrix * modelMatrixObj2;
        mvMatrixLocation = glGetUniformLocation(shaderProgram, "mvMatrix");
        glUniformMatrix4fv(mvMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));

        glBufferData(GL_ARRAY_BUFFER, 3 * bunnyVertices.size() * sizeof(float), &bunnyVertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, bunnyVertices.size() * 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}