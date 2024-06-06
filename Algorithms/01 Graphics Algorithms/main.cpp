#include <iostream>

class Vector3f
{
public:
    float x, y, z;
    Vector3f(){}
    Vector3f(float a,float b,float c):x(a),y(b),z(c){}
    Vector3f operator-(const Vector3f& other) const
    {
        return Vector3f(x - other.x, y - other.y, z - other.z);
    }


};


Vector3f cross(const Vector3f& other1, const Vector3f& other2)
{
    //TODO to be written
    return Vector3f();
}

float dot(const Vector3f& other1, const Vector3f& other2)
{
    float res = 0;
    //TODO to be written
    return res;
}

// Ray Intersect with triangle
// 1. traditional strategy: ray intersect the plane of the triangle at point p, then determine whether point p is inside the triangle.
// 2. Moller-Trumbore algorithm: 直接联立方程得到结果
// 补充：一般来说，重心坐标可以用三个顶点得到，也可以用barycentric coordinate来表示(alpha,beta,gamma),alpha + beta + gamma = 1
bool IntersectWithTriangle(const Vector3f& p0, const Vector3f& p1, const Vector3f& p2, const Vector3f& origin, const Vector3f& ray)
{
    //1. alpha>0,beta>0,gamma>0 => p is in the triangle plane
    //2. ray intersect time t >0 => p is in the triangle

    //equation: origin + ray * t = （1 - b1 - b2) * P0 + b1 * P1 + b2 * P2;
    // 已知，origin,ray,P0,P1,P2。求t,b1,b2。
    // 1)origin - P0 = (P1 - P0) * b1 + (P2 - P0) * b2 - ray * t
    // 2)令P1 - P0 = E1，P2 - P0 = E2，S = origin - P0,则 S = (t,b1,b2)(-ray,E1,E2),问题转变成Ax = c,其中x = (t,b1,b2)。A = (-ray,E1,E2),令ray = D
    // 3)克莱姆法则（cramer's rule）,在系数矩阵非奇异（即其行列式不为零）时，可以用于直接计算线性方程组的解,xi = det(Ai)/det(A),det为行列式。其中Ai为A的第i列换成c
    // 三阶行列式与混合积的关系det(a,b,c) = a *(bxc),其中，abc都为列向量,混合积具有轮换对称性a * (bxc) = b * (cxa) = c * (axb)
    // 这里det(-D,E1,E2) = -D * (E1xE2) = E1 * (D x E2)
    //  t = det(S,E1,E2)/det(-D,E1,E2)
    //  b1 = det(-ray,S,E2)/det(-D,E1,E2)
    //  b2 = det(-ray,E1,S)/det(-D,E1,E2)
    Vector3f E1 = p1 - p0, E2 = p2 - p0, S = origin - p0;
    Vector3f S1 = cross(ray, E2), S2 = cross(S, E1);
    float det_A = dot(S1, E1);
    // t
    float t = dot(S2, E2) / det_A;
    // b1&b2&b3
    float b1 = dot(S1, S) / det_A;
    float b2 = dot(cross(ray, S), E1) / det_A;// det(-ray,E1,S) = dot(E1,cross(ray,S)) = ray * (S x E1) = ray * S2
    float b3 = 1 - b1 - b2;
    return b1 >= 0 && b2 >= 0 && b3 >= 0 && t >= 0;
}

// 如何判断点在三角形内
// 1. 重心法：AP = alpha * AB + beta * AC
// 2. 同向法，右手定理，三角形ABC, 点P；
// AB x AP, BC x BP, CA x CP，结果乘积大于0，在内，小于0在外，等于0，在边上
// 3. 内角和180
int main()
{
    system("pause:");
    return 0;
}