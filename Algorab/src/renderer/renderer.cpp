#include "renderer.h"

void Renderer::renderAllPushed() {
    clearAllBuckets();

    //sort into buckets
    float dist;
    float min;
    float max;
    float furVertDist;
    unsigned int smallestBucket;
    unsigned int largestBucket;
    for (ModelEntity* modEnt : modelEntsToRender) {
        dist = glm::length(modEnt->getPosition());
        furVertDist = modEnt->getFurVertDist() * modEnt->getScale();
        min = dist - furVertDist;
        max = dist + furVertDist;

        if (min < 0) {
            smallestBucket = 0;
        } else {
            smallestBucket = std::floor(std::log(min / minimumCutOff) / std::log(bucketScale)); //change of base to bucketscale
        }
        largestBucket = std::floor(std::log(max / minimumCutOff) / std::log(bucketScale));
        //std::cout << "TEST: " << smallestBucket << ", " << largestBucket << std::endl;

        for (int i = smallestBucket; i <= largestBucket; i++) {
            if (buckets.count(i) == 0) { //if the bucket doesn't exist yet
                buckets.insert({ i, new std::vector<ModelEntity*> });
            }

            buckets[i]->push_back(modEnt);
        }

    }


    //from largest bucket to smallest
    //scale down and draw
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //TODO: Change to render to target then render target to screen
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    currentCamera->setNearPlane(1.0f); //TODO: clean up, camera should always have this near/far plane (or the renderer should control it)
    currentCamera->setFarPlane(1.0f * bucketScale);
    currentCamera->updateProjectionMatrix();

    texturedModelShader.use();
    viewMat = *(currentCamera->getRelativeViewMatrix());
    projMat = *(currentCamera->getProjectionMatrix());
    texturedModelShader.setMat4("view", viewMat);
    texturedModelShader.setMat4("projection", projMat);

    for (auto it = buckets.rbegin(); it != buckets.rend(); it++) {
        renderBucket(it->first, it->second);
    }





    drawCalls = 0;
    //glBindFramebuffer(GL_FRAMEBUFFER, target.getFrameBuffer());

    
    

    /*float plane = minimumCutOff;
    std::cout << "LOOP" << std::endl;
    for (int i = 0; i < 10; i++) {

        //std::cout << "TEST: ";
        currentCamera->setNearPlane(plane);
        //std::cout << plane << ", ";
        plane *= 100;
        currentCamera->setFarPlane(plane);
        //std::cout << plane << std::endl;
        currentCamera->updateProjectionMatrix();
        projMat = *(currentCamera->getProjectionMatrix());

        texturedModelShader.setMat4("projection", projMat);
        for (ModelEntity* modEnt : modelEntsToRender) {
            renderModelEntity(modEnt);
        }
    }*/
    modelEntsToRender = {};
    

    //renderAllPushed
    //sort all entities into bands
    // scale down bands (scale in vertex shader using a given matrix??)
    //split meshes in the future
    //render each band in reverse order
    // done?
    //

    /*for (PointEntity pEnt : pointsToRender) {
        renderPointEntity(pEnt);
    }*/
    pointsToRender = {};

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
#endif // DEBUG    
}

void Renderer::renderModelEntity(ModelEntity* modelEnt, float currentBucketScale) { //will need to change from float to unlimited number
#ifdef DEBUG
    std::cout << "renderModelEntity" << std::endl;
#endif // DEBUG

    int i = 0;

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, (modelEnt->getPosition()-currentCamera->getPosition()) * currentBucketScale); //camera relative world position
    modelMat = glm::scale(modelMat, glm::vec3(modelEnt->getScale() * currentBucketScale));

    texturedModelShader.setMat4("model", modelMat);

    for (Mesh modelMesh : *(modelEnt->getModel()->getMeshes())) {
        i++;
        //load textures and set uniforms
        std::vector<Texture> textures = *(modelMesh.getTextures());
        unsigned int diffuseNum = 1;

        //std::cout << textures.size() << std::endl;
        for (unsigned int i = 0; i < textures.size(); i++) {
            //std::cout << "LOG: SETTING TEXTURE UNIFORM" << std::endl;
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);

            std::string type = textures[i].type;
            std::string number;

            if (type == "texture_diffuse") {
                number = std::to_string(diffuseNum);
                diffuseNum += 1;
            } else {
                std::cout << "Error: Unsupported texture type: " << type << std::endl;
            }

            texturedModelShader.setInt(type + number, i);
        }

        //draw mesh
        //std::cout << i << std::endl;
        glBindVertexArray(modelMesh.getVertexArray());
        glDrawElements(GL_TRIANGLES, modelMesh.getIndices()->size(), GL_UNSIGNED_INT, 0);
        drawCalls++;
        //glBindVertexArray(0);
    }
}

void Renderer::renderPointEntity(PointEntity* pointEnt) {
    ;
}

void Renderer::renderBucket(unsigned int bucketNum, std::vector<ModelEntity*>* modEnts) {
    //work out scale for bucket
    //scale between 1 and 1*bucketscale?
    //same as renderModelEntity but transform/model matrix affected by bucket scale
    glClear(GL_DEPTH_BUFFER_BIT);
    float currentBucketScale = 1 / (minimumCutOff * (std::pow(bucketScale, bucketNum)));
    
    for (ModelEntity* modEnt : *modEnts) {
        renderModelEntity(modEnt, currentBucketScale);
    }
}

void Renderer::clearAllBuckets() { //clears all models pushed to buckets
    for (auto it = buckets.begin(); it != buckets.end(); it++) {
        it->second->clear();
    }
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

void Renderer::PushEntity(ModelEntity* modEnt) {
    modelEntsToRender.push_back(modEnt);
}

void Renderer::PushEntity(PointEntity* pEnt) {
    pointsToRender.push_back(pEnt);
}

Renderer::Renderer(Camera* cam, RenderTarget& tar) :
    currentCamera(cam), target(tar) {
    modelMat = glm::mat4(1.0f);
    viewMat = *(currentCamera->getViewMatrix());
    projMat = *(currentCamera->getProjectionMatrix());
}


