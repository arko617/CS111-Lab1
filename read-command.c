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

//GOOD!
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

//GOOD!
//Checks one char at a time
int isOrdinaryToken(const char c)
{
  return (isalpha(c) || isdigit(c) || c == '!' || c == '%' || c == '+' || 
	  c == ',' || c == '-' || c == '.' || c == '/' || c == ':' || c == '@'
	  || c == '^' || c == '_');
}

//GOOD!
int isOneDigitSpecialToken(const char c)
{
  return (c == ';' || c == '|' || c == '(' || c == ')' || c == '<' || 
	  c == '>');
}

//GOOD!
int isTwoDigitSpecialToken(const char *c)
{ 
  int i;
  //Make sure the array does not go out of bounds
  for(i = 0; c[i] != '\0'; i++)
  {
    if (c[i+1] != EOF)
      { 
	if (c[i] == '&')
	  {
	    if (c[i+1] == '&')
	      return 1;
	  }

	else if (c[i] == '|')
	  {
	    if (c[i+1] == '|')
	      return 1;
	  }
      }
  }

  return 0;
}

char* getSimpleCommand(const char* c, int start, int* end)
{
  int i=0, length = 0;
  while(c[start+i] != '\0')
    {
      if(!isOrdinaryToken(c[start+i]) && c[start+i] != ' ' && c[start+i] != 
	 '\n')
	{
	  end = start+i;
	  break;
	}
      i++;
    }

  length = end - start;
  char* simpleCommand = (char* ) malloc(length * sizeof(char));
  int j = start;
  while(j < end)
    {
      simpleCommand += c[j];
      j++;
    }

  return simpleCommand;
}

int size(char *c)
{
  int i, size = 0;
  for(i=0; c[i] != '\0'; i++)
    size++;
  return size;
}

//Checks entire string to be parsed
//Check | ; && ||
int isSimpleCommandValid(const char* c)
{
  int i=0, length, newlineIter;
  char* curr = c; //Current position of c
  char* simpleCommandTemp; //Store temporary simple command

  int end; //We always know the start based on current position but not the end

  while(c[i] != '\0')
    {
      simpleCommandTemp = getSimpleCommand(c, i, &end);
      length = size(simpleCommandTemp);

      //Check to make sure that there is an operand to match
      if(!isTwoDigitSpecialToken(c[i+length+2]))
	{
	  if(!isOneDigitSpecialToken(c[i+length+2]))
	    return 0;
	}

      
    }

  return 1;
}
/*int isPipeOrSemicolonValid(const char* c)
{
  int i;

  for(i=0; c[i] != '\0'; i++)
    {
      //Make sure to stay in the boundaries
      if(c[i+1] != EOF || c[i+2] != EOF)
	{
	  if(!isOrdinaryToken(c[i]))
	    return 0;
	  else if(!isOrdinaryToken(c[i+2]))
	    {   
	      if(c[i+2] != '\n')
		return 0;

	      c += 3;

	      //Keep going until non-newline
	      while(*c != '\0')
		{
		  if(isOrdinaryToken(*c))
		    return 1;
		  else if(!isOrdinaryToken(*c) && *c != '\n')
		    return 0;
		  c++;
		}
	    }   
	}
    }   
  return 0;
  }*/

//Check && and ||
int isAndOrValid(const char* c)
{ 
  if(c[1] != EOF || c[2] != EOF || c[3] != EOF)
  {
    if(isOrdinaryToken(c[0]))
      return 0;
    else if(isOrdinaryToken(c[3]))
    {
      if(c[3] != '\n')
  return 0;

      c += 4;
      //Keep going until newline
      while(*c != '\0')
      {
	if(isOrdinaryToken(*c))
	  return 1;
	else if(!isOrdinaryToken(*c) && *c != '\n')
	  return 0;
	c++;
      }
    }
  }
  return 0;
}

  //Check < or > 
  int isIOValid(const char *c, int* successiveIOCheck)
  {
    if(c[1] != EOF || c[2] != EOF || c[3] != EOF || c[4] != EOF)
    {
      if(!isOrdinaryToken(c[0]))
	return 0;

      else if(!isOrdinaryToken(c[2]))
	return 0;

      else if(isOrdinaryToken(c[2]) && c[1] == '<')
      {
	if(c[4] == '<')
	  return 0;
	*successiveIOCheck = 1;
      }

      else if(isOrdinaryToken(c[2]) && c[1] == '>')
      {
	if(c[4] == '<' || c[4] == '>')
	  return 0;
	*successiveIOCheck = 1;
      }
      return 1;
    }
    return 0;
  }

//Check ( and )
int isParenthesesValid(const char* c)
{
  int open = 0;
  int close = 0;

  //Make sure number of open and closed parentheses are equal
  while(*c != '\0')
  {
    if(*c == '(')
      open++;
    else if(*c == ')')
      close++;
  }

  if(open == close)
    return 1;
  return 0;
}

//Check #
int isCommentValid(const char* c)
{
  //<operand># is invalid
  if(isOrdinaryToken(c[0]) && c[1] == '#')
    return 0;
  //Any other condition is valid
  while(*c != '\0')
    {
      if(*c == '\n')
	return 1;
      c++;
    }
  return 1;
  //Characters keep going up to (but not including) the next newline
}

int isBufferValid(char *c)
{
  //Make it easier to parse by removing extraneous white space
  removeWhiteSpace(c); 
  
  //Run checks
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
