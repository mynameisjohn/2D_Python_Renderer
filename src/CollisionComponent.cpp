#pragma once

#include "CollisionComponent.h"

#include <pyliason.h>

void CollisionFactory::setPos(float px, float py) {
	m_Box.C = vec2(px, py);
}
void CollisionFactory::setScale(float sx, float sy) {
	m_Box.R = vec2(sx, sy) / 2.f;
}
void CollisionFactory::setVel(float vx, float vy) {
	m_Box.V = vec2(vx, vy);
}
void CollisionFactory::setMass(float m) {
	m_Box.M = m;
}
AABB CollisionFactory::GetData() {
	return m_Box;
}

/*static*/ void CollisionFactory::PyRegister() {
	Python::Register_Class<CollisionFactory>("CollisionFactory");

	std::function<void(CollisionFactory*, float, float)> cf_setPos(&CollisionFactory::setPos);
	Python::_add_Func<__LINE__, CollisionFactory>("SetPos", cf_setPos, METH_VARARGS,
		"Set collision factory's pos");

	std::function<void(CollisionFactory*, float, float)> cf_SetScale(&CollisionFactory::setScale);
	Python::_add_Func<__LINE__, CollisionFactory>("SetPos", cf_SetScale, METH_VARARGS,
		"Set collision factory's scale");

	std::function<void(CollisionFactory*, float, float)> cf_SetVel(&CollisionFactory::setVel);
	Python::_add_Func<__LINE__, CollisionFactory>("SetPos", cf_SetVel, METH_VARARGS,
		"Set collision factory's vel");

	std::function<void(CollisionFactory*, float)> cv_SetMass(&CollisionFactory::setMass);
	Python::_add_Func<__LINE__, CollisionFactory>("SetPos", cv_SetMass, METH_VARARGS,
		"Set collision factory's mass");
}

/*static*/ void CollisionCollection::PyRegister() {
	Python::Register_Class<CollisionCollection>("CollisionCollection");

	std::function<void(CollisionCollection*, CollisionFactory *)> cc_AddComponent(&CollisionCollection::addComponent);
	Python::_add_Func<__LINE__, CollisionCollection>("SetTrans", cc_AddComponent, METH_VARARGS,
		"Add Collision Component");
}