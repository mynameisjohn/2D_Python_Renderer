#pragma once

#include "CollisionComponent.h"

#include <pyliason.h>

/*static*/ void C_Factory::PyRegister() {
	Python::Register_Class<C_Factory>("CollisionFactory");

	std::function<void(C_Factory*, float, float, float, float)> cf_SetState(&C_Factory::setState);
	Python::_add_Func<__LINE__, C_Factory>("SetTrans", cf_SetState, METH_VARARGS,
		"Set collision factory's state");
}

/*static*/ void C_Collection::PyRegister() {
	Python::Register_Class<C_Collection>("CollisionCollection");

	std::function<void(C_Collection*, C_Factory *)> cc_AddComponent(&C_Collection::addComponent);
	Python::_add_Func<__LINE__, C_Collection>("SetTrans", cc_AddComponent, METH_VARARGS,
		"Add Collision Component");
}