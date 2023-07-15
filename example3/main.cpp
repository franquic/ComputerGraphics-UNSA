#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    std::vector<glm::vec3> pyramid =
        {
            glm::vec3{-1.0f, -1.0f, 1.0f},
            glm::vec3(1.0f, -1.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),

            glm::vec3(1.0f, -1.0f, 1.0f),
            glm::vec3(1.0f, -1.0f, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),

            glm::vec3(1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(1.0f, -1.0f, 1.0f),
            glm::vec3(-1.0f, -1.0f, 1.0f),
            glm::vec3(1.0f, -1.0f, 1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(1.0f, -1.0f, -1.0f)};

    std::vector<glm::vec3> cube =
        {
            {-1.0f, 1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, 1.0f},
            {1.0f, 1.0f, -1.0f},
            {1.0f, -1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, -1.0f},
            {1.0f, -1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, 1.0f, -1.0f},
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, -1.0f},
            {1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f},
            {-1.0f, -1.0f, 1.0f},
            {-1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, 1.0f},
            {-1.0f, 1.0f, -1.0f}};

    // Creación y configuración del Vertex Buffer Object (VBO)
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, pyramid.size() * sizeof(glm::vec3), &pyramid[0], GL_STATIC_DRAW);

    // VBO for cube
    GLuint vboCube;
    glGenBuffers(1, &vboCube);
    glBindBuffer(GL_ARRAY_BUFFER, vboCube);
    glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof(glm::vec3), &cube[0], GL_STATIC_DRAW);

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
        "uniform mat4 projMat;"
        "uniform mat4 mvMat;"
        "void main(){"
        //"gl_Position = vec4(vp, 1.0);"
        "gl_Position = projMat * mvMat * vec4(vp, 1.0);"
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

    auto gravity = glm::vec3(0.0f, -0.98f, 0.0f);
    float h = 0.0001f;
    float t = 0.0f;

    std::vector<glm::vec3> velocities{pyramid.size(), {0, 0, 0}};

    // Perspective matrix
    int width, height;
    glfwGetFramebufferSize(window, &width, &height); // this is C
    float aspect = (float)width / (float)height;
    glm::mat4 projMat = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 1000.0f);

    GLuint projMatLoc = glGetUniformLocation(shaderProgram, "projMat");
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));

    float angleCam = 0.0f;
    float angleCube = 0.0f;
    float anglePyra = 0.0f;
    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Cam
        auto viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
        auto rotateMat = glm::rotate(glm::mat4(1.0f), angleCam, {0, 1, 0});
        angleCam = (angleCam >= 2 * M_PI) ? 0 : angleCam + 0.01;
        viewMat = viewMat * rotateMat;

        for (unsigned int i = 0; i < pyramid.size(); i++)
        {
            velocities[i] = velocities[i] + gravity * h;
            pyramid[i] = pyramid[i] + velocities[i] * t;
        }
        t += h;

        // Pyramid

        auto modelMatPy = glm::translate(glm::mat4(1.0f), {1.f, 1.f, -1.f});
        auto mvMat = viewMat * modelMatPy;
        GLuint mvMatLoc = glGetUniformLocation(shaderProgram, "mvMat");
        glUniformMatrix4fv(mvMatLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

        glBufferData(GL_ARRAY_BUFFER, 3 * pyramid.size() * sizeof(float), &pyramid[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, pyramid.size());

        // Cube
        auto modelMatCube = glm::translate(glm::mat4(1.0f), {-1.f, 1.f, -1.f});
        rotateMat = glm::rotate(glm::mat4(1.0f), angleCube, {-1, 0, 0});
        angleCube = (angleCube >= 2 * M_PI) ? 0 : angleCube + 0.05;
        mvMat = viewMat * modelMatCube * rotateMat;
        // mvMatLoc = glGetUniformLocation(shaderProgram, "mvMat");

        glUniformMatrix4fv(mvMatLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

        glBufferData(GL_ARRAY_BUFFER, 3 * cube.size() * sizeof(float), &cube[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, cube.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}