from unittest import TestCase

from assembler import Assembler


class TestAssembler(TestCase):
    @classmethod
    def setUpClass(cls):
        cls.assembler = Assembler()

    def test_A_instruction_with_literal_translation(self):
        self.assertEqual(self.assembler.translate("@33"), "0000000000100001")

    def test_A_instruction_with_symbol_translation(self):
        self.assembler.add_variable("m")
        self.assembler.add_variable("n")
        self.assertEqual(self.assembler.translate("@n"), "0000000000010001")

    def test_C_instruction_without_dst_jmp_translation(self):
        self.assertEqual(self.assembler.translate("D+A"), "1110000010000000")

    def test_C_instruction_without_jmp_translation(self):
        self.assertEqual(self.assembler.translate("A=D&M"), "1111000000100000")

    def test_C_instruction_without_dst_translation(self):
        self.assertEqual(self.assembler.translate("!M;JMP"), "1111110001000111")

    def test_complete_C_instruction_translation(self):
        self.assertEqual(self.assembler.translate("AMD=D-A;JGT"), "1110010011111001")

    def test_instruction_with_spaces_translation(self):
        self.assertEqual(self.assembler.translate("AMD = D-A; JGT"), "1110010011111001")

    def test_instruction_with_trailing_comment_translation(self):
        self.assertEqual(self.assembler.translate("@4   // comment..."),"0000000000000100")

    def test_blank_instruction_translation(self):
        self.assertIsNone(self.assembler.translate("     "))

    def test_A_instruction_with_predefined_symbol_translation(self):
        self.assertEqual(self.assembler.translate("@KBD"), "0110000000000000")
