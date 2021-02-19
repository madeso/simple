#pragma once

#include "engine/quat.h"

namespace SimpleEngine
{
    struct vec2;
    struct Window;

    struct Rotation
    {
        virtual ~Rotation() = default;

        virtual void sendMouse(const vec2& mouse, const vec2& oldmouse) = 0;
        virtual void rotateGl() = 0;

        float sens = 0.25f;
    };

    struct QuatRot : public Rotation
    {
        quat rotation = quat::Identity();

        void rotateGl() override;
    };

    struct EasyRotation : public Rotation
    {
        float rx = 0;
        float ry = 0;

        void sendMouse(const vec2& mouse, const vec2& oldmouse) override;

        void rotateGl() override;
    };

    struct BasicQuatRot : public QuatRot
    {
        void sendMouse(const vec2& current, const vec2& oldmouse) override;
    };

    struct ArcBallRotation : QuatRot
    {
        Window* ba;

        ArcBallRotation(Window* owner);

        float Width();
        float Height();

        vec2 tranform(const vec2& v);

        void sendMouse(const vec2& current, const vec2& oldmouse) override;
    };
}
