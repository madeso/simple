#include "engine/rotation.h"

#include "engine/angle.h"
#include "engine/arcball.h"
#include "engine/axisangle.h"
#include "engine/opengl.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/window.h"

namespace simple
{
    void QuatRotation::RotateOpenGl()
    {
        const auto aa = rotation.GetAxisAngle();
        glRotatef(aa.angle.InDegrees(), aa.axis.x, aa.axis.y, aa.axis.z);
    }

    void EasyRotation::SendMouse(const vec2& mouse, const vec2& oldmouse)
    {
        float dx = (mouse.x - oldmouse.x);
        float dy = -(mouse.y - oldmouse.y);
        float sens = 0.1f;
        rx += sens * dx;
        ry += sens * dy;
    }

    void EasyRotation::RotateOpenGl()
    {
        glRotatef(-ry, 1, 0, 0);
        glRotatef(rx, 0, 1, 0);
    }

    void BasicQuatRotation::SendMouse(const vec2& current, const vec2& oldmouse)
    {
        auto diff = vec2::FromTo(oldmouse, current);
        float dx = diff.x * sensitivity;
        float dy = diff.y * sensitivity;
        const auto rx = quat(AxisAngle::RightHandAround(vec3::Up(), Angle::FromDegrees(dx)));
        const auto ry = quat(AxisAngle::RightHandAround(vec3::Right(), Angle::FromDegrees(dy)));
        const auto final = rx * ry;
        rotation = quat::Combine(rotation, final);
    }

    ArcBallRotation::ArcBallRotation(Window* owner)
        : ba(owner)
    {
    }

    void ArcBallRotation::SendMouse(const vec2& current, const vec2& old_mouse)
    {
        const auto height = static_cast<float>(ba->Height);
        const auto width = static_cast<float>(ba->Width);
        const auto transform = [height](const vec2& v) {
            return vec2(v.x, height - v.y);
        };

        const auto ball = ArcBall(vec2(width / 2.0f, height / 2.0f), std::min(height, width) / 2.0f);
        const auto final = ball.GetRotation(transform(old_mouse), transform(current));

        rotation = quat::Combine(rotation, final);
    }
}
