#include "mesh.h"

#include <fstream>
#include <sstream>

namespace mesh {
    using linear_algebra::Matrix4x4;
    using linear_algebra::Vector3;
    //bool Mesh::load_from_simple_obj(const std::string& filename) {
    //    std::ifstream in(filename);
    //    if (!in.is_open()) {
    //        std::cerr << "Failed to open OBJ file: " << filename << "\n";
    //        return false;
    //    }

    //    vertices_.clear();
    //    other_info_str_list_.clear();
    //    std::string line;
    //    while (std::getline(in, line)) {
    //        std::istringstream iss(line);
    //        std::string type;
    //        iss >> type;
    //        if (type == "v") {
    //            Vertex v;
    //            iss >> v.x >> v.y >> v.z;
    //            vertices_.push_back(v);
    //        }
    //        else {
    //            other_info_str_list_.push_back(line);
    //        }
    //    }

    //    if (vertices_.empty()) {
    //        std::cerr << "Warning: no vertices loaded from " << filename << "\n";
    //    }

    //    return true;
    //}

    //bool Mesh::save_as_simple_obj(const std::string& filename) const {
    //    std::ofstream out(filename);
    //    if (!out.is_open()) {
    //        std::cerr << "Failed to write OBJ file: " << filename << "\n";
    //        return false;
    //    }

    //    for (const auto& v : vertices_) {
    //        out << "v " << v.x << " " << v.y << " " << v.z << "\n";
    //    }
    //    for (const auto& str: other_info_str_list_) {
    //        out << str << "\n";
    //    }

    //    return true;
    //}

    void Mesh::apply_transform(const Matrix4x4& matrix) {
        for (auto& v : vertices_) {
            Vector3 p(v.x, v.y, v.z);
            Vector3 t = matrix * p;
            v.x = t.x;
            v.y = t.y;
            v.z = t.z;
        }
    }

}  // namespace mesh
