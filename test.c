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

  //First character must be an ordinary token or left parentheses 
  if(!isOrdinaryToken(c[0]) && c[0] != '(') //CHECK BUGGY
    return 0;
  
  //Assume true until proven false
  //Check all operators except parentheses (done after this while loop)
  while(c[i] != '\0')
    {
      andOr = 0;
      sub = i; //To perform subchecks without modifying the "i" iterator
      //Check that the token is valid input

      if(!isOrdinaryToken(c[i]) && !isSpecialToken(c[i]) && c[i] != ' ' && c[i] != '\n')
	return 0;
      
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

	removeWhiteSpace(c);
	int a = isValid(c);
	printf("%i\n", a);
}

