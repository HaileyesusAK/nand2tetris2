#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include "command.h"
#include "factory.h"
#include "translator.h"

namespace fs = std::filesystem;
using namespace vm_command;

void Translator::translate(const fs::path &path) {
    if(!fs::exists(path)) {
        throw std::runtime_error(std::string("path does not exist: ") + path.string());
	}

	// open output file
	auto output_path = fs::is_directory(path) ? path / path.filename() : path;
	output_path.replace_extension(".asm");
	std::ofstream ofs {output_path};
	if(!ofs.is_open()) {
		throw std::runtime_error(std::string("cannot open output file: ") + output_path.string());
	}

	if(fs::is_directory(path)) {
        ofs << "\t@261" << std::endl;
        ofs << "\tD=A" << std::endl;
        ofs << "\t@SP" << std::endl;
        ofs << "\tM=D" << std::endl;
        ofs << "\t@Sys.init" << std::endl;
        ofs << "\t0;JMP" << std::endl;

		for(auto const& dir_entry: fs::directory_iterator {path}) {
			if(dir_entry.is_regular_file() && dir_entry.path().extension() == ".vm") {
				translate(ofs, dir_entry);
			}
		}

        ofs << "(END)" << std::endl;
        ofs << "\t@END" << std::endl;
        ofs << "\t0;JMP" << std::endl;
	}
	else {
		translate(ofs, path);
	}
}

void Translator::translate(std::ofstream &ofs, const fs::path& vm_file_path) {
    std::ifstream ifs {vm_file_path};
    if(!ifs.is_open()) {
        throw std::runtime_error(std::string("cannot open input file: ") + vm_file_path.string());
	}

	auto factory = Factory(vm_file_path.filename());
    std::string line;
    while(!ifs.eof()) {
        std::getline(ifs, line);
		Command::compact(line);
        if(line.empty()) {
            continue;
		}

		auto command_ptr = factory.create_instance(line);
		command_ptr->to_asm(ofs);
    }
}
