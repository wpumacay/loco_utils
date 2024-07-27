#pragma once

#include <string>
#include <vector>

#include <math/vec2_t.hpp>
#include <math/vec3_t.hpp>

#include <utils/logging.hpp>

#include <tiny_obj_loader.h>

namespace utils {

using Vec2 = ::math::Vector2<float>;
using Vec3 = ::math::Vector3<float>;

struct UTILS_API VertexData {
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> texcoords;
};

UTILS_API auto ParseObjFromString(const std::string& obj_geom_str,
                                  const std::string& obj_mtl_str) -> VertexData;

UTILS_API auto ParseObjFromFile(const std::string& obj_file) -> VertexData;

auto _ParseObjFromReader(const tinyobj::ObjReader& obj_reader) -> VertexData;

}  // namespace utils
