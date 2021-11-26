from unittest import TestCase

from assembler import Assembler


class TestAssembler(TestCase):
    def test_A_instruction_with_literal_translation(self):
        assembler = Assembler()
        self.assertEqual(assembler.translate("@33"), "0000000000100001")

    def test_A_instruction_with_symbol_translation(self):
        assembler = Assembler()
        assembler.add_variable("m")
        assembler.add_variable("n")
        self.assertEqual(assembler.translate("@n"), "0000000000010001")

    def test_C_instruction_without_dst_jmp_translation(self):
        assembler = Assembler()
        self.assertEqual(assembler.translate("D+A"), "1110000010000000")
