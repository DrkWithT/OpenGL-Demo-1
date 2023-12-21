/**
 * @file files.cpp
 * @author Derek Tan
 * @brief Implements file utilities.
 * @date 2023-12-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "utils/files.hpp"

[[nodiscard]] char* files::loadFilebyPath(std::ifstream& reader, const char* file_path)
{
    char* source_buffer = nullptr;

    reader.open(file_path);

    if (!reader.is_open())
        return source_buffer;
    
    size_t file_size = 0UL;
    reader.seekg(0, reader.end);
    file_size = reader.tellg();
    reader.seekg(0, reader.beg);

    source_buffer = new char[file_size + 1UL];

    if (source_buffer != nullptr)
    {
        reader.read(source_buffer, file_size);
        source_buffer[file_size] = '\0';
    }

    reader.close();

    return source_buffer;
}