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

class VmTranslator {
    public:
        std::vector<std::string> translate(const BinaryAluOp& op);
        std::vector<std::string> translate(const RelOp& op, uint16_t pc);
        std::vector<std::string> translate(const UnaryOp& op);
        std::vector<std::string> translate_push(const Segment& segment, uint16_t idx);
        std::vector<std::string> translate_pop(const Segment& segment, uint16_t idx);
        std::vector<std::string> translate_push_static(const std::string& file_name,  uint16_t idx);
        std::vector<std::string> translate_pop_static(const std::string& file_name,  uint16_t idx);
        std::vector<std::string> translate_push_constant(uint16_t idx);
		void translate(const std::filesystem::path& vm_file_path);

    private:
        uint16_t program_counter;

        void append_push_D(std::vector<std::string>& instructions);
        void append_pop_D(std::vector<std::string>& instructions);
        static std::vector<std::string> split_command(const std::string& vm_cmd);
        std::vector<std::string> translate_and();
        std::vector<std::string> translate_or();
        std::vector<std::string> translate_sub();
        std::vector<std::string> translate_add();
        std::vector<std::string> translate_eq();
        std::vector<std::string> translate_gt();
        std::vector<std::string> translate_lt();
};

#endif
