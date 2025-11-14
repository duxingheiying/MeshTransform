#ifndef MESH_MESH_H_
#define MESH_MESH_H_

#include "transform.h"

#include <string>
#include <vector>
#include <iostream>

namespace mesh {
	struct Face { std::vector<int> vIdx_, vtIdx_, vnIdx_; };

	// 简单网格类（仅支持顶点加载与保存）
	class Mesh {
	public:
		Mesh() = default;

		// 应用变换矩阵
		void apply_transform(const linear_algebra::Matrix4x4& matrix);

		// 获取顶点数组
		const std::vector<linear_algebra::Vector3>& vertices() const { return vertices_; }

	public:
		std::vector<linear_algebra::Vector3> vertices_;
		std::vector<linear_algebra::Vector2> texcoords_;
		std::vector<linear_algebra::Vector3> normals_;
		std::vector<Face> faces_;
		std::vector<std::string> other_info_str_list_;
	};

}  // namespace mesh

#endif  // MESH_MESH_H_
