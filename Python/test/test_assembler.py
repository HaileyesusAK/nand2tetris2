from unittest import TestCase

from assembler import Assembler


class TestAssembler(TestCase):
    def test_A_instruction_with_literal_translation(self):
        assembler = Assembler()
        self.assertEqual(assembler.translate("@33"), "0000000000100001")
