/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"list.h"

using namespace std;

//dfa states
typedef enum {	START_STATE, IN_INT, IN_NUM,IN_ID,IN_NOT, 
				IN_GT, IN_LT, IN_STR, IN_ASSIGN, IN_SUB, 
				IN_ADD, IN_COMMENT} State;

Token dfa(string, int*);

bool isletter(char ch);

bool isnum(char ch);

int isKeyWords(string str);

string errorReport(string,int position);

string errorReport(string content, int start, int end);

void changeState(State* currentState, int state);

void changeState(State* currentState, int state, string* str, char content);

void getDone(Token* t, string* str, char ch, int* index, int type);

void getDone(Token* t, int* index, string str, int type);

void getDone(Token* t, string str, int type);