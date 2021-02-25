#pragma once

#include "engine/quat.h"
#include "engine/vec3.h"

namespace SimpleEngine
{
    struct Camera
    {
        void SendRotationAndPosition() const;

        void SendPosition() const;

        void SendRotation() const;

        void Rotate(const quat& q);

        vec3 location = vec3::Zero();
        quat rotation = quat::Identity();
    };
}
