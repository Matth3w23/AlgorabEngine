#pragma once
#include <glad/glad.h>
#include "../model/model.h"
#include "../camera/camera.h"
#include "../utility/shader.h"
#include "../entity/entity.h"
#include "render_target.h"
#include "../utility/uFVec.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <map>

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtx/string_cast.hpp>


struct Point {
	glm::vec3* pos;
	float rad;

	Point(glm::vec3* p, float r) {
		pos = p;
		rad = r;
	}
};

class Renderer {
private:
	bool textureSetTest = false;
	unsigned int currentlySetTextureID;
	unsigned int lastTextureUnitBoundTo = 14; //max is 15/index 14 (15 reserved for screen texture)

	unsigned int pointVAO, pointVBO;
	std::vector<float> pointData; //[[pos(3)][size(1)][colour(3)]]

	unsigned int drawCalls = 0;
	glm::vec3 posDiff;

	std::vector<unsigned int> textureUnitCurrentIds; //stores id's of textures already loaded in

	std::map<unsigned int, std::pair<std::vector<ModelEntity*>, std::vector<Point>>> buckets;

	const float bucketScale = 10000.0f; //scale between the near plane and the far plane in bucket
	double bucketScaleLog = std::log(bucketScale);
	const float minimumCutOff = 0.1f; //otherwise

	RenderTarget& target; //target framebuffer
	Camera* currentCamera;
	std::vector<ModelEntity*> modelEntsToRender = {};
	std::vector<PointEntity*> pointsToRender = {};
	Shader screenShader = Shader("assets/shaders/screenShader.vs", "assets/shaders/screenShader.fs");;
	Shader texturedModelShader = Shader("assets/shaders/texturedModelRender.vs", "assets/shaders/texturedModelRender.fs");
	Shader pointShader = Shader("assets/shaders/pointShader.vs", "assets/shaders/pointShader.fs");
	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projMat;

	void scanGroup(EntityGrouper* entGroup, UFVec3 curAccPos, float curMulScale, bool fullyOnScreen, int bucket);

	void renderModelEntity(ModelEntity* modelEnt, float currentBucketScale = 1.0f);
	void renderPointEntity(PointEntity* pointEnt);
	void renderBucket(unsigned int bucket, std::pair<std::vector<ModelEntity*>, std::vector<Point>>& ents);

	void clearAllBuckets();
public:
	void renderSceneGraph(EntityGrouper* baseGroup);

	Camera* getCurrentCamera();
	RenderTarget& getTarget();

	void setCurrentCamera(Camera* cam);
	void setTarget(unsigned int tar);

	Renderer(Camera* cam, RenderTarget& tar);

};

//render texture