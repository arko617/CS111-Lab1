// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
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
  while(c[i] != '\0'){
    if(isWord()){
      char c[100];
      while(1){
        
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
  error (1, 0, "command reading not yet implemented");
  return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  error (1, 0, "command reading not yet implemented");
  return 0;
}
