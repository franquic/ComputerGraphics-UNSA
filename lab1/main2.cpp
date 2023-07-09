#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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

    std::vector<glm::vec3> vertices;
    if (!loadObject("Conejo.obj", vertices))
    {
        glfwTerminate();
        return -1;
    }

    // Creación y configuración del Vertex Buffer Object (VBO)
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

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
        "void main(){"
        "gl_Position = vec4(vp, 1.0);"
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

    // Bucle principal de renderizado
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBufferData(GL_ARRAY_BUFFER, 3 * vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}