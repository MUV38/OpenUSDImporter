#pragma once

class mat4
{
public:
    mat4()
        : m{
              1.0f, 0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f, 0.0f,
              0.0f, 0.0f, 1.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 1.0f
          }
    {
    }

    mat4(const float* v)
        : m{
              v[0],  v[1],  v[2],  v[3],
              v[4],  v[5],  v[6],  v[7],
              v[8],  v[9],  v[10], v[11],
              v[12], v[13], v[14], v[15]
          }
    {
    }

    mat4(float m00, float m01, float m02, float m03,
         float m10, float m11, float m12, float m13,
         float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33)
        : m{
              m00, m01, m02, m03,
              m10, m11, m12, m13,
              m20, m21, m22, m23,
              m30, m31, m32, m33
          }
    {
    }

    float& operator()(int row, int column)
    {
        return m[row * 4 + column];
    }

    float operator()(int row, int column) const
    {
        return m[row * 4 + column];
    }

    float m[16];
};
