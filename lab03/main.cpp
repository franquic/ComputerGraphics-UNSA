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
    glClearColor(0.1f, 0.1f, 0.1f, 0.0);

    std::vector<glm::vec3> bunnyVertices;
    if (!loadObject("Bunny.obj", bunnyVertices))
    {
        glfwTerminate();
        return -1;
    }

    // Scale OBJ
    auto scale = glm::scale(glm::mat4(1), glm::vec3(40, 40, 40));
    for (auto &v : bunnyVertices)
    {
        v = scale * glm::vec4(v, 1);
    }

    std::vector<glm::vec3> dragonVertices;
    if (!loadObject("Dragon.obj", dragonVertices))
    {
        glfwTerminate();
        return -1;
    }
    // Scale OBJ
    scale = glm::scale(glm::mat4(1), glm::vec3(40, 40, 40));
    for (auto &v : dragonVertices)
    {
        v = scale * glm::vec4(v, 1);
    }

    // Creación y configuración del Vertex Buffer Object (VBO)
    GLuint vboBunny;
    glGenBuffers(1, &vboBunny);
    glBindBuffer(GL_ARRAY_BUFFER, vboBunny);
    // glBufferData(GL_ARRAY_BUFFER, bunnyVertices.size() * sizeof(glm::vec3), &bunnyVertices[0], GL_STATIC_DRAW);

    // Creación y configuración del Vertex Buffer Object (VBO) para el dragon
    GLuint vboDragon;
    glGenBuffers(1, &vboDragon);
    glBindBuffer(GL_ARRAY_BUFFER, vboDragon);
    // glBufferData(GL_ARRAY_BUFFER, dragonVertices.size() * sizeof(glm::vec3), &dragonVertices[0], GL_STATIC_DRAW);

    // Creación y configuración del Vertex Array Object (VAO)
    GLuint vaoBunny;
    glGenVertexArrays(1, &vaoBunny);

    GLuint vaoDragon;
    glGenVertexArrays(1, &vaoDragon);

    // glBindVertexArray(vao);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void *>(0));
    // glEnableVertexAttribArray(0);

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
        "frag_color = vec4(1.0f, 1.0f, 0.0f, 0.0f);"
        "}";

    const char *fragment_shader_dragon =
        "#version 330 core\n"
        "out vec4 frag_color;"
        "void main(){"
        "frag_color = vec4(1.0f, 0.5f, 0.0f, 1.0);"
        "}";

    // Compilación y enlace de los shaders
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint fsDragon = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsDragon, 1, &fragment_shader_dragon, nullptr);
    glCompileShader(fsDragon);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fs);
    glAttachShader(shaderProgram, vs);
    glLinkProgram(shaderProgram);

    GLuint shaderProgramDragon = glCreateProgram();
    glAttachShader(shaderProgramDragon, fsDragon);
    glAttachShader(shaderProgramDragon, vs);
    glLinkProgram(shaderProgramDragon);

    // Gravity
    auto gravity = glm::vec3(0.0f, -0.98f, 0.0f);
    float h = 0.0001f;
    float t = 0.0f;

    std::vector<glm::vec3> velocities(bunnyVertices.size(), {0, 0, 0});

    // Perspective Matrix
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    auto aspect = static_cast<float>(width) / static_cast<float>(height);
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.f), aspect, 0.1f, 1000.0f);

    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glUseProgram(shaderProgramDragon);
    projectionMatrixLocation = glGetUniformLocation(shaderProgramDragon, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    float angleCam = 0.0f;
    float angleDragon = 0.0f;

    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Cam
        auto viewMatrix = glm::translate(glm::mat4(1), {0.f, 0.f, -10.f});
        auto rotateMatrix = glm::rotate(glm::mat4(1), angleCam, {0, 1, 0});
        angleCam = (angleCam >= 2 * 3.1415f) ? 0 : angleCam + 0.01;
        viewMatrix *= rotateMatrix;

        // for (unsigned int i = 0; i < velocities.size(); i++)
        // {
        //     velocities[i] += gravity * h;
        //     bunnyVertices[i] += velocities[i] * t;
        // }
        // t += h;

        // Gravity Runge-Kutta
        for (unsigned i = 0; i < bunnyVertices.size(); ++i)
        {
            glm::vec3 acceleration = gravity;
            glm::vec3 k1 = h * acceleration;
            glm::vec3 k2 = h * (acceleration + k1 / 2.0f);
            glm::vec3 k3 = h * (acceleration + k2 / 2.0f);
            glm::vec3 k4 = h * (acceleration + k3);

            velocities[i] = velocities[i] + k1 / 6.0f + k2 / 3.0f + k3 / 3.0f + k4 / 6.0f;
            bunnyVertices[i] = bunnyVertices[i] + velocities[i] * t;
        }
        t += h;

        // dragon
        auto modelMatrixDragon = glm::translate(glm::mat4(1.0f), {0.f, 0.f, 0.f});
        rotateMatrix = glm::rotate(glm::mat4(1.f), angleDragon, {0, 1, 0});
        angleDragon = (angleDragon >= 2 * M_PI) ? 0 : angleDragon + 0.05f;
        auto mvMatrix = viewMatrix * modelMatrixDragon * rotateMatrix;

        glUseProgram(shaderProgramDragon);
        GLuint mvMatrixLocation = glGetUniformLocation(shaderProgramDragon, "mvMatrix");
        glUniformMatrix4fv(mvMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));

        glBindVertexArray(vaoDragon);
        glBindBuffer(GL_ARRAY_BUFFER, vboDragon);
        glBufferData(GL_ARRAY_BUFFER, 3 * dragonVertices.size() * sizeof(float), &dragonVertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, dragonVertices.size() * 3);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        // bunny
        auto modelMatrixBunny = glm::translate(glm::mat4(1.0f), {0.f, 0.5f, 0.f});
        mvMatrix = viewMatrix * modelMatrixBunny;

        glUseProgram(shaderProgram);
        mvMatrixLocation = glGetUniformLocation(shaderProgram, "mvMatrix");
        glUniformMatrix4fv(mvMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));

        glBindVertexArray(vaoBunny);
        glBindBuffer(GL_ARRAY_BUFFER, vboBunny);
        glBufferData(GL_ARRAY_BUFFER, 3 * bunnyVertices.size() * sizeof(float), &bunnyVertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, bunnyVertices.size() * 3);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}