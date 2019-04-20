#include "sphere.h"
#include "ray.h"


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    vec3 p = ray.endpoint - center;
    
    // Quadratic formula constants
    double a = 1; // ray is a unit vector
    double b = 2 * dot(ray.direction, p);
    double c = p.magnitude_squared() - std::pow(radius, 2);
    double discriminant = std::pow(b, 2) - (4 * a * c);

    // No solution
    if (discriminant <= 0.0) 
    {
        return false;
    }

    // Calculate time of intersections
    double t1 = (-b - std::sqrt(discriminant)) / (2 * a);
    double t2 = (-b + std::sqrt(discriminant)) / (2 * a);

    // Ray starts inside sphere
    if (t1 < 0.0) 
    {
        /*
        Hit t1_hit;
        t1_hit.object = this;
        t1_hit.t = 0;
        t1_hit.ray_exiting = false;
        hits.push_back(t1_hit);
        */
        
        Hit t2_hit;
        t2_hit.object = this;
        t2_hit.t = t2;
        t2_hit.ray_exiting = true;
        hits.push_back(t2_hit);
    }
    // Ray enters and exits sphere
    else 
    {
        Hit t1_hit;
        t1_hit.object = this;
        t1_hit.t = t1;
        t1_hit.ray_exiting = false;
        hits.push_back(t1_hit);

        Hit t2_hit;
        t2_hit.object = this;
        t2_hit.t = t2;
        t2_hit.ray_exiting = true;
        hits.push_back(t2_hit);
    }
    return true;
}

vec3 Sphere::Normal(const vec3& point) const
{
    return (point - center).normalized();
}
