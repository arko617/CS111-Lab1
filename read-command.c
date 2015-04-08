// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <ctype.h>  //For isalpha() and isdigit() functions
#include <stdlib.h>
#include <stdio.h>  //For EOF
#include <error.h>
<<<<<<< HEAD
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


int isOrdinaryToken(const char c)
{
  return (isalpha(c) || isdigit(c) || c == '!' || c == '%' || c == '+' || c ==
    ',' || c == '-' || c == '.' || c == '/' || c == ':' || c == '@' || c
    == '^' || c == '_');
}

int isOneDigitSpecialToken(const char c)
{
  return (c == ";" || c == "|" || c == "(" || c == ")" || c == "<" || 
    c == ">");
}

int isTwoDigitSpecialToken(const char c[2])
{
  //Make sure the array does not go out of bounds
  if (c[1] != EOF)
  { 
    if (c[0] == '&')
    {
      if (c[1] == '&')
  return 1;
    }

    else if (c[0] == '|')
    {
      if (c[1] == '|')
  return 1;
    }
  }

  return 0;
}

//Check |, ;
int isPipeOrSemicolonValid(const char* c)
{
  if(c[1] != EOF || c[2] != EOF)
  {
    if(!isOrdinaryToken(c[0]))
      return 0;
    else if(!isOrdinaryToken(c[2]))
    {   
      if(c[2] != '\n')
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
  return 0;
}

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
  int isIOValid(const char c[5], int* successiveIOCheck)
  {
    //int successiveIOCheck = 0;
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
  successiveIOCheck = 1;
      }

      else if(isOrdinaryToken(c[2]) && c[1] == '>')
      {
  if(c[4] == '<' || c[4] == '>')
    return 0;
  successiveIOCheck = 1;
      }
      return 1;
    }
    return 0;
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
  
  int i;
  //Returns the root command of command tree every time read_command stream is called, and then advance to the next linked list node

  //Make sure not to go out of bounds

  error (1, 0, "command reading not yet implemented");
  return 0;
}
