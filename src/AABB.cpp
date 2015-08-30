#include "AABB.h"

AABB::AABB() :
	C(0),
	R(0),
	V(0),
	M(1),
	E(1)
{}

float AABB::width() { return 2.f*R.x; }
float AABB::height() { return 2.f*R.y; }
float AABB::left() { return C.x - R.x; }
float AABB::right() { return C.x + R.x; }
float AABB::top() { return C.y + R.y; }
float AABB::bottom() { return C.y - R.y; }

float AABB::dX(AABB& other) {
	return fabs(other.C.x - C.x);
}

float AABB::dY(AABB& other) {
	return fabs(other.C.y - C.y);
}

float AABB::dist(AABB& other) {
	return glm::length(other.C - C);
}

bool AABB::overlaps(AABB& other) {
	if (dX(other) > other.R.x + R.x)
		return false;
	if (dY(other) > other.R.y + R.y)
		return false;
	return true;
}

void AABB::translate(vec2 d) {
	C += d;
}

vec2 AABB::momentum() {
	return M*V;
}

float AABB::kinetic() {
	return 0.5f * M * glm::dot(V, V);
}

// unsure about this
bool AABB::collide(AABB& other) {
	if (overlaps(other)) {
		float Msum_1 = 1.f / (M + other.M); // denominator
		float Cr = 0.5f * (E + other.E); // coef of rest
		vec2 P1 = momentum(), P2 = other.momentum();
		vec2 A = P1 + P2, B = Cr * (P1 - P2);
		vec2 v1 = (A - B) * Msum_1;
		vec2 v2 = (A + B) * Msum_1;
		V = v1;
		other.V = v2;
		return true;
	}
	return false;
}