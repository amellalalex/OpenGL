#ifndef OPENGL_SHADERS_H
#define OPENGL_SHADERS_H

// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * @brief Load shader file into lib
 *
 * @param path Path to .glsl file
 * @param type Specify whether shader is vertex or fragment
 * @return GLuint
 */
GLuint shader_load(const char* path, unsigned int type);

#endif