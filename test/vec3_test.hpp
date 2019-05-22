#pragma once

#include <iostream>
#include <limits>
#include <memory>
#include <random>

#include <catch2/catch.hpp>
#include "vec3.hpp"
#include "rand_engine.hpp"


namespace plemma {

namespace glancy {

constexpr RealNum kToleranceEqualityCheck = Real(1e-4);
constexpr RealNum kMinValueToDivide = Real(1e-7);
constexpr RealNum kMaxRandomGeneration = Real(1e4);
constexpr RealNum kMinRandomGeneration = Real(-1e4);

constexpr auto CanVec3BeUsedToDivide = [](Vec3 const& v) {
        return v.X() > kMinValueToDivide
            && v.Y() > kMinValueToDivide
            && v.Z() > kMinValueToDivide; };

constexpr auto CanScalarBeUsedToDivide = [](RealNum const& k) {
        return k > kMinValueToDivide; };

class Vec3RandomGenerator : public Catch::Generators::IGenerator<Vec3>
{
public:
    Vec3RandomGenerator() :
        dist_(kMinRandomGeneration, kMaxRandomGeneration)
    {
        static_cast<void>(next());
    }

    Vec3RandomGenerator(RealNum min, RealNum max) :
        dist_(min, max)
    {
        static_cast<void>(next());
    }

    Vec3 const& get() const override;
    bool next() override
    {
        current_ = Vec3(dist_(rand_engine()), dist_(rand_engine()), dist_(rand_engine()));
        return true;
    }
private:
    std::default_random_engine& rand_engine()
    {
        static std::default_random_engine eng(Catch::rngSeed());

        return eng;
    }
    Vec3 current_;
    std::uniform_real_distribution<RealNum> dist_;
};

inline Vec3 const& Vec3RandomGenerator::get() const
{
    return current_;
}

inline Catch::Generators::GeneratorWrapper<Vec3> RandomFiniteVec3()
{
    return Catch::Generators::GeneratorWrapper<Vec3>(std::make_unique<Vec3RandomGenerator>());
}

inline Catch::Generators::GeneratorWrapper<Vec3> RandomFiniteVec3(RealNum min, RealNum max)
{
    return Catch::Generators::GeneratorWrapper<Vec3>(std::make_unique<Vec3RandomGenerator>(min, max));
}

} // namespace glancy

} // namespace plemma
