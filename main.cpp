#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <cstring>
#include "compressed.h"
#include "32_bit.h"
#include "ThirtyTwoBit.h"


unsigned int pc = 0x0;

unsigned char memory[8*1024];    // only 8KB of memory located at address 0

map < unsigned int, string> Labels;
map <unsigned int,string> output;

void emitError(char *s)
{
    cout << s;
    exit(0);
}


string CompressedIns ( unsigned int ComInsWord)
{
    unsigned int opcode = ComInsWord & 0x0003;

    switch (opcode)
    {
        case 0:
        {
            return QuadrantZero(ComInsWord);   //opcode 00
            break;
        }
        case 1:
        {
            return QuadrantOne(ComInsWord, pc, Labels);    //opcode 01
            break;
        }
        case 2:
        {
            return QuadrantTwo(ComInsWord);    //opcode 10
            break;
        }
        default:
        {
            // return to main?? & re-read the instruction
//            cout << "Error Reading machine code \n";
            return "Error Reading machine code \n";
            break;
        }
    }

}

string thirtyTwo_bit_inst(unsigned int instWord){
	unsigned int opcode;
	//unsigned int instPC = pc - 4;

	opcode = instWord & 0x0000007F;
    //according to the value of the opcode, the specific function is called

	switch(opcode){
        case 111:
        {
            return JFormat(instWord,pc,Labels);
            break;
        }
        case 55:
        {
            return UFormat(instWord);
            break;
        }
        case 23:
        {
            return UFormat(instWord);
            break;
        }
        case 51:
        {
            return r_type(instWord);
            break;
        }
        case 19:
        {
            return i_type(instWord);
            break;
        }
        case 3:
        {
            return i_type_load(instWord);
            break;
        }
        case 35:
        {
            return SFormat(instWord);
            break;
        }
        case 99:
        {
            return BFormat(instWord,pc,Labels);
            break;
        }
        case 103:
        {
            return jalr_type(instWord);
            break;
        }
        case 115:
        {
            return ecall_func();
            break;
        }
        default:
        {
            // return to main?? & re-read the instruction
//            cout << "Error Reading machine code \n";
            return "Error Reading machine code \n";
            break;
        }
	}
	

}

void RUN (int argc, char *argv[])
{
    unsigned int instWord=0;
    ifstream inFile;
    ofstream outFile;
    string AssemblyInstruction;
    unsigned int pc2 = 0x0;

	memset(memory, 0, sizeof(8*1024));

    if(argc<2) emitError("use: rvcdiss <machine_code_file_name>\n");

    string outputFilename = argv[1];
    string::size_type pos = outputFilename.find('.');

    if (pos != std::string::npos)
    {
        outputFilename = outputFilename.substr(0, pos);
    }

    outputFilename = outputFilename + ".s\n";     
    outFile.open (outputFilename, ios::out);
    inFile.open(argv[1], ios::in | ios::binary | ios::ate);


    if(inFile.is_open())
    {
        int fsize = inFile.tellg();
        inFile.seekg (0, inFile.beg);
        if(!inFile.read((char *)memory, fsize)) emitError("Cannot read from input file\n");

        while(true){
            instWord = (unsigned char)memory[pc] | (((unsigned char)memory[pc+1])<<8);
            pc2 = pc;

            if( pc == fsize ) break;            

            if (!instWord)
            {
                AssemblyInstruction = "Illegal Instruction!!";
                pc += 2;
            }
            else if ((instWord & 0x0003 ) != 0x0003)
            {
                //                compression encoding
                AssemblyInstruction = CompressedIns(instWord);                
                pc += 2;
            }
            else
            {
                instWord = instWord | (((unsigned char)memory[pc+2])<<16) |
                (((unsigned char)memory[pc+3])<<24);
//                normal encoding
                 AssemblyInstruction = thirtyTwo_bit_inst(instWord);
                pc += 4;
            }

                output.insert(make_pair(pc2, AssemblyInstruction));
                
        }
    } else emitError("Cannot access input file\n");

    for (auto itr : output)//loop over output map
    {
    if (Labels.count(itr.first) != 0)// if in this address there exists a label
                {
                    auto it = Labels.find(itr.first);
                    outFile << "0x" << hex << itr.first << setw(4) << "\t" << it->second << ":\n\t\t\t\t" << itr.second << "\n";
                    //then print PC, label and instruction
                }
                else //if in this address there exists no label
                {
                outFile << "0x" << hex << itr.first << setw(6) << "\t\t\t" << itr.second  << "\n";
                //only print PC and instruction
                }
    }
    inFile.close();
    outFile.close();

}



int main(int argc, char *argv[])
{
    RUN (argc, argv);
    return 0;
}