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

    glfwWindowHint(GLFW_SAMPLES, 4); //TODO: Replace with offscreen anti-aliasing
    
    GLFWwindow* testing = glfwCreateWindow(800, 600, "testing", NULL, NULL);
    if (testing == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(testing);
    glfwSwapInterval(0);

    //load gl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_CULL_FACE);
    //glEnable(GL_MULTISAMPLE);

#ifdef _DEBUG
    //enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, NULL);
#endif // DEBUG


    Camera mainCam(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), false);
    mainCam.setProjectionFrustum(70, 800.0f / 600.0f, 0.1, 100000, true);
    mainCam.updateProjectionMatrix();
    mainCam.updateViewMatrix();
    RenderTarget mainRenderTarget;
    Renderer mainRenderer(&mainCam, mainRenderTarget);

    Assimp::Importer modelImporter;

    Model backpackModel("assets/models/backpack/backpack.obj");
    ModelEntity backpackEntity(&backpackModel, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
    /*ModelEntity backpackEntity2(&backpackModel, glm::vec3(2.0f, -1.0f, 5.0f), 0.5f);
    ModelEntity backpackEntity3(&backpackModel, glm::vec3(2.0f, 1.0f, 5.0f), 0.5f);
    ModelEntity backpackEntity4(&backpackModel, glm::vec3(1.0f, -3.0f, 3.0f), 0.5f);
    ModelEntity backpackEntity5(&backpackModel, glm::vec3(2.0f, -4.0f, 0.0f), 0.5f);
    ModelEntity backpackEntity6(&backpackModel, glm::vec3(0.0f, 0.0f, 0.0f), 0.1f);
    ModelEntity backpackEntity7(&backpackModel, glm::vec3(-2.0f, -1.0f, 5.0f), 0.6f);
    ModelEntity backpackEntity8(&backpackModel, glm::vec3(2.0f, 4.0f, 5.0f), 0.8f);
    ModelEntity backpackEntity9(&backpackModel, glm::vec3(1.0f, -1.0f, 2.0f), 0.01f);
    ModelEntity backpackEntity10(&backpackModel, glm::vec3(-2.6f, -4.7f, 3.0f), 0.3f);*/

    const float radius = 10.0f;

    double lastTime = glfwGetTime();
    double currentTime;
    double frameTime;
    double fps;
    double average = 0.0f;
    std::stringstream ss;

    unsigned int counter = 0;
    unsigned int zPos = 0;
    const float moveStep = 1.0f;

    std::cout << "Starting" << std::endl;
    while (!glfwWindowShouldClose(testing)) { //main loop        
        mainCam.setPosition(glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius));
        mainCam.lookAt(glm::vec3(0.0f, 0.0f, 0.0f)); //TODO: Add variants of setPosition and lookAt that don't use vec3

        //backpackEntity.setPosition(glm::vec3(0.0f, 0.0f, zPos * moveStep));
        //backpackEntity.setScale(zPos * moveStep / 10);

        mainCam.updateViewMatrix();

        processInput(testing);
        mainRenderer.PushEntity(&backpackEntity);
       /* mainRenderer.PushEntity(&backpackEntity2);
        mainRenderer.PushEntity(&backpackEntity3);
        mainRenderer.PushEntity(&backpackEntity4);
        mainRenderer.PushEntity(&backpackEntity5);
        mainRenderer.PushEntity(&backpackEntity6);
        mainRenderer.PushEntity(&backpackEntity7);
        mainRenderer.PushEntity(&backpackEntity8);
        mainRenderer.PushEntity(&backpackEntity9);
        mainRenderer.PushEntity(&backpackEntity10);*/
        mainRenderer.renderAllPushed();

        glfwSwapBuffers(testing);
        glfwPollEvents();

        currentTime = glfwGetTime();
        frameTime = currentTime - lastTime;
        fps = 1 / frameTime;
        lastTime = currentTime;

        counter++;
        //zPos++;

        average = (average * (counter - 1) + fps) / counter;
        if (counter >= 20) {
            ss.str(std::string());
            ss << "Frame Time: " << frameTime << ", FPS: " << average << ", Z: " << (zPos * moveStep);
            glfwSetWindowTitle(testing, ss.str().c_str());
            //std::cout << fps << std::endl;
            counter = 0;
            average = 0.0f;
        }
        
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