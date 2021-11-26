class Assembler:
    WSIZE = 16      # word size in bits
    VAR_MEM_BASE = 16 # base address of memory section for variables

    def __init__(self):
        self.symbol_table = {}
        self.variables_count = 0

    def _to_bin(self, value):
        return f'{value:b}'.zfill(self.WSIZE)

    def translate(self, instruction):
        symbol = instruction[1:]
        try:
            value = int(symbol)
        except ValueError:
            value = self.symbol_table[symbol]

        return self._to_bin(value)

    def add_variable(self, var):
        if var not in self.symbol_table:
           self.symbol_table[var] = self.VAR_MEM_BASE + self.variables_count
           self.variables_count += 1

