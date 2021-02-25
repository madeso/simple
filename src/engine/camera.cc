#include "engine/camera.h"

#include "engine/axisangle.h"
#include "engine/opengl.h"

namespace SimpleEngine
{
    void Camera::SendRotationAndPosition() const
    {
        SendRotation();
        SendPosition();
    }

    void Camera::SendPosition() const
    {
        // -position
        glTranslatef(-location.x, -location.y, -location.z);
    }

    void Camera::SendRotation() const
    {
        // -rotatation
        const auto aa = rotation.GetAxisAngle();
        glRotatef(-aa.angle.inDegrees(), aa.axis.x, aa.axis.y, aa.axis.z);
    }

    void Camera::Rotate(const quat& q)
    {
        rotation = quat::Combine(rotation, q);
    }
}
