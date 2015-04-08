// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"

#include <error.h>
#include <ctype.h> //For isalpha() and isdigit() functions
#include <string.h> //Put later
#include <stdio.h> //For EOF

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */
typedef struct command_stream *command_stream_t;
{
  char c;
  char* string;
};

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

    return 0;
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
  for(i = 0; s.string[i] != '\0'; i++)
  {
    if(s.string[1] != EOF && s.string[2] != EOF)
    {
      //Operand before, < or >, operand after
      if(isOrdinaryToken(s.string[0]) && (s.string[1] == '<' || s.string[1] ==
					  '>') && isOrdinaryToken(s.string[2]))
	return 1;
      
      else if(isOrdinaryToken(s.string[0]) && (s.string[1] == '|' || s.string[
1] == "&&" || s.string[1] == "||")
	      if(
    }
  }

  error (1, 0, "command reading not yet implemented");
  return 0;
}
