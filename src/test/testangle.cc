#include "catch.hpp"
#include "engine/angle.h"

using namespace SimpleEngine;

constexpr float fe = 0.0000001f;

TEST_CASE("TestSinCos")
{
    CHECK(Angle::FromDegrees(90).Sin() == Approx(1));
    CHECK(Angle::FromDegrees(90).Cos() == Approx(0).margin(fe));
    CHECK(Angle::FromDegrees(0).Sin() == Approx(0));
    CHECK(Angle::FromDegrees(0).Cos() == Approx(1));
}

TEST_CASE("TestA")
{
    CHECK(Angle::Asin(1).inDegrees() == Approx(90));
    CHECK(Angle::Acos(0).inDegrees() == Approx(90));
    CHECK(Angle::Asin(0).inDegrees() == Approx(0));
    CHECK(Angle::Acos(1).inDegrees() == Approx(0));
}
