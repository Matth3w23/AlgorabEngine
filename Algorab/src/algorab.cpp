#include "algorab.h"


void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "OpenGL error:" << type << ", " << message << std::endl;
}

int main() {
    stbi_set_flip_vertically_on_load(true);

    glfwInit();

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif // DEBUG

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* testing = glfwCreateWindow(800, 600, "testing", NULL, NULL);
    if (testing == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(testing);

    //load gl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

#ifdef _DEBUG
    //enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, NULL);
#endif // DEBUG


    Camera mainCam(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 1.0f), false);
    mainCam.setProjectionFrustum(70, 800.0f / 600.0f, 0.1, 100, true);
    mainCam.updateProjectionMatrix();
    mainCam.updateViewMatrix();
    RenderTarget mainRenderTarget;
    Renderer mainRenderer(&mainCam, mainRenderTarget);

    Assimp::Importer modelImporter;

    Model backpackModel("assets/models/backpack/backpack.obj");
    ModelEntity backpackEntity(backpackModel, glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
    ModelEntity backpackEntity2(backpackModel, glm::vec3(2.0f, -1.0f, 5.0f), 0.5f);

    const float radius = 10.0f;

    std::cout << "Starting" << std::endl;
    while (!glfwWindowShouldClose(testing)) { //main loop
        mainCam.updateProjectionMatrix();
        mainCam.updateViewMatrix();
        
        mainCam.setPosition(glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius));
        mainCam.lookAt(glm::vec3(0.0f, 0.0f, 0.0f)); //TODO: Add variants of setPosition and lookAt that don't use vec3

        mainCam.updateViewMatrix();

        processInput(testing);
        mainRenderer.PushEntity(backpackEntity);
        mainRenderer.PushEntity(backpackEntity2);
        mainRenderer.renderAllPushed();

        glfwSwapBuffers(testing);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//finish mesh
//vertex array, buffer and element abstraction?
//texture abstraction
//coordinates
// model
//      mesh
// renderer
//      give target
//          given meshes/points, render to target
// entity
//      translate
//          x,y,z, scale (future coords)
// 
// 
//