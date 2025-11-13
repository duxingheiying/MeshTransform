#ifndef MESH_MESH_H_
#define MESH_MESH_H_

#include "transform.h"

#include <string>
#include <vector>
#include <iostream>

namespace mesh {
	// 简单网格类（仅支持顶点加载与保存）
	class Mesh {
	public:
		// 简单顶点结构
		struct Vertex {
			double x, y, z;
		};

	public:
		Mesh() = default;

		// 从简单 OBJ 文件读取 (仅 "v x y z")
		//bool load_from_simple_obj(const std::string& filename);

		// 保存为简单 OBJ 文件
		//bool save_as_simple_obj(const std::string& filename) const;

		// 应用变换矩阵
		void apply_transform(const linear_algebra::Matrix4x4& matrix);

		// 获取顶点数组
		const std::vector<Vertex>& vertices() const { return vertices_; }

	public:
		std::vector<Vertex> vertices_;
		std::vector<unsigned int> triangle_index_;
		std::vector<std::string> other_info_str_list_;
	};

}  // namespace mesh

#endif  // MESH_MESH_H_
