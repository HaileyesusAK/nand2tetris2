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

    def __init__(self):
        self.symbol_table = {}
        self.variables_count = 0

    def _to_bin(self, value):
        return f'{value:b}'.zfill(self.WSIZE)

    def _translate_A_inst(self, symbol):
        try:
            value = int(symbol)
        except ValueError:
            value = self.symbol_table[symbol]

        return self._to_bin(value)

    @staticmethod
    def _split_c_inst(inst):
        prefix, _, jmp = inst.partition(';')
        if '=' in prefix:
            dst, _, comp = prefix.partition('=')
        else:
            dst, comp = '', prefix

        return dst, comp, jmp

    def _translate_C_inst(self, inst):
        dst, comp, jmp= self._split_c_inst(inst)
        machine_code = self.COMP_ENCODINGS[comp] << 6
        if dst:
            machine_code |= self.DST_ENCODINGS[dst] << 3

        if jmp:
            machine_code |= self.JMP_ENCODINGS[jmp]

        return self._to_bin(self.CINST_PREFIX + machine_code)

    def translate(self, instruction):
        instruction = ''.join([c for c in instruction if c != ' '])
        if instruction[0] == '@':
            return self._translate_A_inst(instruction[1:])
        else:
            return self._translate_C_inst(instruction)

    def add_variable(self, var):
        if var not in self.symbol_table:
           self.symbol_table[var] = self.VAR_MEM_BASE + self.variables_count
           self.variables_count += 1

