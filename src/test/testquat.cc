#include <cmath>

#include "catch.hpp"
#include "engine/angle.h"
#include "engine/axisangle.h"
#include "engine/mat33.h"
#include "engine/quat.h"
#include "engine/vec3.h"
#include "test.h"

using namespace simple;

quat MakeQuat(float w, float x, float y, float z)
{
    return quat(w, vec3(x, y, z));
}

TEST_CASE("TestCommonRotations")
{
    CHECK(AreEqual(MakeQuat(0, 1, 0, 0), quat(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(180)))));
    CHECK(AreEqual(MakeQuat(0, 0, 1, 0), quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(180)))));
    CHECK(AreEqual(MakeQuat(0, 0, 0, 1), quat(AxisAngle::RightHandAround(vec3::Out(), Angle::FromDegrees(180)))));

    float k = (float)std::sqrt(0.5f);
    CHECK(AreEqual(MakeQuat(k, k, 0, 0), quat(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(90)))));
    CHECK(AreEqual(MakeQuat(k, 0, k, 0), quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(90)))));
    CHECK(AreEqual(MakeQuat(k, 0, 0, k), quat(AxisAngle::RightHandAround(vec3::Out(), Angle::FromDegrees(90)))));

    CHECK(AreEqual(MakeQuat(k, -k, 0, 0), quat(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(-90)))));
    CHECK(AreEqual(MakeQuat(k, 0, -k, 0), quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(-90)))));
    CHECK(AreEqual(MakeQuat(k, 0, 0, -k), quat(AxisAngle::RightHandAround(vec3::Out(), Angle::FromDegrees(-90)))));

    CHECK(AreEqual(MakeQuat(k, k, 0, 0), quat(AxisAngle::RightHandAround(vec3::Left(), Angle::FromDegrees(-90)))));
    CHECK(AreEqual(MakeQuat(k, 0, k, 0), quat(AxisAngle::RightHandAround(vec3::Down(), Angle::FromDegrees(-90)))));
    CHECK(AreEqual(MakeQuat(k, 0, 0, k), quat(AxisAngle::RightHandAround(vec3::In(), Angle::FromDegrees(-90)))));

    CHECK(AreEqual(MakeQuat(k, -k, 0, 0), quat(AxisAngle::RightHandAround(vec3::Left(), Angle::FromDegrees(90)))));
    CHECK(AreEqual(MakeQuat(k, 0, -k, 0), quat(AxisAngle::RightHandAround(vec3::Down(), Angle::FromDegrees(90)))));
    CHECK(AreEqual(MakeQuat(k, 0, 0, -k), quat(AxisAngle::RightHandAround(vec3::In(), Angle::FromDegrees(90)))));
}

TEST_CASE("testMat33")
{
    CHECK(AreEqual(quat::Identity().GetMatrix33().AsQuat(), quat::Identity()));
}

TEST_CASE("testConstants")
{
    CHECK(AreEqual(MakeQuat(1, 0, 0, 0), quat::Identity()));
    CHECK(AreEqual(0, quat::Identity().x));
    CHECK(AreEqual(0, quat::Identity().y));
    CHECK(AreEqual(0, quat::Identity().z));
    CHECK(AreEqual(1, quat::Identity().w));
}

TEST_CASE("testCombine")
{
    CHECK(AreEqual(quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(90))),
                   quat::Combine(quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(90))), quat::Identity())));

    CHECK(AreEqual(quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(90))),
                   quat::Combine(quat::Identity(), quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(90))))));

    CHECK(AreEqual(quat::Combine(quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(-90))),
                                 quat(AxisAngle::RightHandAround(vec3::Out(), Angle::FromDegrees(90)))),
                   quat::Combine(quat(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(90))),
                                 quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(-90))))));
}

TEST_CASE("testLocalAxis")
{
    quat q = quat::Identity();

    CHECK(AreEqual(vec3::Right(), q.Right()));
    CHECK(AreEqual(vec3::Up(), q.Up()));
    CHECK(AreEqual(vec3::In(), q.In()));

    q = quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(90)));
    CHECK(AreEqual(vec3::In(), q.Right()));
    CHECK(AreEqual(vec3::Up(), q.Up()));
    CHECK(AreEqual(vec3::Left(), q.In()));

    q = quat(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(90)));
    CHECK(AreEqual(vec3::Right(), q.Right()));
    CHECK(AreEqual(vec3::Out(), q.Up()));
    CHECK(AreEqual(vec3::Up(), q.In()));

    q = quat(AxisAngle::RightHandAround(vec3::In(), Angle::FromDegrees(90)));
    CHECK(AreEqual(vec3::Down(), q.Right()));
    CHECK(AreEqual(vec3::Right(), q.Up()));
    CHECK(AreEqual(vec3::In(), q.In()));
}

TEST_CASE("testVecOp")
{
    CHECK(AreEqual(vec3(1, 2, 3), quat(4, vec3(1, 2, 3)).GetVec()));
}

TEST_CASE("verifyTestAxisAngle")
{
    CHECK(AreEqual(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(0)), AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(0))));
    CHECK(AreEqual(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(90)), AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(90))));
    CHECK(AreEqual(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(-45)), AxisAngle::RightHandAround(-vec3::Up(), Angle::FromDegrees(45))));
    CHECK(AreEqual(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(90)), AxisAngle::RightHandAround(-vec3::Right(), Angle::FromDegrees(-90))));
}

TEST_CASE("checkAxisAngle")
{
    CHECK(AreEqual(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(90)), quat(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(90))).GetAxisAngle()));
    CHECK(AreEqual(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(-45)), quat(AxisAngle::RightHandAround(-vec3::Up(), Angle::FromDegrees(45))).GetAxisAngle()));
    CHECK(AreEqual(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(90)), quat(AxisAngle::RightHandAround(-vec3::Right(), Angle::FromDegrees(-90))).GetAxisAngle()));
}

quat qa = quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(45)));
quat qb = quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(90)));

TEST_CASE("testSlerp1")
{
    CHECK(AreEqual(quat::Identity(), quat::Slerp(quat::Identity(), 0, qb)));
}
TEST_CASE("testSlerp2")
{
    CHECK(AreEqual(qb, quat::Slerp(quat::Identity(), 1, qb)));
}
TEST_CASE("testSlerp3")
{
    CHECK(AreEqual(quat::Identity(), quat::SlerpShortway(quat::Identity(), 0, qb)));
}
TEST_CASE("testSlerp4")
{
    CHECK(AreEqual(qb, quat::SlerpShortway(quat::Identity(), 1, qb)));
}
TEST_CASE("testSlerp5")
{
    CHECK(AreEqual(qa, quat::SlerpShortway(quat::Identity(), 0.5f, qb)));
}
