#pragma once

#include "Component.h"
#include "Util.h"
#include <vec2.hpp>
#include <glm.hpp>
// we're starting simple,
// so this is just an AABB
struct AABB {
	vec2 C; // Center point of box
	vec2 R; // half widths along x, y
	float width() { return 2.f*R.x; }
	float height() { return 2.f*R.y; }
	float left() { return C.x - R.x; }
	float right() { return C.x + R.x; }
	float top() { return C.y + R.y; }
	float bottom() { return C.y - R.y; }
	float dX(AABB& other) {
		return fabs(other.C.x - C.x);
	}
	float dY(AABB& other) {
		return fabs(other.C.y - C.y);
	}
	float dist(AABB& other) {
		return glm::length(other.C - C);
	}
	bool overlaps(AABB& other) {
		if (dX(other) > other.R.x + R.x)
			return false;
		if (dY(other) > other.R.y + R.y)
			return false;
		return true;
	}
	void translate(vec2 d) {
		C += d;
	}
	// TODO rotate func, Ericson 86
};

class C_Factory : public Factory<AABB>
{
	AABB m_Box;

public:
	C_Factory() :
		m_Box({ {0,0},{0,0} })
	{}
	// Easiest way from python gfx setup
	// Assumes coming from a unit square
	void setState(float px, float py, float sx, float sy) {
		m_Box = { {px,py}, {sx / 2.f, sy / 2.f} };
	}
	virtual AABB GetData() {
		return m_Box;
	}

	// Python class registration
	static void PyRegister();
};

class C_Collection : public Collection<AABB>
{
public:
	C_Collection() {}
	// Just as a heads up, you can't construct python
	// objects with this pointer, since the vector
	// will relocate whether you like it or not
	virtual AABB * addComponent(C_Factory * f) {
		v_Components.push_back(f->GetData());
		return &v_Components.back();
	}

	// Python class registration
	static void PyRegister();
};