#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

/*
	Defines Assembler class that translates a Hack assembly file into
	the correspoding Hack machine code file
*/

#include <filesystem>
#include <fstream>

class Assembler {
	private:
		static constexpr uint8_t WSIZE = 16;

    public:
		/*
			Translates each Hack assembly instruction into their corresponding machine code
			and writes them in an output file.

			The output file will be created in the same directory as the input file, and
			it will have the input file's name except that the extension will be 
			replaced with '.hack'
		*/
        void translate(const std::filesystem::path& asm_file_path);

		/*
			Writes to the stream a string of 16-bit followed by a newline character.
			The 16-bit string is the binary representation of the input code
		*/
		static void write(std::ofstream& ofs, uint16_t code);
};

#endif
