#ifndef __VM_TRANSLATOR_H__
#define __VM_TRANSLATOR_H__

#include <filesystem>
#include <string>
#include <vector>

enum class BinaryAluOp {ADD, SUB, AND, OR};
enum class RelOp {EQ, GT, LT};
enum class UnaryOp {NEG, NOT};
enum class Segment {ARG, LCL, THIS, THAT, POINTER, TEMP};
namespace fs = std::filesystem;
using InstList = std::vector<std::string>;

class VmTranslator {
    public:
        InstList translate(const BinaryAluOp& op);
        InstList translate(const RelOp& op, uint16_t pc);
        InstList translate(const UnaryOp& op);
        InstList translate_push(const Segment& segment, uint16_t idx);
        InstList translate_pop(const Segment& segment, uint16_t idx);
        InstList translate_push_static(const std::string& file_name,  uint16_t idx);
        InstList translate_pop_static(const std::string& file_name,  uint16_t idx);
        InstList translate_push_constant(uint16_t idx);
        void translate(const std::filesystem::path& vm_file_path);

    private:
        uint16_t program_counter;

        void append_push_D(InstList& instructions);
        void append_pop_D(InstList& instructions);
        static InstList split_command(const std::string& vm_cmd);
        InstList translate_and();
        InstList translate_or();
        InstList translate_sub();
        InstList translate_add();
        InstList translate_eq();
        InstList translate_gt();
        InstList translate_lt();
        InstList translate_not();
        InstList translate_neg();
};

#endif
