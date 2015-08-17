#include "GeomFactory.h"

#include <array>
#include <gtx/transform.hpp>

GLint GeomFactory::s_PosHandle = -1;

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

GeomFactory::GeomFactory() :
	m_Trans(0),
	m_Scale(1),
	m_Rot(0, 0, 1, 0),
	m_Color(1)
{}

QuadFactory::QuadFactory() :
	GeomFactory()
{}

GeomFactory::Info QuadFactory::getGeom() {
	std::array<vec3, 4> vertices = {
		vec3(0,0,0),
		vec3(1,0,0),
		vec3(1,1,0),
		vec3(0,1,0)
	};
	std::array<uint32_t, 4> indices = {
		0,1,2,3
	};

	GLuint VAO;
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

	mat4 MV = glm::translate(m_Trans)*glm::rotate(m_Rot.w, vec3(m_Rot))*glm::scale(m_Scale);

	return { VAO, indices.size(), m_Color, MV };
}

//
////Create a Vertex Array Object given some geometry info
//template <uint32_t nVert, uint32_t nIdx>
///*static*/ GLuint GeomFactory::genVAO(std::array<vec3, nVert> verts, std::array<uint32_t, nIdx> indices, GLint posHandle) {
//
//	GLuint VAO;
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	std::array<GLuint, 2> buffers;
//	glGenBuffers(buffers.size(), buffers.data());
//
//	//vertices
//	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*verts.size(), verts.data(), GL_STATIC_DRAW);
//	glEnableVertexAttribArray(posHandle);
//	glVertexAttribPointer(posHandle, 3, GL_FLOAT, 0, 0, 0);
//
//	//indices
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t)*indices.size(), indices.data(), GL_STATIC_DRAW);
//
//	glBindVertexArray(0);
//
//	return VAO;
//}
//
//GLuint GeomFactory::GetQuad(GLint posHandle) {
//	if (posHandle < 0)
//		return 0;
//
//	auto it = m_mapCachedGeometry.find("quad");
//	if (it != m_mapCachedGeometry.end())
//		return it->second;
//
//	std::array<vec3, 4> vertices = {
//		vec3(0,0,0),
//		vec3(1,0,0),
//		vec3(1,1,0),
//		vec3(0,1,0)
//	};
//	std::array<uint32_t, 4> indices = {
//		0,1,2,3
//	};
//
//	uint32_t VAO = genVAO(vertices, indices, posHandle);
//
//	m_mapCachedGeometry["quad"] = VAO;
//
//	return VAO;
//}
//
//GeomFactory::GeomFactory() {
//
//}