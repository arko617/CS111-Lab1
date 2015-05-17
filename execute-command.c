// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"

#include <error.h>
#include <errno.h> //creates the integer errno

#include <unistd.h> //for pipe and fork
#include <stdio.h> //for standard input-output
#include <string.h> 
#include <sys/types.h> //for pid_t
#include <sys/wait.h> //for waitpid
#include <stdlib.h> //for exit
#include <fcntl.h> //for controlling file

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

int
command_status (command_t c)
{
  return c->status;
}

int ctr = 0;

void io(command_t c)
{
    if(c->input){
        int input;
        int dp2;
        input = open(c->input, O_RDWR, 0644);
        
        if(input<0)
            error(3,0,"No such file");
        dp2 = dup2(input, 0);
        if(dp2 < 0)
            error(3, 0, "Redirect failed");
        close(input);
    }
    if(c->output)
    { 
        int output;
        int dp2;
        
        output =  open(c->output, O_CREAT|O_TRUNC|O_WRONLY, 0644);
        if( output < 0)
            error(1,0,"Cannot write to the file");
        dp2 = dup2(output, 1);
      if( dp2 < 0)
        error(1, 0, "Could not use dup2 for output");

      if( close(output) < 0)
        error(1, 0, "Could not close output file");
        
    }
}

void
execute_command (command_t c, bool time_travel)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
  //  print_command(c);   
  //return;
int fd[2];
    pipe(fd);
   pid_t x;
   pid_t firstPid;
   pid_t secondPid;
   //print_command(c);
     switch(c->type)
{    
    case SIMPLE_COMMAND:
        
        
        x = fork();
      if(x == 0) //child
        {   
            //printf("I should be here \n");
            io(c);
            execvp(c->u.word[0], c->u.word);
            
        //  printf("How the hell do I get here \n");
        
        exit(127);
            
        }
      if(x > 0) //parent
        {
            int status;
            waitpid(x, &status,0);
            //printf("status is %d and exit status is %d \n",status,WEXITSTATUS(status));
            c->status = WEXITSTATUS(status);
        }
        
            
    break;
        case AND_COMMAND:
            execute_command(c->u.command[0], time_travel);
                if(c->u.command[0]->status == 0)
                    {
                        execute_command(c->u.command[1], time_travel);
                        c->status = c->u.command[1]->status;
                    }
                else 
                    c->status = c->u.command[0]->status;    
                break;  
        case OR_COMMAND:
            execute_command(c->u.command[0], time_travel);

                if(c->u.command[0]->status != 0)
                    {
                        execute_command(c->u.command[1], time_travel);
                        c->status = c->u.command[1]->status;
                    }
                else 
                    c->status = c->u.command[0]->status;    
                break;
        case SEQUENCE_COMMAND:
            execute_command(c->u.command[0],time_travel);
            execute_command(c->u.command[1],time_travel);
            c->status = c->u.command[1]->status;
            break;
            
        case PIPE_COMMAND:
            
                firstPid = fork();
                if (firstPid == 0) //execute cmd on the right
        {
            close(fd[1]); //close unused write end
            dup2(fd[0], 0);
            //char *a[] = { “sort”, 0 };
            //execvp(a[0], a);
            execute_command(c->u.command[1], time_travel);
            exit(c->u.command[1]->status);
        }
        else 
        {
            
            secondPid = fork();
        if (secondPid == 0) { //execute command on the left
            close(fd[0]); //close unused readend
            dup2(fd[1], 1);
            //char *a[] = { “cat”, “foo.txt”, 0 };
            //execvp(a[0], a);
            execute_command(c->u.command[0], time_travel);
            exit(c->u.command[0]->status);
        }
        else {
            close(fd[0]); //close unused read and write end
            close(fd[1]);
            int status;
            int returnedPid = waitpid(-1, &status, 0);
            if (returnedPid == secondPid) { //this means second finished first so                                         //wait on the first
                waitpid(firstPid, &status, 0);
                //c->status = WEXITSTATUS(status);
                c->status = c->u.command[1]->status;
                //set exit status of second
            }
            if (returnedPid == firstPid)
            {
                waitpid(secondPid, &status, 0);
                //set exit status of first
                c->status = c->u.command[0]->status;
            }
            }
        }
    break;
    
    case SUBSHELL_COMMAND:
        io(c);
        execute_command(c->u.subshell_command,time_travel);
        
        c->status = c->u.subshell_command->status;
        break;
        
  //  error (1, 0, "command execution not yet implemented");
}

}