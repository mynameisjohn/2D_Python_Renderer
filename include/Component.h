#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <vector>

// These classes are completely useless

// Factory singleton
template <typename D>
class Factory
{
	using F_ptr = std::unique_ptr< Factory<D> >;
protected:
	static F_ptr s_Instance;
public:
	static Factory * Instance() { return s_Instance.get(); }
};

template <typename D>
class Collection
{
	using C_ptr = std::unique_ptr< Collection<D> >;

protected:
	std::vector<D> v_Components;
	static C_ptr s_Instance;

public:
	static Collection& Instance() { return *(s_Instance.get()); }

	auto begin() -> decltype(s_Instance->v_Components.begin()) { return s_Instance->v_Components.begin(); }
	auto end() -> decltype(s_Instance->v_Components.end()) { return s_Instance->v_Components.end(); }
};
//
//// Generic Component Class
//// The parameter C is the "data" model, and any subclasses
//// should create interesting functionst to deal with it
//template <typename D>
//class Component
//{
//	// Really Component is a namespace
//	// That encapsulates and templatizes
//	// the Factory and Collection objects
//	// (which feels shitty)
//public:
//	// Forwards for abstract
//	// factory and collection singletons
//	class Factory;
//	class Collection;
//
//	// Unique Ptr type definitions
//	using u_ptr = std::unique_ptr<D>;
//	using F_ptr = std::unique_ptr<Factory>;
//	using C_ptr = std::unique_ptr<Collection>;
//
//	// Factory singleton
//	class Factory
//	{
//	protected:
//		static F_ptr s_Instance;
//		Factory() {}
//	public:
//		static Factory * Instance() { return s_Instance.get(); }
//
//		virtual D GetData() = 0;
//	};
//
//	class Collection
//	{
//	protected:
//		std::vector<D> v_Components;
//		static C_ptr s_Instance;
//	
//	public:
//		Collection() {}
//
//		virtual D * addComponent(Factory *) = 0;
//		
//		static Collection& Instance() { return *(s_Instance.get()); }
//
//		auto begin() -> decltype(s_Instance->v_Components.begin()) {return s_Instance->v_Components.begin();}
//		auto end() -> decltype(s_Instance->v_Components.end()) { return s_Instance->v_Components.end(); }
//	};
//};

#endif