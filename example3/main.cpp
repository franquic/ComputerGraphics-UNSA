#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Función para cargar un objeto en formato OBJ y extraer sus vértices
bool loadObject(const std::string &filename, std::vector<glm::vec3> &vertices)
{
    std::ifstream objFile(filename);
    if (!objFile.is_open())
    {
        std::cout << "Error al abrir el archivo: " << filename << std::endl;
        return false;
    }

    std::vector<glm::vec3> objVertices;
    std::string line;
    while (std::getline(objFile, line))
    {
        std::istringstream iss(line);
        std::string firstSymbol;
        iss >> firstSymbol;

        if (firstSymbol == "v")
        {
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            objVertices.push_back(position);
        }
        else if (firstSymbol == "f")
        {
            std::string vertexIndices;
            for (int i = 0; i < 3; i++)
            {
                iss >> vertexIndices;
                std::istringstream viss(vertexIndices);
                unsigned int index;
                viss >> index;
                vertices.push_back(objVertices[index - 1]);
            }
        }
    }

    objFile.close();
    return true;
}

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

    // std::vector<glm::vec3> vertices;
    // if (!loadObject("Bunny.obj", vertices))
    // {
    //     glfwTerminate();
    //     return -1;
    // }

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
    glm::mat4 projMat = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 100.0f);

    GLuint projMatLoc = glGetUniformLocation(shaderProgram, "projMat");
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, glm::value_ptr(projMat));

    float angle = 0.0f;
    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        for (unsigned int i = 0; i < pyramid.size(); i++)
        {
            velocities[i] = velocities[i] + gravity * h;
            pyramid[i] = pyramid[i] + gravity * t;
        }
        t += h;

        // Cam
        auto viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        auto rotateMat = glm::rotate(glm::mat4(1.0f), angle, {0, 0.5, 0});
        angle = (angle >= 2 * M_PI) ? 0 : angle + 0.01;
        viewMat = viewMat * rotateMat;

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
        mvMat = viewMat * modelMatCube;
        mvMatLoc = glGetUniformLocation(shaderProgram, "mvMat");
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