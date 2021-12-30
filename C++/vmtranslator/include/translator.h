#ifndef __TRANSLATOR_H__
#define __TRANSLATOR_H__

#include <filesystem>
#include <string>

/* A vm-to-assembly translator class */
class Translator {
    public:
		/* translates the vm commands in the input file into their corresponding
		   assembly instructions, and writes them to a file in the same directory.

		   The output file is named as the input file except that its extension
		   is changed to .asm.
		*/
        void translate(const std::filesystem::path& vm_file_path);
};

#endif
