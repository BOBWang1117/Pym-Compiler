#include"automata.h"
#include<stack>
#include<fstream>
#include<iostream>

using namespace std;

int scanner(string cmdFileName);

//scan single line and generate token
//line : string of the current line
//row : index of the current line
//list : the linked list of token
Node* scan(string line,int row, Node* lastNode,int * index);

//check indentation and dedentation, then generate token
//line : the string of the current line
//row : the index of the current line
//indentStack : the indentation stack
//list : the linked list of token
Node* indentation(string line,int row,stack<int>& indentStack,Node* lastNode,int * index);

//append one new node after the last node
//return the new last node
Node* pushIntoList(Node*,Node*);

