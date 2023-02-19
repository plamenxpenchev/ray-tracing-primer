#include "sphere.hpp"

/*
	For a point P = [x, y, z] of a vector to lie on a sphere with center C = [Cx, Cy, Cz] and radius r,
	it must satisfy (x - Cx)^2 + (y - Cy)^2 + (z - Cz)^2 = r^2.
	In vector notation we can write that as (P-C) * (P-C) = r^2.

	For a ray, defined as P(t) = A + tb, to hit the sphere it must satisfy the equation (P(t)-C) * (P(t)-C) = r^2.
	t^2(b * b) + 2t(b * (A-C)) + (A-C) * (A-C) - r^2 = 0.
	The vectors and r are constants and known, thus we can solve the quadratic for t,
	and determine if the ray intersects the sphere at two points (two roots, discriminant > 0).
*/
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

	vec3 oc = r.origin() - center;

	/*
		For any vector V = [v1, v2, ...] its length is ||V|| = sqrt(v1*v1 + v2*v2 + ...).
		Naturally, V*V = ||V||^2
	*/
	auto a = r.direction().length_squared(); // equivalent to a = dot(r.direction(), r.direction());
	auto half_b = dot(oc, r.direction()); // where b = 2.0 * dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius; // equivalent to c = dot(oc, oc) - radius * radius

	/*
		Using half_b, we can extract the 4 from the discriminant square root term
		in determining the roots. Also then we reduce the fraction by simplifying the 2 out.
		We look for the nearest root in the acceptable [t_min, t_max] range.
	*/
	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) {

		return false;
	}

	auto sqrtd = sqrt(discriminant);
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || root > t_max) {

		root = (-half_b + sqrtd) / a;
		if (root < t_min || root > t_max) {

			return false;
		}
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);

	return true;
}