#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define BUFF_SIZE LINE_MAX
#define READ_END 0
#define WRITE_END 1

/*
 * Driver program to fake interactivity with the prolog shell.
 * Brandon Chisham
 * March 10, 2010
 */

int main( int argc, char** argv ){
 
  int driver_to_prolog[2];
  int prolog_to_driver[2];
  //fd_set input_des_set;
  //fd_set output_des_set;
  //fd_set except_set;
  //FILE* pipe_in = NULL;
  //FILE* pipe_out = NULL;
  pid_t prolog_proc;
  //pid_t driver_proc;
  //int ch;
  int pstats;
  //int dstats;
  //int read_size;
  //int i =0;
  //int max_des;
  //int des_ready;
  ssize_t read_size;
  size_t len;
  char out_buff[BUFF_SIZE];
  char in_buff[BUFF_SIZE];
  //setup communication channels.
  if ( pipe(driver_to_prolog) < 0){ perror("Unable to create pipe"); exit(1); }
  if ( pipe(prolog_to_driver) < 0 ){ perror("Unable to create pipe"); exit(1); }
  /*
   * Start the sepecified compliled prolog program.
   */
  if ( (prolog_proc = fork()) ){
       close(driver_to_prolog[WRITE_END]);
       close(prolog_to_driver[READ_END] );
       if (dup2( driver_to_prolog[READ_END], STDIN_FILENO ) < 0){ perror("error duping stdin"); exit(1);}
      // if (dup2(prolog_to_driver[WRITE_END], STDOUT_FILENO) < 0){ perror("error duping stdout"); exit(1); }
       if (dup2(prolog_to_driver[WRITE_END], STDERR_FILENO) < 0){ perror("error duping stderr"); exit(1); }
       if (execlp(argv[1], argv[1])<0){ perror("error starting prolog executable"); exit(1); }
  }
  //close the descriptiors the parent won't be using.
  close( driver_to_prolog[READ_END] );
  close( prolog_to_driver[WRITE_END] );
  //initialize the output buffer
  bzero( out_buff, BUFF_SIZE );
  //copy the goal state into the output buffer appending newline.
  len = strnlen(argv[2], BUFF_SIZE);
  strncpy(out_buff, argv[2], len);
  out_buff[len] = '\n'; 
  len++;
  //write initial command
  write( driver_to_prolog[WRITE_END], out_buff, len );
  //clear the buffer
  do {
    bzero(in_buff, BUFF_SIZE);
    bzero( out_buff, BUFF_SIZE );
    //read the result of the last operation.
    read_size = read( prolog_to_driver[READ_END], in_buff, BUFF_SIZE );
    if ( strncmp(in_buff, "false.", sizeof("false.")-1) == 0 ) break;
    printf("%s\n", in_buff);
    bzero( out_buff, BUFF_SIZE );
    //fgets(out_buff, BUFF_SIZE, stdin);
    strncpy( out_buff, ";\n", sizeof(";\n") );
    len = strnlen(out_buff, BUFF_SIZE);
    //strncpy(out_buff, ";\n", sizeof(";\n"));
    write(driver_to_prolog[ WRITE_END ], out_buff, len  );
    //fprintf(stderr, "wrote \"%s\" to the prolog shell\n", out_buff);

  }while ( strncmp( in_buff, "false.", sizeof("false.")-1 ) );
   
  //end the session.

    bzero(out_buff, BUFF_SIZE);
    strncpy(out_buff, "\nhalt.\n", BUFF_SIZE);
    len = strnlen(out_buff, BUFF_SIZE );
    write(driver_to_prolog[ WRITE_END ], out_buff, len );
    close(driver_to_prolog[ WRITE_END ]);
    close(prolog_to_driver[READ_END]);
    waitpid( prolog_proc, &pstats, 0 );
  return 0;
}


