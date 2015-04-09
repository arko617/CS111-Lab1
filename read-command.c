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

// Stack implementation reference to:
// http://groups.csail.mit.edu/graphics/classes/6.837/F04/cpp_notes/stack1.html
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
int isSpecialToken(const char c)
{
  return (c == ';' || c == '|' || c == '(' || c == ')' || c == '<' || 
	  c == '>' || c == '&');
}

int isValid(char *c)
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


command_tree make_command_tree (char *c) {
  struct Stack cmdStack;
  struct Stack oprStack;

  Stack_Init(&cmdStack);
  Stack_Init(&oprStack);

  int i = 0;
  while(c[i] != '\0'){

    if(isOrdinaryToken(c[i])){
      struct command x;
      int count = 0;
      x.type = SIMPLE_COMMAND;
      x.u.word = (char**)malloc(sizeof(char*) * 100);
      
      int j = 0;
      while(j < 100){
        x.u.word[j] = (char*)malloc(sizeof(char) * 100);
        j++;
      }
      
      while(isOrdinaryToken(c[i]) || c[i] == ' '){
        int len = 0;
        while(isOrdinaryToken(c[i])){
          x.u.word[count][len] = c[i];
          len++;
          i++;
      }

      if(len > 0){
        x.u.word[count][len] = '\0';
        count++;
      }

      if(c[i] == ' '){
        i++;
      }
    }

    Stack_Push(&cmdStack, &x)
  }    

    else if(c[i] == '('){
      struct command x;
      x.type = SUBSHELL_COMMAND;
    }

    else if(c[i] == '&' || c[i] == '|'){
      if(oprStack.size == 0){

      }

      else{

      }
    }
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
