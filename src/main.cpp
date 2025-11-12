#include "mesh/mesh.h"
#include "mesh/transform.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <corecrt_math_defines.h>

namespace mesh_app {

    using mesh::Matrix4x4;
    using mesh::Mesh;
    using mesh::Vector3;

    void PrintUsage() {
        std::cout << "Usage: mesh_transform_app <input.obj> <output.obj> [options]\n\n"
            << "Options (order matters):\n"
            << "  --translate tx ty tz\n"
            << "  --scale s\n"
            << "  --scale-nonuniform sx sy sz\n"
            << "  --rotate-x angle_deg\n"
            << "  --rotate-y angle_deg\n"
            << "  --rotate-z angle_deg\n"
            << "  --rotate-axis ax ay az angle_deg\n"
            << "  --shear sxy sxz syx syz szx szy\n"
            << "  --log <path>       specify custom log file path\n"
            << "  --verbose [0|1]    print transformations to stdout (default=1)\n"
            << "  --help\n\n"
            << "Example:\n"
            << "  mesh_transform_app input.obj output.obj "
            "--translate 1 2 3 --rotate-z 45 --scale 2 "
            "--log transform.log --verbose 0\n";
    }

    // degrees → radians
    inline double DegToRad(double deg) { return deg * M_PI / 180.0; }

    void PrintMatrix(std::ostream& os, const mesh::Matrix4x4& m) {
        auto data = m.data();
        os << std::fixed << std::setprecision(4);
        for (int r = 0; r < 4; ++r) {
            os << "  ";
            for (int c = 0; c < 4; ++c) {
                os << std::setw(9) << data[r * 4 + c] << " ";
            }
            os << "\n";
        }
        os << std::endl;
    }

    std::string GetDefaultLogPath(const std::string& output_path) {
        std::string path = output_path;
        size_t pos = path.find_last_of('.');
        if (pos != std::string::npos) {
            path = path.substr(0, pos);
        }
        return path + ".log";
    }

}  // namespace mesh_app

