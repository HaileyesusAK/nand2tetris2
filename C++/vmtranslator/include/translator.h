#ifndef __TRANSLATOR_H__
#define __TRANSLATOR_H__

#include <filesystem>
#include <fstream>
#include <string>

/* A vm-to-assembly translator class */
class Translator {
    public:
		/*
			Translates files containing vm commands.

			First it creates an output file that is named as the input file except that
			its extension is changed to .asm. The file will be created under the same
			directory of the input path.

			If the input path is a directory, each vm file under it will be translated
			and the result will be written in the output path.
		*/
		void translate(const std::filesystem::path& path);

	private:
		/* translates the vm commands in the input file into their corresponding
		   assembly instructions, and writes them to a file in the same directory.

		   The output file is named as the input file except that its extension
		   is changed to .asm.
		*/
        void translate(std::ofstream &out, const std::filesystem::path& vm_file_path);
};

#endif
