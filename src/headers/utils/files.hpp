#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <fstream>

namespace files
{
    class FileReader
    {
    private:
        std::ifstream reader;
    public:
        FileReader();
        ~FileReader() = default;

        [[nodiscard]] bool useFile(const char* file_path);
        void closeFile();
        [[nodiscard]] bool loadFileTo(const char* file_path, char* buffer);
    };
}

#endif