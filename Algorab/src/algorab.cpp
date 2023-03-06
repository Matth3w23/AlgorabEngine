#include "algorab.h"

double deltaTime;

//float moveSpeed = 9999999999999999;
//double moveSpeed = 100;
UFloat moveSpeed = UFloat(100);
double moveSpeedScrollMultFactor = 1.258925411;

Camera mainCam(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), false); //TODO: Change camera from being a global variable/change input functions

unsigned int windowWidth = 1200;
unsigned int windowHeight = 800;

//mouse input
bool firstMouse = true;
float mouseLastX = (float)windowWidth / 2.0f;
float mouseLastY = (float)windowHeight / 2.0f;
float sensitivity = 0.1f;

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
    std::cout << "STARTING" << std::endl;
    //std::string testFloat = "-1111";
    //UFloat t1("-0.981707");
    //UFloat t2("0.0670635");
    //UFloat t3("1111.56723");
    //UFloat t4("3754893675893465897654897685497689547698547868934876.3728756436893432684872635465");
    //UFloat t5("-84359086785367398657438965348.2184764656");
    //UFloat t6("-1732648235487.172881623457");
    //UFloat t7("-1482864826.2147823756847");
    //std::cout << UFloat::uFloatToString(t1) << std::endl;
    //std::cout << UFloat::uFloatToString(t2) << std::endl;
    //t2.add(t1);
    //std::cout << UFloat::uFloatToString(t2) << std::endl;
    //std::cout << UFloat::uFloatToString(UFloat::sum(t2, t1)) << std::endl;

    //std::exit(0);






    //random distribution tools
    std::random_device rd;
    std::mt19937 gen(rd());
    //std::uniform_real_distribution<> dist(-9999999999999999, 9999999999999999);
    std::uniform_real_distribution<> dist(0, 1);
    std::uniform_real_distribution<> rand(0, 1);
    std::uniform_real_distribution<> randn(-1, 1);

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
    glfwSetScrollCallback(window, scroll_callback);



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


    //SCENE GRAPH
    EntityGrouper sceneGraphBase = EntityGrouper();


    //model importing
    Assimp::Importer modelImporter;

    //test model
    Model backpackModel("assets/models/backpack/backpack.obj");
    //Model spaceShuttleModel("assets/models/spaceShuttleGLTF/scene.gltf");
    Model spaceShuttleModel("assets/models/spaceShuttleGLTF/scene.gltf");
    
    //bunch of test entities
    std::vector<ModelEntity*> bpEnts;
    ModelEntity backpackEntity(&backpackModel, glm::vec3(1.0f, 2.0f, 10.0f), 0.1f);
    //ModelEntity backpackEntity(&backpackModel, glm::vec3(0.0f, 0.0f, 10.0f), 1.0f);
    ModelEntity backpackEntityA(&spaceShuttleModel, glm::vec3(5.0f, 0.0f, 3), 0.1f);
    ModelEntity backpackEntity1(&backpackModel, glm::vec3(randn(gen)*40, randn(gen) * 40, randn(gen) * 40), 1.0f);
    ModelEntity backpackEntity2(&spaceShuttleModel, glm::vec3(randn(gen) * 40, randn(gen) * 40, randn(gen) * 40), 0.1f);
    ModelEntity backpackEntity3(&backpackModel, glm::vec3(randn(gen) * 40, randn(gen) * 40, randn(gen) * 40), 1.0f);
    ModelEntity backpackEntity4(&spaceShuttleModel, glm::vec3(randn(gen) * 40, randn(gen) * 40, randn(gen) * 40), 0.1f);
    ModelEntity backpackEntity5(&backpackModel, glm::vec3(randn(gen) * 40, randn(gen) * 40, randn(gen) * 40), 1.0f);
    ModelEntity backpackEntity6(&spaceShuttleModel, glm::vec3(randn(gen) * 40, randn(gen) * 40, randn(gen) * 40), 0.1f);
    ModelEntity backpackEntity7(&backpackModel, glm::vec3(randn(gen) * 40, randn(gen) * 40, randn(gen) * 40), 1.0f);
    ModelEntity backpackEntity8(&spaceShuttleModel, glm::vec3(randn(gen) * 40, randn(gen) * 40, randn(gen) * 40), 0.1f);
    ModelEntity backpackEntity9(&backpackModel, glm::vec3(randn(gen) * 40, randn(gen) * 40, randn(gen) * 40), 1.0f);
    ModelEntity backpackEntity10(&spaceShuttleModel, glm::vec3(randn(gen) * 40, randn(gen) * 40, randn(gen) * 40), 0.1f);
    sceneGraphBase.addChild(&backpackEntity);
    sceneGraphBase.addChild(&backpackEntityA);
    sceneGraphBase.addChild(&backpackEntity1);
    sceneGraphBase.addChild(&backpackEntity2);
    sceneGraphBase.addChild(&backpackEntity3);
    sceneGraphBase.addChild(&backpackEntity4);
    sceneGraphBase.addChild(&backpackEntity5);
    sceneGraphBase.addChild(&backpackEntity6);
    sceneGraphBase.addChild(&backpackEntity7);
    sceneGraphBase.addChild(&backpackEntity8);
    sceneGraphBase.addChild(&backpackEntity9);
    sceneGraphBase.addChild(&backpackEntity10);



    //for (int i = 0; i < 100; i++) {
    //    ModelEntity* test = new ModelEntity(&backpackModel, glm::vec3(dist(gen), dist(gen), dist(gen)), 1.0f);
    //    if (i < 1) {
    //        test->setScale(2);
    //    }
    //    childTest.addChild(test);
    //}
    //childTest.setPosition(UFVec3(
    //    UFloat("999"),
    //    UFloat("0"),
    //    UFloat("0")
    //    ));
    //childTest.setScale(99);

    for (int i = 0; i <= 20; i++) {
        //i=0 
        ModelEntity* test = new ModelEntity(&backpackModel, glm::vec3(((i+1)/10.0f) * pow(10, i), 0.0f, 1*pow(10,i)), 1.0f);
        test->setScale(glm::length(uFVecToVec(test->getPosition())) * 0.1f);
        sceneGraphBase.addChild(test);
    }

    //for (ModelEntity* bp : bpEnts) {
    //    float d = 10.0f;
    //    //if (bp->getModel()->getMeshes().size() == spaceShuttleModel.getMeshes().size()) { d = 100.0; } //very scrappy, just for testing. Have way to check if same model?
    //    bp->setScale(glm::length(uFVecToVec(bp->getPosition())) * (1.0f / d));
    //}



    //DEMO SETUP
    //stars
    EntityGrouper* innerStars = new EntityGrouper();
    for (int i = 0; i < 100 * rand(gen) + 100; i++) {
        EntityGrouper* system = new EntityGrouper();
        UFloat baseMult = UFloat("9999999999");
        system->setPosition(UFVec3(
            UFloat::floatMult(baseMult, randn(gen)),
            UFloat::floatMult(baseMult, randn(gen)),
            UFloat::floatMult(baseMult, randn(gen))
        ));
        for (int j = 0; j < 20 * rand(gen) + 10; j++) {
            ModelEntity* planet = new ModelEntity(&backpackModel, glm::vec3(randn(gen)*100, randn(gen)*100, randn(gen)* 100), 1.0f);
            system->addChild(planet);
        }

        innerStars->addChild(system);
    }

    EntityGrouper* outerStars = new EntityGrouper();
    for (int i = 0; i < 100 * rand(gen) + 100; i++) {
        EntityGrouper* system = new EntityGrouper();
        UFloat baseMult = UFloat("999999999999999");
        system->setPosition(UFVec3(
            UFloat::floatMult(baseMult, randn(gen)),
            UFloat::floatMult(baseMult, randn(gen)),
            UFloat::floatMult(baseMult, randn(gen))
        ));
        for (int j = 0; j < 20 * rand(gen) + 10; j++) {
            ModelEntity* planet = new ModelEntity(&backpackModel, glm::vec3(randn(gen) * 100, randn(gen) * 100, randn(gen) * 100), 1.0f);
            system->addChild(planet);
        }

        outerStars->addChild(system);
    }

    sceneGraphBase.addChild(innerStars);
    sceneGraphBase.addChild(outerStars);

    EntityGrouper* system = new EntityGrouper();
    UFloat baseMult = UFloat("9999999999999999999");
    system->setPosition(UFVec3(
        UFloat::floatMult(baseMult, randn(gen)),
        UFloat::floatMult(baseMult, randn(gen)),
        UFloat::floatMult(baseMult, randn(gen))
    ));
    for (int j = 0; j < 10 * rand(gen) + 100; j++) {
        ModelEntity* planet = new ModelEntity(&backpackModel, glm::vec3(randn(gen) * 100, randn(gen) * 100, randn(gen) * 100), 1.0f);
        system->addChild(planet);
    }

    sceneGraphBase.addChild(system);

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
    double average = 0.0;
    double timer = 0.0;
    std::stringstream ss;
    std::stringstream frameString;
    frameString << "Frame Time: -, FPS: -";


    UFloat adjustedPos = UFloat();
    UFloat minusFive = UFloat("-5.0");
    UFloat one = UFloat("1"); //TODO: Add float addition and subtraction to UFLOAT, and maybe increment

    std::cout << "Starting Main Loop" << std::endl;

    while (!glfwWindowShouldClose(window)) { //main loop

        //mainCam.setPosition(glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius));
        //mainCam.lookAt(glm::vec3(0.0f, 0.0f, 0.0f)); //TODO: Add variants of setPosition and lookAt that don't use vec3

        //adjustedPos = UFloat::floatMult(zPos, moveStep);

        //backpackEntity.setPosition(UFVec3(0.0f, 0.0f, adjustedPos));
        //mainCam.setPosition(UFVec3(0.0f, 0.0f, UFloat::sum(adjustedPos, minusFive)));
        //backpackEntity.setScale(zPos * moveStep / 10);



        processInput(window);

        mainCam.updateRelativeViewMatrix();

        mainRenderer.renderSceneGraph(&sceneGraphBase);

        glfwSwapBuffers(window);
        glfwPollEvents();




        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        fps = 1 / deltaTime;
        lastTime = currentTime;
        timer += deltaTime;

        counter++;
        //zPos.add(one);

        average = (average * (counter - 1) + fps) / counter;
        if (timer >= 0.25) {
            frameString.str(std::string());
            frameString << "Frame Time: " << 1.0 / average << ", FPS: " << average;
            counter = 0;
            average = 0.0;
            timer = 0.0;
        }

        ss.str(std::string());
        ss << frameString.str();
        if (moveSpeed.integral.size() <= 35) {
            ss << ", MoveSpeed: " << UFloat::uFloatToFloat(moveSpeed) << ", Pos: [" << mainCam.getPosition().x.toString() << ", " << mainCam.getPosition().y.toString() << ", " << mainCam.getPosition().z.toString() << "] ";
        } else {
            ss << ", MoveSpeed: " << "BIG" << ", Pos: [" << mainCam.getPosition().x.toString() << ", " << mainCam.getPosition().y.toString() << ", " << mainCam.getPosition().z.toString() << "] ";
        }
        
        glfwSetWindowTitle(window, ss.str().c_str());


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
    mainCam.moveRelative(UFVec3(
        UFloat::floatMult(moveSpeed, movement.x),
        UFloat::floatMult(moveSpeed, movement.y),
        UFloat::floatMult(moveSpeed, movement.z)
        ));
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

    mainCam.turn(offsetX * sensitivity, offsetY * sensitivity, true); //TODO: Add sensitivity
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    moveSpeed = UFloat::floatMult(moveSpeed, pow(moveSpeedScrollMultFactor, yoffset));
    std::cout << yoffset << std::endl;
}