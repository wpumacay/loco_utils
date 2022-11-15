#include <utils/logging.hpp>
#include <utils/path_handling.hpp>

auto main() -> int {
    utils::Logger::Init(utils::Logger::eType::CONSOLE_LOGGER);

    {
        std::string abs_path = "/home/ratatouille/dish/animal/object.obj";
        LOG_TRACE("FROM ABSOLUTE PATH: ");
        LOG_TRACE("filename: {0}", utils::GetFilename(abs_path));
        LOG_TRACE("folder_name: {0}", utils::GetFoldername(abs_path));
        LOG_TRACE("folder_path: {0}", utils::GetFolderpath(abs_path));
        LOG_TRACE("filename_no_ext: {0}",
                  utils::GetFilenameNoExtension(abs_path));
    }

    {
        std::string rel_path = "./dish/animal/object.obj";
        LOG_INFO("FROM RELATIVE PATH: ");
        LOG_INFO("filename: {0}", utils::GetFilename(rel_path));
        LOG_INFO("folder_name: {0}", utils::GetFoldername(rel_path));
        LOG_INFO("folder_path: {0}", utils::GetFolderpath(rel_path));
        LOG_INFO("filename_no_ext: {0}",
                 utils::GetFilenameNoExtension(rel_path));
    }

    return 0;
}
