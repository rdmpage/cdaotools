/**
 * Program to iteratively search a text file.
 */
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cctype>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include <strings.h>
#include <fstream>

using namespace std;
/*
 * Find and return matches from the specifed file.
 */
string getmatches( string pattern, string src_file );

int main(int argc, char** argv){
  int ch =0;
  string search = "";
  string results = "";
   string results_file = "/dev/null";

  if ( argc > 2){
       results_file = argv[2];
  }
  //int pfd[2];
  //curses init
  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  //read until there's a space or end of file.
  while (  (ch = getch()) != EOF && !isspace( ch )){
      if ( ch == KEY_BACKSPACE ){ 
           int r = search.size() - 1; 
           search = search.substr(0, r > 0 ? r : 0 ); 
      }

     else {     
           search += (char)ch;
     }
     results = getmatches( search, argv[1] );
     clear(); 
     printw( "%s\n%s",search.c_str(), results.c_str());
     move(0, search.size() );
  }
  //curses clean-up
  endwin();
  //display final result list
  cout << results;
  ofstream resultf( results_file.c_str());
  //resultf.open( );
  resultf << results;
  resultf.close();
  return 0;
}
/*
 * search the specified file for the specified pattern.
 */
string getmatches( string pattern, string src_file ){
   const unsigned READ_END = 0;
   const unsigned WRITE_END = 1;
   int pfd[2];
   int ch = 0;
   string ret = "";
   pipe( pfd );
   pid_t child = fork();
   if ( 0 == child ){
       close( pfd[READ_END] );
       dup2( pfd[WRITE_END], STDOUT_FILENO );
       execlp( "grep", "grep", pattern.c_str(), src_file.c_str() , (char*)NULL );
    }
    else {
      close( pfd[WRITE_END] );
      FILE* pipestream = fdopen(pfd[READ_END], "r");
      while ( (ch = fgetc(pipestream)) != EOF ){  ret += (char)ch; }
      fclose( pipestream );
      wait(NULL);
    }
    return ret;
}
