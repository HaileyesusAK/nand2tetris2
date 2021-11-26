from enum import Enum
import re
import os


class InstType(Enum):
    A = 1
    C = 2
    LABEL = 3
    BLANK = 4
    UNKNOWN = 5


class Assembler:
    WSIZE = 16      # word size in bits
    VAR_MEM_BASE = 16 # base address of memory section for variables
    COMP_ENCODINGS = {
        "0": 42, "1": 63, "-1": 58, "D": 12, "A": 48, "!D": 13, "!A": 49,
        "-D": 15, "-A": 51, "D+1": 31, "A+1": 55, "D-1": 14, "A-1": 50,
        "D+A": 2, "D-A": 19, "A-D": 7, "D&A": 0, "D|A": 21, "M": 112, "!M": 113,
        "-M": 115, "M+1": 119, "M-1": 114, "D+M": 66, "D-M": 83, "M-D": 71,
        "D&M": 64, "D|M": 85
    }
    DST_ENCODINGS = {"M": 1, "D": 2, "MD": 3, "A": 4, "AM": 5, "AD": 6, "AMD": 7}
    JMP_ENCODINGS = {
        "JGT": 1, "JEQ": 2, "JGE": 3, "JLT": 4, "JNE": 5, "JLE": 6, "JMP": 7
    }
    CINST_PREFIX = 7 << 13
    VALID_SYMBOL_PATTERN = re.compile(r'[a-zA-Z_.$:][\w.$:]*')

    def __init__(self):
        self.symbol_table = {}
        self._reset_symbol_table()

    def _reset_symbol_table(self):
        self.symbol_table.update({
            "SP": 0, "LCL": 1, "ARG": 2, "THIS": 3, "THAT": 4, "R0":0,
            "SCREEN": 16384, "KBD": 24576
        })
        self.symbol_table.update({f'R{i}': i for i in range(1,16)})
        self.variables_count = 0

    def _compact_inst(self, inst):
        original = inst
        if '//' in inst:
            inst = inst[:inst.index('//')]
        inst = ''.join([c for c in inst if not c.isspace()])

        if not inst:
            return InstType.BLANK, inst

        if len(inst) > 1 and inst[0] == '@':
            symbol = inst[1:]
            if symbol.isnumeric() or self.VALID_SYMBOL_PATTERN.fullmatch(symbol):
                return InstType.A, symbol

            return InstType.UNKNOWN, original

        if len(inst) > 2 and inst[0] == '(' and inst[-1] == ')':
            return InstType.LABEL, inst[1:-1]

        dst, comp, jmp = self._split_c_inst(inst)
        if comp not in self.COMP_ENCODINGS:
            return InstType.UNKNOWN, original

        if dst and dst not in self.DST_ENCODINGS:
            return InstType.UNKNOWN, original

        if jmp and jmp not in self.JMP_ENCODINGS:
            return InstType.UNKNOWN, original

        return InstType.C, (dst, comp, jmp)


    def _to_bin(self, value):
        return f'{value:b}'.zfill(self.WSIZE)

    @staticmethod
    def _split_c_inst(inst):
        prefix, _, jmp = inst.partition(';')
        if '=' in prefix:
            dst, _, comp = prefix.partition('=')
        else:
            dst, comp = '', prefix

        return dst, comp, jmp

    def _translate_A_inst(self, symbol):
        try:
            value = int(symbol)
        except ValueError:
            value = self.symbol_table[symbol]

        return self._to_bin(value)

    def _translate_C_inst(self, dst, comp, jmp):
        machine_code = self.COMP_ENCODINGS[comp] << 6
        if dst:
            machine_code |= self.DST_ENCODINGS[dst] << 3

        if jmp:
            machine_code |= self.JMP_ENCODINGS[jmp]

        return self._to_bin(self.CINST_PREFIX + machine_code)

    def add_variable(self, var):
        if var not in self.symbol_table:
            self.symbol_table[var] = self.VAR_MEM_BASE + self.variables_count
            self.variables_count += 1

    def build_symbol_table(self, asm_path):
        self._reset_symbol_table()
        program_counter = 0
        variables = {}

        with open(asm_path) as asm_file:
            for inst in asm_file:
                inst_type, inst = self._compact_inst(inst)
                if inst_type == InstType.A:
                    if not inst.isnumeric():
                        variables[inst] = program_counter
                    program_counter += 1
                elif inst_type == InstType.LABEL:
                    variables.pop(inst, None)
                    self.symbol_table[inst] = program_counter
                elif inst_type == InstType.C:
                    program_counter += 1

        for var in variables:
            self.add_variable(var)

    def get_address(self, symbol):
        return self.symbol_table.get(symbol)

    def translate(self, instruction):
        inst_type, inst = self._compact_inst(instruction)
        if inst_type in (InstType.BLANK, InstType.LABEL):
            return

        if inst_type == InstType.A:
            return self._translate_A_inst(inst)

        if inst_type == InstType.C:
            return self._translate_C_inst(*inst)

    def translate_file(self, asm_path):
        self.build_symbol_table(asm_path)
        hack_path = asm_path.replace(".asm", ".hack")
        with open(hack_path, "w") as hack_file:
            with open(asm_path) as asm_file:
                for inst in asm_file:
                    machine_code = self.translate(inst)
                    if not machine_code:
                        continue

                    hack_file.write(machine_code + os.linesep)
