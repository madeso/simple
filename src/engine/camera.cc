#include "engine/camera.h"

#include "engine/axisangle.h"
#include "engine/opengl.h"

namespace SimpleEngine
{
    void Camera::sendRotationAndPosition() const
    {
        sendRotation();
        sendPosition();
    }

    void Camera::sendPosition() const
    {
        // -position
        glTranslatef(-location.x, -location.y, -location.z);
    }

    void Camera::sendRotation() const
    {
        // -rotatation
        const auto aa = rotation.AxisAngle();
        glRotatef(-aa.angle.inDegrees(), aa.axis.x, aa.axis.y, aa.axis.z);
    }

    void Camera::rotate(const quat& q)
    {
        rotation = quat::Combine(rotation, q);
    }
}
