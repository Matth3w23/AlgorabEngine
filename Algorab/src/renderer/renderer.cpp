#include "renderer.h"

void Renderer::renderAllPushed() {
    glBindFramebuffer(GL_FRAMEBUFFER, target.getFrameBuffer());
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //TODO: Change to render to target then render target to screen
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    viewMat = currentCamera->getViewMatrix();
    projMat = currentCamera->getProjectionMatrix();

    texturedModelShader.use();
    texturedModelShader.setMat4("view", viewMat);
    texturedModelShader.setMat4("projection", projMat);
    for (ModelEntity modEnt : modelsToRender) {
        renderModelEntity(modEnt);
    }
    modelsToRender = {};

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
}

void Renderer::renderModelEntity(ModelEntity modelEnt) {
    //std::cout << "renderModelEntity" << std::endl;
    int i = 0;

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, modelEnt.getPosition());
    modelMat = glm::scale(modelMat, glm::vec3(modelEnt.getScale()));

    texturedModelShader.setMat4("model", modelMat);

    for (Mesh modelMesh : modelEnt.getModel().getMeshes()) {
        i++;
        //load textures and set uniforms
        std::vector<Texture> textures = modelMesh.getTextures();
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
        glDrawElements(GL_TRIANGLES, modelMesh.getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Renderer::renderPointEntity(PointEntity pointEnt) {
    ;
}

Camera* Renderer::getCurrentCamera() {
    return currentCamera;
}

RenderTarget Renderer::getTarget() {
    return target;
}

void Renderer::setCurrentCamera(Camera* cam) {
    currentCamera = cam;
}

void Renderer::setTarget(unsigned int tar) {
    target = tar;
}

void Renderer::PushEntity(ModelEntity modEnt) {
    modelsToRender.push_back(modEnt);
}

void Renderer::PushEntity(PointEntity pEnt) {
    pointsToRender.push_back(pEnt);
}

Renderer::Renderer(Camera* cam, RenderTarget tar) :
    currentCamera(cam), target(tar) {
    ;
}


