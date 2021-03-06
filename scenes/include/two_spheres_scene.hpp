#pragma once

#include <memory>

#include "checker_texture.hpp"
#include "constant_texture.hpp"
#include "dielectric.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "scene.hpp"
#include "sphere.hpp"

namespace plemma::glancy {

class TwoSpheresScene : public Scene
{
  public:
    void LoadWorld() noexcept final;
    [[nodiscard]] HittableList const& World() const noexcept final { return world_; }
    ~TwoSpheresScene() final = default;

  private:
    HittableList world_;
};

inline void TwoSpheresScene::LoadWorld() noexcept
{
    Vec3 center_from(Real(0), Real(1), Real(1));
    Vec3 center_to(Real(0), Real(1.1), Real(1));
    auto center = [=](RealNum t) { return center_from + t * (center_to - center_from); };
    auto radius = []([[maybe_unused]] RealNum t) { return Real(1); };
    world_.Add(std::make_shared<Sphere<Vec3, RealNum> >(
        Vec3(Real(0), Real(-1000), Real(0)),
        Real(1000),
        std::make_shared<Lambertian>(std::make_shared<CheckerTexture>(
            std::make_shared<ConstantTexture>(Vec3(Real(0.5), Real(0.5), Real(0.5))),
            std::make_shared<ConstantTexture>(Vec3(Real(0.9), Real(0.9), Real(0.9)))))));
    world_.Add(std::make_shared<Sphere<decltype(center), decltype(radius)> >(
        center,
        radius,
        std::make_shared<Lambertian>(std::make_shared<CheckerTexture>(
            std::make_shared<ConstantTexture>(Vec3(Real(0.5), Real(0.5), Real(0.5))),
            std::make_shared<ConstantTexture>(Vec3(Real(0.9), Real(0.9), Real(0.9)))))));
}

}  // namespace plemma::glancy
