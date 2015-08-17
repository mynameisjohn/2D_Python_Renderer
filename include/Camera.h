#ifndef CAMERA_H
#define CAMERA_H

#include "GL_Includes.h"
#include "Util.h"

#include <mat4x4.hpp>
#include <vec3.hpp>
#include <gtx/quaternion.hpp>

// Transformable (pos, rot) camera class
// Owns a projection matrix and generates a
// transformation given its current pos, rot
// both are needed to render, and calculating
// the eye and half vectors are easier when the
// camera transform is separate

class Camera {
public:
	// Type enumeration
	enum Type {
		ORTHO,
		PERSP,
		NIL
	};

	// Constructors (default, ortho, persp)
	Camera();
	Camera(const vec2 X, const vec2 Y, const vec2 Z);
	Camera(const float fovy, const float aspect, const vec2 nf);

	// Functions to modify projection transform
	void ResetRot();
	void ResetPos();
	void Reset();
	void Translate(vec3 T);
	void Rotate(fquat Q);

	// Functions to access camera info
	vec3 getView() const;
	vec3 GetPos() const;
	fquat GetRot() const;
	mat4 GetMat() const;
	mat4 GetTransform() const;
	mat4 GetProj() const;

	// Access to static shader handles
	static GLint GetProjHandle();
	static GLint GetPosHandle();

	static void SetProjHandle(GLint p);
	static void SetPosHandle(GLint C);
private:
	// Camera Type, position, rotation, projection
	Type m_Type;
	vec3 m_v3Pos;
	fquat m_qRot;
	mat4 m_m4Proj;

	// Static shader handles
	static GLint s_ProjHandle;
	static GLint s_PosHandle;
};

#endif
