#include "compressed.h"
#include <iostream>
#include <string>
#include <cstring>
#include <istream>
using namespace std;

string CLW (unsigned int ComInsWord)  // compressed lw
{
    string AssemblyInstruction;
    unsigned int rd, rs, offset;
    rd = ((ComInsWord >> 2 ) & 0x0007) + 8 ;
    rs = ((ComInsWord >> 7 ) & 0x0007) + 8 ;
    offset = ((((ComInsWord >> 6)& 0x1))|(((ComInsWord >> 10) & 0x7 ) << 1)|(((ComInsWord >> 5 ) &0x1 ) << 4))* 4;  
    AssemblyInstruction = "lw\t\t\tx" + to_string(rd) + ", " + to_string(offset) + "(x" + to_string(rs) + ")";
    return AssemblyInstruction;
}

string CSW (unsigned int ComInsWord)  // compressed sw
{
    string AssemblyInstruction;
    unsigned int rs1, rs2, offset;
    rs2 = ((ComInsWord >> 2 ) & 0x0003)+ 8;
    rs1 = ((ComInsWord >> 7 ) & 0x0007)+ 8;
    offset = (((ComInsWord >> 6)& 0x1)|(((ComInsWord >> 10) & 0x7 ) << 1)|(((ComInsWord >> 5 ) &0x1 ) << 4))* 4;  
    AssemblyInstruction = "sw\t\t\tx" + to_string(rs2) + ", " + to_string (offset) + "(x" + to_string(rs1) + ")";
    return AssemblyInstruction;
}

string CADDI4SPN (unsigned int ComInsWord) // addi to the stack pointer scaled by 4
{
    string AssemblyInstruction;
    unsigned int rd, Imm;
    rd = ((ComInsWord >> 2) & 0x0007)+ 8;
    Imm = (((ComInsWord >> 6) & 0x0001) | (((ComInsWord >> 5) & 0x0001) << 1) | (((ComInsWord >> 11) & 0x0003) << 2) | (((ComInsWord >> 7) & 0x000F) << 4))*4;
    if (Imm == 0 && rd == 0) cout << "zero" << endl;
    AssemblyInstruction = "addi\t\tx" + to_string(rd) + ", sp, " + to_string(Imm);
    return AssemblyInstruction;
}

string QuadrantZero (unsigned int ComInsWord) // opcode zero
{
    unsigned int fun3;
    fun3 = (ComInsWord >> 13) & 0x0007;

    switch (fun3)
    {
        case 0:
        {
            return CADDI4SPN(ComInsWord);
        }
        case 2:
        {
            return CLW(ComInsWord);
            break;
        }
        case 6:
        {
            return CSW(ComInsWord);
            break;
        }
        default:
        {
            return "Unsupported Instruction!";
            break;
        }

    }
    return "errorr";

}


/////////////////////////////////////////////////////////////////////////

string CSLLI (unsigned int ComInsWord)    //compressed shift left logical (immediate)
{
    string AssemblyInstruction;
    unsigned int rd, Imm;
    rd = (ComInsWord >> 7 ) & 0x1F;
    Imm = ((ComInsWord >> 2) & 0x001F) | (((ComInsWord >> 12) & 0x0001) << 5);
    if (!rd)
        AssemblyInstruction = "Error in loading rd in CSLLI!!";
    else
        AssemblyInstruction = "slli\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", " + to_string(Imm);
    return AssemblyInstruction;
}


string CJR_MV (unsigned int ComInsWord)     //JR and MV pseudo code
{
    unsigned int rs, rd;
    string AssemblyInstruction;
    rs = ((ComInsWord >> 2) & 0x1F);
    rd = ((ComInsWord >> 7) & 0x1F);

    if (rs)
    {
        //  MV
        AssemblyInstruction = "add\t\t\tx" + to_string(rd) + ", x0, x" + to_string(rs);
    }
    else
    {
        //  JR
    AssemblyInstruction = "jalr\t\tx0, x" + to_string(rd) + ", 0";
    }

    return AssemblyInstruction;
}


