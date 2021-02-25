#include "engine/rotation.h"

#include "engine/angle.h"
#include "engine/arcball.h"
#include "engine/axisangle.h"
#include "engine/opengl.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/window.h"

namespace SimpleEngine
{
    void QuatRot::rotateGl()
    {
        const auto aa = rotation.AxisAngle();
        glRotatef(aa.angle.inDegrees(), aa.axis.x, aa.axis.y, aa.axis.z);
    }

    void EasyRotation::sendMouse(const vec2& mouse, const vec2& oldmouse)
    {
        float dx = (mouse.x - oldmouse.x);
        float dy = -(mouse.y - oldmouse.y);
        float sens = 0.1f;
        rx += sens * dx;
        ry += sens * dy;
    }

    void EasyRotation::rotateGl()
    {
        glRotatef(-ry, 1, 0, 0);
        glRotatef(rx, 0, 1, 0);
    }

    void BasicQuatRot::sendMouse(const vec2& current, const vec2& oldmouse)
    {
        auto diff = vec2::FromTo(oldmouse, current);
        float dx = diff.x * sens;
        float dy = diff.y * sens;
        const auto rx = quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(dx)));
        const auto ry = quat(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(dy)));
        const auto final = rx * ry;
        rotation = quat::Combine(rotation, final);
    }

    ArcBallRotation::ArcBallRotation(Window* owner)
        : ba(owner)
    {
    }

    float ArcBallRotation::Width()
    {
        return ba->Width;
    }
    float ArcBallRotation::Height()
    {
        return ba->Height;
    }

    vec2 ArcBallRotation::tranform(const vec2& v)
    {
        return vec2(v.x, Height() - v.y);
    }

    void ArcBallRotation::sendMouse(const vec2& current, const vec2& oldmouse)
    {
        ArcBall ball = ArcBall(vec2(Width() / 2.0f, Height() / 2.0f), std::min(Height(), Width()) / 2.0f);
        quat final = ball.rotation(tranform(oldmouse), tranform(current));

        rotation = quat::Combine(rotation, final);
    }
}
