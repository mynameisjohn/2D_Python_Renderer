#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/quaternion.hpp>

#include "GL_Includes.h"
#include "Util.h"
#include "Component.h"

struct G_Data
{
	GLuint VAO;
	GLuint nIdx;
	vec4 color;
	mat4 MV;

	static GLint GetPosHandle();
//protected:
	static void SetPosHandle(GLint);
private:
	static GLint s_PosHandle;
};

class GraphicsFactory : public Factory<G_Data>
{
protected:
	// I guess it's assumed every geom needs this stuff
	vec3 m_Trans, m_Scale;
	vec4 m_Color;
	fquat m_Rot;

	// Used for caching, may have to move to subclasses
	std::map<std::string, GLuint> m_CachedVAOs;
public:
	GraphicsFactory();

	void setTrans(float, float, float);
	void setScale(float, float, float);
	void setRot(float, float, float, float);
	void setColor(float, float, float, float a = 1.f);

	virtual G_Data GetData() = 0;
};

class GraphicsCollection : public Collection<G_Data>
{
public:
	GraphicsCollection() {}

	// Just as a heads up, you can't construct python
	// objects with this pointer, since the vector
	// will relocate whether you like it or not
	virtual G_Data * addComponent(GraphicsFactory * f) {
		v_Components.push_back(f->GetData());
		return &v_Components.back();
	}
};

class QuadFactory : public GraphicsFactory
{
public:
	QuadFactory() : GraphicsFactory() {}
	G_Data GetData() override;
};

//
//
//class Graphics : public Component<G_Data>
//{
//public:
//	class Factory : public Component<G_Data>::Factory
//	{
//	protected:
//		// I guess it's assumed every geom needs this stuff
//		vec3 m_Trans, m_Scale;
//		vec4 m_Rot, m_Color;
//
//		// Used for caching, may have to move to subclasses
//		std::map<std::string, GLuint> m_CachedVAOs;
//
//		static GLint s_PosHandle;
//
//		Factory();
//	public:
//		static void setPosHandle(GLint posHandle);
//
//		void setTrans(float, float, float);
//		void setScale(float, float, float);
//		void setRot(float, float, float, float);
//		void setColor(float, float, float, float);
//
//		static void Init();
//		static void PyInit();
//	};
//
//	class Collection : public Component<G_Data>::Collection
//	{
//		Collection();
//	public:
//
//		G_Data * addComponent(Component<G_Data>::Factory * f) override;
//
//		static void Init();
//		static void PyInit();
//	};
//
//	static void PyInit();
//};
//
//// Until I'm rid of ECS
//using GraphicsFactory = Graphics::Factory;
//using GraphicsCollection = Graphics::Collection;
//
//class QuadFactory : public GraphicsFactory
//{
//public:
//	QuadFactory();
//	G_Data GetData() override;
//};

//using G_Component = Component<GraphicsComponent>;
//
//class GraphicsFactory : public G_Component::Factory
//{
//public:
//	G_Component::u_ptr getInfo();
//	// Static handle to position attribute
	//static GLint s_PosHandle;
	//static void setPosHandle(GLint posHandle) {
	//	s_PosHandle = posHandle;
	//}
//
//	// I guess it's assumed every geom needs this stuff
//	vec3 m_Trans, m_Scale;
//	vec4 m_Rot, m_Color;
//
//	// Used for caching, may have to move to subclasses
//	std::map<std::string, GLuint> m_CachedVAOs;
//
//	static GraphicsFactory * Instance() {
//		return static_cast<GraphicsFactory *>(s_Instance.get());
//	}
//
//
//	void setTrans(float, float, float) {
//
//	}
//	void setScale(float, float, float) {
//
//	}
//	void setRot(float, float, float, float) {
//
//	}
//	void setColor(float, float, float, float) {
//
//	}
//
//	static void Init() {
//
//	}
//};

//class GraphicsCollection : public G_Component::Collection
//{
//public:
//	GraphicsComponent * addComponent(G_Component::Factory * fact) override;
//
//	static void Init() {
//
//	}
//	static GraphicsCollection * Instance() {
//		return static_cast<GraphicsCollection *>(s_Instance.get());
//	}
//};

#endif