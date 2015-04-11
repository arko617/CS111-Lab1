#include <ctype.h>  //For isalpha() and isdigit() functions
#include <stdlib.h>
#include <stdio.h>  //For EOF
#include <error.h>
#include <string.h>

#define STACK_MAX 100

// UCLA CS 111 Lab 1 command printing, for debugging

typedef struct command *command_t;

enum command_type
  {
    AND_COMMAND,         // A && B
    SEQUENCE_COMMAND,    // A ; B
    OR_COMMAND,          // A || B
    PIPE_COMMAND,        // A | B
    SIMPLE_COMMAND,      // a simple command
    SUBSHELL_COMMAND,    // ( A )
  };

// Data associated with a command.
struct command
{
  enum command_type type;

  // Exit status, or -1 if not known (e.g., because it has not exited yet).
  int status;

  // I/O redirections, or null if none.
  char *input;
  char *output;

  union
  {
    // for AND_COMMAND, SEQUENCE_COMMAND, OR_COMMAND, PIPE_COMMAND:
    struct command *command[2];

    // for SIMPLE_COMMAND:
    char **word;

    // for SUBSHELL_COMMAND:
    struct command *subshell_command;
  } u;
};



struct Stack
{
    command_t data[STACK_MAX];
    int size;
};


void Stack_Init(struct Stack *S)
{
    S->size = 0;
}


command_t Stack_Top(struct Stack *S)
{
    if (S->size == 0) {
        fprintf(stderr, "Error: stack empty\n");
        exit(0);
    } 

    return S->data[S->size-1];
}


void Stack_Push(struct Stack *S, command_t c)
{
    if (S->size < STACK_MAX){
        S->data[S->size] = c;
        S->size++;
    }
        
    else
        fprintf(stderr, "Error: stack full\n");
}


void Stack_Pop(struct Stack *S)
{
    if (S->size == 0)
        fprintf(stderr, "Error: stack empty\n");
    else
        S->size--;
}

//PERFECT!
void removeWhiteSpace(char *c)
{
    int i, curr;
    char *temp, *tab, *first;

    //Replace tab characters with spaces
    for(i = 0, tab = c; tab[i] != '\0'; i++)
    {
        if(tab[i] == '\t')
    	    tab[i] = ' ';
    }

    //First character must always be an operand
    first = c;
    while(first[0] == ' ' || first[0] == '\n') 
    {
        for(i = 0; first[i] != '\0'; i++)
	         first[i] = first[i+1];
    }


    for(i = 0; c[i] != '\0' ; i++)
    {
        //Remove white spaces if more than one space
        while (c[i] == ' ' && c[i+1] == ' ')
	     {
	         //Move everything back one element if there is a space character
	         for(curr = i, temp = c; temp[curr] != '\0'; curr++)
	           temp[curr] = temp[curr+1];
	     } 
    }
}

//PERFECT!
//Checks one char at a time
int isOrdinaryToken(const char c)
{
    return (isalpha(c) || isdigit(c) || c == '!' || c == '%' || c == '+' || c == ',' || c == '-' || c == '.' || c == '/' || c == ':' 
            || c == '@' || c == '^' || c == '_');
}

//GOOD!
int isSpecialToken(const char c)
{
  return (c == ';' || c == '|' || c == '(' || c == ')' || c == '<' || 
	  c == '>' || c == '&');
}

