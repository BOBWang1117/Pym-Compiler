/* homework 3. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"automata.h"
#include<stack>
#include<fstream>
#include<iostream>

using namespace std;

TokenList* scanner(string cmdFileName,bool* status);

//scan single line and generate token
//line : string of the current line
//row : index of the current line
//list : the linked list of token
TokenNode* scan(string line,int row, TokenNode* lastNode,int * index);

//check indentation and dedentation, then generate token
//line : the string of the current line
//row : the index of the current line
//indentStack : the indentation stack
//list : the linked list of token
TokenNode* indentation(string line,int row,stack<int>& indentStack,TokenNode* lastNode,int * index);

//append one new node after the last node
//return the new last node
TokenNode* pushIntoList(TokenNode*,TokenNode*);

