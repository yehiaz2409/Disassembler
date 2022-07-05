#pragma once
#include <string>
using namespace std;
string r_type (unsigned int instWord);
string i_type (unsigned int instWord);
string i_type_load (unsigned int instWord);
string jalr_type (unsigned int instWord);
string ecall_func ();