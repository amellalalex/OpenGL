// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard C libraries
#include <stdio.h>
#include <unistd.h>

// Project headers
#include "shaders.h"

static void error_callback(int error, const char *desc) {
    fprintf(stderr, "[!] {# %d} %s\n", error, desc);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    // Initialize glfw lib
    if (!glfwInit()) {
        error_callback(-1, "Failed to initialize GLFW");
        return -1;
    }

    // Configure error callback
    glfwSetErrorCallback(error_callback);

    // Set window hints before creation
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window and context
    GLFWwindow *window = glfwCreateWindow(640, 480, "Game", NULL, NULL);
    if (!window) {
        error_callback(-1, "Failed to initialize window");
        return -1;
    }

    // Set this window as the current context
    glfwMakeContextCurrent(window);

    // Load extensions (this is where glad comes in)
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Set key callbacks
    glfwSetKeyCallback(window, key_callback);

    // Set frame buffer swap interval (0=do not wait for screen updates, 1=wait
    // 1 screen update)
    glfwSwapInterval(1);

    // Setup viewport (NOTE: framebuffer size is subject to change)
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Create triangle
    float vertices[] = {
        -0.5f,  -0.5f, 0.0f,\ 
        0.5f, -0.5f, 0.0f,\ 
        0.0f, 0.5f,  0.0f,\ 
        0.5f, 0.5f,  0.0f,
    };

    float colours[] = {
        1.0f, 0.0f, 0.0f,\ 
        0.0f, 1.0f, 0.0f,\ 
        0.0f, 0.0f, 1.0f,
    };

    // Copy vertices cinto vertex buffer object
    GLuint vbo_points = 0;
    glGenBuffers(1, &vbo_points);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_points);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

    GLuint vbo_colours = 0;
    glGenBuffers(1, &vbo_points);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colours);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);

    // Store vertex buffer in vertex array
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_points);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colours);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Load shaders
    shader vs, fs;
    if ((vs = shader_load("src/shaders/vertex.glsl", GL_VERTEX_SHADER)) == 0) {
        error_callback(-1, "Failed to load shader");
        return -1;
    }
    if ((fs = shader_load("src/shaders/fragment.glsl", GL_FRAGMENT_SHADER)) ==
        0) {
    }

    // Combine shaders into GPU programme
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Update time
        double time = glfwGetTime();

        glPolygonMode(GL_FRONT, GL_LINE);
        // Clear drawing surface
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme);
        glBindVertexArray(vao);

        // Draw points from current vertice array
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // Swap frame buffer (update current screen)
        glfwSwapBuffers(window);
        // Refresh events
        glfwPollEvents();
    }

    // Destroy window
    glfwDestroyWindow(window);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}