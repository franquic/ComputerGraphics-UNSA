#include <iostream>
#include <vector>

#include <GL/glew.h>    //opengl calls
#include <GLFW/glfw3.h> //windows managemet

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    glewExperimental = GL_TRUE; // mac

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW failed\n";
        glfwTerminate(); // delete
        return -1;
    }

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

    // Crate vertex buffer objects
    GLuint vboPyramid;                         // vertex buffer object
    glGenBuffers(1, &vboPyramid);              // generate
    glBindBuffer(GL_ARRAY_BUFFER, vboPyramid); //'constructor'

    GLuint vboCube;                         // vertex buffer object
    glGenBuffers(1, &vboCube);              // generate
    glBindBuffer(GL_ARRAY_BUFFER, vboCube); //'constructor'

    // Create vertex array objects
    GLuint vaoPyramid;
    // Generate a name for a new array.
    glGenVertexArrays(1, &vaoPyramid);

    GLuint vaoCube;
    // Generate a name for a new array.
    glGenVertexArrays(1, &vaoCube);

    // Shaders
    const char *vertex_shader =
        "#version 330 core\n"
        "in vec3 vp;"           // Loc -> 0
        "uniform mat4 projMat;" // GPU
        "uniform mat4 mvMat;"
        "void main(){"
        "gl_Position = projMat*mvMat*vec4(vp, 1.0);"
        "}";

    const char *fragment_shader =
        "#version 330 core\n"
        "out vec4 frag_color;"
        "void main(){"
        "frag_color = vec4(0.5, 0.0, 0.0, 1.0);"
        "}";

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

    // glValidateProgram(shaderProgram);

    glUseProgram(shaderProgram);

    auto gravity = glm::vec3{0, -0.98, 0};
    float h = 0.0001f;
    float t = 0.f;

    std::vector<glm::vec3> velocities{pyramid.size(), {0, 0, 0}};

    // SETUP projection matrix
    int width, height;
    glfwGetFramebufferSize(window, &width, &height); // C puro
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    glm::mat4 projMat = glm::perspective(glm::radians(90.f), aspect, 0.1f, 1000.f); // CPU

    GLuint projLoc = glGetUniformLocation(shaderProgram, "projMat");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMat));

    float angleCam = 0.f;
    float angleCube = 0.f;
    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);

        // SETUP camera
        auto viewMat = glm::translate(glm::mat4(1), {0.f, 0.f, -10.f});
        auto rotateMat = glm::rotate(glm::mat4(1), angleCam, {0, 1, 0});
        angleCam = (angleCam >= 2 * 3.1415f) ? 0 : angleCam + 0.01;
        viewMat *= rotateMat;

        // Gravity + Euler simple
        for (unsigned i = 0; i < pyramid.size(); ++i) // per vertex
        {

            velocities[i] = velocities[i] + h * gravity;
            pyramid[i] = pyramid[i] + velocities[i] * t;
        }
        t += h;

        // Pyramid
        auto modelMatPy = glm::translate(glm::mat4(1), {1.f, 1.f, -1.f}); // model matrix
        auto mvMat = viewMat * modelMatPy;                                // multiply with viewMatrix (cam)

        GLuint mvLoc = glGetUniformLocation(shaderProgram, "mvMat");
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

        glBindVertexArray(vaoPyramid);
        glBindBuffer(GL_ARRAY_BUFFER, vboPyramid);
        glBufferData(GL_ARRAY_BUFFER, 3 * pyramid.size() * sizeof(float), &pyramid[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, pyramid.size() * 3);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        // cube

        auto modelMatCube = glm::translate(glm::mat4(1), {-1.f, -1.f, 0.f});
        rotateMat = glm::rotate(glm::mat4(1), angleCube, {-1, 0, 0}); // rotation matrix
        angleCube = (angleCube >= 2 * 3.1415f) ? 0 : angleCube + 0.05;
        mvMat = viewMat * modelMatCube * rotateMat; // multiply all cube matrices

        // set mvMat
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

        glBindVertexArray(vaoCube);
        glBindBuffer(GL_ARRAY_BUFFER, vboCube);
        glBufferData(GL_ARRAY_BUFFER, 3 * cube.size() * sizeof(float), &cube[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, cube.size() * 3);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        glfwSwapBuffers(window); // opengl
        glfwPollEvents();        // user
    }

    glfwTerminate(); // delete

    std::cout << "Hello World!" << std::endl;
    return 0;
}
