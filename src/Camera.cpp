#include "Camera.h"
#include <gtx/transform.hpp>
#include <pyliason.h>

GLint Camera::s_ProjHandle(-1);
GLint Camera::s_PosHandle(-1);
using glm::normalize;

// Default constructor, sets type to NIL
Camera::Camera() :
	m_Type(Type::NIL),
	m_v3Pos(0),
	m_qRot(1, 0, 0, 0),
	m_m4Proj(1) {
}

// Ortho constructor
Camera::Camera(const vec2 X, const vec2 Y, const vec2 Z) :
	m_Type(Type::ORTHO),
	m_v3Pos(0),
	m_qRot(1, 0, 0, 0),
	m_m4Proj(glm::ortho(X[0], X[1], Y[0], Y[1], Z[0], Z[1])) {
}

void Camera::InitOrtho(float left, float right, float bottom, float top, float near, float far) {
	*this = Camera(vec2(left, right), vec2(bottom, top), vec2(near, far));
}

// Perspective constructor
Camera::Camera(const float fovy, const float aspect, const vec2 nf) :
	m_Type(Type::PERSP),
	m_v3Pos(0, 0, 0),
	m_qRot(1, 0, 0, 0),
	m_m4Proj(glm::perspective(fovy, aspect, nf[0], nf[1])) {
}

void Camera::InitPersp(float fovy, float aspect, float near, float far) {
	*this = Camera(fovy, aspect, vec2(near, far));
}

vec3 Camera::GetPos() const {
	return m_v3Pos;
}

fquat Camera::GetRot() const {
	return m_qRot;
}

vec3 Camera::getView() const {
	return m_qRot * vec3(0, 0, 1);
}

// Why am I inverting the translation but not the rotation?
mat4 Camera::GetTransform()  const {
	return glm::mat4_cast((m_qRot))*glm::translate(-m_v3Pos);
}

// TODO bring QuatVecs back, you need a TRT

mat4 Camera::GetMat() const {
	// The camera is always at the origin.
	// We're really moving and rotating everyone else
	return m_m4Proj*GetTransform();
}

void Camera::Rotate(fquat Q) {
	m_qRot = glm::normalize(Q * m_qRot);
}

void Camera::Translate(vec3 T) {
	// I honestly have no clue why this works
	vec3 Tp = glm::inverse(m_qRot) * T;
	m_v3Pos += Tp;
}

void Camera::ResetRot() {
	m_qRot = { 1, 0, 0, 0 };
}

void Camera::ResetPos() {
	m_v3Pos = { 0, 0, 0 };
}

void Camera::Reset() {
	ResetPos();
	ResetRot();
}

mat4 Camera::GetProj() const {
	return m_m4Proj;
}

/*static*/ GLint Camera::GetPosHandle() {
	return s_PosHandle;
}

/*static*/ GLint Camera::GetProjHandle() {
	return s_ProjHandle;
}

/*static*/ void Camera::SetPosHandle(GLint P) {
	s_PosHandle = P;
}

/*static*/ void Camera::SetProjHandle(GLint p) {
	s_ProjHandle = p;
}

/*static*/ void Camera::PyRegister() {
	Python::Register_Class<Camera>("Camera");

	std::function<void(Camera *, float, float, float, float, float, float)> camInitFn(&Camera::InitOrtho);
	Python::_add_Func<__LINE__, Camera>("InitOrtho", camInitFn, METH_VARARGS,
		"Create an Orthographic Camera");
}