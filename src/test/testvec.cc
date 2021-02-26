#include "catch.hpp"
#include "engine/vec3.h"
#include "test.h"

using namespace simple;

TEST_CASE("testX")
{
    CHECK(AreEqual(vec3(1, 0, 0), vec3(1, 0, 0)));
    CHECK(AreEqual(vec3(2, 0, 0), vec3(1, 0, 0) * 2));
    CHECK(AreEqual(vec3(2, 0, 0), vec3(1, 0, 0) + vec3(1, 0, 0)));
}

TEST_CASE("testY")
{
    CHECK(AreEqual(vec3(0, 1, 0), vec3(0, 1, 0)));
    CHECK(AreEqual(vec3(0, 2, 0), vec3(0, 1, 0) * 2));
    CHECK(AreEqual(vec3(0, 2, 0), vec3(0, 1, 0) + vec3(0, 1, 0)));
}

TEST_CASE("testZ")
{
    CHECK(AreEqual(vec3(0, 0, 1), vec3(0, 0, 1)));
    CHECK(AreEqual(vec3(0, 0, 2), vec3(0, 0, 1) * 2));
    CHECK(AreEqual(vec3(0, 0, 2), vec3(0, 0, 1) + vec3(0, 0, 1)));
}

TEST_CASE("testDot")
{
    CHECK(AreEqual(0, vec3::dot(vec3(1, 0, 0), vec3(0, 1, 0))));
    CHECK(AreEqual(1, vec3::dot(vec3(1, 0, 0), vec3(1, 0, 0))));
}

TEST_CASE("testCross")
{
    CHECK(AreEqual(vec3(0, 1, 0), vec3::cross(vec3(0, 0, -1), vec3(-1, 0, 0))));
}

TEST_CASE("testInterpolate")
{
    CHECK(AreEqual(vec3(0, 1, 0), vec3::Interpolate(vec3(0, 1, 0), 0, vec3(2, 4, 6))));
    CHECK(AreEqual(vec3(2, 4, 6), vec3::Interpolate(vec3(0, 1, 0), 1, vec3(2, 4, 6))));
    CHECK(AreEqual(vec3(3, 5, 7), vec3::Interpolate(vec3(2, 4, 6), 0.5f, vec3(4, 6, 8))));
}
