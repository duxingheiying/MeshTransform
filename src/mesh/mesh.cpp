#include "mesh.h"

#include <fstream>
#include <sstream>

namespace mesh {
    using linear_algebra::Matrix4x4;
    using linear_algebra::Vector3;

    void Mesh::apply_transform(const Matrix4x4& matrix) {
        for (auto& v : vertices_) {
            Vector3 p(v.x_, v.y_, v.z_);
            v = matrix * p;
        }
    }

}  // namespace mesh
