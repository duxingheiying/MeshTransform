#ifndef OBJ_FILE_H_
#define OBJ_FILE_H_

#include <string>
#include <vector>
#include <memory>

namespace mesh {
	class Mesh;
}

namespace file {
	class CObjFile {
	public:
		explicit CObjFile();

	public:
		// 从简单 OBJ 文件读取 (仅 "v x y z")
		bool read(const std::string& obj_file_path);
		// 保存为简单 OBJ 文件
		bool write(const std::string& obj_file_path) const;

		std::shared_ptr<mesh::Mesh> mesh();

	private:
		std::vector<std::string> other_info_str_list_;
		std::shared_ptr<mesh::Mesh> mesh_ = nullptr;
	};
}

#endif // OBJ_FILE_H_