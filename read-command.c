// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"

#include <error.h>
#include <ctype.h> //For isalpha() and isdigit() functions
#include <stdio.h> //For EOF

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

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

int isParenthesesValid();

int isCommentValid();
//TODO parentheses
//TODO comments

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
