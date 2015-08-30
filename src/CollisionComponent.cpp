#pragma once

#include "CollisionComponent.h"

#include <pyliason.h>

void C_Factory::setPos(float px, float py) {
	m_Box.C = vec2(px, py);
}
void C_Factory::setScale(float sx, float sy) {
	m_Box.R = vec2(sx, sy) / 2.f;
}
void C_Factory::setVel(float vx, float vy) {
	m_Box.V = vec2(vx, vy);
}
void C_Factory::setMass(float m) {
	m_Box.M = m;
}
AABB C_Factory::GetData() {
	return m_Box;
}

/*static*/ void C_Factory::PyRegister() {
	Python::Register_Class<C_Factory>("CollisionFactory");

	std::function<void(C_Factory*, float, float)> cf_setPos(&C_Factory::setPos);
	Python::_add_Func<__LINE__, C_Factory>("SetPos", cf_setPos, METH_VARARGS,
		"Set collision factory's pos");

	std::function<void(C_Factory*, float, float)> cf_SetScale(&C_Factory::setScale);
	Python::_add_Func<__LINE__, C_Factory>("SetPos", cf_SetScale, METH_VARARGS,
		"Set collision factory's scale");

	std::function<void(C_Factory*, float, float)> cf_SetVel(&C_Factory::setVel);
	Python::_add_Func<__LINE__, C_Factory>("SetPos", cf_SetVel, METH_VARARGS,
		"Set collision factory's vel");

	std::function<void(C_Factory*, float)> cv_SetMass(&C_Factory::setMass);
	Python::_add_Func<__LINE__, C_Factory>("SetPos", cv_SetMass, METH_VARARGS,
		"Set collision factory's mass");
}

/*static*/ void C_Collection::PyRegister() {
	Python::Register_Class<C_Collection>("CollisionCollection");

	std::function<void(C_Collection*, C_Factory *)> cc_AddComponent(&C_Collection::addComponent);
	Python::_add_Func<__LINE__, C_Collection>("SetTrans", cc_AddComponent, METH_VARARGS,
		"Add Collision Component");
}