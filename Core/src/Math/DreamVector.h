#pragma once
namespace Core
{
    class DreamVector
    {
    public:
        float x;
        float y;
        float z;
        float w;	//0代表向量 1代表点

    public:
        DreamVector() = default;
        DreamVector(float x1, float y1, float z1, float w1 = 0) :x(x1), y(y1), z(z1), w(w1) {}
        DreamVector(const DreamVector& rhs) :x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w) {}
        DreamVector& operator= (const DreamVector& rhs)
        {
            if (this == &rhs)
                return *this;
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
            return *this;
        }

    public:
        //向量长度
        float Length() const;
        //归一化
        DreamVector Normalize();
        //点乘
        float Dot(DreamVector v) const;
        //叉乘
        DreamVector Cross(DreamVector rhs) const;
        //重载== 
        bool operator==(const DreamVector& rhs) const;
        //与变换矩阵相乘,由于是按列存储，所以为了保证左乘，把该函数放到Matrix里
        //DreamVector operator* (const DreamMatrix4f& rhs) const;
        //与DreamVector分量相乘
        DreamVector operator* (const DreamVector& rhs) const;
        //与float相乘
        DreamVector operator*(float factor) const;
        //加
        DreamVector operator+ (const DreamVector& rhs) const;
        //减
        DreamVector operator- (const DreamVector& rhs) const;
        //负号
        DreamVector operator-() const;

    };
}