#include "catch.hpp"
#include "engine/angle.h"

using namespace SimpleEngine;

constexpr float fe = 0.0000001f;

TEST_CASE("TestSinCos")
{
    CHECK(angle::FromDegrees(90).Sin() == Approx(1));
    CHECK(angle::FromDegrees(90).Cos() == Approx(0).margin(fe));
    CHECK(angle::FromDegrees(0).Sin() == Approx(0));
    CHECK(angle::FromDegrees(0).Cos() == Approx(1));
}

TEST_CASE("TestA")
{
    CHECK(angle::Asin(1).inDegrees() == Approx(90));
    CHECK(angle::Acos(0).inDegrees() == Approx(90));
    CHECK(angle::Asin(0).inDegrees() == Approx(0));
    CHECK(angle::Acos(1).inDegrees() == Approx(0));
}
