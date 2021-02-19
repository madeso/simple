#include "catch.hpp"
#include "engine/axisangle.h"
#include "engine/mat33.h"
#include "engine/mat44.h"
#include "engine/matrixhelper.h"
#include "engine/vec3.h"
#include "test.h"

using namespace SimpleEngine;

TEST_CASE("TestTransormation")
{
    CHECK(AreEqual(mat44::TranslationFor(vec3(1, 2, 3)).Location(), vec3(1, 2, 3)));
}

TEST_CASE("TestRight")
{
    CHECK(AreEqual(mat44::Identity().mat33().XAxis(), vec3::Right()));
}

TEST_CASE("TestUp")
{
    CHECK(AreEqual(mat44::Identity().mat33().YAxis(), vec3::Up()));
}

TEST_CASE("TestIn")
{
    CHECK(AreEqual(mat44::Identity().mat33().ZAxis(), vec3::In()));
}

TEST_CASE("TestRotation")
{
    vec3 r = mat44::Identity().Help().Rotate(AxisAngle::RightHandAround(vec3::Up(), angle::FromDegrees(-90))).transform(vec3(0, 0, 5));
    CHECK(AreEqual(r, vec3(5, 0, 0)));
}

TEST_CASE("TestCombined")
{
    vec3 r = mat44::Identity().Help().Rotate(AxisAngle::RightHandAround(vec3::Up(), angle::FromDegrees(-90))).Translate(vec3(0, 0, 5)).transform(vec3(0, 0, 0));
    CHECK(AreEqual(r, vec3(5, 0, 0)));
}

TEST_CASE("TestTranslation")
{
    vec3 r = mat44::Identity().Help().Translate(vec3(1, 2, 3)).transform(vec3(7, 8, 9));
    CHECK(AreEqual(r, vec3(8, 10, 12)));
}

TEST_CASE("TestIentityTransform")
{
    vec3 r = mat44::Identity().Help().transform(vec3(1, 2, 3));
    CHECK(AreEqual(r, vec3(1, 2, 3)));
}

TEST_CASE("TestIentityMultiply")
{
    CHECK(AreEqual(mat44::Identity() * mat44::Identity(), mat44::Identity()));
}
