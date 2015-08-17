#ifndef GEOMFACTORY_H
#define GEOMFACTORY_H

#include "Util.h"
#include "GL_Includes.h"

#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>

class GeomFactory {
public:
	GeomFactory();
	struct Info {
		GLuint VAO;
		GLuint nIdx;
		vec4 m_Color;
		mat4 m_MV;
	};
	static GLint s_PosHandle;
	static void setPosHandle(GLint posHandle) {
		s_PosHandle = posHandle;
	}
protected:
	vec3 m_Trans, m_Scale;
	vec4 m_Rot, m_Color;
public:
	virtual Info getGeom() = 0;
	void setTrans(float, float, float);
	void setScale(float, float, float);
	void setRot(float, float, float, float);
	void setColor(float, float, float, float);
};

class QuadFactory : public GeomFactory {
public:
	QuadFactory();
	GeomFactory::Info getGeom() override;
};

//
//// Should this be a singleton?
//class GeomFactory{
//public:
//	GeomFactory();
//	GLuint GetQuad(GLint posHandle);
//
//	template <uint32_t nVert, uint32_t nIdx>
//	static GLuint genVAO(std::array<vec3, nVert> verts, std::array<uint32_t, nIdx> indices, GLint posHandle);
//private:
//	// Maps strings (names or IQM filenames) to VAOs to be instanced
//	std::map<std::string, GLuint> m_mapCachedGeometry;
//};

#endif