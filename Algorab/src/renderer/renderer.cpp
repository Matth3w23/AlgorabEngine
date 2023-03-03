#include "renderer.h"

//float pointAngle = M_PI*(0.1f/180.0f);
//float disappearAngle = M_PI * (0.01f / 180.0f); //TODO: Implement these differently
float pointPix = 10;
float disPix = -1;
float pointAngle = atan((pointPix * 2 * tan(M_PI * (0.5 * 60 / 180)))/800);
float disappearAngle = atan((disPix * 2 * tan(M_PI * (0.5 * 60 / 180))) / 800);
//float disappearAngle = -0.1;
float pointMinRadius = 5;

//WINDOW IS CURRENTLY HARD SET AT 800*600, IN FUTURE USE TARGET STUFF

void Renderer::renderSceneGraph(EntityGrouper baseGroup) {

    //iterate over tree, checking if children to render
    //if find marked child, push to model ents to render
    //should integrate buckets into this


    drawCalls = 0;
    clearAllBuckets();
    textureSetTest = false;
    currentlySetTextureID = -1; //currently just assuming I won't have loads of textures really

    viewMat = currentCamera->getRelativeViewMatrix();
    projMat = currentCamera->getProjectionMatrix();

    //retrieve
    scanGroup(&baseGroup, UFVec3(0), 1.0f, false, -1);

    //from largest bucket to smallest
    //scale down and draw
    glBindFramebuffer(GL_FRAMEBUFFER, target.getFrameBuffer()); //TODO: Change to render to target then render target to screen
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    currentCamera->setNearPlane(1.0f); //TODO: clean up, camera should always have this near/far plane (or the renderer should control it)
    currentCamera->setFarPlane(1.1f * bucketScale);
    currentCamera->updateProjectionMatrix();

    pointShader.use();
    pointShader.setMat4("view", viewMat);
    pointShader.setMat4("projection", projMat); //uniforms are persistent

    texturedModelShader.use();
    texturedModelShader.setMat4("view", viewMat);
    texturedModelShader.setMat4("projection", projMat); //uniforms are persistent

    for (auto it = buckets.rbegin(); it != buckets.rend(); it++) {
        renderBucket(it->first, it->second);
    }

    //draw buffer texture to screen
    glBindFramebuffer(GL_READ_FRAMEBUFFER, target.getFrameBuffer());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST); //blit to default frame buffer, resolving multisampling

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glClearColor(0.1f, 0.5f, 0.1f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    //screenShader.use();j
    //screenShader.setInt("screenTexture", 15);
    //glBindVertexArray(target.getScreenVAO());
    //glDisable(GL_DEPTH_TEST);
    //glActiveTexture(GL_TEXTURE0 + 15);
    //glBindTexture(GL_TEXTURE_2D, target.getTextureBuffer());
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    //glEnable(GL_DEPTH_TEST);

    
    

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

    /*for (PointEntity pEnt : pointsToRender) {
        renderPointEntity(pEnt);
    }*/
    pointsToRender = {};

    //float vertices[] = {
    //        0.5f, -0.5f, 0.0f,
    //        -0.5f, -0.5f, 0.0f,
    //        0.0f,  0.5f, 0.0f
    //};

    //glClear(GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT);
    //unsigned int VAO2;
    //unsigned int VBO2;
    //glGenBuffers(1, &VBO2);
    //glGenVertexArrays(1, &VAO2);
    //glBindVertexArray(VAO2);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Shader testShader("assets/shaders/testShader.vs", "assets/shaders/testShader.fs");
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //testShader.use();
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glClear(GL_COLOR_BUFFER_BIT);

    //screenShader.use();
    //TODO: SHADERS
    //bind targets vertex array
    //disable depth test
    //bind current texture to texture colour buffer

#ifdef _DEBUG
    std::cout << "Draw Calls: " << drawCalls << std::endl;
    std::cout << "------------" << std::endl;
#endif // DEBUG    
}

