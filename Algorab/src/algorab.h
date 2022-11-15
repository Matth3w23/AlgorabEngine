#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera/camera.h"
#include "renderer/renderer.h"
#include "renderer/render_target.h"
#include "entity/entity.h"

#include <iostream>
#include <random>

void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main();

void processInput(GLFWwindow* window);