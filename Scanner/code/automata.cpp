#include"automata.h"
using namespace std;

#define KEY_WORDS_LENGTH 14
string keyWords[] = { "int", "num", "str", "if", "else", "elif", "while", "for",
						"def", "return", "and", "or", "not","in" };	//cannot change the order

Token dfa(string inputString, int* index) {
	int start = *index; //changed by cr, 20201109
	Token t;
	State currentState = START_STATE;
	char ch;
	string str = "";
	int numLoc = 0;	//detect the first character of num after '\' 
	while ((*index) < inputString.length()) {
		ch = inputString[*index];
		if		(currentState == START_STATE) {
			if (isletter(ch) || ch == '_') {
				changeState(&currentState, 3, &str, ch);
			}
			else if (isnum(ch)) {
				changeState(&currentState, 1, &str, ch);
			}
			else if (ch == '#') {
				changeState(&currentState, 11);
			}
			else if (ch == '!') {
				changeState(&currentState, 4, &str, ch);
			}
			else if (ch == '>') {
				changeState(&currentState, 5, &str, ch);
			}
			else if (ch == '<') {
				changeState(&currentState, 6, &str, ch);
			}
			else if (ch == '"') {
				changeState(&currentState, 7, &str, ch);
			}
			else if (ch == '=') {
				changeState(&currentState, 8, &str, ch);
			}
			else if (ch == '-') {
				changeState(&currentState, 9, &str, ch);
			}

			else if (ch == '+') {
				changeState(&currentState, 10, &str, ch);
			}
			else if (ch == ' ' || ch == '\t') {
				// do nothing.
			}
			else if (ch == '(') {
				getDone(&t, &str, ch, index, 16);
				break;
			}
			else if (ch == ')') {
				getDone(&t, &str, ch, index, 17);
				break;
			}
			else if (ch == '[') {
				getDone(&t, &str, ch, index, 18);
				break;
			}
			else if (ch == ']') {
				getDone(&t, &str, ch, index, 19);
				break;
			}
			else if (ch == '{') {
				getDone(&t, &str, ch, index, 20);
				break;
			}
			else if (ch == '}') {
				getDone(&t, &str, ch, index, 21);
				break;
			}

			//changed by cr 20201109
			else if (ch == '\n') {
				getDone(&t, index, "", 15);
				break;
			}
			else if (ch == '*') {
				getDone(&t, &str, ch, index, 24);
				break;
			}
			else if (ch == '/') {
				getDone(&t, &str, ch, index, 25);
				break;
			}
			else if (ch == '%') {
				getDone(&t, &str, ch, index, 26);
				break;
			}
			else if (ch == ',') {
				getDone(&t, &str, ch, index, 34);
				break;
			}
			else if (ch == ':') {
				getDone(&t, &str, ch, index, 35);
				break;
			}
			else if (ch == ';') {
				getDone(&t, &str, ch, index, 36);
				break;
			}
			else {
				getDone(&t, index, errorReport("Cannot start with this character", start, *index), 41);
				break;
			}
		}	//END OF START_STATE
		else if (currentState == IN_ID) {
			if (isletter(ch) || isnum(ch) || ch == '_') {
				str += ch;
			}
			else {
				t.setContent(str);
				int check = isKeyWords(str);
				if (check == -1) {	//str is not a key word
					t.setType(ID);
					break;
				}
				else {
					t.setType((TokenType)check);
					break;
				}
			}
		}	// END OF IN_ID
		else if (currentState == IN_INT) {
			if (isnum(ch)) {
				str += ch;
			}
			else if (ch == '\\') {
				changeState(&currentState,2, &str, ch);
				numLoc = (*index)+1;
			}
			else {
				getDone(&t, str, 45);
				break;
			}
		}	//END OF IN_INT
		else if (currentState == IN_NUM) {
			if (isnum(ch)) {
				str += ch;
			}
			else if (*index == numLoc) {	//the first character after '\'
				getDone(&t, errorReport("This num is incomplete", start, numLoc - 1), 41);
				break;
			}
			else {
				getDone(&t, str, 46);
				break;
			}
		}	//END OF IN_NUM
		else if (currentState == IN_COMMENT) {
		if (*index == inputString.length() - 1) {	//this character is the last of this line
			getDone(&t, "", 15);
			}
			//otherwise, do nothing.
		}
		else if (currentState == IN_NOT) {
			if (ch == '=') {
				getDone(&t, &str, ch, index, 33);
				break;
			}
			else {
				getDone(&t, errorReport("The meaning of '!' has not been defined", start, *index), 41);
				break;
			}
		}	//END OF IN_NOT
		else if (currentState == IN_GT) {
			if (ch == '=') {
				getDone(&t, &str, ch, index, 30);
				break;
			}
			else {
				getDone(&t, str, 29);
				break;
			}
		}	//END OF IN_GT
		else if (currentState == IN_LT) {
			if (ch == '=') {
				getDone(&t, &str, ch, index, 28);
				break;
			}
			else {
				getDone(&t, str, 27);
				break;
			}
		}
		else if (currentState == IN_STR) {
			str += ch;
			if (ch == '"') {
				if(inputString[(*index)-1]=='\\'){
					str = str.substr(0, str.length() - 2);
					str += '"';
				}
				else{
				getDone(&t, &str, '\0', index, 2);	
				break;
				}
			}
			else {
				if (*index == inputString.length() - 1) { // the last one
					getDone(&t, errorReport("This string is incomplete", start, *index), 41);
					break;
				}
			}
		}	//END OF IN_STR
		else if (currentState == IN_ASSIGN) {
			if (ch == '=') {
				getDone(&t, &str, ch, index, 32);
				break;
			}
			else {
				getDone(&t, str, 31);
				break;
			}
		}	//END OF IN_ASSIGN
		else if (currentState == IN_SUB) {
			if (ch == '>') {
				getDone(&t, &str, ch, index, 37);
				break;
			}
			else if (ch == '=') {
				getDone(&t, &str, ch, index, 38);
				break;
			}
			else {
				getDone(&t, str, 23);
				break;
			}
		}
		else if (currentState == IN_ADD) {
			if (ch == '=') {
				getDone(&t, &str, ch, index, 39);
				break;
				}
			else {
				getDone(&t, str, 22);
				break;
			}
		}
		(*index)++;
	}	//END OF WHILE
	return t;
}

