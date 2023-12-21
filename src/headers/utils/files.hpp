#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <fstream>

namespace files
{
    [[nodiscard]] char* loadFilebyPath(std::ifstream& reader, const char* file_path);
}

#endif