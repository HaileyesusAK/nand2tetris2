#ifndef __TRANSLATOR_H__
#define __TRANSLATOR_H__

#include <filesystem>
#include <string>

class Translator {
    public:
        void translate(const std::filesystem::path& vm_file_path);
};

#endif