//Check entire buffer
int isValid(char *c)
{
  int i = 0, sub; //iterators for first while loop
  int par = 0; //iterators for second while loop
  int andOr = 0; //boolean to see if it is an "and-or"
  int leftPar = 0, rightPar = 0; //counters for "parentheses"
  
  //Make it easier to parse by removing extraneous white space
  removeWhiteSpace(c);

  //First character must be an ordinary token, left parentheses, or start of a comment 
  if(!isOrdinaryToken(c[0]) && c[0] != '(' && c[0] != '#') 
    return 0;
  
  //Assume true until proven false
  //Check all operators except parentheses (done after this while loop)
  while(c[i] != '\0')
    {
      andOr = 0;
      sub = i; //To perform subchecks without modifying the "i" iterator
      
      //Check that the token is valid input
      if(!isOrdinaryToken(c[i]) && !isSpecialToken(c[i]) && c[i] != ' ' && c[i] != '\n' && c[i] != '#')
		return 0;
      
	//Enter this check if you have a comment that is NOT at the beginning of a line
	/*if(c[0] != '#')
	{
		//Check that there is no ordinary token immediately preceding a comment
		if(c[i] == '#' && (c[i-1] != ' ' || c[i-1] != '\n'))
			return 0;
	}*/	//Buggy
	
      //Check that < and > have valid tokens on either side
      else if(c[i] == '<' || c[i] == '>')
	{
	  while(c[sub-1] == ' ')
	    sub--;
	  //If previous token is special or newline, error
	  if(c[sub-1] == '\n' || isSpecialToken(c[sub-1]))
	    return 0;
	  sub = i; //Reset sub
	  while(c[sub+1] == '\n' || c[sub+1] == ' ')
	    sub++;
	  //If next token is special or blank, error
	  if(!isOrdinaryToken(c[sub+1]))
	    return 0;
	}

      //Check that ; has valid tokens on AT LEAST the left side
      else if(c[i] == ';')
	{
	  while(c[sub-1] == ' ')
	    sub--;
	  //if previous token is special or newline, error
	  if(c[sub-1] == '\n' || isSpecialToken(c[sub-1]))
	    return 0;

	  sub = i; //Reset sub

	  //If next non-space or non-newline is an operator, error

	  while(c[sub+1] == '\n' || c[sub+1] == ' ')
	    sub++;
	  if(isSpecialToken(c[sub+1]))
	    return 0;
	}

      //Check that | has valid tokens on AT LEAST the left side
      else if(c[i] == '|')
	{
	  while(c[sub-1] == ' ')
	    sub--;

	  //if previous token is special or newline, error
	  if(c[sub-1] == '\n' || isSpecialToken(c[sub-1]))
	    return 0;
	  sub = i; //Reset sub

	  //If next non-space or non-newline is an operator or blank, error
	  while(c[sub+1] == '\n' || c[sub+1] == ' ')
	    sub++;

	  //Take into account that it could be ||
	  if(!isOrdinaryToken(c[sub+1]) && c[sub+1] != '|')
	    return 0;
	}

      //Check and-ors
      else if(c[i] == '|' && (c[i+1] != '|' || isOrdinaryToken(c[i+1]) || c[i+1] != ' ' || c[i+1] != '\n'))
	return 0;

      else if(c[i] == '&' && c[i+1] != '&')
	return 0;

      //Check that and-ors have an appropriate lhs and rhs
      if((c[i] == '&' && c[i+1] == '&') || (c[i] == '|' && c[i+1] == '|'))
	{
	  andOr = 1;

	  while(c[sub-1] == ' ')
	    sub--;

	  //If the previous token is a newline or a special token, error
	  if(c[sub-1] == '\n' || isSpecialToken(c[sub-1]))
	    return 0;

	  sub = i; //Reset sub

	  //Jump an extra element because the special operator is 2 digits
	  //If next non-space or non-newline is an operator, error
	  while(c[sub+2] == '\n' || c[sub+2] == ' ')
	    sub++;

	  //If the next token is a special token or blank, error
	  if(!isOrdinaryToken(c[sub+2])) 
	    return 0; 
	}

      if(andOr)
	i += 2;

      else
	i++;
    }

  while(c[par] != '\0')
    {
      if(c[par] == '(')
		leftPar++;

      else if(c[par] == ')')
		rightPar++;

      par++;
    }

  //If different number of left and right parentheses, error
  if(leftPar != rightPar)
    return 0;
  return 1;
}

typedef struct command_tree *command_tree_t;

