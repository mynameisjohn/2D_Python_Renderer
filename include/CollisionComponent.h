#pragma once

#include "Component.h"
#include "Util.h"
#include <vec2.hpp>

class C_Factory : public Factory<AABB>
{
	AABB m_Box;

public:
	C_Factory() :
		m_Box({ {0,0},{0,0}, {0,0} , 0 })
	{}
	// Easiest way from python gfx setup
	// Assumes coming from a unit square
	void setPos(float px, float py) {
		m_Box.C = vec2(px, py);
	}
	void setScale(float sx, float sy) {
		m_Box.R = vec2(sx, sy) / 2.f;
	}
	void setVel(float vx, float vy) {
		m_Box.V = vec2(vx, vy);
	}
	void setMass(float m) {
		m_Box.M = m;
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