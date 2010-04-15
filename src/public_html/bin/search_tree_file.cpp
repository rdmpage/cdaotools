/**
 * Program to iteratively search a text file.
 */
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cctype>
#include <sys/types.h>
#include <sys/wait.h>
#include <wstring>
#include <ncurses.h>
#include <unistd.h>
#include <wstrings.h>
#include <fstream>

using namespace std;
/*
 * Find and return matches from the specifed file.
 */
wstring getmatches( wstring pattern, wstring src_file );

int main(int argc, wchar_t** argv){
  int ch =0;
  wstring search = L"";
  wstring results = L"";
   wstring results_file = L"/dev/null";

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
     if ( ch == KEY_BACKSPACE || ch == KEY_DC ){ 
           int r = search.size() - 1; 
           search = search.substr(0, r > 0 ? r : 0 ); 
     }
     else if ( ch == KEY_DL ){ search = L"";  }
     else {     
           search += (wchar_t)ch;
     }
     results = getmatches( search, argv[1] );
     clear(); 
     printw( L"%s\n%s",search.c_str(), results.c_str());
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
wstring getmatches( wstring pattern, wstring src_file ){
   const unsigned READ_END = 0;
   const unsigned WRITE_END = 1;
   int pfd[2];
   int ch = 0;
   wstring ret = L"";
   pipe( pfd );
   pid_t child = fork();
   if ( 0 == child ){
       close( pfd[READ_END] );
       dup2( pfd[WRITE_END], STDOUT_FILENO );
       execlp( L"grep", L"grep", pattern.c_str(), src_file.c_str() , (wchar_t*)NULL );
    }
    else {
      close( pfd[WRITE_END] );
      FILE* pipestream = fdopen(pfd[READ_END], L"r");
      while ( (ch = fgetc(pipestream)) != EOF ){  ret += (wchar_t)ch; }
      fclose( pipestream );
      wait(NULL);
    }
    return ret;
}