struct command_tree {
  command_t root;
  command_tree_t next;
  command_tree_t prev;
};


struct command_stream {
  command_tree_t head;
  command_tree_t current;
  command_tree_t tail;
};


// Each COMMAND TREE creator
command_tree_t make_command_tree (char *c) {
  struct Stack cmdStack;
  struct Stack oprStack;

  Stack_Init(&cmdStack);
  Stack_Init(&oprStack);
 
  // Keep iterating through the entire buffer given
  int i = 0;
  while(c[i] != '\0'){

    // Don't forget to allocate memory for command
    command_t x;
    x = (command_t)malloc(sizeof(struct command));

    // For ordinary token and creation of SIMPLE commands
    if(isOrdinaryToken(c[i])){
      x->type = SIMPLE_COMMAND;
      int count = 0;
      x->u.word = (char**)malloc(sizeof(char*) * 100);
      
      int j = 0;
      while(j < 100){
        x->u.word[j] = (char*)malloc(sizeof(char) * 100);
        j++;
      }
      
      while(isOrdinaryToken(c[i]) || c[i] == ' '){
        
        int len = 0;

        while(isOrdinaryToken(c[i])){
          x->u.word[count][len] = c[i];
          len++;
          i++;
        }

        if(len > 0){
          x->u.word[count][len] = '\0';
          count++;
        }

        if(c[i] == ' '){
          i++;
        }
      }

      x->u.word[count] = '\0';
      Stack_Push(&cmdStack, x);      
    }   

    // For PIPELINE commands
    else if(c[i] == '|' && c[i+1] != '|'){

      x->type = PIPE_COMMAND;

      while(oprStack.size != 0 && Stack_Top(&oprStack)->type == PIPE_COMMAND){
        command_t oprTemp = Stack_Top(&oprStack);
        Stack_Pop(&oprStack);

        command_t cmdTemp1 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);
        command_t cmdTemp2 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);

        oprTemp->u.command[0] = cmdTemp2;
        oprTemp->u.command[1] = cmdTemp1;

        Stack_Push(&cmdStack, oprTemp);
      }

      Stack_Push(&oprStack, x);
      i++;
    }

    // For OR Commands
    else if(c[i] == '|' && c[i+1] == '|'){
      x->type = OR_COMMAND;

      while(oprStack.size != 0 && (Stack_Top(&oprStack)->type == PIPE_COMMAND || Stack_Top(&oprStack)->type == OR_COMMAND || Stack_Top(&oprStack)->type == AND_COMMAND)){
        command_t oprTemp = Stack_Top(&oprStack);
        Stack_Pop(&oprStack);

        command_t cmdTemp1 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);
        command_t cmdTemp2 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);

        oprTemp->u.command[0] = cmdTemp2;
        oprTemp->u.command[1] = cmdTemp1;

        Stack_Push(&cmdStack, oprTemp);
      }

      Stack_Push(&oprStack, x);
      i+=2;
    }

    // For AND Commands
    else if(c[i] == '&'){
      x->type = AND_COMMAND;

      while(oprStack.size != 0 && (Stack_Top(&oprStack)->type == PIPE_COMMAND || Stack_Top(&oprStack)->type == OR_COMMAND || Stack_Top(&oprStack)->type == AND_COMMAND)){
        command_t oprTemp = Stack_Top(&oprStack);
        Stack_Pop(&oprStack);

        command_t cmdTemp1 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);
        command_t cmdTemp2 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);

        oprTemp->u.command[0] = cmdTemp2;
        oprTemp->u.command[1] = cmdTemp1;

        Stack_Push(&cmdStack, oprTemp);
      }

      Stack_Push(&oprStack, x);
      i+=2;
    }

    // For NEWLINE or SEQUENCE commands
    else if((c[i] == '\n' || c[i] == ';') && c[i+i] != '\0'){
      x->type = SEQUENCE_COMMAND;

      while(oprStack.size != 0 && (Stack_Top(&oprStack)->type == PIPE_COMMAND || Stack_Top(&oprStack)->type == OR_COMMAND || Stack_Top(&oprStack)->type == AND_COMMAND || Stack_Top(&oprStack)->type == SEQUENCE_COMMAND)){
        command_t oprTemp = Stack_Top(&oprStack);
        Stack_Pop(&oprStack);

        command_t cmdTemp1 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);
        command_t cmdTemp2 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);

        oprTemp->u.command[0] = cmdTemp2;
        oprTemp->u.command[1] = cmdTemp1;

        Stack_Push(&cmdStack, oprTemp);
      }

      Stack_Push(&oprStack, x);
      i++;
    }


    // For beginning parenthesis leading to SUBSHELL commands
    else if(c[i] == '('){
      x->type = SUBSHELL_COMMAND;
      Stack_Push(&oprStack, x);
      i++;
    }

    // For ending parenthesis taking care of the creation of SUBSHELL commands
    else if(c[i] == ')'){ 
      while(Stack_Top(&oprStack)->type != SUBSHELL_COMMAND){
        command_t oprTemp = Stack_Top(&oprStack);
        Stack_Pop(&oprStack);

        command_t cmdTemp1 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);
        command_t cmdTemp2 = Stack_Top(&cmdStack);
        Stack_Pop(&cmdStack);

        oprTemp->u.command[0] = cmdTemp2;
        oprTemp->u.command[1] = cmdTemp1;

        Stack_Push(&cmdStack, oprTemp);
      }

      command_t oprTemp = Stack_Top(&oprStack);
      Stack_Pop(&oprStack);

      command_t cmdTemp = Stack_Top(&cmdStack);
      Stack_Pop(&cmdStack);

      oprTemp->u.subshell_command = cmdTemp;
      Stack_Push(&cmdStack, oprTemp);
      i++;
    }

    // For INPUTS
    else if(c[i] == '<'){

      i++;
      while(!isOrdinaryToken(c[i])){
        i++;
      }

      command_t cmdTemp = Stack_Top(&cmdStack);
      Stack_Pop(&cmdStack);

      cmdTemp->output = (char*)malloc(sizeof(char) * 100);
        
      int len = 0;
      while(isOrdinaryToken(c[i])){
        cmdTemp->output[len] = c[i];
        len++;
        i++;  
      }

      cmdTemp->output[len] = '\0';
      Stack_Push(&cmdStack, cmdTemp);   
    } 


    // For OUTPUTS
    else if(c[i] == '>'){

      i++;
      while(!isOrdinaryToken(c[i])){
        i++;
      }

      command_t cmdTemp = Stack_Top(&cmdStack);
      Stack_Pop(&cmdStack);

      cmdTemp->input = (char*)malloc(sizeof(char) * 100);
        
      int len = 0;
      while(isOrdinaryToken(c[i])){
        cmdTemp->input[len] = c[i];
        len++;
        i++;  
      }

      cmdTemp->input[len] = '\0';
      Stack_Push(&cmdStack, cmdTemp);            
    } 

    // For empty spaces
    else{
      i++;
    }
  }

  // Creating the tree from whatever is left
  while(oprStack.size != 0){
    command_t oprTemp = Stack_Top(&oprStack);
    Stack_Pop(&oprStack);

    command_t cmdTemp1 = Stack_Top(&cmdStack);
    Stack_Pop(&cmdStack);
    command_t cmdTemp2 = Stack_Top(&cmdStack);
    Stack_Pop(&cmdStack);
    
    oprTemp->u.command[0] = cmdTemp2;
  oprTemp->u.command[1] = cmdTemp1;
  Stack_Push(&cmdStack, oprTemp); 
  }

  struct command_tree t;
  command_tree_t tree;
  tree = &t;
  
  struct command temp;
  temp = *(Stack_Top(&cmdStack));
  tree->root = &temp;
 
  Stack_Pop(&cmdStack);

  return tree;
}