string CLWSP (unsigned int ComInsWord)      //  compressed lw with base sp and the offset is divisable by 4
{
    string AssemblyInstruction;
    unsigned int rd, offset;
    rd = ((ComInsWord >> 7 ) & 0x1F);
    offset = (((ComInsWord >> 4) & 0x0007)|(((ComInsWord >> 12) & 0x1) << 3)|(((ComInsWord >> 2 ) & 0x3) << 4))*4;
    AssemblyInstruction = "lw\t\t\tx" + to_string(rd) + ", " + to_string(offset) + "(sp)";

    return AssemblyInstruction;
}


string CSWSP (unsigned int ComInsWord)      //  compressed sw with base sp and the offset is divisable by 4
{
    string AssemblyInstruction;
    unsigned int rs2, offset;
    rs2 = ((ComInsWord >> 2 ) & 0x1F);
    offset = (((ComInsWord >> 9) & 0x0F) |(((ComInsWord >> 7) & 0x03) << 4))*4;
    AssemblyInstruction = "sw\t\t\tx" + to_string(rs2) + ", " + to_string(offset) + "(sp)";
    return AssemblyInstruction;

}

string CJALR_ADD_EBREAK (unsigned int ComInsWord)       // ecall, add if the address and the irst register are the same, jalr if the next address is saved in the ra register
{
    unsigned int rs, rd;
    string AssemblyInstruction;

    rs = ((ComInsWord >> 2) & 0x1F);
    rd = ((ComInsWord >> 7) & 0x1F);

    if (rs == 0 & rd ==0)
    {
    AssemblyInstruction = "ebreak";
    }
    else if (rs)
    {
    AssemblyInstruction = "add\t\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", x" + to_string(rs);
    }
    else
    {
    AssemblyInstruction = "jalr\t\tx1, x" + to_string(rd) + ", 0";
    }

    return AssemblyInstruction;
}

string QuadrantTwo (unsigned int ComInsWord) // opcode two
{
    unsigned int fun3, Bit12;

    fun3 = (ComInsWord >> 13) & 0x0007;

    switch (fun3)
    {
        case 0:
        {
            return CSLLI(ComInsWord);
            break;
        }
        case 2:
        {
            return CLWSP(ComInsWord);
            break;
        }
        case 4:
        {
            Bit12 = (ComInsWord >>12) & 0x0001;

            if (Bit12)
                return CJALR_ADD_EBREAK(ComInsWord);
            else
                return CJR_MV(ComInsWord);
            break;
        }

        case 6:
        {
            return CSWSP(ComInsWord);
            break;
        }
        default:
        {
            return "Unsupported Instruction.";
            break;
        }

    }   
    return "errorr";
}

///////////////////////////////////////////////////////////////////////


string CJ (unsigned int ComInsWord, unsigned int pc, map < unsigned int, string> &Labels)   // pseudo code J
{
    signed int offset;
    string AssemblyInstruction;
    string Label;
    offset = (((ComInsWord >> 3 ) & 0x0007 )) | (((ComInsWord >> 11) & 0x0001) << 3) | (((ComInsWord >> 2) & 0x0001) << 4)  | (((ComInsWord >> 7) & 0x0001) << 5) | (((ComInsWord >> 6) & 0x0001) << 6) | (((ComInsWord >> 9) & 0x0003) << 7) | (((ComInsWord >> 8 ) & 0x0001) << 9)| (((ComInsWord >> 12) & 0x0001) << 10);  
    if (((ComInsWord >> 12) & 0x0001))
    {
        offset = offset ^ 0x07FF;
        offset = (offset + 1) * (-1) ;
    } 
    offset = offset * 2 +pc;
    stringstream ss;
    // create the labek by adding the word label concatinated with its original address in hexdecimal
    ss << "Label0x" << hex << offset; 
    AssemblyInstruction = "jal\t\t\t x0, " + ss.str();
    // save this label in the map for the targeted address
    Labels[offset] = ss.str();
    return AssemblyInstruction;
}


