#include "renderer.h"

void Renderer::renderAllPushed() {
    drawCalls = 0;
    modelDraws = 0;
    clearAllBuckets(); //Needs changing

    currentCamera->setNearPlane(1.0f); //TODO: clean up, camera should always have this near/far plane (or the renderer should control it)
    currentCamera->setFarPlane(1.0f * bucketScale);
    currentCamera->updateProjectionMatrix();

    viewMat = currentCamera->getRelativeViewMatrix();
    projMat = currentCamera->getProjectionMatrix();

    texturedModelShader.use();
    texturedModelShader.setMat4("view", viewMat);
    texturedModelShader.setMat4("projection", projMat);

    //get object buckets
    float dist;
    float min;
    float max;
    float furVertDist;
    unsigned int smallestBucket;
    unsigned int largestBucket;
    for (ModelEntity* modEnt : modelEntsToRender) {
        dist = (viewMat * glm::vec4(modEnt->getPosition() - currentCamera->getPosition(), 1.0f)).z; //TODO probably very inefficient, needs looking at
        furVertDist = modEnt->getFurVertDist() * modEnt->getScale();
        min = dist - furVertDist;
        max = dist + furVertDist;

        if (max < minimumCutOff) { //behind the camera
            continue;
        }

        if (min < minimumCutOff) {
            smallestBucket = 0;
        } else {
            smallestBucket = std::floor(std::log(min / minimumCutOff) / bucketScaleLog); //change of base to bucketscale, both should always be positive
        }
        largestBucket = std::floor(std::log(max / minimumCutOff) / bucketScaleLog);
        //std::cout << "TEST: " << smallestBucket << ", " << largestBucket << std::endl;

        /*for (int i = smallestBucket; i <= largestBucket; i++) {
            //std::cout << "BUCKET: " << i << std::endl;
            if (buckets.count(i) == 0) { //if the bucket doesn't exist yet
                buckets.insert({ i, RenderTarget(800, 600)});
                std::cout << i << std::endl;
            }
        }*/

        renderModelEntity(modEnt, smallestBucket, largestBucket);

    }
    modelEntsToRender = {};

    /*for (PointEntity pEnt : pointsToRender) {
       renderPointEntity(pEnt);
   }*/
    pointsToRender = {};

    //draw all bucket framebuffers/textures
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //TODO: Change to render to target then render target to screen
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    for (auto it = buckets.rbegin(); it != buckets.rend(); it++) {
        screenShader.use();
        glBindVertexArray(screenVAO);

        glBindTexture(GL_TEXTURE_2D, it->second.getTextureBuffer()); //TODO: Maybe only render texture if it has been rendered to this frame?
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //glBindFramebuffer(GL_FRAMEBUFFER, target.getFrameBuffer());

    //debugging stuff
    /*float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    unsigned int VAO;
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    Shader testShader("assets/shaders/testShader.vs", "assets/shaders/testShader.fs");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    testShader.use();
    glDrawArrays(GL_TRIANGLES, 0, 3);*/

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glClear(GL_COLOR_BUFFER_BIT);

    //screenShader.use();
    //TODO: SHADERS
    //bind targets vertex array
    //disable depth test
    //bind current texture to texture colour buffer

#ifdef _DEBUG
    std::cout << "Draw Calls: " << drawCalls << std::endl;
    //std::cout << "Model Draws: " << modelDraws << std::endl;
    //std::cout << "------------" << std::endl;
#endif // DEBUG    
}


void Renderer::renderModelEntity(ModelEntity* modelEnt, unsigned int smallestBucket, unsigned int largestBucket) {
    //std::cout << "MODEL" << std::endl;
    modelDraws++;

    float currentBucketScale = 0.0f;
    modelMatrices = {};

    modelMatGeneral = glm::mat4(1.0f);
    scales = {};
    modelMatGeneral = glm::translate(modelMatGeneral, (modelEnt->getPosition() - currentCamera->getPosition())); //camera relative world position
    modelMatGeneral = glm::scale(modelMatGeneral, glm::vec3(modelEnt->getScale()));

    texturedModelShader.setMat4("model", modelMatGeneral);

    for (unsigned int bNum = smallestBucket; bNum <= largestBucket; bNum++) {
        currentBucketScale = 1 / (minimumCutOff * (std::pow(bucketScale, bNum)));
        scales.push_back(currentBucketScale);
    }


    //load model textures
    std::string number;
    for (Mesh& modelMesh : modelEnt->getModel()->getMeshes()) {

        //load textures and set uniforms
        //std::vector<Texture>& textures = modelMesh.getTextures();
        std::vector<Texture> textures = {};
        unsigned int diffuseNum = 1;


        //std::cout << textures.size() << std::endl;
        for (unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);

            std::string& type = textures[i].type;
            number = "";

            if (type == "texture_diffuse") {
                number = std::to_string(diffuseNum);
                diffuseNum += 1;
            } else {
                std::cout << "Error: Unsupported texture type: " << type << std::endl;
            }

            texturedModelShader.setUInt(type + number, i);
        }

        glBindVertexArray(modelMesh.getVertexArray());
        for (unsigned int bNum = smallestBucket; bNum <= largestBucket; bNum++) {
            //std::cout << "BUCKETNUM: " << bNum << ", Scale: " << scales[bNum - smallestBucket] << std::endl;
            //std::cout << "ModelMat: " << glm::to_string(modelMatGeneral);
            texturedModelShader.setFloat("scale", scales[bNum - smallestBucket]);
            glBindFramebuffer(GL_FRAMEBUFFER, buckets[bNum].getFrameBuffer());
            //drawCalls += buckets[bNum].getFrameBuffer();
            //glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDrawElements(GL_TRIANGLES, modelMesh.getIndices().size(), GL_UNSIGNED_INT, 0);
            drawCalls++;
            //std::cout << "DRAWN" << std::endl;
        }
    }


    //render the entity to each bucket
}

void Renderer::clearAllBuckets() { //clears all models pushed to buckets
    for (auto it = buckets.begin(); it != buckets.end(); it++) {
        glBindFramebuffer(GL_FRAMEBUFFER, it->second.getFrameBuffer());
        glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Renderer::renderPointEntity(PointEntity* pointEnt) {
    ;
}

Camera* Renderer::getCurrentCamera() {
    return currentCamera;
}

RenderTarget& Renderer::getTarget() {
    return target;
}

void Renderer::setCurrentCamera(Camera* cam) {
    currentCamera = cam;
}

void Renderer::setTarget(unsigned int tar) {
    target = tar;
}

void Renderer::PushEntity(ModelEntity& modEnt) {
    modelEntsToRender.push_back(&modEnt);
}

void Renderer::PushEntity(PointEntity& pEnt) {
    pointsToRender.push_back(&pEnt);
}

Renderer::Renderer(Camera* cam, RenderTarget& tar) :
    currentCamera(cam), target(tar) {
    modelMat = glm::mat4(1.0f);
    viewMat = currentCamera->getViewMatrix();
    projMat = currentCamera->getProjectionMatrix();

    float screenTextureVertices[] = { //for rendering to a screen texture
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &screenVBO);
    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenTextureVertices), &screenTextureVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



    for (int i = 0; i <= 16; i++) {
        //std::cout << "BUCKET: " << i << std::endl;
        if (buckets.count(i) == 0) { //if the bucket doesn't exist yet
            buckets.insert({ i, RenderTarget(800, 600, true) });
            std::cout << i << std::endl;
        }
    }
}


