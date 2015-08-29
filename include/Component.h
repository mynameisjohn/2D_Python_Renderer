#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <vector>

// These classes are completely useless

// Factory singleton
template <typename D>
class Factory
{
public:
	Factory() {}
	using F_ptr = std::unique_ptr< Factory<D> >;
};

template <typename D>
class Collection
{
public:
	using C_ptr = std::unique_ptr< Collection<D> >;

protected:
	Collection() {}
	std::vector<D> v_Components;

public:
	auto begin() -> decltype(s_Instance->v_Components.begin()) { return s_Instance->v_Components.begin(); }
	auto end() -> decltype(s_Instance->v_Components.end()) { return s_Instance->v_Components.end(); }
};

#endif