int main(){
	char c[100] = "(a && b)\0";
	char a1[100] = "`";
  	char a2[100] = ">"; 
  	char a3[100] = "<";
  	char a4[100] = "a >b <"; 
  	char a5[100] = ";";
  	char a6[100] = "; a";
  	char a7[100] = "a ||";
  	char a8[100] = "a\n  || b"; 
  	char a9[100] = "a\n  | b";
  	char a10[100] = "a\n  ; b";
  	char a11[100] = "a;;b";
  	char a12[100] = "a&&&b"; 
  	char a13[100] = "a|||b";
  	char a14[100] = "|a";
  	char a15[100] = "< a";
  	char a16[100] = "&& a";
  	char a17[100] = "||a";
  	char a18[100] = "(a|b";
  	char a19[100] = "a;b)";
  	char a20[100] = "( (a)";
  	char a21[100] = "a>>>b";

  	char x1[100] = "true";
	char x2[100] = "g++ -c foo.c";
  	char x3[100] = ": : :";
  	char x4[100] = "cat < /etc/passwd | tr a-z A-Z | sort -u || echo sort failed!";
  	char x5[100] = "a b<c > d";
  	char x6[100] = "cat < /etc/passwd | tr a-z A-Z | sort -u > out || echo sort failed!";
  	char x7[100] = "a&&b||\nc &&\n   d | e && f|\n\ng<h";
  	char x8[100] = "# This is a weird example: nobody would ever want to run this.\na<b>c|d<e>f|g<h>i\nEOF";

	//removeWhiteSpace(c);
	int a = isValid(c);	//1

	//All 0
	int b1 = isValid(a1);
	int b2 = isValid(a2);
	int b3 = isValid(a3);
	int b4 = isValid(a4);
	int b5 = isValid(a5);
	int b6 = isValid(a6);
	int b7 = isValid(a7);
	int b8 = isValid(a8);
	int b9 = isValid(a9);
	int b10 = isValid(a10);
	int b11 = isValid(a11);
	int b12 = isValid(a12);
	int b13 = isValid(a13);
	int b14 = isValid(a14);
	int b15 = isValid(a15);
	int b16 = isValid(a16);
	int b17 = isValid(a17);
	int b18 = isValid(a18);
	int b19 = isValid(a19);
	int b20 = isValid(a20);
	int b21 = isValid(a21);

	//All 1
	int z1 = isValid(x1);
	int z2 = isValid(x2);
	int z3 = isValid(x3);
	int z4 = isValid(x4);
	int z5 = isValid(x5);
	int z6 = isValid(x6);
	int z7 = isValid(x7);
	int z8 = isValid(x8);

	char christine[69] = "a ; b";
	int matthew = isValid(christine);
	

	printf("%i\n", a);
	printf("ZEROES MOTHER FUCKER\n");
	printf("%i\n", b1);
	printf("%i\n", b2);
	printf("%i\n", b3);
	printf("%i\n", b4);
	printf("%i\n", b5);
	printf("%i\n", b6);
	printf("%i\n", b7);
	printf("%i\n", b8);
	printf("%i\n", b9);
	printf("%i\n", b10);
	printf("%i\n", b11);
	printf("%i\n", b12);
	printf("%i\n", b13);
	printf("%i\n", b14);
	printf("%i\n", b15);
	printf("%i\n", b16);
	printf("%i\n", b17);
	printf("%i\n", b18);
	printf("%i\n", b19);
	printf("%i\n", b20);
	printf("%i\n", b21);

	printf("ONES MOTHER FUCKER\n");
	printf("%i\n", z1);
	printf("%i\n", z2);
	printf("%i\n", z3);
	printf("%i\n", z4);
	printf("%i\n", z5);
	printf("%i\n", z6);
	printf("%i\n", z7);
	printf("%i\n", z8);

	printf("%i\n", matthew);
}

