#include "QuatVec.h"

quatvec::quatvec(vec3 v, fquat q, quatvec::Type t) :
	T(v), R(glm::normalize(q)), type(t) {}

quatvec quatvec::blend(const quatvec& other, float a) {
	if (type != other.type) {
		std::cout << "Error: Incompatible quatvec types being blended" << std::endl;
		return *this;
	}

	const float EPS = 0.0001f;
	vec3 T = glm::mix(T, other.T, a);
	fquat c(other.R);

	//cout << this->rot << "\t" << this->trans << "\n" << other.rot << "\t" << other.trans << endl;

	float cosTheta = glm::dot(R, c);

	if (cosTheta < 0) {
		c = -c;
		cosTheta = -cosTheta;
	}
	if (cosTheta >(1 - EPS)) {//if the interpolation angle is very small, just lerp
		return quatvec(T, fquat(
			glm::mix(R.w, c.w, a),
			glm::mix(R.x, c.x, a),
			glm::mix(R.y, c.y, a),
			glm::mix(R.z, c.z, a)
			), type);
	}

	// Essential Mathematics, page 467
	float angle = acos(cosTheta);
	return quatvec(T, (R*(float)sin(angle*(1 - a)) + c*(float)sin(a*angle)) / (float)sin(angle), type);
}

quatvec quatvec::inverse() {
	return quatvec(-T, glm::inverse(R), type);
}

#include <gtx/transform.hpp>
mat4 quatvec::toMat4() {
	switch (type) {
	case TR:
		return glm::translate(T)*glm::mat4_cast(R);
	case RT:
		return glm::mat4_cast(R)*glm::translate(T);
	case TRT:
		return glm::translate(T)*glm::mat4_cast(R)*glm::translate(-T);
	}
	return mat4(1);
}