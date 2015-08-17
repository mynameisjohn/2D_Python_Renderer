#include "GeomFactory.h"

#include <array>
#include <gtx/transform.hpp>

// Static handle to position attribute
GLint GeomFactory::s_PosHandle = -1;

// Factory modifiers
void GeomFactory::setTrans(float tx, float ty, float tz) {
	m_Trans = vec3(tx, ty, tz);
}
void GeomFactory::setScale(float sx, float sy, float sz) {
	m_Scale = vec3(sx, sy, sz);
}
void GeomFactory::setRot(float r, float x, float y, float z) {
	m_Rot = vec4(r, x, y, z); // quaternion?
}
void GeomFactory::setColor(float r, float g, float b, float a) {
	m_Color = vec4(r, g, b, a);
}

// Default constructor, makes state as blank as possible
GeomFactory::GeomFactory() :
	m_Trans(0),
	m_Scale(1),
	m_Rot(0, 0, 1, 0),
	m_Color(1)
{}

// This doesn't need much
QuadFactory::QuadFactory() :
	GeomFactory()
{}

// QuadFactory's getGeom function
// which really only gets called once
GeomFactory::Info QuadFactory::getGeom() {
	// This is a cheap trick....
	// I need the index count, but I'd rather
	// figure something else out
	// Vertices, centered around origin
	const std::array<vec3, 4> vertices = {
		vec3(-1,-1,0),
		vec3(1,-1,0),
		vec3(1,1,0),
		vec3(-1,1,0)
	};

	// Indices
	// right now these only work for triangle fan
	const std::array<uint32_t, 4> indices = {
		0,1,2,3
	};

	// For quads just make the VAO once
	GLuint VAO(0);
	if (m_CachedVAOs.size())
		VAO = m_CachedVAOs.begin()->second;

	// If the VAO doesn't exist, create it	
	else {
		// Generate bufferse for vertices and indices
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		std::array<GLuint, 2> buffers;
		glGenBuffers(buffers.size(), buffers.data());

		//vertices
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(s_PosHandle);
		glVertexAttribPointer(s_PosHandle, 3, GL_FLOAT, 0, 0, 0);

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*indices.size(), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	// Create MV given current state
	mat4 MV = glm::translate(m_Trans)*glm::rotate(m_Rot.w, vec3(m_Rot))*glm::scale(m_Scale);
	
	// return info given current state
	return { VAO, indices.size(), m_Color, MV };
}
