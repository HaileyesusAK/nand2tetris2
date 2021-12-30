#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include "command.h"
#include "factory.h"
#include "translator.h"

namespace fs = std::filesystem;
using namespace vm_command;

void Translator::translate(const fs::path& vm_file_path) {
    if(!fs::exists(vm_file_path))
        throw std::runtime_error(std::string("input file does not exist: ") + vm_file_path.string());

	// open output file
    auto asm_file_path = vm_file_path;
    asm_file_path.replace_extension(".asm");
	std::ofstream ofs {asm_file_path};
    if(!ofs.is_open())
        throw std::runtime_error(std::string("cannot open output file: ") + asm_file_path.string());

	// open input file
    std::ifstream ifs {vm_file_path};
    if(!ifs.is_open())
        throw std::runtime_error(std::string("cannot open input file: ") + vm_file_path.string());

    std::string file_name = vm_file_path.filename();

    std::string line;
    while(!ifs.eof()) {
        getline(ifs, line);
		Command::compact(line);
        if(line.empty())
            continue;

		auto command_ptr = Factory::create_instance(line, file_name);

		command_ptr->to_asm(ofs);
    }
}