int main(int argc, char** argv) {
    using namespace mesh_app;
    using namespace mesh;

    if (argc < 3) {
        PrintUsage();
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    std::string log_path = GetDefaultLogPath(output_path);
    bool verbose = true;  // 默认输出到 stdout

    // 预扫描命令行：提取 --log 和 --verbose 参数
    for (int i = 3; i < argc - 1; ++i) {
        std::string arg = argv[i];
        if (arg == "--log") {
            log_path = argv[i + 1];
        }
        else if (arg == "--verbose") {
            std::string val = argv[i + 1];
            if (val == "0" || val == "false" || val == "False")
                verbose = false;
            else if (val == "1" || val == "true" || val == "True")
                verbose = true;
        }
    }

    std::ofstream log_file(log_path);
    if (!log_file.is_open()) {
        std::cerr << "❌ Error: failed to create log file: " << log_path << "\n";
        return 1;
    }

    //std::ostream& os = verbose ? std::cout : *(new std::ostringstream);
    std::ostream& os = std::cout;

    log_file << "=== Mesh Transformation Log ===\n";
    log_file << "Input file: " << input_path << "\n";
    log_file << "Output file: " << output_path << "\n";
    log_file << "Verbose: " << (verbose ? "true" : "false") << "\n\n";

    Mesh mesh;
    if (!mesh.load_from_simple_obj(input_path)) {
        std::cerr << "❌ Error: failed to load input file: " << input_path << "\n";
        log_file << "❌ Failed to load input mesh\n";
        return 1;
    }

    os << "✅ Loaded mesh with " << mesh.vertices().size()
        << " vertices from " << input_path << "\n";
    log_file << "Loaded mesh with " << mesh.vertices().size() << " vertices\n";

    mesh::Matrix4x4 transform;
    os << "\n=== Begin Transformation Sequence ===\n";
    log_file << "\n=== Begin Transformation Sequence ===\n";

    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];

        // 跳过提前处理过的参数
        if (arg == "--log" || arg == "--verbose") {
            ++i;
            continue;
        }

        try {
            if (arg == "--translate" && i + 3 < argc) {
                double tx = std::stod(argv[++i]);
                double ty = std::stod(argv[++i]);
                double tz = std::stod(argv[++i]);
                os << "\n[Transform] Translate (" << tx << ", " << ty << ", " << tz << ")\n";
                log_file << "\n[Transform] Translate (" << tx << ", " << ty << ", " << tz << ")\n";
                transform = mesh::Matrix4x4::Translate(tx, ty, tz) * transform;
                if (verbose) {
                    PrintMatrix(os, transform);
                }  
                PrintMatrix(log_file, transform);

            }
            else if (arg == "--scale" && i + 1 < argc) {
                double s = std::stod(argv[++i]);
                os << "\n[Transform] Scale (" << s << ")\n";
                log_file << "\n[Transform] Scale (" << s << ")\n";
                transform = mesh::Matrix4x4::Scale(s) * transform;
                if (verbose) {
                    PrintMatrix(os, transform);
                }
                PrintMatrix(log_file, transform);

            }
            else if (arg == "--scale-nonuniform" && i + 3 < argc) {
                double sx = std::stod(argv[++i]);
                double sy = std::stod(argv[++i]);
                double sz = std::stod(argv[++i]);
                os << "\n[Transform] ScaleNonUniform (" << sx << ", " << sy << ", " << sz << ")\n";
                log_file << "\n[Transform] ScaleNonUniform (" << sx << ", " << sy << ", " << sz << ")\n";
                transform = mesh::Matrix4x4::ScaleNonUniform(sx, sy, sz) * transform;
                if (verbose) {
                    PrintMatrix(os, transform);
                }
                PrintMatrix(log_file, transform);

            }
            else if (arg == "--rotate-x" && i + 1 < argc) {
                double angle = DegToRad(std::stod(argv[++i]));
                os << "\n[Transform] RotateX (" << angle << " rad)\n";
                log_file << "\n[Transform] RotateX (" << angle << " rad)\n";
                transform = mesh::Matrix4x4::RotateX(angle) * transform;
                if (verbose) {
                    PrintMatrix(os, transform);
                }
                PrintMatrix(log_file, transform);

            }
            else if (arg == "--rotate-y" && i + 1 < argc) {
                double angle = DegToRad(std::stod(argv[++i]));
                os << "\n[Transform] RotateY (" << angle << " rad)\n";
                log_file << "\n[Transform] RotateY (" << angle << " rad)\n";
                transform = mesh::Matrix4x4::RotateY(angle) * transform;
                if (verbose) {
                    PrintMatrix(os, transform);
                }
                PrintMatrix(log_file, transform);

            }
            else if (arg == "--rotate-z" && i + 1 < argc) {
                double angle = DegToRad(std::stod(argv[++i]));
                os << "\n[Transform] RotateZ (" << angle << " rad)\n";
                log_file << "\n[Transform] RotateZ (" << angle << " rad)\n";
                transform = mesh::Matrix4x4::RotateZ(angle) * transform;
                if (verbose) {
                    PrintMatrix(os, transform);
                }
                PrintMatrix(log_file, transform);

            }
            else if (arg == "--rotate-axis" && i + 4 < argc) {
                double ax = std::stod(argv[++i]);
                double ay = std::stod(argv[++i]);
                double az = std::stod(argv[++i]);
                double angle = DegToRad(std::stod(argv[++i]));
                os << "\n[Transform] RotateAxis axis=(" << ax << ", " << ay << ", " << az
                    << "), angle=" << angle << " rad\n";
                log_file << "\n[Transform] RotateAxis axis=(" << ax << ", " << ay << ", " << az
                    << "), angle=" << angle << " rad\n";
                transform = mesh::Matrix4x4::RotateAroundAxis(Vector3(ax, ay, az), angle) * transform;
                if (verbose) {
                    PrintMatrix(os, transform);
                }
                PrintMatrix(log_file, transform);

            }
            else if (arg == "--shear" && i + 6 < argc) {
                double sxy = std::stod(argv[++i]);
                double sxz = std::stod(argv[++i]);
                double syx = std::stod(argv[++i]);
                double syz = std::stod(argv[++i]);
                double szx = std::stod(argv[++i]);
                double szy = std::stod(argv[++i]);
                os << "\n[Transform] Shear ("
                    << sxy << ", " << sxz << ", " << syx << ", "
                    << syz << ", " << szx << ", " << szy << ")\n";
                log_file << "\n[Transform] Shear ("
                    << sxy << ", " << sxz << ", " << syx << ", "
                    << syz << ", " << szx << ", " << szy << ")\n";
                transform = mesh::Matrix4x4::Shear(sxy, sxz, syx, syz, szx, szy) * transform;
                if (verbose) {
                    PrintMatrix(os, transform);
                }
                PrintMatrix(log_file, transform);

            }
            else {
                std::cerr << "\n⚠️ Unknown or malformed option: " << arg << "\n";
                PrintUsage();
                return 1;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "❌ Error parsing option " << arg << ": " << e.what() << "\n";
            log_file << "❌ Exception parsing " << arg << ": " << e.what() << "\n";
            return 1;
        }
    }

    os << "\n=== Final Transform Matrix ===\n";
    log_file << "\n=== Final Transform Matrix ===\n";
    if (verbose) PrintMatrix(os, transform);
    PrintMatrix(log_file, transform);

    mesh.apply_transform(transform);

    if (!mesh.save_as_simple_obj(output_path)) {
        std::cerr << "❌ Error: failed to save output file: " << output_path << "\n";
        log_file << "❌ Failed to save output mesh\n";
        return 1;
    }

    std::cout << "\n✅ Transformation complete.\n"
        << "Input:  " << input_path << "\n"
        << "Output: " << output_path << "\n"
        << "Log:    " << log_path << "\n";
    log_file << "\n✅ Transformation complete.\n";

    return 0;
}
