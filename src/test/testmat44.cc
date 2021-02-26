#include "catch.hpp"
#include "engine/axisangle.h"
#include "engine/mat33.h"
#include "engine/mat44.h"
#include "engine/matrixhelper.h"
#include "engine/vec3.h"
#include "test.h"

using namespace simple;

TEST_CASE("TestTransormation")
{
    CHECK(AreEqual(mat44::FromTranslation(vec3(1, 2, 3)).GetLocation(), vec3(1, 2, 3)));
}

TEST_CASE("TestRight")
{
    CHECK(AreEqual(mat44::Identity().AsMat33().GetXAxis(), vec3::Right()));
}

TEST_CASE("TestUp")
{
    CHECK(AreEqual(mat44::Identity().AsMat33().GetYAxis(), vec3::Up()));
}

TEST_CASE("TestIn")
{
    CHECK(AreEqual(mat44::Identity().AsMat33().GetZAxis(), vec3::In()));
}

TEST_CASE("TestRotation")
{
    vec3 r = mat44::Identity().Help().Rotate(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(-90))).GetTransform(vec3(0, 0, 5));
    CHECK(AreEqual(r, vec3(5, 0, 0)));
}

TEST_CASE("TestCombined")
{
    vec3 r = mat44::Identity().Help().Rotate(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(-90))).Translate(vec3(0, 0, 5)).GetTransform(vec3(0, 0, 0));
    CHECK(AreEqual(r, vec3(5, 0, 0)));
}

TEST_CASE("TestTranslation")
{
    vec3 r = mat44::Identity().Help().Translate(vec3(1, 2, 3)).GetTransform(vec3(7, 8, 9));
    CHECK(AreEqual(r, vec3(8, 10, 12)));
}

TEST_CASE("TestIentityTransform")
{
    vec3 r = mat44::Identity().Help().GetTransform(vec3(1, 2, 3));
    CHECK(AreEqual(r, vec3(1, 2, 3)));
}

TEST_CASE("TestIentityMultiply")
{
    CHECK(AreEqual(mat44::Identity() * mat44::Identity(), mat44::Identity()));
}
