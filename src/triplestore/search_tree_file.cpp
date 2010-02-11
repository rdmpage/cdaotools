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
#include <curses.h>
#include <unistd.h>
#include <strings.h>

using namespace std;
/*
 * Find and return matches from the specifed file.
 */
string getmatches( string pattern, string src_file );

int main(int argc, char** argv){
  int ch =0;
  string search = "";
  string results = "";
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
      //if ( ch != ERR ){
        //pipe( pfd );
        clear();
        search += (char)ch;
        results = getmatches( search, argv[1] );
        printw( "%s\n%s",search.c_str(), results.c_str());
        move(0, search.size() );
       // cout << results << endl;
        //cout << search;
     //}
  }
  //curses clean-up
  endwin();
  //display final result list
  cout << results;
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
