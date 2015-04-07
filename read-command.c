// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"

#include <error.h>
#include <ctype.h> //For isalpha() and isdigit() functions

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

int isWord(char c)
{
  return (isalpha(c) || isdigit(c) || c == '!' || c == '%' || c == '+' || c ==
	  ',' || c == '-' || c == '.' || c == '/' || c == ':' || c == '@' || c
	  == '^' || c == '_');
}

int isSpecialToken(char* c)
{
  return (c == ";" || c == "|" || c == "&&" || c == "||" || c == "(" || c == ")" || c == "<" || c == ">")
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */

  //Valid input
  /*alphabets, digits, !, %, +, ",", - , ., /, :, @, ^, _ */

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
  
  error (1, 0, "command reading not yet implemented");
  return 0;
}
