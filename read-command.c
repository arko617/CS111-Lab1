// UCLA CS 111 Lab 1 command reading


#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <ctype.h>  //For isalpha() and isdigit() functions
#include <stdlib.h>
#include <stdio.h>  //For EOF
#include <error.h>
#include <string.h>

#define STACK_MAX 100

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

typedef struct command_tree *command_tree_t;

// Stack implementation reference to:
// http://groups.csail.mit.edu/graphics/classes/6.837/F04/cpp_notes/stack1.html

typedef struct StackChar Stack_Char;
typedef struct StackCommand Stack_Command;

struct StackChar {
    char data[STACK_MAX];
    int size;
};

struct StackCommand {
    command_t data[STACK_MAX];
    int size;
};

void Stack_Char_Init(Stack_Char *S)
{
    S->size = 0;
}

void Stack_Command_Init(Stack_Command *S)
{
    S->size = 0;
}

int Stack_Char_Top(Stack_Char *S)
{
    if (S->size == 0) {
        fprintf(stderr, "Error: stack empty\n");
        return -1;
    } 

    return S->data[S->size-1];
}

int Stack_Command_Top(Stack_Command *S)
{
    if (S->size == 0) {
        fprintf(stderr, "Error: stack empty\n");
        return -1;
    } 

    return S->data[S->size-1];
}

void Stack_Char_Push(Stack_Char *S, char c)
{
    if (S->size < STACK_MAX){
        S->data[S->size] = c;
        S->size++;
    }

    else
        fprintf(stderr, "Error: stack full\n");
}

void Stack_Command_Push(Stack_Command *S, command_t c)
{
    if (S->size < STACK_MAX){
        S->data[S->size] = c;
        S->size++;
    }
        
    else
        fprintf(stderr, "Error: stack full\n");
}

void Stack_Char_Pop(Stack_Char *S)
{
    if (S->size == 0)
        fprintf(stderr, "Error: stack empty\n");
    else
        S->size--;
}

void Stack_Command_Pop(Stack_Command *S)
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
  while(first[0] == ' ') 
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
  return (isalpha(c) || isdigit(c) || c == '!' || c == '%' || c == '+' || 
	  c == ',' || c == '-' || c == '.' || c == '/' || c == ':' || c == '@'
	  || c == '^' || c == '_');
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

  //First character must NOT be a special token
  if(isSpecialToken(c[i]))
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
	  if(c[sub-1] == '\n' || isSpecialToken(c[sub-1]))//WHY THE FUCK IS THIS WRONG?::::::::: \n|\n
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

struct command_tree {
  command_t *root;
  command_t *next;
  command_t *prev;
};

struct command_stream {
  command_tree_t *current;
  command_tree_t *head;
  command_tree_t *tail;
};

command_tree_t
make_command_tree (char *c) {
  int i = 0;
  while(c[i] != '\n' && c[i+1] != '\n'){
    break;
  }
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */

 //Produce a linked list of command trees, used as input to read_command_stream

  //Precedence from lowest to highest: ';' < '&&' == '||' < '|'
  

  //Command trees are separated by 2 or more newlines
  if(get_next_byte_argument == '\n' && get_next_byte(get_next_byte_argument) == '\n')
    {
      printf("New command tree created");
      return -69;
    }

  error (1, 0, "command reading not yet implemented");
  return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  //  if (s.make_command_stream(!isWord(
  
  //int i;
  //Returns the root command of command tree every time read_command stream is called, and then advance to the next linked list node

  //Make sure not to go out of bounds

  error (1, 0, "command reading not yet implemented");
  return 0;
}
