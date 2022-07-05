Project 1:

Members: Rana El Gahawy, Yehya Elkasas, Ali Yassine.

Topic: Disassembler project that takes a binary file as an input and creates an output file containing the risc-v true instructions respectively.

Implementation Details:
In the main file we have a function called "RUN".
What does "RUN" do?
-initializes pc to 0x0;
- specifies maximum size of binary file 
- Suppose the input file is called "test.bin", the output file is supposed to be "test.s".
To achieve this, we read character by character from the name of the input file "test.bin" and stop reading when we reach "."
All what we read is saved in a string and concatinated with ".s". Thus, this is the name of the output file to be created.
- opens binary file  to start reading
- creates while loop that reads from the file and does the below operations (refer to 1,2,3,4,5,6,7 explained below).


In this  project we had two kinds of instructions: The true 32 bit instructions and the c-extension which are true instructions of 16 bit.

The first step was to read the input file:
 
1-In this process we started by reading the first two bits
if (first two bits are not 11)
{
   we got to the compresed function which then reads 16 bits called "CompressedIns"
}

else 
{
  we got to the 32 bit function which reads 32 bits and is called "thirtyTwo_bit_inst"
}

2- What does "CompressedIns" do?

It takes the least two significant bits in a variable (called opcode) and according to its value it goes to either Quadrant zero (if they are 00), Quadrant one (if they are 01), or Quadrant two (if they are 10).

Quadrant Zero has three functions:
- sw
- lw
- addi4spn

Quadrant One:
- Nop
- addi
- jal
- li
- addi16spn
- lui
- srli
- srai
- andi
- sub
- xor
- or
- and
- j
- Beqz
- Bnez

Quadrant Two:
- slli
- lwsp
- swsp
- jr
- mv
- ebreak
- jalr
- add

Most of these instructions have different ways of encoding so they have separate functions to represent them and to get the bits accoring to the equivalent way of formatting.
Each function returns the equivalent assembly instruction to the main.
Jal and branch instructions update the label map for their offset.

3- What does "thirtyTwo_bit_inst" do?
This function saves the first 7 bits of the instruction word in a unsigned int called "opcode".
Based on the opcide and using case statements, each opcode value takes us to a different function which represents a different instruction format.

4-
We differentiatd these string functions into:
-SFormat
-JFormat
-UFormat
-BFormat
-r_type
-i_type
-i_type_load
-jalr_type
-ecall_func

5-
In each function we mask the values needed according to the format and save them in unsigned integers( or signed integers in case of offset or immediate).
In functions that include "funct3", we mask the respectable values and do switch statements to see which instruction we wil be printing according to the value of funct3.

At the end we use the function "to_string()" to transform integers to string. 
The last step here is to concatinate all strings into one string and save them in what we named "toprint" or"x" or"AssemblyInstruction". (According to each member).
This string value is returned by the function.

6-
In the main function we have an integere variable called "PC" which is incremented by either 2 or 4 each time according to what function we call ( 32 bit function or compressed).

7- We print the pc and the concatinated string in output file each time we loop.
8- The while loop breaks when we read NULL which means there is nothing left to read

We have a new branch in github for the bonus named Final while the master contains the last version of the project without the bonus.

BONUS:
-In the branch and jal functions we pass a map "Label" by reference as a parameter
-We generate a string Label by concatinating the word Label with the hexadecimal address.
-This string is saved in the map "Label"
- so now the map "Label" has the label to each PC 

In the while loop were we call the respective functions.
-We store each string returned in every loop in an ordered map "output" that takes int and string (PC and instruction respectively).
-Now at the end of the while loop we have a map that contains each PC and the respective instruction
-After we break from the while loop, we enter a for loop that loops over all the elements of the map "output"
-in each loop we check if the PC in the output map exists in the label map
-If so we print the PC label(second element in label map) and we print the instruction (second element in output map)
-If not we print the PC and the instruction only.

Contributions:
Rana: Worked on compressed instructions(all formats), testing, main
Ali:Worked on 32 bit instructions (SFormat,BFormat,JFormat,UFormat),testing,main
Yehya: Worked on 32 bit instructions (RFormat,IFormat,(with jalr)),testing, main

BONUS Contribution:
All members equally

Limitations:
- We couldnot test the compressed functions lui, JR, Jalr because we couldn't find their equivalent compressed code.
- There are some functions that we did't support like fence, csrrw, etc.

References:
The RISC-V Compressed Instruction Set Manual, https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf

              

