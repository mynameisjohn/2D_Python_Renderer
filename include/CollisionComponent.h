#pragma once

#include "Component.h"
#include "Util.h"
#include "AABB.h"
#include <vec2.hpp>

class C_Factory : public Factory<AABB>
{
	AABB m_Box;

public:
	C_Factory() {}
	// Easiest way from python gfx setup
	// Assumes coming from a unit square
	void setPos(float px, float py);
	void setScale(float sx, float sy);
	void setVel(float vx, float vy);
	void setMass(float m);
	virtual AABB GetData();

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
		if (!f) return nullptr;
		v_Components.push_back(f->GetData());
		return &v_Components.back();
	}

	// Python class registration
	static void PyRegister();
};