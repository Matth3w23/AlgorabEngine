#include "uFVec.h"

UFVec3::UFVec3(float a, float b, float c) {
	x = UFloat(a);
	y = UFloat(b);
	z = UFloat(c);
}

UFVec3::UFVec3(UFloat a, UFloat b, UFloat c) { //not by reference
	x = a;
	y = b;
	z = c;
}

UFVec3::UFVec3(glm::vec3 vec) {
	x = UFloat(vec.x);
	y = UFloat(vec.y);
	z = UFloat(vec.z);
}

void UFVec3::add(UFVec3 a) {
	x.add(a.x);
	y.add(a.y);
	z.add(a.z);
}

void UFVec3::sub(UFVec3 a) {
	x.sub(a.x);
	y.sub(a.y);
	z.sub(a.z);
}

UFVec4::UFVec4(float a, float b, float c, float d) {
	x = UFloat(a);
	y = UFloat(b);
	z = UFloat(c);
	w = UFloat(d);
}

UFVec4::UFVec4(UFloat a, UFloat b, UFloat c, UFloat d) {
	x = a;
	y = b;
	z = c;
	w = d;
}

UFVec4::UFVec4(glm::vec4 vec) {
	x = UFloat(vec.x);
	y = UFloat(vec.y);
	z = UFloat(vec.z);
	w = UFloat(vec.w);
}

void UFVec4::add(UFVec4 a) {
	x.add(a.x);
	y.add(a.y);
	z.add(a.z);
	w.add(a.w);
}

void UFVec4::sub(UFVec4 a) {
	x.sub(a.x);
	y.sub(a.y);
	z.sub(a.z);
	w.sub(a.w);
}

UFVec3 uFVecSum(UFVec3 a, UFVec3 b) {
	UFVec3 temp = UFVec3();
	temp.add(a);
	temp.add(b);
	return temp;

}

UFVec4 uFVecSum(UFVec4 a, UFVec4 b) {
	UFVec4 temp = UFVec4();
	temp.add(a);
	temp.add(b);
	return temp;
}

UFVec3 uFVecSub(UFVec3 a, UFVec3 b) {
	UFVec3 temp = UFVec3();
	temp.add(a);
	temp.sub(b);
	return temp;
}

UFVec4 uFVecSub(UFVec4 a, UFVec4 b) {
	UFVec4 temp = UFVec4();
	temp.add(a);
	temp.sub(b);
	return temp;
}

glm::vec3 uFVecToVec(UFVec3 uVec) {
	glm::vec3 temp = glm::vec3(
		UFloat::uFloatToFloat(uVec.x),
		UFloat::uFloatToFloat(uVec.y),
		UFloat::uFloatToFloat(uVec.z));
	return temp;

}

glm::vec4 uFVecToVec(UFVec4 uVec) {
	glm::vec4 temp = glm::vec4(
		UFloat::uFloatToFloat(uVec.x),
		UFloat::uFloatToFloat(uVec.y),
		UFloat::uFloatToFloat(uVec.z),
		UFloat::uFloatToFloat(uVec.w));
	return temp;
}
