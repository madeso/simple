#pragma once

#include "engine/quat.h"

namespace simple
{
    struct vec2;
    struct Window;

    struct Rotation
    {
        virtual ~Rotation() = default;

        virtual void SendMouse(const vec2& mouse, const vec2& oldmouse) = 0;
        virtual void RotateOpenGl() = 0;

        float sensitivity = 0.25f;
    };

    struct QuatRotation : public Rotation
    {
        quat rotation = quat::Identity();

        void RotateOpenGl() override;
    };

    struct EasyRotation : public Rotation
    {
        float rx = 0;
        float ry = 0;

        void SendMouse(const vec2& mouse, const vec2& oldmouse) override;

        void RotateOpenGl() override;
    };

    struct BasicQuatRotation : public QuatRotation
    {
        void SendMouse(const vec2& current, const vec2& oldmouse) override;
    };

    struct ArcBallRotation : QuatRotation
    {
        Window* ba;

        ArcBallRotation(Window* owner);

        void SendMouse(const vec2& current, const vec2& old_mouse) override;
    };
}
