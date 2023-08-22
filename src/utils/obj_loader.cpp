
#include <stdexcept>

#include <utils/obj_loader.hpp>

namespace utils {

auto ParseObjFromString(const std::string &obj_geom_str,
                        const std::string &obj_mtl_str) -> VertexData {
    tinyobj::ObjReader obj_reader;
    obj_reader.ParseFromString(obj_geom_str, obj_mtl_str);

    if (!obj_reader.Valid()) {
        throw std::runtime_error(
            "Something went wrong while trying to parse OBJ file");
    }

    VertexData ret;
    const auto &attrib = obj_reader.GetAttrib();

    LOG_CORE_INFO("# of vertices: {0}", attrib.vertices.size() / 3);
    LOG_CORE_INFO("# of normals: {0}", attrib.normals.size() / 3);
    LOG_CORE_INFO("# of texcoords: {0}", attrib.texcoords.size() / 2);
    LOG_CORE_INFO("# of materials: {0}", obj_reader.GetMaterials().size());
    LOG_CORE_INFO("# of shapes: {0}", obj_reader.GetShapes().size());

    if (!obj_reader.GetShapes().empty()) {
        LOG_CORE_TRACE("Ready to collect vertex information");
    }

    return ret;
}

}  // namespace utils
