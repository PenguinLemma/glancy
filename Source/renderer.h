#pragma once

#include "scene.h"
#include "camera.h"
#include "image.h"
#include <algorithm>
#include <iostream>
#include <limits>

namespace plemma
{
namespace glancy
{

template <typename UnaryOp>
class Renderer
{
public:
    Renderer(UnaryOp gc, size_t h_pixels, size_t v_pixels,
             size_t rays_pixel, uint16_t maxd)
    :
        GammaCorrection(gc),
        num_horizontal_pixels_(h_pixels),
        num_vertical_pixels_(v_pixels),
        num_rays_per_pixel_(rays_pixel),
        maximum_depth_(maxd)
    { }

    void ProcessScene(const Scene& scene, const Camera& camera, Image& image) const;
private:
    Vec3 GetColor(const HitableList& world, const Ray& r, uint16_t depth) const;

    UnaryOp GammaCorrection;
    const size_t num_horizontal_pixels_;
    const size_t num_vertical_pixels_;
    const size_t num_rays_per_pixel_;
    const uint16_t maximum_depth_;
};

template <typename UnaryOp>
void Renderer<UnaryOp>::ProcessScene(const Scene& scene, const Camera& camera, Image& image) const
{

    std::cout << "0% processing completed." << std::endl;
    int percentage_completed = 0;
    int prev_percentage_written = 0;
    const int initial_depth = 0;
    const RealNum horizontal_length = static_cast<RealNum>(num_horizontal_pixels_);
    const RealNum vertical_length = static_cast<RealNum>(num_vertical_pixels_);
    const HitableList& world(scene.World());
    std::uniform_real_distribution<RealNum> dist(0.0, 1.0);
    for (size_t j = num_vertical_pixels_; j >= 1; --j)
    {
        size_t index_ver = j - 1;
        for (size_t index_hor = 0; index_hor < num_horizontal_pixels_; ++index_hor)
        {
            Vec3 color(0.0, 0.0, 0.0);
            for (size_t s = 0; s < num_rays_per_pixel_; ++s)
            {
                RealNum u = (static_cast<RealNum>(index_hor) + dist(my_engine())) / horizontal_length;
                RealNum v = (static_cast<RealNum>(index_ver) + dist(my_engine())) / vertical_length;

                Ray r = camera.GetRay(u, v);

                color += GetColor(world, r, initial_depth);
            }

            color /= static_cast<RealNum>(num_rays_per_pixel_);

            std::transform(color.Begin(), color.End(), color.Begin(), GammaCorrection);
            color *= static_cast<RealNum>(255.9999);
            image.PaintPixel(index_hor, index_ver, color);
        }

        percentage_completed = static_cast<int>(100.0 * (1.0 - static_cast<RealNum>(index_ver + 1) / vertical_length));
        if (static_cast<int>(percentage_completed) >= prev_percentage_written + 5)
        {
            int to_write = percentage_completed - (percentage_completed % 5);
            std::cout << to_write << "% processing completed." << std::endl;
            prev_percentage_written = to_write;
        }
    }

    std::cout << "100% processing completed." << std::endl;
    std::cout << std::endl;
}

template <typename UnaryOp>
Vec3 Renderer<UnaryOp>::GetColor(const HitableList& world, const Ray& r, uint16_t depth) const
{
    HitRecord rec;
    // We increase the minimum parameter to avoid finding out the original intersection again
    RealNum min_param = 0.001;
    if (world.Hit(r, min_param, std::numeric_limits<RealNum>::max(), rec))
    {
        Ray scattered_ray;
        Vec3 attenuation;
        if (depth < maximum_depth_ && rec.mat->Scatter(r, rec, attenuation, scattered_ray))
        {
            return attenuation * GetColor(world, scattered_ray, depth + 1);
        }
        else
        {
            return Vec3(0.0, 0.0, 0.0);
        }
    }
    else
    {
        Vec3 unit_direction = UnitVector(r.Direction());
        RealNum t = 0.5*(unit_direction.Y() + 1.0);
        return (1.0 - t)*Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
    }
}


} // namespace glancy

} // namespace plemma