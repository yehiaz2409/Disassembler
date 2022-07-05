#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <string>
#include <cstring>
#include "ThirtyTwoBit.h"
using namespace std;
#include <cstdint>

#include <type_traits>




string ecall_func (){   //This function returns the ecall
    return "ecall";
}

string r_type (unsigned int instWord){ //This function returns the instruction format for the R type
    string x,b,c,d;
    unsigned int rd = (instWord >> 7) & 0x0000001F;     //Extract the fields - registers and functions using shifting to the right and anding
	unsigned int funct3 = (instWord >> 12) & 0x00000007;
	unsigned int rs1 = (instWord >> 15) & 0x0000001F;
	unsigned int rs2 = (instWord >> 20) & 0x0000001F;
	unsigned int funct7 = (instWord >> 25) & 0x0000007F;
	if (funct3 == 0){   //Print the instruction type based on the functions fields
		if (funct7 == 0){
    
                x = "add";
		}
		else {    
                x = "sub";
		}
	}
	else if (funct3 == 1){
    
                x = "sll";
	}
	else if (funct3 == 2){
    
                x = "slt";
	}
	else if (funct3 == 3){
    
                x = "sltu";
	}
	else if (funct3 == 4){
                x = "xor";
	}
	else if (funct3 == 5){
		if (funct7 == 0){
    
                x = "srl";
		}
		else {
                x = "sra";
		}
	}
	else if (funct3 == 6){
                x = "or";
	}
	else if (funct3 == 7){
    
                x = "and";
	}
	else {
            return "Instruction not found";      //If no function is found then return this statement  
	}
    b = to_string(rd);
    c = to_string(rs1);
    d = to_string(rs2);
    x += " \t\tx" + b + ",x" + c + ",x" + d;   //Concatinate the fields into one string and return it
    return x;
}

string i_type (unsigned int instWord){  //This function returns the instruction format for the I type
    string x,b,c,d;
	unsigned int rd = (instWord >> 7) & 0x0000001F;     //Extract the fields - registers, immediates and functions using shifting to the right and anding
	unsigned int funct3 = (instWord >> 12) & 0x00000007;
	unsigned int rs1 = (instWord >> 15) & 0x0000001F;
	signed int imm = (instWord >> 20) & 0x000007FF;
    signed int check = (instWord >> 31) & 0x1;
    unsigned int imm3 = (instWord >> 20) & 0x00000FFF;
    unsigned int imm2 = (instWord >> 20) & 0x0000001F;
    unsigned int funct7 = (instWord >> 25) & 0x0000007F;
    if (check){     //Check if the immediate is negativeto get its 2's complement and multiply by -1
        signed int r = imm ^ 0x000007FF;
        imm = r + 0x1;
        imm = imm * (-1);
    }
	if (funct3 == 0){       //Print the instruction type based on the functions fields
                x = "addi";
	}
	else if (funct3 == 2){ 
                x = "slti";
	}
	else if (funct3 == 3){
				x = "sltiu";
	}
	else if (funct3 == 4){
				x = "xori";
	}
	else if (funct3 == 6){
				x = "ori";
	}
	else if (funct3 == 7){
				x = "andi";
	}
    else if (funct3 == 1){   
                x = "slli";
                imm = imm2;
	}
	else if (funct3 == 5){
		if (funct7 == 0){
				x = "srli";
                imm = imm2;
		}
		else {
				x = "srai";
                imm = imm2;
		}
	}
	else {
            return "Instruction not found";     //If function not found return this statement
	}
    b = to_string(rd);
    c = to_string(rs1);
    d = to_string(imm);
    x += " \t\tx" + b + ",x" + c + "," + d;        //Concatinate the fields into one string and return it
    return x;
}



string i_type_load (unsigned int instWord){     //This function returns the instruction format for the I_load type (different opcode and format than previous)
    string x,b,c,d;
	unsigned int rd = (instWord >> 7) & 0x0000001F;     //Extract the fields - registers, immediates and functions using shifting to the right and anding
	unsigned int funct3 = (instWord >> 12) & 0x00000007;
	unsigned int rs1 = (instWord >> 15) & 0x0000001F;
	signed int imm = (instWord >> 20) & 0x000007FF;
    signed int check = (instWord >> 31) & 0x1;
    if (check){ //Check if the immediate is negativeto get its 2's complement and multiply by -1
        signed int r = imm ^ 0x000007FF;
        imm = r + 0x1;
        imm = imm * (-1);
    }
    if (funct3 == 0){   //Print the instruction type based on the functions fields
        x = "lb";
    }
    else if (funct3 == 1){
        x = "lh";
    }
	else if (funct3 == 2){
        x = "lw";
    }
	else if (funct3 == 4){
        x = "lbu";
    }
    else if (funct3 == 5){
        x = "lhu";
    }
    else {
        return "Instruction not found"; //If function not found return this statement
    }
    b = to_string(rd);
    c = to_string(rs1);
    d = to_string(imm);
    x += "\t\t\tx" + b + "," + d + "(x" + c + ")";   //Concatinate the fields into one string and return it
    return x;
}

string jalr_type (unsigned int instWord){   //Just like the I type but it has a different opcode
    string x,b,c,d;
    unsigned int rd = (instWord >> 7) & 0x0000001F;
	unsigned int funct3 = (instWord >> 12) & 0x00000007;
	unsigned int rs1 = (instWord >> 15) & 0x0000001F;
	signed int imm = (instWord >> 20) & 0x000007FF;
    signed int check = (instWord >> 31) & 0x1;
    if (check){
        signed int r = imm ^ 0x000007FF;
        imm = r + 0x1;
        imm = imm * (-1);
    }
    x = "jalr";
    b = to_string(rd);
    c = to_string(rs1);
    d = to_string(imm);
    x += "\t\tx" + b + ",x" + c + "," + d;
    return x;
}

