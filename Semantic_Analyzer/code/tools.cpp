/* homework 4. Group members:
  Chen, Rui 1809853Z-I011-0017
  Li, Yichu
  Wang, Hongbo
*/

#include"tools.h"

using namespace std;

TokenList * scanner(string cmdFileName, bool* isError) {

	string fileName=cmdFileName;
	if (fileName == "") {
		//prompt
		cout <<endl<< "--------------------------------------" << endl;
		cout << ":) Hello, what is the name of the Pym source file" << endl << ">>> ";
		cin >> fileName;
	}
	else{
		cout <<endl<< "--------------------------------------" << endl;
		cout <<":) Your Pym source file is " << cmdFileName << endl;
	}

	ifstream file(fileName);
	//ifstream file("C:\\Users\\Alienware\\Desktop\\Scanner\\scannerTest\\" + fileName);

	if (!file.is_open()) {
		cout << "File open error";
		return NULL;
	}

	int row = 1;//row of file
	TokenNode* lastNode = new TokenNode(); //the last node of the list
	string line = ""; //the content of current line
	int index = 0; //the index of current token of the current line
	stack<int> indentStack; //indentation stack
	indentStack.push(0); //the 0 will never be popped

	//stop when the eof character show up
	while (getline(file, line)) {
		line += "\n";
		index = 0;
		lastNode=indentation(line, row, indentStack, lastNode, &index);
		lastNode=scan(line, row, lastNode, &index);
		row++;
	}

	//append DEDENT at the final
	while (indentStack.top() != 0) {
		indentStack.pop();
		lastNode = pushIntoList(lastNode, new TokenNode(row - 1, Token(DEDENT)));
	}

	//append EOF token
	lastNode = pushIntoList(lastNode, new TokenNode(row - 1, Token(ENDF)));

	//int in = 0;
	//int de = 0;

	//prompt
	cout << endl<<":) The listed of tokens are printed as follows:"<<endl<<endl;
	TokenNode* n = lastNode->findFirst();
	TokenNode* firstNode = n;
	while (n != NULL) {
		
		//output the string format of current token 
		cout << n->toString() << endl;
		if (n->getToken().getType() == ERROR) {
			(*isError) = true;
		}
		n = n->getNext();
	}

	if (*isError) {
		cout << "Error found in the scanner"<<endl;
	}

	return new TokenList(firstNode,lastNode);
}

//scan single line and generate token
TokenNode* scan(string line, int row, TokenNode* lastNode, int* index) {
	while ((*index) < line.length()) {
		Token t = dfa(line, index);
		lastNode=pushIntoList(lastNode, new TokenNode(row, t));
	}
	return lastNode;
}

//check indentation and dedentation, then generate token
TokenNode* indentation(string line, int row, stack<int>& indentStack, TokenNode* lastNode, int* index) {

	bool tabFlag = false;

	//count the number of space and detect tab
	while (line[*index] == ' '|| line[*index]=='\t') {

		//if tab is used, report an error
		if (line[*index] == '\t') {
			lastNode = pushIntoList(lastNode, new TokenNode(row, Token(ERROR, errorReport("Tab is not allowed ", (*index)-1))));
			tabFlag = true;
		}

		(*index)++;
	}

	//empty line or tab is appeared, do not generate indent or dedent token
	if (*index == (line.length() - 1)||tabFlag) {
		return lastNode;
	}

	//dedentation
	if (indentStack.top() > (*index)) {
		indentStack.pop();
		lastNode = pushIntoList(lastNode, new TokenNode(row, Token(DEDENT)));

		while (indentStack.top() > (*index)) {
			indentStack.pop();
			lastNode = pushIntoList(lastNode, new TokenNode(row, Token(DEDENT)));
		}

		//if the dedentation does not comply the previous indentation, omit it and generate an error token.
		if (indentStack.top() != (*index)){
			lastNode = pushIntoList(lastNode, new TokenNode(row, Token(ERROR, errorReport("Dedentation error ",(*index)-1))));
			return lastNode;
		}
		
	}
	//indentation
	else if (indentStack.top() < (*index)) {
		lastNode=pushIntoList(lastNode, new TokenNode(row, Token(INDENT)));
		indentStack.push(*index);
	}

	return lastNode;
}



TokenNode* pushIntoList(TokenNode* prev, TokenNode* n) {

	//the first node
	if ((*prev).getToken().getType() == DEFAULT&& (*prev).getPrev()==NULL) {
		prev = n;
		return prev;
	}

	//skip the empty line
	if ((*n).getToken().getType()!=DEFAULT) {
		(*prev).setNext(n);
		n->setPrev(prev);
		return n;
	}

	//the following nodes
	else {
		return prev;
	}
}