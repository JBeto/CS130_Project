#include "plane.h"
#include "ray.h"
#include <cfloat>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    double denom = dot(normal, ray.direction);
    if (fabs(denom) <= 0.00000001)
    {
        return false;
    }
    double t = dot(normal, x1 - ray.endpoint) / denom;
    if (t < 0)
    {
        return false;
    }
    
    // Hit
    Hit hit;
    hit.object = this;
    hit.ray_exiting = false;
    hit.t = t;
    hits.push_back(hit);
    return true;
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}