void Renderer::scanGroup(EntityGrouper* entGroup, UFVec3 curAccPos, float curMulScale, bool fullyOnScreen, int bucket) {
    //check if group has child to be rendered
    //check if group on screen
    //calculate min/max buckets

    //push models to !renderer/buckets
    //remove all child to be rendered tags

    
    bool foScreen = false;
    int bckt = -1;

    float curGroupScale = entGroup->getScale() * curMulScale;
    UFVec3 currentGroupPos = uFVecSum(entGroup->getPosition(), curAccPos);
    
    

    if (entGroup->getHidden()) {
        return;
    }
    
    glm::vec3 posDif = uFVecToVec(uFVecSub(currentGroupPos, currentCamera->getPosition()));
    glm::vec4 relCamPos = viewMat *glm::vec4(posDif, 1.0f);
    entGroup->posDif = posDif;
    entGroup->relCamPos = relCamPos;
    float distance = glm::length(posDif);

    //ENTITY GROUP
    if (!fullyOnScreen) {
        //frustum stuff
        //if not on screen stop
        //if half continue
        //if fully foScreen = true
    }

    //check if render as single point
    float zDist;
    float min;
    float max;
    float furVertDist;

    zDist = relCamPos.z;
    float entScaledFurDist = entGroup->getFurthestDistance() * curGroupScale;
    double angle = std::atan(entScaledFurDist / distance);
    //std::cout << "ANGLE: " << angle << std::endl;
    //std::cout << "POSD: " << posDif.x << "," << posDif.y << "," << posDif.z << std::endl;
    //std::cout << "L: " << distance << std::endl;
    //std::cout << "---------------------------------" << std::endl;

    if (angle < disappearAngle) {
        //don't render
        return;
    } else if (angle < pointAngle) { //currently need to check if the group has content, right now I assume it has a model child somewhere
        float pointRadius = (800 * tan(angle)) / (2 * tan(M_PI * (0.5 * 60 / 180)));
        //*2 and pointMinRad considered later on

        float bucket = std::floor(std::log(zDist / minimumCutOff) / bucketScaleLog);

        if (buckets.count(bucket) == 0) { //if the bucket doesn't exist yet
            buckets.insert({ bucket, std::pair<
                std::vector<ModelEntity*>,
                std::vector<Point>
            >() });
        }

        Point p = Point(&entGroup->posDif, pointRadius);
        

        buckets[bucket].second.push_back(p);

        //don't draw further
        return;
    }
    



    unsigned int smallestBucket;
    unsigned int largestBucket;
    if (bucket == -1) { //not one bucket yet
        
        min = zDist - entScaledFurDist;
        max = zDist + entScaledFurDist;
        if (max < minimumCutOff) { //behind the camera
            return;
        }

        if (min < minimumCutOff) {
            smallestBucket = 0;
        } else {
            smallestBucket = std::floor(std::log(min / minimumCutOff) / bucketScaleLog); //change of base to bucketscale, both should always be positive
        }
        largestBucket = std::floor(std::log(max / minimumCutOff) / bucketScaleLog);

        if (smallestBucket == largestBucket) {
            bckt = smallestBucket;
        }
    } else {
        bckt = bucket;
    }


    //CHILD MODELS
    for (ModelEntity* modEnt : *entGroup->getChildModels()) {
        //std::cout << "RENDERMODENT" << std::endl;
        float modelTrueScale = modEnt->getScale() * curGroupScale;
        modEnt->relScale = modelTrueScale;
        modEnt->posDif = uFVecToVec(uFVecSub(
            uFVecSum(modEnt->getPosition(), currentGroupPos),
            currentCamera->getPosition()
        ));
        modEnt->relCamPos = viewMat * glm::vec4(modEnt->posDif, 1.0f);
        zDist = modEnt->relCamPos.z;
        furVertDist = modEnt->getFurVertDist() * modelTrueScale;
        //TODO: check if in view
        if (!foScreen) {
            ;
        }

        //assign to bucket
        //check if as point?
        min = zDist - furVertDist;
        max = zDist + furVertDist;
        if (max < minimumCutOff) { //behind the camera near plane
            continue;
        }
        if (min < minimumCutOff) {
            smallestBucket = 0;
        } else {
            smallestBucket = std::floor(std::log(min / minimumCutOff) / bucketScaleLog); //change of base to bucketscale, both should always be positive
        }
        largestBucket = std::floor(std::log(max / minimumCutOff) / bucketScaleLog);
        //std::cout << "TEST: " << smallestBucket << ", " << largestBucket << std::endl;

        for (int i = smallestBucket; i <= largestBucket; i++) {
            //std::cout << "BUCKET: " << i << std::endl;
            if (buckets.count(i) == 0) { //if the bucket doesn't exist yet
                buckets.insert({ i, std::pair<
                    std::vector<ModelEntity*>,
                    std::vector<Point>
                >()});
            }

            buckets[i].first.push_back(modEnt);
        }
    }

    //CHILD GROUPS
    for (EntityGrouper* entGrp : *entGroup->getChildGroups()) {
        scanGroup(entGrp, currentGroupPos, curGroupScale, foScreen, bckt);
    }
}


