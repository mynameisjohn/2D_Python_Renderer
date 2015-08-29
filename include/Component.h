#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <vector>

// These classes are completely useless
// Basically the idea is that every component
// will need a factory and a collection. The rest
// is pretty open, but these two classes do
// suprisingly little

// Factory singleton
template <typename D>
class Factory
{
public:
	Factory() {}
	using u_ptr = std::unique_ptr< Factory<D> >;
};

template <typename D>
class Collection
{
public:
	using u_ptr = std::unique_ptr< Collection<D> >;

protected:
	Collection() {}
	std::vector<D> v_Components;

public:
	auto begin() -> decltype(v_Components.begin()) { return v_Components.begin(); }
	auto end() -> decltype(v_Components.end()) { return v_Components.end(); }
};

#endif