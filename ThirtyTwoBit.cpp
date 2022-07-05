#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <cstring>
#include <string>
#include <bits/stdc++.h>
#include"ThirtyTwoBit.h"
using namespace std;

string SFormat(unsigned int InstWord)
{   string x="x";
    string sb="sb";
    string sh="sh";
    string sw="sw";
    string toprint,register2,offset,register1;
    unsigned int rs2,rs1,Imm1,Imm2,FinalImm,funct3;
    signed int result;

    rs2= (InstWord>>20) &0x0000001F; //saving value of rs2
    register2 =x+to_string(rs2);//concatinate x with rs2 value

    rs1=(InstWord>>15)&0x0000001F;//saving value of rs1
    register1=x+to_string(rs1);//concatinate x with rs1

    Imm1= (InstWord>>7) & 0x0000001F;   //first 5 bits of offset
    Imm2= (InstWord>>25) & 0x0000007F; // bits 6 to 12 of offset
   
    FinalImm= (Imm2<<5) | Imm1 ; //get all 12 bits together

    signed int check=(InstWord>>31); //most significant bit
    if(check) // if immediate is negative
    {
        result=FinalImm ^0x00000FFF;
        result=result+0x1;
        result=result*(-1);
        offset=to_string(result);

    }
    else {
        offset=to_string(FinalImm);
    }
    

    funct3= InstWord & 0x00007000;      //specific instruction
    
    if (funct3==0x00000000)
     toprint= sb +"\t\t\t"+ register2 +","+offset+"("+register1+")";
    else if (funct3==0x00001000)
     toprint= sh+"\t\t\t"+ register2 +","+offset+"("+register1+")";
    else if (funct3==0x00002000)
    toprint= sw+"\t\t\t"+ register2+ ","+offset+"("+register1+")";
     else 
    toprint="Instruction not found!";

    return toprint;

}


string BFormat( unsigned int InstWord,signed int PC,map < unsigned int, string> &Labels)
{
   string x="x";
   string toprint;
   string x1;
   string x2;
   unsigned int rs1,rs2,eleventh,oneto4,fiveto10,twelvth,funct3;
   signed int finalImm,result;

   rs1=(InstWord>>15)& 0x0000001F;//number of rs1
   x1=x+to_string(rs1); // saving it in form of xnumber as string

   rs2=(InstWord>>20) & 0x0000001F;//number of rs2
   x2=x+to_string(rs2);//saving it in form of xnumber

   eleventh= (InstWord>>7) & 0x00000001;     //eleventh bit in unsigned 
   oneto4=(InstWord>>8) & 0x0000000F;        //storing from one to fourth bit 
   fiveto10=(InstWord>>25) & 0x0000003F;     //storing from fifth to tenth bit 
   twelvth=(InstWord>>31) & 0x00000001;       //storing twelvth bit 
   finalImm= oneto4 |(fiveto10<<4) |(eleventh<<10) | (twelvth<<11);

    if (twelvth)//handling if displacement is minus
   {
       result=finalImm^0x00000FFF;
       result=result+0x1;
       result=result*(-1);
       result=result*2+PC;
   }
   else
   {
       result=finalImm*2+PC;
   }

  
   funct3=(InstWord>>12) & 0x00000007;// to identify the instruction
   if (funct3==0)
   toprint="beq\t\t\t" +x1+","+x2+",";
   else if (funct3==1)

    toprint="bne\t\t\t"+x1+","+x2+",";
   else if (funct3==4)

    toprint="blt\t\t\t"+x1+","+x2+",";
   else if (funct3==5)

    toprint="bge\t\t\t"+x1+","+x2+",";
   else if (funct3==6)
 
    toprint="bltu\t\t"+x1+","+x2+",";
   else if (funct3==7)

    toprint="bgeu\t\t"+x1+","+x2+",";
    else 
    toprint="Instruction not found!";

  stringstream ss;
  ss << "Label0x" << hex <<result; //concatinating the word label with the specific address
  toprint =toprint + ss.str();
  Labels[result] = ss.str();//saving the label in Labels map 
  return toprint;
  

}     


string UFormat (unsigned int InstWord)

{

    string x="x";
    string lui="lui";
    string auipc="auipc";
    string toprint;
    unsigned int rd,Imm,spec;

    rd= (InstWord>>7)& 0x0000001F;//getting destination register number
    x=x+to_string(rd);  //writing it in form of xnumber

    Imm= (InstWord>>12)& 0x000FFFFF; // getting immediate

    stringstream ss;
    ss << hex << Imm;     //saving immediate as hexadecimal so that compiler does not change it to decimal
    string res = ss.str();


    spec= (InstWord>>5)& 0x00000003;  // saving bits that specify the instruction
    if(spec==0)
    toprint= auipc+"\t\t\t"+x+",0x"+res;    //saving the instruction to be printed in string
    else if(spec==1)
    toprint= lui+"\t\t\t"+x+",0x"+res;
     else 
    toprint="Instruction not found!";


    return toprint;


}


string JFormat(unsigned int InstWord,  unsigned int PC,map < unsigned int, string> &Labels)
{
    string x="x";
    string toprint;
    string AddressString;
    unsigned int rd, Imm1to10,Imm11,Imm12to19,Imm20;
    signed int FinalImm,address;

    rd=(InstWord>>7) & 0x0000001F;  //saving number of destination register 
    x=x+to_string(rd);

    Imm1to10=(InstWord>>21) & 0x000003FF; //bits 1 to 10 of immediate
    Imm11=(InstWord>>20) & 0x00000001;    //bit 11
    Imm12to19=(InstWord>>12) & 0x000000FF; //bit 12 to 19
    Imm20=(InstWord>>31) & 0x00000001;   //bit 20

    FinalImm=(Imm1to10 | (Imm11<<10)| (Imm12to19<<11)|(Imm20<<19)); //displaying all bits
    signed int check=(InstWord>>31)& 0x00000001;

    if(check)// if immediate is negative
    {
        signed int result=FinalImm^0x000FFFFF;
        FinalImm=result+0x1;
        FinalImm=FinalImm*(-1);
        address=FinalImm*2+PC;
    }
    else 
    address=FinalImm*2+PC;//address that will be jumped to 

    stringstream ss;
    ss << "Label0x" << hex <<address; //concatinating the word label with the address
    toprint= "jal\t\t\t" + x + ", " + ss.str();
    Labels[address] = ss.str();//saving the label in the map to respective address
    return toprint; 
   

}


