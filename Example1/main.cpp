// Author Oscar Cuadros Linares
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Permite trabjar con operacion matriciales
#include <glm/glm.hpp>

int main()
{

    GLFWwindow *window;
    if (!glfwInit()) // init GLFW -> allocate memory
    {
        std::cerr << "GLFW failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(600, 600, "Hello triangle OpenGL", nullptr, nullptr);
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
        glfwTerminate(); // delete
        return -1;
    }

    glClearColor(0.2, 0.2, 0.5, 0.0);

    std::vector<glm::vec3> triangle =
        {
            glm::vec3{-1.0f, -1.0f, 0.0f},
            glm::vec3{1.0f, -1.0f, 0.0f},
            glm::vec3{0.0f, 1.0f, 0.0f},
        };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * triangle.size() * sizeof(float), &triangle[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    GLuint array;
    // Generate a name for a new array.
    glGenVertexArrays(1, &array);
    // Make the new array active, creating it if necessary.
    glBindVertexArray(array);
    // Make the buffer the active array buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Enable the vertex attribute
    glEnableVertexAttribArray(0);

    const char *verter_shader =
        "#version 330 core\n"
        "in vec3 vp;"
        "void main(){"
        "gl_Position = vec4(vp, 1.0);"
        "}";

    const char *fragment_shader =
        "#version 330 core\n"
        "out vec4 frag_color;"
        "void main(){"
        "frag_color = vec4(0.5, 0.0, 0.0, 0.5);"
        "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &verter_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fs);
    glAttachShader(shaderProgram, vs);
    glLinkProgram(shaderProgram);

    glValidateProgram(shaderProgram);

    glUseProgram(shaderProgram);

    std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    auto gravity = glm::vec3{0.0f, -0.98f, 0.0f};
    float h = 0.0001f;
    float t = 0.f;

    std::vector<glm::vec3> velocities(triangle.size(), {0, 0, 0});

    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);

        for (unsigned i = 0; i < triangle.size(); i++)
        {
            velocities[i] = velocities[i] + h * gravity;
            triangle[i] = triangle[i] + t * velocities[i];
        }

        t += h;
        glBufferData(GL_ARRAY_BUFFER, 3 * triangle.size() * sizeof(float), &triangle[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window); // opengl
        glfwPollEvents();        // user
    }

    glfwTerminate(); // delete

    return 0;
}
