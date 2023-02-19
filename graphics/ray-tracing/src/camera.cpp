#include "camera.hpp"

ray camera::get_ray(double u, double v) const {

	return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}