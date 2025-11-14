#include "obj_file.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "mesh/mesh.h"

namespace file {
    // 将类似 "1/2/3", "1//3", "1/5", "1" 的字符串解析为 Index
    struct Index {
        int v = -1;
        int vt = -1;
        int vn = -1;
    };
    /*static*/ Index parseOBJIndex(const std::string& token) {
        Index idx;
        int slash1 = token.find('/');
        if (slash1 == std::string::npos) {
            // 格式：v
            idx.v = std::stoi(token) - 1;
            return idx;
        }

        int slash2 = token.find('/', slash1 + 1);

        if (slash2 == std::string::npos) {
            // 格式：v/vt
            idx.v = std::stoi(token.substr(0, slash1)) - 1;
            idx.vt = std::stoi(token.substr(slash1 + 1)) - 1;
            return idx;
        }

        // 格式：v//vn 或 v/vt/vn
        idx.v = std::stoi(token.substr(0, slash1)) - 1;

        if (slash2 == slash1 + 1) {
            // 格式：v//vn
            idx.vn = std::stoi(token.substr(slash2 + 1)) - 1;
        }
        else {
            // 格式：v/vt/vn
            idx.vt = std::stoi(token.substr(slash1 + 1, slash2 - slash1 - 1)) - 1;
            idx.vn = std::stoi(token.substr(slash2 + 1)) - 1;
        }

        return idx;
    }

	CObjFile::CObjFile() {
        mesh_ = std::make_shared<mesh::Mesh>();
	}

    bool CObjFile::read(const std::string& obj_file_path, mesh::Mesh& mesh) {
        std::ifstream file(obj_file_path);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << obj_file_path << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                linear_algebra::Vector3 v;
                iss >> v.x_ >> v.y_ >> v.z_;
                mesh.vertices_.push_back(v);
            }
            else if (prefix == "vt") {
                linear_algebra::Vector2 vt;
                iss >> vt.u_ >> vt.v_;
                mesh.texcoords_.push_back(vt);
            }
            else if (prefix == "vn") {
                linear_algebra::Vector3 vn;
                iss >> vn.x_ >> vn.y_ >> vn.z_;
                mesh.normals_.push_back(vn);
            }
            else if (prefix == "f") {
                mesh::Face face;
                std::string vertex;
                while (iss >> vertex) {
                    //int v = 0, vt = 0, vn = 0;
                    //const int value = sscanf(vertex.c_str(), "%d/%d/%d", &v, &vt, &vn);
                    const Index& index = parseOBJIndex(vertex);
                    face.vIdx_.push_back(index.v);
                    face.vtIdx_.push_back(index.vt);
                    face.vnIdx_.push_back(index.vn);
                }
                mesh.faces_.push_back(face);
            }
        }
        return true;
    }

    bool saveOBJ(const std::string& file_name, const mesh::Mesh& mesh) {
        std::ofstream file(file_name);
        if (!file.is_open()) return false;

        for (auto& v : mesh.vertices_)
            file << "v " << v.x_ << " " << v.y_ << " " << v.z_ << "\n";
        for (auto& vt : mesh.texcoords_)
            file << "vt " << vt.u_ << " " << vt.v_ << "\n";
        for (auto& vn : mesh.normals_)
            file << "vn " << vn.x_ << " " << vn.y_ << " " << vn.z_ << "\n";
        for (auto& f : mesh.faces_) {
            file << "f";
            for (size_t i = 0; i < f.vIdx_.size(); ++i)
                file << " " << (f.vIdx_[i] + 1) << "/" << (f.vtIdx_[i] + 1)
                << "/" << (f.vnIdx_[i] + 1);
            file << "\n";
        }
        return true;
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
                linear_algebra::Vector3 v;
                iss >> v.x_ >> v.y_ >> v.z_;
                mesh_->vertices_.push_back(v);
            }
            else if (type == "vt") {
                linear_algebra::Vector2 vt;
                iss >> vt.u_ >> vt.v_;
                mesh_->texcoords_.push_back(vt);
            }
            else if (type == "vn") {
                linear_algebra::Vector3 vn;
                iss >> vn.x_ >> vn.y_ >> vn.z_;
                mesh_->normals_.push_back(vn);
            }
            else if (type == "f") {
				mesh::Face face;
				std::string vertex;
				while (iss >> vertex) {
					// int v = 0, vt = 0, vn = 0;
					// const int value = sscanf(vertex.c_str(), "%d/%d/%d", &v, &vt, &vn);
					const Index& index = parseOBJIndex(vertex);
					face.vIdx_.push_back(index.v);
					face.vtIdx_.push_back(index.vt);
					face.vnIdx_.push_back(index.vn);
				}
				mesh_->faces_.push_back(face);
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
            out << "v " << v.x_ << " " << v.y_ << " " << v.z_ << "\n";
        }
        for (const auto& vt : mesh_->texcoords_) {
            out << "vt " << vt.u_ << " " << vt.v_ << "\n";
        }
        for (const auto& vn : mesh_->normals_) {
            out << "vn " << vn.x_ << " " << vn.y_ << " " << vn.z_ << "\n";
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