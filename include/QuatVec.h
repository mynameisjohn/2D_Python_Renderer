#ifndef QUATVEC_H
#define QUATVEC_H

#include <gtc/quaternion.hpp>
#include <vec3.hpp>

#include "Util.h"

class quatvec {
public:
	enum Type {
		TR,
		RT,
		TRT,
		NIL
	};
	quatvec(vec3 v = vec3(0), fquat q = fquat(0,0,0,0), Type t = NIL);
	quatvec blend(const quatvec& other, float a);
	quatvec inverse();
	mat4 toMat4();
private:
	vec3 T;
	fquat R;
	Type type;
};

#endif