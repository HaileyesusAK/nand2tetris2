#ifndef __VM_TRANSLATOR_H__
#define __VM_TRANSLATOR_H__

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>


enum class BinaryAluOp {ADD, SUB, AND, OR};
enum class RelOp {EQ, GT, LT};
enum class UnaryOp {NEG, NOT};
enum class PushableSegment {ARG, LCL, THIS, THAT, POINTER, TEMP, STATIC, CONSTANT};
enum class PopableSegment {ARG, LCL, THIS, THAT, POINTER, TEMP, STATIC};

using InstList = std::vector<std::string>;

struct TranslatorVisitor;

struct VmCommand {
	std::string cmd;
	std::string arg1;
	uint16_t arg2;
};

namespace fs = std::filesystem;

class VmTranslator {
    public:
        InstList translate(const BinaryAluOp& op);
        InstList translate(const RelOp& op);
        InstList translate(const UnaryOp& op);
        InstList translate_push(const PushableSegment& segment, uint16_t idx);
        InstList translate_pop(const PopableSegment& segment, uint16_t idx);
        void translate(const std::filesystem::path& vm_file_path);
		void set_filename(const std::string& filename);

    private:
        uint16_t program_counter = 0;
        std::string file_name;
		VmCommand current_command;

        void append_push_D(InstList& instructions);
        void append_pop_D(InstList& instructions);
		bool parse_command(const std::string& vm_cmd);
        InstList translate_push(const std::string& segment, uint16_t idx);
        InstList translate_pop(const std::string& segment, uint16_t idx);

		friend TranslatorVisitor; 
};

#endif
