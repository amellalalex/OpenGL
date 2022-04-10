#include "shaders.h"

/**
 * @brief Load shader file into lib
 *
 * @param path Path to .glsl file
 * @param type Specify whether shader is vertex or fragment
 * @return GLuint
 */
GLuint shader_load(const char* path, unsigned int type) {
    GLuint shader;

    // Load shader
    FILE* fshader = fopen(path, "r");
    if (fshader == NULL) {
        return 0;
    }

    // Get file size
    fseek(fshader, 0L, SEEK_END);
    size_t fsize = ftell(fshader);
    fseek(fshader, 0L, SEEK_SET);

    // Load shader text into memory
    char* shader_text = (char*)malloc(fsize + 1);
    if (shader_text == NULL) {
        goto cleanup;
    }
    if (fread(shader_text, sizeof(char), fsize, fshader) < fsize) {
        goto cleanup;
    }
    shader_text[fsize] = '\0';

    // DEBUG
    printf("%s\n", shader_text);

    // Load text into shader
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_text, NULL);
    glCompileShader(shader);

cleanup:
    // Free memory
    free(shader_text);

    // Close file
    fclose(fshader);

    return shader;
}