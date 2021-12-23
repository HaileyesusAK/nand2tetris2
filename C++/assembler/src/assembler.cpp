#include <algorithm>
#include <bitset>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include "assembler.h"
#include "instruction.h"
#include "symbol_table.h"

namespace fs = std::filesystem;

void Assembler::write(std::ofstream& ofs, uint16_t code) {
    ofs << std::bitset<WSIZE>(code).to_string() << std::endl;
}

void Assembler::translate(const fs::path& asm_file_path) {
    if(!fs::exists(asm_file_path))
        return;

    // create output file
    auto output_path = asm_file_path;
    output_path.replace_extension(".hack");
    std::ofstream ofs {output_path};

	/*
		A instructions are encoded in a second stage; hence, need to
		maintain their positions both in the output and input files;
	*/
	std::vector<std::pair<uint16_t, std::streampos>> unencoded_lines;

    SymbolTable symbol_table;
    uint16_t program_counter = 0;	// necessary to determine a label address
    std::string line;
    std::ifstream ifs {asm_file_path};

	/*
		First stage translation encodes Label and C instructions while
		collecting the positions of A instructions for second stage translation 
	*/
    while(!ifs.eof()) {
		auto input_pos = ifs.tellg();
        std::getline(ifs, line);
		line = Instruction::compact(line);
		if(line.empty())
			continue;
		
        try {
            auto instruction_ptr = InstructionFactory::create_instance(line);
            switch(instruction_ptr->type()) {
				case InstructionType::LABEL:
					symbol_table.insert(instruction_ptr->peek_value(), program_counter);
				break;

				case InstructionType::C:
					write(ofs, instruction_ptr->encode());
					program_counter++;
				break;
				
				case InstructionType::A:
					write(ofs, 0);	// placeholder, will be replaced in the second stage
					unencoded_lines.emplace_back(program_counter, input_pos);
					program_counter++;
				break;

				default:
					break;
            }
        }
        catch (const std::exception& exp) {
            throw std::runtime_error("invalid input file");
        }

    }

	/* Second stage translation encodes A instructions */
	ifs.clear();
	for(const auto& [pc, stream_pos] : unencoded_lines) {
		ifs.seekg(stream_pos, std::ios::beg);
        std::getline(ifs, line);
		line = Instruction::compact(line);
		AInstruction a_instruction {line};

		// seek to the position of the placeholder
		ofs.seekp(pc * (WSIZE + 1), std::ios::beg); // +1 accounts for the newline character

		if(!a_instruction.is_constant()) {
			symbol_table.insert(a_instruction.peek_value());
        }

		write(ofs, a_instruction.encode(symbol_table));
	}
}
