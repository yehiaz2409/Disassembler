#pragma once
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <cstring>
#include <string>
#include <bits/stdc++.h>
#include"ThirtyTwoBit.h"
using namespace std;

string SFormat(int unsigned InstWord);
string BFormat(int unsigned InstWord,int signed PC,map < unsigned int, string> &Labels);
string UFormat(int unsigned InstWord);
string JFormat(int unsigned InstWord,int unsigned PC,map < unsigned int, string> &Labels);