void Renderer::renderBucket(unsigned int bucketNum, std::pair<std::vector<ModelEntity*>, std::vector<Point>>& ents) {
    //work out scale for bucket
    //scale between 1 and 1*bucketscale?
    //same as renderModelEntity but transform/model matrix affected by bucket scale
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    float currentBucketScale = 1 / (minimumCutOff * (std::pow(bucketScale, bucketNum)));

    pointData.clear();

    for (ModelEntity* modEnt : ents.first) {
        renderModelEntity(modEnt, currentBucketScale);
    }

    for (Point p : ents.second) {
        glm::vec3 pPos = *p.pos * currentBucketScale;
        pointData.insert(pointData.end(), { pPos.x, pPos.y, pPos.z });
        pointData.push_back(std::max(p.rad * 2, pointMinRadius));
        pointData.insert(pointData.end(), { 0.2, 0.3, 0.6 }); //TODO: Colour
    }

    //std::cout << "TEST";

    if (pointData.empty() == false) {
        //Shader testShader("assets/shaders/testShader.vs", "assets/shaders/testShader.fs");
        //testShader.use();
        pointShader.use();
        glBindVertexArray(pointVAO);
        glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
        //glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*pointData.size(), &pointData[0], GL_STATIC_DRAW);
        //std::cout << "PAIN" << std::endl;
        ////std::cout << sizeof(float) * pointData.size() << std::endl;
        ////std::cout << sizeof(test) << std::endl;
        //for (auto it = pointData.begin(); it != pointData.end(); it++) {
        //    std::cout << *it << ", ";
        //}
        //std::cout << std::endl;
        glDrawArrays(GL_POINTS, 0, pointData.size()/7);

        texturedModelShader.use();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    //std::cout << "END" << std::endl;
    
}


void Renderer::renderModelEntity(ModelEntity* modelEnt, float currentBucketScale) { //will need to change from float to unlimited number?
#ifdef DEBUG
    std::cout << "renderModelEntity" << std::endl;
#endif // DEBUG

    int i = 0;
    std::string number;

    modelMat = glm::mat4(1.0f);
    posDiff = modelEnt->posDif;
    //std::cout << "[" << posDiff.x << "," << posDiff.y << "," << posDiff.z << "]" << std::endl;

    //work out whether to render as point or not render at all
    float radius = modelEnt->getFurVertDist()*modelEnt->relScale;
    float distance = glm::length(posDiff);
    double angle = std::atan(radius / distance);
    //std::cout << angle << std::endl;
    //std::cout << (angle / M_PI) * 180 << std::endl;
    //std::cout << disappearAngle << "," << pointAngle << std::endl;
    //std::cout << "----" << std::endl;

    if (angle < disappearAngle) {
        //just don't render
        ;
    } else if (angle < pointAngle) {
        posDiff *= currentBucketScale;
        pointData.insert(pointData.end(), { posDiff.x, posDiff.y, posDiff.z });
        float pointRadius = (800 * tan(angle)) / (2 * tan(M_PI * (0.5 * 60 / 180)));
        //std::cout << "POINTRADIUS: " << pointRadius << ", " << std::max(pointRadius * 2, pointMinRadius) << std::endl;
        pointData.push_back(std::max(pointRadius * 2, pointMinRadius));
        //pointData.push_back(((180*angle/M_PI)/currentCamera->getFov())*800*2); //Isn't quite correct but good approximation
        pointData.insert(pointData.end(), { 0.2, 0.3, 0.6 }); //TODO: Colour
        

        //render all points at once
        //create array/vertex of points (position, size, colour)
        //change shader program, pass all
    } else {

        modelMat = glm::translate(modelMat, (posDiff)*currentBucketScale); //camera relative world position
        modelMat = glm::scale(modelMat, glm::vec3(modelEnt->relScale * currentBucketScale));
        texturedModelShader.setMat4("model", modelMat);

        //TODO: Work out/store model matrices separately, send scale to vertex shader at start, send model matrix every time (is this more efficient?)
        for (Mesh& modelMesh : modelEnt->getModel()->getMeshes()) {
            std::vector<Texture>& textures = modelMesh.getTextures();

            for (unsigned int i = 0; i < textures.size(); i++) { //for every texture
                //std::cout << "LOG: SETTING TEXTURE UNIFORM" << std::endl;

                std::string& type = textures[i].type;
                number = "1"; //only one diffuse map fopr now

                if (type != "texture_diffuse") { //check the texture is a diffuse map
                    std::cout << "Error: Unsupported texture type: " << type << std::endl;
                }

                unsigned int texID = textures[i].id;

                if (texID != currentlySetTextureID) {
                    //TODO: Set up texture cache

                    bool textureBound = false;
                    for (unsigned int j = 0; j < textureUnitCurrentIds.size(); j++) {
                        if (textureUnitCurrentIds[j] == texID) {
                            texturedModelShader.setUInt(type + number, j);

                            textureBound = true;
                        }
                    }

                    if (!textureBound) {
                        lastTextureUnitBoundTo = (lastTextureUnitBoundTo + 1) % 15; 
                        glActiveTexture(GL_TEXTURE0 + lastTextureUnitBoundTo);
                        glBindTexture(GL_TEXTURE_2D, texID);

                        if (textureUnitCurrentIds.size() < lastTextureUnitBoundTo + 1) {
                            textureUnitCurrentIds.push_back(texID);
                        } else {
                            textureUnitCurrentIds[lastTextureUnitBoundTo] = texID;
                        }

                    }


                    currentlySetTextureID = texID;


                }

                glBindVertexArray(modelMesh.getVertexArray()); //TODO: Create a vertex array of all meshes
                glDrawElements(GL_TRIANGLES, modelMesh.getIndices().size(), GL_UNSIGNED_INT, 0);
                drawCalls++;

            }

        }
    }
        //std::cout << i << std::endl;
        
        //glBindVertexArray(0);
}

void Renderer::renderPointEntity(PointEntity* pointEnt) {
    ;
}



void Renderer::clearAllBuckets() { //clears all models pushed to buckets
    for (auto it = buckets.begin(); it != buckets.end(); it++) {
        it->second.first.clear();
        it->second.second.clear();
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

Renderer::Renderer(Camera* cam, RenderTarget& tar) :
    currentCamera(cam), target(tar) {
    modelMat = glm::mat4(1.0f);
    viewMat = currentCamera->getRelativeViewMatrix();
    projMat = currentCamera->getProjectionMatrix();

    //setup point vertex buffer
    glGenVertexArrays(1, &pointVAO);
    glGenBuffers(1, &pointVBO);

    glBindVertexArray(pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);

   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //position
    
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); //size
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2); //colour
}