bool isletter(char ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
		return true;
	}

	return false;
}

bool isnum(char ch) {
	if (ch >= '0' && ch <= '9') {
		return true;
	}

	return false;
}

int isKeyWords(string str) {
	int length = KEY_WORDS_LENGTH;
	int i = 0;
	while (i < length) {
		if (keyWords[i] == str) {
			return i;
		}
		i++;
	}
	return -1;
}

//generate error report
//content: error description
//position: error character position
string errorReport(string content,int position)
{
	return content+"(error occurs at position "+ to_string(position+1)+")";
}

//generate error report
//content: error description
//start: error start position
//end: error end postition
string errorReport(string content, int start,int end)
{
	return content + "(error occurs at position " + to_string(start+1) + " to position "+to_string(end+1)+") ";
}

void changeState(State* currentState, int state)
{
	*currentState = State(state);
}

void changeState(State* currentState, int state, string* str, char ch)
{
	*currentState = State(state);
	*str += ch;
}

void getDone(Token* t, string* str, char ch, int* index, int type)
{
	*str += ch;
	(*index)++;
	t->setType(TokenType(type));
	t->setContent(*str);
}

void getDone(Token* t, int* index, string str, int type)
{
	(*index)++;
	t->setType(TokenType(type));
	t->setContent(str);
}

void getDone(Token* t, string str, int type)
{
	t->setType(TokenType(type));
	t->setContent(str);
}
