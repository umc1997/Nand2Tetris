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
(MAIN)
	@SCREEN
	D=A        //Pixel index
	@0
	M=D        //R0=Pixel index
(KBDCHECK)
	@KBD
	D=M        //D=keybroad input

	@BLACK
	D;JGT

	@WHITE
	D;JEQ

	@KBDCHECK
	0;JMP
(BLACK)
	@1
	M=-1
	@CHANGE
	0;JMP
(WHITE)
	@1
	M=0
	@CHANGE
	0;JMP

(CHANGE)
	@1
	D=M       //D=R1=pixel value

	@0
	A=M       //A=R0=pixel index
	M=D       //R[A]=D  ->R[pixel index]=pixel value

	@0
	D=M+1     //D=R0+1=pixel index + 1

	@KBD
	D=A-D     //24576-16384

	@0
	M=M+1     //R0=R0+1
	A=M

	@CHANGE
	D;JGT

	@MAIN
	0;JMP