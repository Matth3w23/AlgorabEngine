#pragma once
#include "unlimited_float.h"
#include <glm/glm.hpp>


class UFVec3 {
private:
	;
public:
	UFloat x;
	UFloat y;
	UFloat z;

	UFVec3(float a, float b = 0, float c = 0);
	UFVec3(UFloat a = UFloat(), UFloat b = UFloat(), UFloat c = UFloat());
	UFVec3(glm::vec3 vec);

	void add(UFVec3 a);
	void sub(UFVec3 a);

	//void add(glm::vec3);
	//void sub(glm::vec3);
};

class UFVec4 {
private:
	;
public:
	UFloat x;
	UFloat y;
	UFloat z;
	UFloat w;

	UFVec4(float a, float b = 0, float c = 0, float d = 0);
	UFVec4(UFloat a = UFloat(), UFloat b = UFloat(), UFloat c = UFloat(), UFloat d = UFloat());
	UFVec4(glm::vec4 vec);

	void add(UFVec4 a);
	void sub(UFVec4 a);
};

UFVec3 uFVecSum(UFVec3 a, UFVec3 b);
UFVec4 uFVecSum(UFVec4 a, UFVec4 b);
UFVec3 uFVecSub(UFVec3 a, UFVec3 b);
UFVec4 uFVecSub(UFVec4 a, UFVec4 b);

UFVec3 uFVecFloatMult(UFVec3 a, float b);
UFVec4 uFVecFloatMult(UFVec4 a, float b);

glm::vec3 uFVecToVec(UFVec3 uVec);
glm::vec4 uFVecToVec(UFVec4 uVec);
