// function Sys.init 0
	(Sys.init)
	@LCL
	D=M
	@i
	M=D
	@0
	D=D+A
	@n
	M=D
	(Sys.init_SET_LCL)
	@n
	D=M
	@i
	D=M-D
	@Sys.init_SET_LCL_END
	D;JGE
	@i
	A=M
	M=0
	@i
	M=M+1
	@SP
	M=M+1
	@Sys.init_SET_LCL
	0;JMP
	(Sys.init_SET_LCL_END)
// push constant 4000
	@4000
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// pop pointer 0
	@SP
	AM=M-1
	D=M
	@3
	M=D
// push constant 5000
	@5000
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// pop pointer 1
	@SP
	AM=M-1
	D=M
	@4
	M=D
	// call Sys.main 0
	@Sys.main$end1
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@LCL
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@ARG
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@THIS
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@THAT
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@5
	D=A
	@0
	D=D+A
	@SP
	D=M-D
	@ARG
	M=D
	@SP
	D=M
	@LCL
	M=D
	@Sys.main
	0;JMP
	(Sys.main$end1)
// pop temp 1
	@SP
	AM=M-1
	D=M
	@6
	M=D
(Sys.init$LOOP)
// goto LOOP
	@Sys.init$LOOP
	0;JMP
// function Sys.main 5
	(Sys.main)
	@LCL
	D=M
	@i
	M=D
	@5
	D=D+A
	@n
	M=D
	(Sys.main_SET_LCL)
	@n
	D=M
	@i
	D=M-D
	@Sys.main_SET_LCL_END
	D;JGE
	@i
	A=M
	M=0
	@i
	M=M+1
	@SP
	M=M+1
	@Sys.main_SET_LCL
	0;JMP
	(Sys.main_SET_LCL_END)
// push constant 4001
	@4001
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// pop pointer 0
	@SP
	AM=M-1
	D=M
	@3
	M=D
// push constant 5001
	@5001
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// pop pointer 1
	@SP
	AM=M-1
	D=M
	@4
	M=D
// push constant 200
	@200
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// pop local 1
	@1
	D=A
	@LCL
	M=D+M
	@SP
	AM=M-1
	D=M
	@LCL
	A=M
	M=D
	@1
	D=A
	@LCL
	M=M-D
// push constant 40
	@40
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// pop local 2
	@2
	D=A
	@LCL
	M=D+M
	@SP
	AM=M-1
	D=M
	@LCL
	A=M
	M=D
	@2
	D=A
	@LCL
	M=M-D
// push constant 6
	@6
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// pop local 3
	@3
	D=A
	@LCL
	M=D+M
	@SP
	AM=M-1
	D=M
	@LCL
	A=M
	M=D
	@3
	D=A
	@LCL
	M=M-D
// push constant 123
	@123
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
	// call Sys.add12 1
	@Sys.add12$end2
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@LCL
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@ARG
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@THIS
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@THAT
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
	@5
	D=A
	@1
	D=D+A
	@SP
	D=M-D
	@ARG
	M=D
	@SP
	D=M
	@LCL
	M=D
	@Sys.add12
	0;JMP
	(Sys.add12$end2)
// pop temp 0
	@SP
	AM=M-1
	D=M
	@5
	M=D
// push local 0
	@LCL
	D=M
	@0
	A=D+A
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
// push local 1
	@LCL
	D=M
	@1
	A=D+A
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
// push local 2
	@LCL
	D=M
	@2
	A=D+A
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
// push local 3
	@LCL
	D=M
	@3
	A=D+A
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
// push local 4
	@LCL
	D=M
	@4
	A=D+A
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
// add
	@SP
	AM=M-1
	D=M
	@SP
	A=M-1
	M=M+D
// add
	@SP
	AM=M-1
	D=M
	@SP
	A=M-1
	M=M+D
// add
	@SP
	AM=M-1
	D=M
	@SP
	A=M-1
	M=M+D
// add
	@SP
	AM=M-1
	D=M
	@SP
	A=M-1
	M=M+D
// return
	@LCL
	D=M
	@frame
	M=D
	@5
	D=A
	@frame
	A=M-D
	D=M
	@ret
	M=D
	@SP
	A=M-1
	D=M
	@ARG
	A=M
	M=D
	@ARG
	D=M+1
	@SP
	M=D
	@frame
	AM=M-1
	D=M
	@THAT
	M=D
	@frame
	AM=M-1
	D=M
	@THIS
	M=D
	@frame
	AM=M-1
	D=M
	@ARG
	M=D
	@frame
	AM=M-1
	D=M
	@LCL
	M=D
	@ret
	A=M
	0;JMP
// function Sys.add12 0
	(Sys.add12)
	@LCL
	D=M
	@i
	M=D
	@0
	D=D+A
	@n
	M=D
	(Sys.add12_SET_LCL)
	@n
	D=M
	@i
	D=M-D
	@Sys.add12_SET_LCL_END
	D;JGE
	@i
	A=M
	M=0
	@i
	M=M+1
	@SP
	M=M+1
	@Sys.add12_SET_LCL
	0;JMP
	(Sys.add12_SET_LCL_END)
// push constant 4002
	@4002
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// pop pointer 0
	@SP
	AM=M-1
	D=M
	@3
	M=D
// push constant 5002
	@5002
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// pop pointer 1
	@SP
	AM=M-1
	D=M
	@4
	M=D
// push argument 0
	@ARG
	D=M
	@0
	A=D+A
	D=M
	@SP
	A=M
	M=D
	@SP
	M=M+1
// push constant 12
	@12
	D=A
	@SP
	A=M
	M=D
	@SP
	M=M+1
// add
	@SP
	AM=M-1
	D=M
	@SP
	A=M-1
	M=M+D
// return
	@LCL
	D=M
	@frame
	M=D
	@5
	D=A
	@frame
	A=M-D
	D=M
	@ret
	M=D
	@SP
	A=M-1
	D=M
	@ARG
	A=M
	M=D
	@ARG
	D=M+1
	@SP
	M=D
	@frame
	AM=M-1
	D=M
	@THAT
	M=D
	@frame
	AM=M-1
	D=M
	@THIS
	M=D
	@frame
	AM=M-1
	D=M
	@ARG
	M=D
	@frame
	AM=M-1
	D=M
	@LCL
	M=D
	@ret
	A=M
	0;JMP
