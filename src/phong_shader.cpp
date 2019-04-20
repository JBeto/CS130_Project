#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include <algorithm>
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const 
{
    vec3 normal = is_exiting ? -same_side_normal : same_side_normal;

    // Ambient
    vec3 ambient = world.ambient_color * world.ambient_intensity * color_ambient;
    //intersection_point = .0001 + intersection_point;
    // TODO: fix cancer
    vec3 diffuse;
    vec3 specular;
    if (world.enable_shadows) 
    {
        for (Light* light : world.lights) 
        {
            // Vector from intersection to light
            vec3 l = light->position - intersection_point;

            // Shadows
            Hit intersect;
            Ray light_ray = Ray(intersection_point, l);
            Object* exists = world.Closest_Intersection(light_ray, intersect);
            
            if (exists == nullptr || intersect.t >= l.magnitude())
            {
                vec3 color_light = light->Emitted_Light(ray) / l.magnitude_squared();
                // Diffuse
                diffuse += color_diffuse * color_light * std::max(0.0, dot(l.normalized(), normal));
                // Specular
                vec3 view = (world.camera.position - intersection_point).normalized();
                vec3 reflected_direction = (2 * dot(l, normal) * normal - l).normalized();
                double specular_intensity = pow(std::max(dot(reflected_direction, view), 0.0), specular_power);
                specular += color_specular * color_light * specular_intensity;
            }
        }
    }
    else
    {
        for (Light* light : world.lights) 
        {
            // Vector from intersection to light
            vec3 l = light->position - intersection_point;
            vec3 color_light = light->Emitted_Light(ray) / l.magnitude_squared();
            // Diffuse
            diffuse += color_diffuse * color_light * std::max(0.0, dot(l.normalized(), normal));
            // Specular
            vec3 view = (world.camera.position - intersection_point).normalized();
            vec3 reflected_direction = (2 * dot(l, normal) * normal - l).normalized();
            double specular_intensity = pow(std::max(dot(reflected_direction, view), 0.0), specular_power);
            specular += color_specular * color_light * specular_intensity;
        }
    }

    // Calculate total color
    return ambient + diffuse + specular;
}