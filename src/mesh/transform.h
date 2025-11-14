#ifndef MESH_TRANSFORM_H_
#define MESH_TRANSFORM_H_

#include <array>
#include <cmath>
#include <iostream>

namespace linear_algebra {
    // 二维维向量
    struct Vector2 {
        union {
            double x_;
            double u_;
        };
        union {
            double y_;
            double v_;
        };
		Vector2() : x_(0), y_(0) {}
        Vector2(double x, double y) : x_(x), y_(y) {}

        Vector2 operator+(const Vector2& rhs) const {
            return { x_ + rhs.x_, y_ + rhs.y_};
        } 

        Vector2 operator-(const Vector2& rhs) const {
			return { x_ - rhs.x_, y_ - rhs.y_ };
        }

        // scale vec2
        Vector2 operator*(double s) const { return { x_ * s, y_ * s }; }

        // vec2 dot
        double dot(const Vector2& rhs) const { return x_ * rhs.x_ + y_ * rhs.y_; }

        // vec2 cross
        double cross(const Vector2& rhs) const {
            return { this->x_ * rhs.y_ - this->y_ * rhs.x_};
        }

        Vector2 normalized() const {
            double len = std::sqrt(std::pow( x_, 2) + std::pow(y_, 2));
            if (len < 1e-9) return { 0, 0};
            return { x_ / len, y_ / len };
        }
    };

    // 三维向量
    struct Vector3 {
        double x_, y_, z_;
        Vector3() : x_(0), y_(0), z_(0) {}
        Vector3(double x, double y, double z) : x_(x), y_(y), z_(z) {}

        Vector3 operator+(const Vector3& rhs) const {
            return { x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_ };
        }

        Vector3 operator-(const Vector3& rhs) const {
            return { x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_ };
        }

        Vector3 operator*(double s) const { return { x_ * s, y_ * s, z_ * s }; }

        double dot(const Vector3& rhs) const { return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_; }

        Vector3 cross(const Vector3& rhs) const {
            return { y_ * rhs.z_ - z_ * rhs.y_, z_ * rhs.x_ - x_ * rhs.z_, x_ * rhs.y_ - y_ * rhs.x_ };
        }

        Vector3 normalized() const {
            double len = std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
            if (len < 1e-9) return { 0, 0, 0 };
            return { x_ / len, y_ / len, z_ / len };
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
