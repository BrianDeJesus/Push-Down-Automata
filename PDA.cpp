/**
Brian DeJesus
cs 421
Dr. Guillen
Programming assignment 2: Parse a text file and simulate a PDA to check correctly paired if's and else's
Tool: C++
**/
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<stack>
using namespace std;
//---------Prototypes--------
string readFile();//Reads file and returns as string
bool read(int , string file, string);//checks subsequent chars in sequence
bool PDA(string, stack<char>& PDAstack);//Reads an parses every single input symbol of sequence and simulates PDA
void clearStack(stack<char> &PDAstack);//method for clearing stack for next sequence
//void displayStack(stack<char> PDAstack);//used for debug purposes
//--------------------------
int main(){
  stack<char> PDAstack;//Object of stack
  PDAstack.push('z');//Push a z into stack for PDA simulation
  string file;//string variable to receive content from file reading method
  file = readFile();//String to receive whole file
  vector<string> sQuence;//Vector of string of each line

  int i =0;//index for file
  int start=0;//start index for new sequence

  while(file[i]!= '\0'){//While it's not the end of string
    if(file[i]== '\n'){//if end line is reached set start to start of next line
      start = i+1;//set start equal to char after endline
    }
    if(file[i+1]== '\n'|| file[i+1]== '\0'){//if endline or end of string
      sQuence.push_back(file.substr(start, i+1-start));//Push line as a string into string vector
    }
    i++;//go to next index
  }

  for(int x =0;x< sQuence.size(); x++){//check every sequence; there could be any amount of sequences
    clearStack(PDAstack);//Clear stack for new sequence each line
    cout << x+1 << ")\t";
  if(PDA(sQuence[x],PDAstack))//If final state reach and accepted by PDA
    cout<< "Correct sequence." << endl;
  else
    cout<< "Sequence is syntactically incorrect." << endl;
  }
  //displayStack(PDAstack);
  return 0;
}
//==============================================================
string readFile(){//This method reads the input file and makes it a string to return
  ifstream fin;
  fin.open("sequence.txt");
  if(!fin){//If file is unreadable for whatever reason
    cout<< "File doesn't exist." << endl;
  }
  if(fin){//if readable
    string content;
    content.assign((istreambuf_iterator<char>(fin)),//Make a string out of file so every symbol can be read
		   (istreambuf_iterator<char>()));
    return content;}
  fin.close();
}
//The following method checks if each symbol and its subsequent symbols match if or else
bool read(int spot, string word, string str1){
  if(str1==(word.substr(spot, str1.length())))return true;
  else return false;
}
//The following method simulates my PDA's transition functions
bool PDA(string seq, stack<char> &PDAstack){
  string strIf = "if";//if match symbols i then f
  string strElse = "else";//else match symbols e l s e
  int state = 0;//Start state q0

  for(int i=0; i<seq.size();i++){//Read and process every input symbol
    if(state == 0 && read(i, seq, strIf)&& PDAstack.top()=='a'){//state 0 read an if top of stack is an a
      PDAstack.push('a');//push an a and stay in q0
    }
    if(state==1&& seq[i]=='}'&&PDAstack.top()=='c'){//state 1 read a closing bracket, c on top of stack, pop
      while(PDAstack.top()=='c'){//state 1 read nothing, c on top of stack, pop go to state 2
	PDAstack.pop();
      }
      state = 2;
    }
    if(state == 0 && read(i, seq, strIf)&& PDAstack.top()=='z'){//state 0, read an if, z on top of stack, push two a's, stay in q0
      PDAstack.push('a');//and stay in q0
    }
    if(state == 0 &&  seq[i]=='}' && PDAstack.top() == 'a'){//state 0, read a closing bracket, a on top of stack
      state = 5;//go to error state q5
    }
    if(state == 0 && read(i, seq, strElse) && PDAstack.top() == 'z'){//state 0, read an else, there's a z on the stack, go to error state
      state = 5;
    }
    if(state==0 && read(i,seq,strElse)&& PDAstack.top()=='a'){//state 0, read an else, there's an a on top of the stack, pop it
      PDAstack.pop();//stay in q0
    }
    if(state==1 && seq[i]=='{'&&PDAstack.top()=='b' ){//state 1, read an open bracket, there's a b on top of stack, push a b
      PDAstack.push('b');//stay in q1
    }
    if(state==1 && seq[i]=='{'&&PDAstack.top()=='c'){// state 1, read an open bracket, there's a c on top of the stack, push a b
      PDAstack.push('b');//stay in q1
    }
    if(state==0 && seq[i]=='{'&&PDAstack.top()=='a' ){// state 0, read an open bracket, there's an a on top of stack, push a b
      PDAstack.push('b');
      state = 1;
    }
    if(state==0 && seq[i]=='{'&&PDAstack.top()=='z' ){// state 0, read an open bracket, there's a z on top of stack, go to error state
      state = 5;
    }
    if(state==1&& read(i,seq,strIf) && PDAstack.top()== 'c'){// state 1, read an if, there's a c on top of stack, push a c
      PDAstack.push('c');//stay in q1
    }
    if(state==1&& read(i,seq,strIf) && PDAstack.top()== 'b'){// state 1, read an if, there's a b on top of stack, push a c
      PDAstack.push('c');//stay in q1
    }
    if(state==1&&read(i,seq,strElse)&& PDAstack.top() =='b'){//state 1, read an else, there's a b on top of stack, go to error state
      state = 5;
    }
    if(state==1&&read(i,seq,strElse) &&PDAstack.top() =='c'){//state 1, read an else, there's a c on top of stack, pop it
      PDAstack.pop();
    }
    if(state ==1&& seq[i]=='}'&& PDAstack.top()=='b'){//state 1, read a closing bracket, there's a b on top of stack, pop it
      PDAstack.pop();
    }
    if(state==2 && PDAstack.top()=='b' ){//state 2, read nothing, there's a b on top of the stack, pop it, go to q1
      PDAstack.pop();
      state = 1;
    }
    if(state ==1 && PDAstack.top()== 'a'){//state 1, read nothing, there's an a on top of stack, go to q0
      state = 0;
    }
    if(state ==0 && seq[i+1]=='\0'&& (PDAstack.top()=='a'|| PDAstack.top()=='z')){//if you're in q0, done reading string, a on top of the stack, go to final state.
      state = 3;//3 is the final state
    }
  }
  if(state==3 || seq.size()==0)return true;//empty string is accepted as well
  else return false;
}
void clearStack(stack<char> &PDAstack){//this method clears the stack
  while(PDAstack.top() !='z'){
    PDAstack.pop();
  }
}
/*void displayStack(stack<char> PDAstack){
cout << endl;
  while(!PDAstack.empty()){
    cout << PDAstack.top();
    PDAstack.pop();
  }
  cout << endl;
}*/