string CJAL (unsigned int ComInsWord, unsigned int pc, map < unsigned int, string> &Labels)     //jal instruction
{
    signed int offset;
    string AssemblyInstruction;
    offset = (((ComInsWord >> 3 ) & 0x0007 )) | (((ComInsWord >> 11) & 0x0001) << 3) | (((ComInsWord >> 2) & 0x0001) << 4)  | (((ComInsWord >> 7) & 0x0001) << 5) | (((ComInsWord >> 6) & 0x0001) << 6) | (((ComInsWord >> 9) & 0x0003) << 7) | (((ComInsWord >> 8 ) & 0x0001) << 9)| (((ComInsWord >> 12) & 0x0001) << 10);  
    if (((ComInsWord >> 12) & 0x0001))
    {
        offset = offset ^ 0x07FF;
        offset = (offset + 1) * (-1) ;
    } 
    offset = offset * 2 +pc;
    stringstream ss;
    // create the labek by adding the word label concatinated with its original address in hexdecimal
    ss << "Label0x" << hex << offset; 
    AssemblyInstruction = "jal\t\t\t x1, " + ss.str();
    // save this label in the map for the targeted address
    Labels[offset] = ss.str();
    return AssemblyInstruction;
}

string CBEQZ (unsigned int ComInsWord, unsigned int pc, map < unsigned int, string> &Labels)        //beqz pseudo instruction expands to beq when the second register is zero
{
    string AssemblyInstruction;
    unsigned int rs1;
    signed int offset;
    signed int result;

    rs1 = ((ComInsWord >> 7) & 0x0007)+ 8;
    if (((ComInsWord >> 12 ) & 0x0001)) // if the MSB is 1 -> get the two's complement and multiply by negative one
    {
        offset =(((ComInsWord >> 3) & 0x0003) |(((ComInsWord >> 10) & 0x0003) << 2) | (((ComInsWord >> 2 ) & 0x0001) << 4) | (((ComInsWord >> 5) & 0x0003) << 5) | (((ComInsWord >> 12 ) & 0x0001) << 7));    //not multiplied by 2
        offset = offset ^ 0x0ff;
        offset = (offset + 1) * -1;
    }
    else 
    {
        offset =(((ComInsWord >> 3) & 0x0003) |(((ComInsWord >> 10) & 0x0003) << 2) | (((ComInsWord >> 2 ) & 0x0001) << 4) | (((ComInsWord >> 5) & 0x0003) << 5) | (((ComInsWord >> 12 ) & 0x0001) << 7));    //not multiplied by 2

    }

    offset = offset*2 + pc;
    stringstream ss;
    // create the labek by adding the word label concatinated with its original address in hexdecimal
    ss << "Label0x" << hex << offset;
    // save this label in the map for the targeted address
    Labels[offset] = ss.str();
    AssemblyInstruction = "beq\t\tx" + to_string(rs1) + ", x0, " + ss.str(); 

    return AssemblyInstruction;
}

string CBNEZ (unsigned int ComInsWord, unsigned int pc, map < unsigned int, string> &Labels )       //bnez pseudo instruction expands to bne when the second register is zero
{
    string AssemblyInstruction;
    unsigned int rs1;
    signed int offset;

    rs1 = ((ComInsWord >> 7) & 0x0007)+ 8;
    if (((ComInsWord >> 12 ) & 0x0001)) // if the MSB is 1 -> get the two's complement and multiply by negative one
    {
        offset =(((ComInsWord >> 3) & 0x0003) |(((ComInsWord >> 10) & 0x0003) << 2) | (((ComInsWord >> 2 ) & 0x0001) << 4) | (((ComInsWord >> 5) & 0x0003) << 5) | (((ComInsWord >> 12 ) & 0x0001) << 7));
        offset = offset ^ 0x0ff;
        offset = (offset + 1) * -1;
    }
    else 
    {
        offset =(((ComInsWord >> 3) & 0x0003) |(((ComInsWord >> 10) & 0x0003) << 2) | (((ComInsWord >> 2 ) & 0x0001) << 4) | (((ComInsWord >> 5) & 0x0003) << 5) | (((ComInsWord >> 12 ) & 0x0001) << 7));
    }

    offset = offset*2 + pc;
    stringstream ss;
    // create the labek by adding the word label concatinated with its original address in hexdecimal
    ss << "Label0x" << hex << offset;
    // save this label in the map for the targeted address
    Labels[offset] = ss.str();
    AssemblyInstruction = "bne\t\tx" + to_string(rs1) + ", x0, " + ss.str(); 
    return AssemblyInstruction;
}

