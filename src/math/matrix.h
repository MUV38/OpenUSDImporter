#pragma once

#include "mat4.h"
#include "vec3.h"

namespace matrix
{
    inline vec3 transformCoord(const mat4& matrix, const vec3& vector)
    {
        const float x =
            matrix(0, 0) * vector.x +
            matrix(0, 1) * vector.y +
            matrix(0, 2) * vector.z +
            matrix(0, 3);
        const float y =
            matrix(1, 0) * vector.x +
            matrix(1, 1) * vector.y +
            matrix(1, 2) * vector.z +
            matrix(1, 3);
        const float z =
            matrix(2, 0) * vector.x +
            matrix(2, 1) * vector.y +
            matrix(2, 2) * vector.z +
            matrix(2, 3);
        return vec3(x, y, z);
    }
}
