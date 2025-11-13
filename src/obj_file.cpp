#include "obj_file.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "mesh/mesh.h"

namespace file {
	CObjFile::CObjFile() {
        mesh_ = std::make_shared<mesh::Mesh>();
	}

	bool CObjFile::read(const std::string& obj_file_path) {
        std::ifstream in(obj_file_path);
        if (!in.is_open()) {
            std::cerr << "Failed to open OBJ file: " << obj_file_path << "\n";
            return false;
        }
        mesh_->vertices_.clear();
        other_info_str_list_.clear();
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            if (type == "v") {
                mesh::Mesh::Vertex v;
                iss >> v.x >> v.y >> v.z;
                mesh_->vertices_.push_back(v);
            }
            else {
                other_info_str_list_.push_back(line);
            }
        }

        if (mesh_->vertices_.empty()) {
            std::cerr << "Warning: no vertices loaded from " << obj_file_path << "\n";
        }

        return true;
	}

	bool CObjFile::write(const std::string& obj_file_path) const {
        std::ofstream out(obj_file_path);
        if (!out.is_open()) {
            std::cerr << "Failed to write OBJ file: " << obj_file_path << "\n";
            return false;
        }

        for (const auto& v : mesh_->vertices_) {
            out << "v " << v.x << " " << v.y << " " << v.z << "\n";
        }
        for (const auto& str : other_info_str_list_) {
            out << str << "\n";
        }

        return true;
	}

    std::shared_ptr<mesh::Mesh> CObjFile::mesh()
    {
        return mesh_;
    }
}