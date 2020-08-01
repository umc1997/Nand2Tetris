// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
	@16384
	D=A        //Pixel index
	@0
	M=D        //R0=Pixel index
	@24576
	D=M        //D=RAM[24576]  keybroad input
	@WHITE
	D;JEQ
	@BLACK
	D;JNE
(WHITE)
	@0
	D=M       //D=R0=Pixel index
	@i
	M=D
	@i
	
	@0
	M=D
	@1
	D=D-A
	@0
	M=D
	@END
	0;JMP
(BLACK)
	@0
	D=M
	@1
	D=A
	@1
	D=D+A
	@0
	M=D
	@END
	0;JMP
(END)
	@END
	0;JMP