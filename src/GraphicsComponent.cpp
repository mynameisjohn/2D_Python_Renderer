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

/*static*/ void GraphicsFactory::PyRegister(){
	Python::Register_Class<GraphicsFactory>("GraphicsFactory");

	// All this shit
	std::function<void(GraphicsFactory*, float, float, float)> gf_setTrans(&GraphicsFactory::setTrans);
	Python::_add_Func<__LINE__, GraphicsFactory>("SetTrans", gf_setTrans, METH_VARARGS,
		"Set factory's translation state");

	std::function<void(GraphicsFactory*, float, float, float)> gf_setScale(&GraphicsFactory::setTrans);
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
	Python::Register_Class<GraphicsCollection>("GraphicsCollection");

	std::function<G_Data *(GraphicsCollection *, GraphicsFactory *)> gc_Add(&GraphicsCollection::addComponent);
	Python::_add_Func<__LINE__, GraphicsCollection>("AddComponent", gc_Add, METH_VARARGS,
		"add graphics component to collection");
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
	}

	// Create MV given current state
	mat4 MV = glm::translate(m_Trans)*glm::mat4_cast(m_Rot)*glm::scale(m_Scale);

	// return info given current state
	return{ VAO, indices.size(), m_Color, MV };
}