string CLI (unsigned int ComInsWord)  // expand load immediate to addi with register x0
{
    string AssemblyInstruction;
    unsigned int rd;
    signed int Imm;

    rd = ((ComInsWord >> 7) & 0x001F);

    if(!rd)
    {
        return "Unsupported Instruction!!";
    }
   
    Imm = (((ComInsWord >> 2) & 0x000F ) | ((ComInsWord >> 12) & 0x0001) << 4);     // if the MSB is 1 -> get the two's complement and multiply by negative one
    if (((ComInsWord >> 12) & 0x0001))
    {
        Imm = Imm ^ 0x1f;
        Imm++;
        Imm = Imm * (-1);        
    }
    AssemblyInstruction = "addi\t\tx" + to_string(rd) + ", x0, " + to_string(Imm);

    return AssemblyInstruction;
}

string CLUI_ADDI16SP (unsigned int ComInsWord)      //ADDI16SP or LUI
{
    string AssemblyInstruction;
    unsigned int rd;
    signed int Imm;

    rd = ((ComInsWord >> 7) & 0x001F);

    if(!rd )
    {
        return "Unsupported Instruction!!";
    }
    else if (rd == 2)
    {
        // ADDI16SP -> compressed instruction expands to andi sp, sp, immediat divisible by 16  
        Imm = (((ComInsWord >> 6) & 0x0001 ) | (((ComInsWord >> 2) & 0x0001) << 1 ) |  (((ComInsWord >> 5) & 0x0001) << 2 ) |  (((ComInsWord >> 3) & 0x0003) << 3 ) |  (((ComInsWord >> 12) & 0x0001) << 5 )); // << 4
        if ((ComInsWord >> 12) & 0x0001)
        {
            Imm = Imm ^ 0x3f;
            Imm++;
            Imm = Imm * (-1);             
        }

        Imm *= 16;

        AssemblyInstruction = "addi\t\tsp, sp, " + to_string(Imm);
    }
    else 
    {
        // LUI -> load the upper 12 bit immediate to the regiter if the 13 least significant bits are zeroes
        Imm = (((ComInsWord >> 2) & 0x001F ) | ((ComInsWord >> 12) & 0x0001) << 5);

        if ((ComInsWord >> 12) & 0x0001)     // if the MSB is 1 -> get the two's complement and multiply by negative one
        {
            Imm = Imm ^ 0x3f;
            Imm++;
            Imm = Imm * (-1);  
        }

        Imm = Imm * 4096;
        AssemblyInstruction = "lui\t\tx" + to_string(rd) + ", x0, " + to_string(Imm);
    }
    return AssemblyInstruction;
}

string CADDI_NOP (unsigned int ComInsWord)      // nop, addi
{
    string AssemblyInstruction;
    unsigned int rd;
    signed int Imm;

    rd = ((ComInsWord >> 7) & 0x001F);
    Imm = (((ComInsWord >> 2) & 0x000F ) | ((ComInsWord >> 12) & 0x0001) << 4);

    if (((ComInsWord >> 12) & 0x0001)) // if the MSB is 1 -> get the two's complement and multiply by negative one
    {
        Imm = Imm ^ 0x1f;
        Imm++;
        Imm = Imm * (-1);
    }
    if ((rd == 0) & (Imm == 0))
    {
        AssemblyInstruction = "addi\t\tx0, x0, 0";
    }
    else
    {
    AssemblyInstruction = "addi\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", " + to_string(Imm);
    }
    return AssemblyInstruction;

}

