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

files::FileReader::FileReader()
: reader {}
{}

[[nodiscard]] bool files::FileReader::useFile(const char* file_path)
{
    reader.open(file_path, std::ios_base::in | std::ios_base::ate);

    return reader.is_open();
}

void files::FileReader::closeFile()
{
    reader.close();
}

[[nodiscard]] bool files::FileReader::loadFileTo(const char* file_path, char* buffer)
{
    // Avoid overwrite of an existing buffer!
    if (buffer != nullptr)
        return false;

    size_t file_length = reader.tellg();
    char* temp_buffer = new char[file_length + 1];

    if (!temp_buffer)
        return false;

    temp_buffer[file_length] = '\0';
    bool load_status = reader.read(temp_buffer, file_length).fail() == false;

    buffer = temp_buffer;

    return load_status;
}
