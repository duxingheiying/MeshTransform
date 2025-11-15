#include "mesh.h"

#include <fstream>
#include <sstream>

namespace mesh {
    using linear_algebra::Matrix4x4;
    using linear_algebra::Vector3;

    void Mesh::apply_transform(const Matrix4x4& matrix) {
        // transform all vertex
        for (auto& v : vertices_) {
            Vector3 p(v.x_, v.y_, v.z_);
            v = matrix * p;
        }
        // transform all normal
        for (auto& vu : normals_) {
            Vector3 p(vu.x_, vu.y_, vu.z_);
            vu = matrix * p;
            vu = vu.normalized();
        }
    }

    void Mesh::append(const Mesh& other) {
        const unsigned int offset = vertices_.size();
        for (const auto& vec : other.vertices_) {
            vertices_.push_back(vec);
        }
        for (const auto& vt : other.texcoords_) {
            texcoords_.push_back(vt);
        }

        for (const auto& vn : other.normals_) {
            normals_.push_back(vn);
        }

        for (const auto& face : other.faces_) {
            Face temp_face = face;
            for (auto& v_idx : temp_face.vIdx_) {
                v_idx = v_idx < 0 ? v_idx : v_idx + offset;
            }
            for (auto& vt_idx : temp_face.vtIdx_) {
                vt_idx = vt_idx < 0 ? vt_idx : vt_idx + offset;
            }
            for (auto& vn_idx : temp_face.vnIdx_) {
                vn_idx = vn_idx < 0 ? vn_idx : vn_idx + offset;
            }
            faces_.push_back(temp_face);
        }
    }
}  // namespace mesh
