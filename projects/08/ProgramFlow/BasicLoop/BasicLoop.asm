//Start translating a new vm File: BasicLoop.vm
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@0
D=D+A
@13
M=D
@SP
AM=M-1
D=M
M=0
@13
A=M
M=D
(LOOP_START)
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
@SP
AM=M-1
D=M
M=0
@14
M=D
@SP
AM=M-1
D=M
M=0
@13
M=D
@13
D=M
@14
D=D+M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@0
D=D+A
@13
M=D
@SP
AM=M-1
D=M
M=0
@13
A=M
M=D
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
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
AM=M-1
D=M
M=0
@14
M=D
@SP
AM=M-1
D=M
M=0
@13
M=D
@13
D=M
@14
D=D-M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@0
D=D+A
@13
M=D
@SP
AM=M-1
D=M
M=0
@13
A=M
M=D
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
@SP
AM=M-1
D=M
@LOOP_START
D;JNE
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
