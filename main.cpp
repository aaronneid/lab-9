//============================================================================
// Name        : Lab9.cpp
// Author      : Aaron Neidlinger
// Description : Lab 8 Morse Code Modified to use TS Board
//============================================================================
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<vector>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

class Message{
     protected:
	//private data
          string message;
	//private functions

	public:
	//constructors
	     Message();
	     Message(string input);
	//deconstructor
	     ~Message();
	//public functions
          virtual void print();
};

class MorseCodeMessage : public Message{
     private:
          string morse;
     public:
     //constructors
	     MorseCodeMessage();
	     MorseCodeMessage(string input) : Message(input){} //explicit call to base class parametric constructor
         void MorseCodeToLights();
	     void translate();
         void print();
};

class MessageStack{
	private:
		vector<Message> stack;
	public:
		MessageStack();
		MessageStack(Message);
		~MessageStack();
		void push(Message);
		void pop();
		void printStack();
};

void MorseCodeMessage::MorseCodeToLights(){
	int fd;		// for the file descriptor of the special file we need to open.
	unsigned long *BasePtr;		// base pointer, for the beginning of the memory page (mmap)
	unsigned long *PBDR, *PBDDR;	// pointers for port B DR/DDR

	fd = open("/dev/mem", O_RDWR|O_SYNC);	// open the special file /dem/mem
	if(fd == -1){
		printf("\n error\n");
	}

	// We need to map Address 0x80840000 (beginning of a memory page)
	// An entire memory page is mapped. We need to point to the beginning of the page.
	BasePtr = (unsigned long*)mmap(NULL,getpagesize(),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
	if(BasePtr == MAP_FAILED){
	    printf("\n Unable to map memory space \n");
	}  // failed mmap

	PBDR = BasePtr + 1;		// Address of port B DR is 0x80840004
	PBDDR = BasePtr + 5;	// Address of port B DDR is 0x80840014

	*PBDDR |= 0xE0;		// sets LEDs as outputs

	for(unsigned int i = 0; i < morse.length(); i++){
		if(morse[i] == '.'){
			*PBDR |= 0x20;		//red on
			sleep(1);
			*PBDR &= 0xDF;		//red off
		}
		if(morse[i] == '-'){
			*PBDR |= 0x40;		//yellow on
			sleep(1);
			*PBDR &= 0xBF;		//yellow off
		}
		if(morse[i] == '/'){
			*PBDR |= 0x80;	//green on
			sleep(1);
			*PBDR &= ~0x80;	//green off
		}
		sleep(1);
	}
		 close(fd);	// close the special file
}

Message::Message(){
     cout << "Please input a string for the Message default constructor:";
     getline(cin,message);
}

Message::Message(string input){
     message = input;
}

Message::~Message(){

}

MorseCodeMessage::MorseCodeMessage(){

}


MessageStack::MessageStack(Message mes){
	stack.push_back(mes);
}

MessageStack::~MessageStack(){
	while(!stack.size()){
		stack.pop_back();
	}
}

void MessageStack::push(Message mes){
	stack.push_back(mes);
}
void MessageStack::pop(){
	stack.pop_back();
}
void MessageStack::printStack(){
	int i = stack.size() - 1;
	string tempst = " ";
	Message temp(tempst);

	while(i >= 0){
		temp = stack[i];
		temp.print();
		i--;
	}
}

void Message::print(){
     cout << "Message class message: " << message << endl;
}

void MorseCodeMessage::print(){
     cout << "MorseCodeMessage class message: " << message << endl;
     cout << "MorseCodeMessage class translation: " << morse << endl;
}

void MorseCodeMessage::translate(){
     stringstream morsestream;
     string word;
     char ch;
     string morseCode[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.",      "---",".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-","-.--", "--.."};

     morsestream << message;

     while( morsestream >> word){
          for(unsigned int i = 0; i < word.length(); i++){
               if(isalpha(word[i])){
                    ch = word[i];
                    ch = toupper(ch);
                    morse += morseCode[ch - 'A'];
                    morse += " ";
               }
          }
          morse += "/ ";
     }
}

int main(void){
     string mess;
	 string stackable1;
	 string stackable2;
	 string stackable3;
	 string stackable4;
     string morsmess;
/*
     cout << "<------------------Message Class default constructor------------------>" << endl << endl;

     Message message1;

     cout << endl;

     cout << "<------------------Message Class parametric constructor:-------------->" << endl << endl;

     cout << "Please input a string for initialization: ";
     getline(cin,mess);
     cout << endl << endl;
     Message message2(mess);

	 cout << "<------------------Message Class Results:----------------------------->" << endl << endl;

     cout << "The message for message1: " << endl;
     message1.print();

     cout << endl;

     cout << "The message for message2: " << endl;
     message2.print();

     cout << endl << endl;
*/
	 cout << "<-----------------MorseCodeMessage Class parametric constructor:----->" << endl << endl;

     cout << "Please input a string for initialization: ";
     getline(cin,morsmess);
     cout << endl << endl;
     MorseCodeMessage mors1(morsmess);

	 cout << "<-----------------MorseCode Translation and Print:------------------>" << endl << endl;

     mors1.translate();
     mors1.print();
/*
	cout << endl << endl;
	cout << "<-----------------MessageStack Initialization:----------------------->" << endl << endl;

	cout << "Please input a string for initialization: ";
     getline(cin,stackable1);
	Message stacking1(stackable1);
	MessageStack stack(stacking1);

	 cout << "Please input a string for initialization: ";
     getline(cin,stackable2);
	 Message stacking2(stackable2);
	 stack.push(stacking2);

	 cout << "Please input a string for initialization: ";
     getline(cin,stackable3);
	 Message stacking3(stackable3);
	 stack.push(stacking3);

	 cout << "Please input a string for initialization: ";
     getline(cin,stackable4);
	 Message stacking4(stackable4);
	 stack.push(stacking4);

	 cout << "<-----------------MessageStack Print:-------------------------------->" << endl << endl;

	 stack.printStack();

	 cout << "<-----------------MessageStack Deleting Two Elements:---------------->" << endl << endl;

	 stack.pop();
	 stack.pop();

	 cout << "<-----------------MessageStack Print:-------------------------------->" << endl << endl;

	 stack.printStack();
*/
	 cout << "<-----------------Morse Code Lights:-------------------------------->" << endl << endl;

	 sleep(3);
	 mors1.MorseCodeToLights();

     return 0;
}

