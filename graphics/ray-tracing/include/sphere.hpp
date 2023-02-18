#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class sphere : public hittable {
public:
	point3 center;
	double radius;
public:
	sphere() {}
	sphere(point3 cen, double r) : center{ cen }, radius{ r } {}
	virtual bool hit(
		const ray& r, double t_min, double t_max, hit_record& rec) const override;
};
