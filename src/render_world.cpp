#include <vector>
#include <algorithm>
#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include <limits>
#include "ray.h"


Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3),disable_fresnel_reflection(false),disable_fresnel_refraction(false)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(const Ray& ray,Hit& hit)
{
    // TODO
    Hit closest;
    closest.t = std::numeric_limits<double>::infinity();

    for (Object* o : objects)
    {
        std::vector<Hit> rayHits;
        o->Intersection(ray, rayHits);
        for (Hit h : rayHits)
        {
            // Ignore hits <= small_t
            if (h.t < closest.t && h.t > small_t) 
            {
                closest = h;
            }
        }
    }
    if (closest.t == std::numeric_limits<double>::infinity())
    {
        return nullptr;
    }
    hit = closest;
    return const_cast<Object*>(closest.object);
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    vec3 direction = (camera.World_Position(pixel_index) - camera.position).normalized();
    Ray ray = Ray(camera.position, direction); // TODO: set up the initial view ray here
    
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray, int recursion_depth)
{
    vec3 color;

    Hit closest_hit;
    Object* closest = Closest_Intersection(ray, closest_hit);

    vec3 dummy;
    if (closest != nullptr && recursion_depth <= recursion_depth_limit) {
        vec3 intersection_point = ray.Point(closest_hit.t);
        color = closest->material_shader->Shade_Surface(ray, intersection_point, closest->Normal(intersection_point), recursion_depth, closest_hit.ray_exiting);
    }
    else {
        color = background_shader->Shade_Surface(ray, ray.endpoint, ray.direction, recursion_depth_limit, false);
    }
    return color;
}