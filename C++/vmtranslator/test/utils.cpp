#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include "command.h"
#include "utils.h"

namespace fs = std::filesystem;
const fs::path DATA_DIR = fs::path(TEST_DIR) / "data";
const fs::path EXPECTED_DATA_DIR = DATA_DIR / "expected";
const fs::path TOOLS_DIR = fs::path(NT_SUITE);

CPUResult Utils::run_cpu_emulator(const std::string &output_asm_file) {
	fs::path asm_path = EXPECTED_DATA_DIR / output_asm_file;

	fs::path tst_path = asm_path;
	tst_path.replace_extension(".tst");

	std::ostringstream cmd_os;
	fs::path executable_path = TOOLS_DIR / "CPUEmulator.sh";
	cmd_os << executable_path << " " << tst_path << " 2>&1";

	std::array<char, 128> buffer;
	std::string result;
	auto pipe = popen(cmd_os.str().c_str(), "r");
	if (!pipe) throw std::runtime_error("popen() failed!");

	while (!feof(pipe)) {
		if (fgets(buffer.data(), 128, pipe) != nullptr)
			result += buffer.data();
	}

	return std::make_pair(result, pclose(pipe));
}

CPUResult Utils::run_cpu_emulator(const vm_command::Command &command, const std::string &output_asm_file) {
	fs::path asm_path = EXPECTED_DATA_DIR / output_asm_file;
	std::ofstream ofs {asm_path};
	command.to_asm(ofs);

	return Utils::run_cpu_emulator(output_asm_file);
}
