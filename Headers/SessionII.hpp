// Preprocessor Directives
#pragma once


// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <shader.hpp>
#include <camera.hpp>
#include <heightmap.hpp>


#include <iostream>
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // takes care of getting around having to code your own image loader converter :) 
// JPEG, PNG format only 


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);




