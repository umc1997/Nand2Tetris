@256
D=A
@SP
M=D
@retOfSys.init1
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
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.init
0;JMP
(retOfSys.init1)
//Start translating a new vm File: Sys.vm
(Sys.init)
@4000
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
@3
M=D
@5000
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
@4
M=D
@retOfSys.main2
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
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.main
0;JMP
(retOfSys.main2)
@SP
AM=M-1
D=M
M=0
@6
M=D
(LOOP)
@LOOP
0;JMP
(Sys.main)
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@4001
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
@3
M=D
@5001
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
@4
M=D
@200
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@1
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
@40
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@2
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
@6
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@3
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
@123
D=A
@SP
A=M
M=D
@SP
M=M+1
@retOfSys.add123
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
@SP
D=M
@1
D=D-A
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.add12
0;JMP
(retOfSys.add123)
@SP
AM=M-1
D=M
M=0
@5
M=D
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
@FRAME
M=D
@5
A=D-A
D=M
@15
M=D
@SP
AM=M-1
D=M
M=0
@2
A=M
M=D
@ARG
D=M+1
@SP
M=D
@FRAME
AM=M-1
D=M
@THAT
M=D
@FRAME
AM=M-1
D=M
@THIS
M=D
@FRAME
AM=M-1
D=M
@ARG
M=D
@FRAME
AM=M-1
D=M
@LCL
M=D
@15
A=M
0;JMP
(Sys.add12)
@4002
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
@3
M=D
@5002
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
@4
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
@12
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
D=D+M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@FRAME
M=D
@5
A=D-A
D=M
@15
M=D
@SP
AM=M-1
D=M
M=0
@2
A=M
M=D
@ARG
D=M+1
@SP
M=D
@FRAME
AM=M-1
D=M
@THAT
M=D
@FRAME
AM=M-1
D=M
@THIS
M=D
@FRAME
AM=M-1
D=M
@ARG
M=D
@FRAME
AM=M-1
D=M
@LCL
M=D
@15
A=M
0;JMP