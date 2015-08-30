#pragma once

#include "Component.h"
#include "Util.h"
#include "AABB.h"
#include <vec2.hpp>

class CollisionFactory : public Factory<AABB>
{
	AABB m_Box;

public:
	CollisionFactory() {}
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

class CollisionCollection : public Collection<AABB>
{
public:
	CollisionCollection() {}
	// Just as a heads up, you can't construct python
	// objects with this pointer, since the vector
	// will relocate whether you like it or not
	virtual AABB * addComponent(CollisionFactory * f) {
		if (!f) return nullptr;
		v_Components.push_back(f->GetData());
		return &v_Components.back();
	}

	// Python class registration
	static void PyRegister();
};