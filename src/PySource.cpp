#include "GraphicsComponent.h"
#include "CollisionComponent.h"
#include "Camera.h"
#include "Shader.h"

#include <pyliason.h>

/*static*/ void Shader::PyRegister() {
	Python::Register_Class<Shader, __LINE__>("Shader");

	//std::function<int(Shader *)> shaderC_L(&Shader::CompileAndLink);
	//Python::_add_Func<__LINE__, Shader>("CompileAndLink", shaderC_L, METH_VARARGS,
		//"Compile and Link a Shader Program");

	std::function<void(Shader *, std::string)> shader_vSrc(&Shader::SetSourceFile_V);
	Python::_add_Func<__LINE__, Shader>("SetSourceFile_V", shader_vSrc, METH_VARARGS,
		"Set vshader src");

	std::function<void(Shader *, std::string)> shader_fSrc(&Shader::SetSourceFile_F);
	Python::_add_Func<__LINE__, Shader>("SetSourceFile_F", shader_fSrc, METH_VARARGS,
		"set fshader src");
}


/*static*/ void CollisionFactory::PyRegister() {
	Python::Register_Class<CollisionFactory, __LINE__>("CollisionFactory");

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
	Python::Register_Class<CollisionCollection,__LINE__>("CollisionCollection");

	std::function<void(CollisionCollection*, CollisionFactory *)> cc_AddComponent(&CollisionCollection::addComponent);
	Python::_add_Func<__LINE__, CollisionCollection>("SetTrans", cc_AddComponent, METH_VARARGS,
		"Add Collision Component");
}


/*static*/ void GraphicsFactory::PyRegister() {
	Python::Register_Class<GraphicsFactory, __LINE__>("GraphicsFactory");

	// All this shit
	std::function<void(GraphicsFactory*, float, float, float)> gf_setTrans(&GraphicsFactory::setTrans);
	Python::_add_Func<__LINE__, GraphicsFactory>("SetTrans", gf_setTrans, METH_VARARGS,
		"Set factory's translation state");

	std::function<void(GraphicsFactory*, float, float, float)> gf_setScale(&GraphicsFactory::setScale);
	Python::_add_Func<__LINE__, GraphicsFactory>("SetScale", gf_setScale, METH_VARARGS,
		"Set factory's scale state");

	std::function<void(GraphicsFactory*, float, float, float, float)> gf_setRot(&GraphicsFactory::setRot);
	Python::_add_Func<__LINE__, GraphicsFactory>("SetRot", gf_setRot, METH_VARARGS,
		"Set factory's rotation state");

	std::function<void(GraphicsFactory*, float, float, float, float)> gf_setColor(&GraphicsFactory::setColor);
	Python::_add_Func<__LINE__, GraphicsFactory>("SetColor", gf_setColor, METH_VARARGS,
		"Set factory's color state");
}

/*static*/ void GraphicsCollection::PyRegister() {
	Python::Register_Class<GraphicsCollection, __LINE__>("GraphicsCollection");

	std::function<G_Data *(GraphicsCollection *, GraphicsFactory *)> gc_Add(&GraphicsCollection::addComponent);
	Python::_add_Func<__LINE__, GraphicsCollection>("AddComponent", gc_Add, METH_VARARGS,
		"add graphics component to collection");
}

/*static*/ void Camera::PyRegister() {
	Python::Register_Class<Camera, __LINE__>("Camera");

	std::function<void(Camera *, float, float, float, float, float, float)> camInitFn(&Camera::InitOrtho);
	Python::_add_Func<__LINE__, Camera>("InitOrtho", camInitFn, METH_VARARGS,
		"Create an Orthographic Camera");
}