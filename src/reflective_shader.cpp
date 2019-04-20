#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"
#include "light.h"
#include <algorithm>

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const
{
    vec3 normal = is_exiting ? -same_side_normal : same_side_normal;
    vec3 l = ray.endpoint - intersection_point;
    vec3 reflected_direction = (2 * dot(l, normal) * normal - l).normalized();
    vec3 reflected_color = world.Cast_Ray(Ray(intersection_point, reflected_direction), recursion_depth + 1);
    
    return reflectivity * reflected_color + (1 - reflectivity) * 
        shader->Shade_Surface(ray, intersection_point, normal, recursion_depth, is_exiting);
}