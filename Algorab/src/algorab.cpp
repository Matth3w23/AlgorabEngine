#include "algorab.h"

double deltaTime;

float moveSpeed = 9999999999999999;
//float moveSpeed = 99;
//float moveSpeed = 1.0f;
Camera mainCam(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), false); //TODO: Change camera from being a global variable/change input functions

//mouse input
bool firstMouse = true;
float mouseLastX = 800.0f / 2.0f;
float mouseLastY = 600.0f / 2.0f;

//performance variables
bool performanceRunning = false;
bool performanceCheck = false;

double performanceTotal = 0.0;
unsigned int performanceIts = 0;
bool pDown = false;

void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "OpenGL error[" << severity << "]" << ": " << type << ", " << message << std::endl;
}

int main() {
    //random distribution tools
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, 9999999999999999);
    //sstd::uniform_real_distribution<> dist(0, 99);



    stbi_set_flip_vertically_on_load(true); //texture flip

    glfwInit();

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif // DEBUG

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4); //TODO: Replace with offscreen anti-aliasing


    //window and context setup
    GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL); //probably ought to abstract this out but fine for now
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); //v-sync off




    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);



    //load gl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW); //due to different coordinate systems
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef _DEBUG
    //enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, NULL);
#endif // DEBUG


    //Camera setup
    //Camera mainCam(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), false);
    mainCam.setProjectionFrustum(70, 800.0f / 600.0f, 100, 100000, true);
    mainCam.updateProjectionMatrix();
    mainCam.updateViewMatrix();
    RenderTarget mainRenderTarget;
    Renderer mainRenderer(&mainCam, mainRenderTarget);



    //model importing
    Assimp::Importer modelImporter;

    //test model
    Model backpackModel("assets/models/backpack/backpack.obj");
    
    //bunch of test entities
    std::vector<ModelEntity*> bpEnts;
    //ModelEntity backpackEntity(&backpackModel, glm::vec3(1.0f, 2.0f, 10.0f), 0.1f);
    ModelEntity backpackEntity(&backpackModel, glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
    ModelEntity backpackEntityA(&backpackModel, glm::vec3(5.0f, 0.0f, 10.0f), 0.3f);
    ModelEntity backpackEntity1(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity2(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity3(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity4(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity5(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity6(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity7(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity8(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity9(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity10(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    bpEnts.push_back(&backpackEntity);
    bpEnts.push_back(&backpackEntityA);
    bpEnts.push_back(&backpackEntity1);
    bpEnts.push_back(&backpackEntity2);
    bpEnts.push_back(&backpackEntity3);
    bpEnts.push_back(&backpackEntity4);
    bpEnts.push_back(&backpackEntity5);
    bpEnts.push_back(&backpackEntity6);
    bpEnts.push_back(&backpackEntity7);
    bpEnts.push_back(&backpackEntity8);
    bpEnts.push_back(&backpackEntity9);
    for (ModelEntity* bp : bpEnts) {
        bp->setScale(glm::length(bp->getPosition()) * (1.0f / 10.0f));
    }


    //variables for testing
    const float radius = 10.0f;
    unsigned int counter = 0;
    unsigned int zPos = 0;
    const float moveStep = 10000.0f;



    //frame time stuff
    double lastTime = glfwGetTime();
    double currentTime;
    //double deltaTime;
    double fps;
    double average = 0.0f;
    std::stringstream ss;

    std::cout << "Starting" << std::endl;

    while (!glfwWindowShouldClose(window)) { //main loop        
        //mainCam.setPosition(glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius));
        //mainCam.lookAt(glm::vec3(0.0f, 0.0f, 0.0f)); //TODO: Add variants of setPosition and lookAt that don't use vec3

        //backpackEntity.setPosition(glm::vec3(0.0f, 0.0f, zPos * moveStep));
        //mainCam.setPosition(glm::vec3(0.0f, 0.0f, zPos * moveStep - 5.0f));
        //backpackEntity.setScale(zPos * moveStep / 10);



        processInput(window);

        mainCam.updateRelativeViewMatrix();

        for (ModelEntity* bp : bpEnts) {
            mainRenderer.PushEntity(*bp);
        }
        mainRenderer.renderAllPushed();

        glfwSwapBuffers(window);
        glfwPollEvents();




        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        fps = 1 / deltaTime;
        lastTime = currentTime;

        counter++;
        //zPos++;

        average = (average * (counter - 1) + fps) / counter;
        if (counter >= 20) {
            ss.str(std::string());
            ss << "Frame Time: " << deltaTime << ", FPS: " << average << ", Z: " << (zPos * moveStep);
            //ss << "[" << mainCam.getPosition().x << ", " << mainCam.getPosition().y << ", " << mainCam.getPosition().z << "]";
            glfwSetWindowTitle(window, ss.str().c_str());
            //std::cout << fps << std::endl;
            counter = 0;
            average = 0.0f;
        }


        //rudimentary performance stuff
        if (performanceRunning) {

            if (!performanceCheck) { //just stopped performance
                if (performanceIts > 0) {
                    performanceRunning = false;
                    //std::cout << "Average frame time: " << performanceTotal / performanceIts << "(Average FPS: " << performanceIts / performanceTotal << ")" << std::endl;
                    ss.str(std::string());
                    ss << "Average frame time: " << performanceTotal / performanceIts << "(Average FPS: " << performanceIts / performanceTotal << ")";
                    //glfwSetWindowTitle(window, ss.str().c_str());
                } else {
                    std::cout << "Performance ran for 0 iterations" << std::endl;
                }

            } else {
                performanceIts += 1;
                performanceTotal += deltaTime;
            }

        } else {
            if (performanceCheck) {
                performanceIts = 0;
                performanceTotal = 0.0f;
                performanceRunning = true;
                std::cout << "Performance Started" << std::endl;
            }
        }
        
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) { //could use keyboard callback instead
    glm::vec3 movement = glm::vec3(0.0f);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        movement.z += 1.0f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        movement.z -= 1.0f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        movement.x += 1.0f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        movement.x -= 1.0f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        movement.y += 1.0f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        movement.y -= 1.0f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!pDown) {
            if (performanceCheck) {
                performanceCheck = false;
            } else {
                performanceCheck = true;
            }
            pDown = true;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
        pDown = false;
    }




    if (length(movement) >= 0.0f) {
        glm::normalize(movement);
    }
    mainCam.moveRelative(movement * moveSpeed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        mouseLastX = xpos;
        mouseLastY = ypos;
        firstMouse = false;
    }

    float offsetX = xpos - mouseLastX;
    float offsetY = -(ypos - mouseLastY); //y = 0 at top
    mouseLastX = xpos;
    mouseLastY = ypos;

    mainCam.turn(offsetX * 0.25, offsetY * 0.25, true); //TODO: Add sensitivity
}