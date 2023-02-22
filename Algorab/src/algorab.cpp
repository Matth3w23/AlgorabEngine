#include "algorab.h"

double deltaTime;

//float moveSpeed = 9999999999999999;
float moveSpeed = 9;
Camera mainCam(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), false); //TODO: Change camera from being a global variable/change input functions

unsigned int windowWidth = 800;
unsigned int windowHeight = 600;

//mouse input
bool firstMouse = true;
float mouseLastX = (float)windowWidth / 2.0f;
float mouseLastY = (float)windowHeight / 2.0f;

//performance variables
bool performanceRunning = false;
bool performanceCheck = false;

double performanceTotal = 0.0;
unsigned int performanceIts = 0;
bool pDown = false;

void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "OpenGL error:" << type << ", " << message << std::endl;
}

int main() {
    std::string testFloat = "-1111";
    UFloat t1("-0.981707");
    UFloat t2("0.0670635");
    UFloat t3("1111.56723");
    UFloat t4("3754893675893465897654897685497689547698547868934876.3728756436893432684872635465");
    UFloat t5("-84359086785367398657438965348.2184764656");
    UFloat t6("-1732648235487.172881623457");
    UFloat t7("-1482864826.2147823756847");
    std::cout << UFloat::uFloatToString(t1) << std::endl;
    std::cout << UFloat::uFloatToString(t2) << std::endl;
    t2.add(t1);
    std::cout << UFloat::uFloatToString(t2) << std::endl;
    //std::cout << UFloat::uFloatToString(UFloat::sum(t2, t1)) << std::endl;

    //std::exit(0);






    //random distribution tools
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_real_distribution<> dist(-9999999999999999, 9999999999999999);
    std::uniform_real_distribution<> dist(0, 99);

    stbi_set_flip_vertically_on_load(true); //texture flip

    glfwInit();

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif // DEBUG

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfwWindowHint(GLFW_SAMPLES, 4); //TODO: Replace with offscreen anti-aliasing


    //window and context setup
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Window", NULL, NULL); //probably ought to abstract this out but fine for now
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
    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

#ifdef _DEBUG
    //enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, NULL);
#endif // DEBUG


    //Camera setup
    //Camera mainCam(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), false);
    mainCam.setProjectionFrustum(70, (float)windowWidth / (float)windowHeight, 1, 10, true);
    mainCam.updateProjectionMatrix();
    //mainCam.updateViewMatrix();
    RenderTarget mainRenderTarget(windowWidth, windowHeight);
    Renderer mainRenderer(&mainCam, mainRenderTarget);



    //model importing
    Assimp::Importer modelImporter;

    //test model
    Model backpackModel("assets/models/backpack/backpack.obj");
    //Model spaceShuttleModel("assets/models/spaceShuttleGLTF/scene.gltf");
    Model spaceShuttleModel("assets/models/backpack/backpack.obj");
    
    //bunch of test entities
    std::vector<ModelEntity*> bpEnts;
    //ModelEntity backpackEntity(&backpackModel, glm::vec3(1.0f, 2.0f, 10.0f), 0.1f);
    ModelEntity backpackEntity(&backpackModel, glm::vec3(0.0f, 0.0f, 10.0f), 1.0f);
    /*ModelEntity backpackEntityA(&spaceShuttleModel, glm::vec3(5.0f, 0.0f, 3), 0.3f);
    ModelEntity backpackEntity1(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity2(&spaceShuttleModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity3(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity4(&spaceShuttleModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity5(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity6(&spaceShuttleModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity7(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity8(&spaceShuttleModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity9(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    ModelEntity backpackEntity10(&spaceShuttleModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);*/
    bpEnts.push_back(&backpackEntity);
    /*bpEnts.push_back(&backpackEntityA);
    bpEnts.push_back(&backpackEntity1);
    bpEnts.push_back(&backpackEntity2);
    bpEnts.push_back(&backpackEntity3);
    bpEnts.push_back(&backpackEntity4);
    bpEnts.push_back(&backpackEntity5);
    bpEnts.push_back(&backpackEntity6);
    bpEnts.push_back(&backpackEntity7);
    bpEnts.push_back(&backpackEntity8);
    bpEnts.push_back(&backpackEntity9);
    bpEnts.push_back(&backpackEntity10);*/

    for (int i = 0; i < 100; i++) {
        ModelEntity* test = new ModelEntity(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
        bpEnts.push_back(test);
    }

    //for (int i = 0; i <= 10; i++) {
    //    ModelEntity* test = new ModelEntity(&backpackModel, glm::vec3((i/10.0f) * pow(10, i), 0.0f, 1*pow(10,i)), 1.0f);
    //    bpEnts.push_back(test);
    //}

    //for (ModelEntity* bp : bpEnts) {
    //    float d = 10.0f;
    //    //if (bp->getModel()->getMeshes().size() == spaceShuttleModel.getMeshes().size()) { d = 100.0; } //very scrappy, just for testing. Have way to check if same model?
    //    bp->setScale(glm::length(uFVecToVec(bp->getPosition())) * (1.0f / d));
    //}


    //variables for testing
    const float radius = 10.0f;
    unsigned int counter = 0;
    UFloat zPos = UFloat("0");
    const float moveStep = 100000000000000.0f;



    //frame time stuff
    double lastTime = glfwGetTime();
    double currentTime;
    //double deltaTime;
    double fps;
    double average = 0.0f;
    std::stringstream ss;

    std::cout << "Starting" << std::endl;

    UFloat adjustedPos = UFloat();
    UFloat minusFive = UFloat("-5.0");
    UFloat one = UFloat("1"); //TODO: Add float addition and subtraction to UFLOAT, and maybe increment

    while (!glfwWindowShouldClose(window)) { //main loop

        //mainCam.setPosition(glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius));
        //mainCam.lookAt(glm::vec3(0.0f, 0.0f, 0.0f)); //TODO: Add variants of setPosition and lookAt that don't use vec3

        //adjustedPos = UFloat::floatMult(zPos, moveStep);

        //backpackEntity.setPosition(UFVec3(0.0f, 0.0f, adjustedPos));
        //mainCam.setPosition(UFVec3(0.0f, 0.0f, UFloat::sum(adjustedPos, minusFive)));
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
        zPos.add(one);

        average = (average * (counter - 1) + fps) / counter;
        if (counter >= 20) {
            ss.str(std::string());
            ss << "Frame Time: " << deltaTime << ", FPS: " << average << ", Z: " << (adjustedPos.toString());
            ss << "[" << mainCam.getPosition().x.toString() << ", " << mainCam.getPosition().y.toString() << ", " << mainCam.getPosition().z.toString() << "]";
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
                    std::cout << "Average frame time: " << performanceTotal / performanceIts << "(Average FPS: " << performanceIts / performanceTotal << ")" << std::endl;
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