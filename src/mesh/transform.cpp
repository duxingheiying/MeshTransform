#include "transform.h"

#include <cmath>

namespace linear_algebra {

    Matrix4x4::Matrix4x4() {
        data_.fill(0.0);
        data_[0] = data_[5] = data_[10] = data_[15] = 1.0;
    }

    Matrix4x4 Matrix4x4::Identity() { return Matrix4x4(); }

    Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
        Matrix4x4 result;
        result.data_.fill(0.0);

        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                for (int k = 0; k < 4; ++k) {
                    result.data_[row * 4 + col] += data_[row * 4 + k] * rhs.data_[k * 4 + col];
                }
            }
        }
        return result;
    }

    Vector3 Matrix4x4::operator*(const Vector3& v) const {
        double x = data_[0] * v.x + data_[1] * v.y + data_[2] * v.z + data_[3];
        double y = data_[4] * v.x + data_[5] * v.y + data_[6] * v.z + data_[7];
        double z = data_[8] * v.x + data_[9] * v.y + data_[10] * v.z + data_[11];
        double w = data_[12] * v.x + data_[13] * v.y + data_[14] * v.z + data_[15];
        if (std::abs(w) < 1e-9) w = 1.0;
        return { x / w, y / w, z / w };
    }

    // Æ½ÒÆ
    Matrix4x4 Matrix4x4::Translate(double tx, double ty, double tz) {
        Matrix4x4 m;
        m = Matrix4x4::Identity();
        m.data_[3] = tx;
        m.data_[7] = ty;
        m.data_[11] = tz;
        return m;
    }

    // ¾ùÔÈËõ·Å
    Matrix4x4 Matrix4x4::Scale(double s) {
        Matrix4x4 m;
        m.data_[0] = m.data_[5] = m.data_[10] = s;
        m.data_[15] = 1.0;
        return m;
    }

    // ·Ç¾ùÔÈËõ·Å
    Matrix4x4 Matrix4x4::ScaleNonUniform(double sx, double sy, double sz) {
        Matrix4x4 m;
        m.data_[0] = sx;
        m.data_[5] = sy;
        m.data_[10] = sz;
        m.data_[15] = 1.0;
        return m;
    }

    // ÈÆ X ÖáÐý×ª
 	Matrix4x4 Matrix4x4::RotateX(double angle) {
		Matrix4x4 m = Matrix4x4::Identity();
		double c = std::cos(angle);
		double s = std::sin(angle);
		m.data_[5] = c;
		m.data_[6] = -s;
		m.data_[9] = s;
		m.data_[10] = c;
		return m;
	}

    // ÈÆ Y ÖáÐý×ª
    Matrix4x4 Matrix4x4::RotateY(double angle) {
        Matrix4x4 m = Matrix4x4::Identity();
        double c = std::cos(angle), s = std::sin(angle);
        m.data_[0] = c;
        m.data_[2] = s;
        m.data_[8] = -s;
        m.data_[10] = c;
        return m;
    }

    // ÈÆ Z ÖáÐý×ª
    Matrix4x4 Matrix4x4::RotateZ(double angle) {
        Matrix4x4 m = Matrix4x4::Identity();
        double c = std::cos(angle), s = std::sin(angle);
        m.data_[0] = c;
        m.data_[1] = -s;
        m.data_[4] = s;
        m.data_[5] = c;
        return m;
    }

    // ÈÆÈÎÒâÖáÐý×ª
    Matrix4x4 Matrix4x4::RotateAroundAxis(const Vector3& axis, double angle) {
        Vector3 n = axis.normalized();
        double c = std::cos(angle);
        double s = std::sin(angle);
        double t = 1 - c;

        Matrix4x4 m;
        m.data_ = {
            t * n.x * n.x + c, t * n.x * n.y - s * n.z, t * n.x * n.z + s * n.y, 0,
            t * n.x * n.y + s * n.z, t * n.y * n.y + c, t * n.y * n.z - s * n.x, 0,
            t * n.x * n.z - s * n.y, t * n.y * n.z + s * n.x, t * n.z * n.z + c, 0,
            0, 0, 0, 1 };
        return m;
    }

    // ¼ôÇÐ¾ØÕó
    Matrix4x4 Matrix4x4::Shear(double sxy, double sxz, double syx, double syz, double szx, double szy) {
        Matrix4x4 m = Matrix4x4::Identity();
        m.data_[1] = sxy;
        m.data_[2] = sxz;
        m.data_[4] = syx;
        m.data_[6] = syz;
        m.data_[8] = szx;
        m.data_[9] = szy;
        return m;
    }

}  // namespace mesh
