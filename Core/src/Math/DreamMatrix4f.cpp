#include <cmath>
#include "DreamMatrix4f.h"

namespace Core
{
    void DreamMatrix4f::Identity()
    {
        _11 = 1.f; _12 = 0.f; _13 = 0.f; _14 = 0.f;
        _21 = 0.f; _22 = 1.f; _23 = 0.f; _24 = 0.f;
        _31 = 0.f; _32 = 0.f; _33 = 1.f; _34 = 0.f;
        _41 = 0.f; _42 = 0.f; _43 = 0.f; _44 = 1.f;
    }

    void DreamMatrix4f::Zero()
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                matrix[i][j] = 0.0f;
    }

    // m1 == m2
    bool DreamMatrix4f::operator==(const DreamMatrix4f& other) const
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (abs(matrix[i][j] - other.matrix[i][j]) >= 0.000001f)
                    return false;
        return true;
    }

    //m1 = m2 + m3
    DreamMatrix4f DreamMatrix4f::operator+(const DreamMatrix4f& other) const
    {
        DreamMatrix4f res;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                res.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
        return res;
    }

    //m1 = m2 - m3;
    DreamMatrix4f DreamMatrix4f::operator-(const DreamMatrix4f& other) const
    {
        DreamMatrix4f res;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                res.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
        return res;
    }

    //m1 = m2 * m3
    // 需注意我们是按列存储的，所以变换矩阵在左边
    DreamMatrix4f DreamMatrix4f::operator*(const DreamMatrix4f& other) const
    {
        DreamMatrix4f res;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
            {
                res.matrix[i][j] = (matrix[i][0] * other.matrix[0][i]) +
                    (matrix[i][1] * other.matrix[1][i]) +
                    (matrix[i][2] * other.matrix[2][i]) +
                    (matrix[i][3] * other.matrix[3][i]);
            }
        return res;
    }
    // 矩阵乘以向量,rhs,right hand side
    DreamVector DreamMatrix4f::operator* (const DreamVector& rhs) const
    {
        DreamVector res;
        for (int i = 0; i < 4; i++)
        {
            res.x = matrix[i][0] * rhs.x + matrix[i][1] * rhs.y + matrix[i][2] * rhs.z + matrix[i][3] * rhs.w;
        }
        return res;
    }

}