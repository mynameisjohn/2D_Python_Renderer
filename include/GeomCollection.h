#ifndef GEOMCOLLECTION_H
#define GEOMCOLLECTION_H

#include "GL_Includes.h"
#include "Util.h"
#include "GeomFactory.h"

#include <mat4x4.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

// Make something out of Geom::

class GeomCollection {
	std::vector<GeomFactory::Info> m_vecGeom;
public:
	GeomCollection() {}
	auto begin() -> decltype(m_vecGeom.begin()) { return m_vecGeom.begin(); }
	auto end() -> decltype(m_vecGeom.end()) { return m_vecGeom.end(); }

	// Do something about this... what?
	void AddGeom(void * factory) {
		if (GeomFactory * fact = static_cast<GeomFactory *>(factory))
			m_vecGeom.push_back(fact->getGeom());
	}
};

#endif