string CSRLI_SRAI_ANDI (unsigned int ComInsWord)        // same as true instructions if the return register and the first register are the same
{
    string AssemblyInstruction;
    unsigned int rd, Imm, fun2;
    signed int ImmAnd;

    Imm = (((ComInsWord >> 2) & 0x1F) | (((ComInsWord >> 12) & 0x1) << 5));
    ImmAnd = (((ComInsWord >> 2) & 0x1F) | (((ComInsWord >> 12) & 0x1) << 5));
    rd = ((ComInsWord >> 7) & 0x7)+ 8;
    fun2 = (ComInsWord >> 10) & 0x3;

    if (fun2 == 0)
    {
        AssemblyInstruction = "srli\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", " +  to_string(Imm);
    }
    else if (fun2 == 1)
    {
        AssemblyInstruction = "srai\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", " +  to_string(ImmAnd);
    }
    else if (fun2 == 2)
    {
        if ((ComInsWord >> 12) & 0x1)   // if the MSB is 1 -> get the two's complement and multiply by negative one
        {
            ImmAnd = ImmAnd ^ 0x3f;
            ImmAnd++;
            ImmAnd = ImmAnd * (-1);            
        }
        AssemblyInstruction = "andi\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", " +  to_string(ImmAnd);
    }
    else 
        AssemblyInstruction = "Instruction not supported!";

    return AssemblyInstruction;
}

string CAND_OR_XOR_SUB (unsigned int ComInsWord)        // same as true instructions if the return register and the first register are the same
{
    string AssemblyInstruction;
    unsigned int rd, rs, func;
    rd = ((ComInsWord >> 7) & 0x7)+ 8;
    rs = ((ComInsWord >> 2) & 0x7)+ 8;
    func = (ComInsWord >> 5) & 0x3;

    if (func == 0)
    {
        AssemblyInstruction = "sub\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", x" +  to_string(rs);
    }
    else if (func == 1)
    {
        AssemblyInstruction = "xor\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", x" +  to_string(rs);
    }
    else if (func == 2)
    {
        AssemblyInstruction = "or\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", x" +  to_string(rs);
    }
    else if (func == 3)
    {
        AssemblyInstruction = "and\t\tx" + to_string(rd) + ", x" + to_string(rd) + ", x" +  to_string(rs);
    }    
    else 
        AssemblyInstruction = "Instruction not supported!";

    return AssemblyInstruction;

}

string QuadrantOne (unsigned int ComInsWord, unsigned int pc, map < unsigned int, string> &Labels) // opcode two
{
    string AssemblyInstruction;
    unsigned int func, func2;
    func = (ComInsWord >> 13) & 0x7;
    func2 = (ComInsWord >> 10) & 0x3;

    switch (func)
    {
        case 0:
        {
            AssemblyInstruction = CADDI_NOP (ComInsWord);
            break;
        }
        case 1:
        {
            AssemblyInstruction = CJAL (ComInsWord, pc, Labels);
            break;
        }
        case 2:
        {
            AssemblyInstruction = CLI (ComInsWord);
            break;           
        }
        case 3:
        {
            AssemblyInstruction = CLUI_ADDI16SP (ComInsWord);
            break;            
        }
        case 4:
        {
            if (func2 == 3)
            {
                AssemblyInstruction = CAND_OR_XOR_SUB (ComInsWord);
            }
            else 
            {
                AssemblyInstruction = CSRLI_SRAI_ANDI (ComInsWord);
            }
            break;            
        }      
        case 5:
        {
            AssemblyInstruction = CJ (ComInsWord, pc, Labels);
            break;            
        }  
        case 6:
        {
            AssemblyInstruction = CBEQZ (ComInsWord, pc, Labels);
            break;            
        }
        case 7:
        {
            AssemblyInstruction = CBNEZ (ComInsWord, pc, Labels);
            break;            
        }
    }

    return AssemblyInstruction;
}