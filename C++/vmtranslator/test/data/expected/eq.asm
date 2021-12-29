// eq
	@SP
	AM=M-1
	D=M
	@SP
	A=M-1
	D=M-D
	M=-1
	@EQ_END_0
	D;JEQ
	@SP
	A=M-1
	M=0
	(EQ_END_0)
