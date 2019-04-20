#include "refractive_shader.h"
#include "ray.h"
#include "render_world.h"
#include <cmath>

vec3 Refractive_Shader::
Shade_Surface(const Ray& ray, const vec3& intersection_point,
        const vec3& same_side_normal, int recursion_depth,bool is_exiting) const
{
    //Hints: Use REFRACTIVE_INDICES::AIR for air refractive_index
    //       Use is_exiting to decide the refractive indices on the ray and transmission sides
    vec3 normal = is_exiting ? -same_side_normal : same_side_normal;

    vec3 reflection_color;
    vec3 refraction_color;
    double reflectance_ratio=-1;
    if(!world.disable_fresnel_refraction)
    {
        //TODO (Test 27+): Compute the refraction_color:
        // - Check if it is total internal reflection. 
        //      If so update the reflectance_ratio for total internal refraction
        //
        //      else, follow the instructions below
        //
        //        (Test 28+): Update the reflectance_ratio 
        //
        //        (Test 27+): Cast the refraction ray and compute the refraction_color
        //
        double n_i, n_r;
        if (!is_exiting)
        {
            n_r = refractive_index;
            n_i = REFRACTIVE_INDICES::AIR;
        }
        else {
            n_r = REFRACTIVE_INDICES::AIR;
            n_i = refractive_index;
        }

        // Value to test internal reflection
        double n_values = std::pow(n_i, 2) / std::pow(n_r, 2);
        double cos_theta_i = dot(-ray.direction, normal) / ray.direction.magnitude();
        double star = 1 - n_values * (1 - std::pow(cos_theta_i, 2)); 

        // No refraction, just reflection
        if (star < 0)
        {
            reflectance_ratio = 1;
        }
        else // There is refraction
        {
            // Calculate cos_theta_r component
            double cos_theta_r = std::sqrt(star);
            
            // Calculate refraction color
            vec3 t_direction = (n_i / n_r) * (ray.direction - dot(ray.direction, normal) * normal) - cos_theta_r * normal;
            Ray t = Ray(intersection_point, t_direction);
            refraction_color = world.Cast_Ray(t, recursion_depth + 1);
            
            // Calculate reflectance ratio
            double r_parallel = std::pow((n_r * cos_theta_i - n_i * cos_theta_r) / (n_r * cos_theta_i + n_i * cos_theta_r), 2);
            double r_perpendicular = std::pow((n_i * cos_theta_i - n_r * cos_theta_r) / (n_i * cos_theta_i + n_r * cos_theta_r), 2);
            reflectance_ratio = (r_parallel + r_perpendicular) / 2;
        }
    }

    if(!world.disable_fresnel_reflection)
    {
        //TODO:(Test 26+): Compute reflection_color:
        // - Cast Reflection Ray and get color
        //
        vec3 l = ray.endpoint - intersection_point;
        vec3 reflected_direction = (2 * dot(l, normal) * normal - l).normalized();
        reflection_color = world.Cast_Ray(Ray(intersection_point, reflected_direction), recursion_depth + 1);
    }

    Enforce_Refractance_Ratio(reflectance_ratio);
    vec3 color;
    // TODO: (Test 26+) Compute final 'color' by blending reflection_color and refraction_color using 
    //                  reflectance_ratio
    //
    color = reflectance_ratio * reflection_color + (1 - reflectance_ratio) * refraction_color;
    return color;
}

void Refractive_Shader::
Enforce_Refractance_Ratio(double& reflectance_ratio) const
{
    if(world.disable_fresnel_reflection) reflectance_ratio=0;
    else if(world.disable_fresnel_refraction) reflectance_ratio=1;
}

