#pragma once

#include "Util.h"
#include <vec2.hpp>
#include <glm.hpp>
// we're starting simple,
// so this is just an AABB
struct AABB {
	AABB();
	vec2 C; // Center point of box
	vec2 R; // half widths along x, y
	vec2 V; // velocity

	// TODO consider moving mass, elasticity
	// and collide to base collision primitive
	float M;// mass
	float E;// elasticity

	float width();
	float height();
	float left();
	float right();
	float top(); 
	float bottom();
	float dX(AABB& other);
	float dY(AABB& other);
	float dist(AABB& other);
	bool overlaps(AABB& other);
	void advance(float dt = 0.001f);
	void translate(vec2 d);
	vec2 momentum();
	float kinetic();
	bool collide(AABB& other);
	// TODO rotate func, Ericson 86
};
