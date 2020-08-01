// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
	@2
	D=M    
	@0
	D=A
	@2
	M=D     //set R2=0
(LOOP)
	@0
	D=M     //D=R0
	@1
	D=D-A   //D=D-1
	@END
	D;JLT   //if R0<0 goto END
	@0
	M=D     //R0=D
	@2
	D=M     //D=R2
	@1
	D=D+M   //D=D+R1
	@2  
	M=D     //R2=D   
	@LOOP
	0;JMP
(END)
	@END
	0;JMP