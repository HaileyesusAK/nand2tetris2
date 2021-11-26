class Assembler:
    WSIZE = 16

    def translate(self, instruction):
        literal = int(instruction[1:])
        return f'{literal:b}'.zfill(self.WSIZE)
