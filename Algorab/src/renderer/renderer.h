#pragma once
#include <glad/glad.h>
#include "../model/model.h"
#include "../camera/camera.h"
#include "../utility/shader.h"
#include "../entity/entity.h"
#include "render_target.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <math.h>

#include <glm/gtx/string_cast.hpp>




class Renderer {
private:
	unsigned int screenVAO, screenVBO;

	unsigned int drawCalls = 0;
	glm::vec3 posDiff;

	std::vector<unsigned int> loadedTextures; //stores id's of textures already loaded in

	std::map<unsigned int, RenderTarget> buckets;

	const float bucketScale = 100.0f; //scale between the near plane and the far plane in bucket
	double bucketScaleLog = std::log(bucketScale);
	const float minimumCutOff = 0.1f; //otherwise

	RenderTarget& target; //main target framebuffer
	Camera* currentCamera;
	std::vector<ModelEntity*> modelEntsToRender = {};
	std::vector<PointEntity*> pointsToRender = {};
	Shader screenShader = Shader("assets/shaders/screenShader.vs", "assets/shaders/screenShader.fs");;
	Shader texturedModelShader = Shader("assets/shaders/texturedModelRender.vs", "assets/shaders/texturedModelRender.fs");
	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projMat;

	void renderModelEntity(ModelEntity* modelEnd, unsigned int smallestBucket, unsigned int largestBucket);
	//void renderModelEntity(ModelEntity* modelEnt, float currentBucketScale = 1.0f);
	void renderPointEntity(PointEntity* pointEnt);
	void renderBucket(unsigned int bucket, std::vector<ModelEntity*>& modEnts);

	void clearAllBuckets();
public:
	void renderAllPushed();

	Camera* getCurrentCamera();
	RenderTarget& getTarget();

	void setCurrentCamera(Camera* cam);
	void setTarget(unsigned int tar);

	void PushEntity(ModelEntity& modEnt);
	void PushEntity(PointEntity& pEnt);

	Renderer(Camera* cam, RenderTarget& tar);
};

//render texture

