#pragma once
#include "DreamVector.h"
namespace Core
{
    class DreamMatrix4f
    {
    public:
        // union允许相同内存空间存储不同数据类型
        // 所有成员共享一块内存，所以一次只能访问其中一个成员，例如同时std::cout会发生问题。
        // 用于节省内存 或者处理需要不同类型解释的数据
        union
        {
            //基本理解成同一块内存，但是不同的数据解释类型，这里就是等价
            float matrix[4][4];
            struct
            {
                // 这里都是按列存储的
                float _11; float _21; float _31; float _41;
                float _12; float _22; float _23; float _24;
                float _13; float _32; float _33; float _34;
                float _14; float _42; float _43; float _44;
            };
        };
    public:
        DreamMatrix4f() = default;
        // 拷贝构造函数
        DreamMatrix4f(const DreamMatrix4f& other) 
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    matrix[i][j] = other.matrix[i][j];
                }
            }
        }
    public:
        //运算符重载与矩阵基本函数
        //单位化
        void Identity();
        //0矩阵
        void Zero();
        //赋值
        DreamMatrix4f& operator= (const DreamMatrix4f& other)
        {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    matrix[i][j] = other.matrix[i][j];
            return *this;
        }
        //判断相等
        bool operator== (const DreamMatrix4f& other) const;
        //加
        DreamMatrix4f operator+ (const DreamMatrix4f& other) const;
        //减
        DreamMatrix4f operator- (const DreamMatrix4f& other) const;
        //矩阵相乘
        DreamMatrix4f operator* (const DreamMatrix4f& other) const;

        //矩阵乘以向量
        DreamVector operator* (const DreamVector& rhs) const;

    };
}