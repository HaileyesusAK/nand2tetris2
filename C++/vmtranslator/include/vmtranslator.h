#ifndef __VM_TRANSLATOR_H__
#define __VM_TRANSLATOR_H__

#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

enum class BinaryAluOp {ADD, SUB, AND, OR};
enum class RelOp {EQ, GT, LT};
enum class UnaryOp {NEG, NOT};
enum class PushableSegment {ARG, LCL, THIS, THAT, POINTER, TEMP, STATIC, CONSTANT};
enum class PopableSegment {ARG, LCL, THIS, THAT, POINTER, TEMP, STATIC};

class VmTranslator;

using BoundAluTranslator = std::function<void(VmTranslator*)>;
using DoubleParamTranslator = void (VmTranslator::*)(const std::string&, uint16_t);
using Translator = std::variant<DoubleParamTranslator, BoundAluTranslator>;
using Translators = std::unordered_map<std::string, Translator>;

struct VmCommand {
	std::string cmd;
	std::string arg1;
	uint16_t arg2;
};

namespace fs = std::filesystem;

class VmTranslator {
    public:
        void translate(const std::filesystem::path& vm_file_path);
		void operator()(const BoundAluTranslator& translator);
		void operator()(const DoubleParamTranslator& translator);

    private:
        uint16_t program_counter = 0;
        std::string file_name;
		VmCommand current_command;
		std::ofstream out;

        void translate(const RelOp& op);
        void translate(const UnaryOp& op);
        void translate(const BinaryAluOp& op);
        void translate(const PushableSegment& segment, uint16_t idx);
        void translate(const PopableSegment& segment, uint16_t idx);
		bool parse_command(const std::string& vm_cmd);
        void translate_push(const std::string& segment, uint16_t idx);
        void translate_pop(const std::string& segment, uint16_t idx);
		static const Translators& get_translators(); 
};

#endif
