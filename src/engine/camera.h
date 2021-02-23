#pragma once

#include "engine/quat.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    struct Camera
    {
        void sendRotationAndPosition() const;

        void sendPosition() const;

        void sendRotation() const;

        void rotate(const quat& q);

        vec3 location = vec3::Zero();
        quat rotation = quat::Identity();
    };
}
