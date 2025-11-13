#ifndef MESH_TRANSFORM_H_
#define MESH_TRANSFORM_H_

#include <array>
#include <cmath>
#include <iostream>

namespace linear_algebra {
    // 三维向量
    struct Vector2 {
        double x, y;
		Vector2() : x(0), y(0) {}
        Vector2(double _x, double _y) : x(_x), y(_y) {}

        Vector2 operator+(const Vector2& rhs) const {
            return { x + rhs.x, y + rhs.y};
        }

        Vector2 operator-(const Vector2& rhs) const {
			return { x - rhs.x, y - rhs.y };
        }

        // 二维向量放缩
        Vector2 operator*(double s) const { return { x * s, y * s }; }

        // 二维向量点成
        double dot(const Vector3& rhs) const { return x * rhs.x + y * rhs.y; }

        // 二维向量叉乘
        double cross(const Vector2& rhs) const {
            return { this->x * rhs.y - this->y * rhs.x};
        }

        Vector2 normalized() const {
            double len = std::sqrt(x * x + y * y);
            if (len < 1e-9) return { 0, 0};
            return { x / len, y / len };
        }
    };

    // 三维向量
    struct Vector3 {
        double x, y, z;
        Vector3() : x(0), y(0), z(0) {}
        Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

        Vector3 operator+(const Vector3& rhs) const {
            return { x + rhs.x, y + rhs.y, z + rhs.z };
        }

        Vector3 operator-(const Vector3& rhs) const {
            return { x - rhs.x, y - rhs.y, z - rhs.z };
        }

        Vector3 operator*(double s) const { return { x * s, y * s, z * s }; }

        double dot(const Vector3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }

        Vector3 cross(const Vector3& rhs) const {
            return { y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x };
        }

        Vector3 normalized() const {
            double len = std::sqrt(x * x + y * y + z * z);
            if (len < 1e-9) return { 0, 0, 0 };
            return { x / len, y / len, z / len };
        }
    };

    // 4x4 矩阵
    class Matrix4x4 {
    public:
        explicit Matrix4x4();

        // 单位矩阵
        static Matrix4x4 Identity();

        // 矩阵乘法
        Matrix4x4 operator*(const Matrix4x4& rhs) const;

        // 向量变换 (假设 w=1)
        Vector3 operator*(const Vector3& v) const;

        const std::array<double, 16>& data() const { return data_; }

        // 工具函数：生成常见变换矩阵
        static Matrix4x4 Translate(double tx, double ty, double tz);
        static Matrix4x4 Scale(double s);
        static Matrix4x4 ScaleNonUniform(double sx, double sy, double sz);
        static Matrix4x4 RotateX(double angle);
        static Matrix4x4 RotateY(double angle);
        static Matrix4x4 RotateZ(double angle);
        static Matrix4x4 RotateAroundAxis(const Vector3& axis, double angle);
        static Matrix4x4 Shear(double sxy, double sxz, double syx, double syz, double szx, double szy);

    private:
        std::array<double, 16> data_;  // 行主序存储
    };
}  // namespace mesh

#endif  // MESH_TRANSFORM_H_
