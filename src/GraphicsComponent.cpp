#include "GraphicsComponent.h"

#include <gtx/transform.hpp>

#include <array>

#include <pyliason.h>

GLint G_Data::s_PosHandle(-1);

/*static*/ void G_Data::SetPosHandle(GLint pH) {
	s_PosHandle = pH;
}

/*static*/ GLint G_Data::GetPosHandle() {
	return s_PosHandle;
}

void G_Data::translate(vec3 T) {
	MV = glm::translate(T)*MV;
}

void G_Data::rotate(fquat Q) {
	MV = glm::mat4_cast(Q) * MV;
}

void G_Data::scale(vec3 S) {
	MV = glm::scale(S)*MV;
}

void G_Data::identity() {
	MV = mat4(1);
}

GraphicsFactory::GraphicsFactory():
	m_Trans(0),
	m_Scale(1),
	m_Rot(0, 0, 0, 1),
	m_Color(1)
{}

void GraphicsFactory::setTrans(float x, float y, float z) {
	m_Trans = vec3(x, y, z);
}

void GraphicsFactory::setScale(float sX, float sY, float sZ) {
	m_Scale = vec3(sX, sY, sZ);
}

void GraphicsFactory::setRot(float w, float x, float y, float z) {
	m_Rot = fquat(w, x, y, z);
}

void GraphicsFactory::setColor(float r, float g, float b, float a /*= 1.f*/) {
	m_Color = vec4(r, g, b, a);
}

G_Data QuadFactory::GetData() {
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
		glEnableVertexAttribArray(G_Data::GetPosHandle());
		glVertexAttribPointer(G_Data::GetPosHandle(), 3, GL_FLOAT, 0, 0, 0);

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*indices.size(), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		m_CachedVAOs["quad"] = VAO;
	}

	// Create MV given current state
	mat4 MV = glm::translate(m_Trans)*glm::mat4_cast(m_Rot)*glm::scale(m_Scale);

	// return info given current state
	return{ VAO, GLuint(indices.size()), m_Color, MV };
}