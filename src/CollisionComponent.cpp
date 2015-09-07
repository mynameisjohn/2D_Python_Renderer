#pragma once

#include "CollisionComponent.h"

#include <pyliason.h>

void CollisionFactory::setPos(float px, float py) {
	m_Box.C = vec2(px, py);
}
void CollisionFactory::setScale(float sx, float sy) {
	m_Box.R = vec2(sx, sy) / 2.f;
}
void CollisionFactory::setVel(float vx, float vy) {
	m_Box.V = vec2(vx, vy);
}
void CollisionFactory::setMass(float m) {
	m_Box.M = m;
}
AABB CollisionFactory::GetData() {
	return m_Box;
}
