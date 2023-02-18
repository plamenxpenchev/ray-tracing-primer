#include <iostream>

#include "ray.hpp"
#include "vec3.hpp"
#include "color.hpp"

/*
	For a point P = [x, y, z] of a vector to lie on a sphere with center C = [Cx, Cy, Cz] and radius r,
	it must satisfy (x - Cx)^2 + (y - Cy)^2 + (z - Cz)^2 = r^2.
	In vector notation we can write that as (P-C) * (P-C) = r^2.

	For a ray, defined as P(t) = A + tb, to hit the sphere it must satisfy the equation (P(t)-C) * (P(t)-C) = r^2.
	t^2(b * b) + 2t(b * (A-C)) + (A-C) * (A-C) - r^2 = 0.
	The vectors and r are constants and known, thus we can solve the quadratic for t, 
	and determine if the ray intersects the sphere at two points (two roots, discriminant > 0).
*/
bool hit_sphere(const point3& center, double radius, const ray& r) {

	vec3 oc = r.origin() - center;

	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius * radius;

	auto discriminant = b * b - 4 * a * c;
	return discriminant > 0;
}

color ray_color(const ray& r) {

	// Color in red all points that intersect sphere
	if (hit_sphere(point3(0, 0, -1), 0.5, r)) {

		return color(1, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);

	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0; // distance between projection point and projection plane

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - (horizontal / 2) - (vertical / 2) - vec3(0, 0, focal_length);

    // Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int j = image_height - 1; j >= 0; --j) {

		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {

            // RGB range from 0.0 to 1.0
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);

            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);

            write_color(std::cout, pixel_color);
		}
	}

	std::cerr << "\nDone.\n";
}
