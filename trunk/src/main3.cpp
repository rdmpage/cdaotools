#include "grammar/nexus/tree_description_parser.hpp"
#include "data_representation.hpp"
#include <RawStream.hpp>

#include "util.hpp"
#include "node.hpp"
#include "codegen/codegen.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>


using namespace std;
using namespace CDAO;

DataRepresentation* do_extraction( const string& nexus_tree_file_path );

Node* parse_description( const string& newick );

DataRepresentation* populate_model( vector< string > TUs, map< string, Node* > trees );

int main( int argc, char** argv ){
  if (argc > 1 ){
     processArgs( argc, argv, NULL );
     do_extraction( getInputFile() );       
  }

  return 0;
}


DataRepresentation* do_extraction( const string& nfp ){
  const string extractor_script = "grammar/nexus/extract_trees.pl";
  const int READ_END = 0;
  const int WRITE_END = 1;
  pid_t extractor = fork();
  int pipefd[2];
  int pipe_err = pipe( pipefd );
  if ( 0 == extractor ){
     /* child code */
     if (close( pipefd[ READ_END ] ) > -1 ){
       if ( dup2( pipefd[ WRITE_END ], STDOUT_FILENO ) > -1 ){
         if(  execlp( extractor_script.c_str(), extractor_script.c_str(), nfp.c_str(), (char*)NULL ) <0 ){
            perror("exec failed");
            exit( 1 );
         }
       }
       else { perror( "dup failed"); exit(1); }
     }
     else { perror("close failed"); exit(1); }
  }
  if ( close( pipefd[ WRITE_END ] ) > -1){
      pid_t writer = fork();
      if (0 == writer){
          /* child code */
          if (dup2( pipefd[READ_END], STDIN_FILENO ) > -1){
              if (execlp("cdao-read-tree", "cdao-read-tree", nfp.c_str(), (char*)NULL) < 0 ){
                perror("Exec Failed");
              }
          }
      }
      close(pipefd[READ_END]);
  }
  else { perror( "close failed" ); exit( 1 ); }
  return NULL;
}

Node* parse_description( const string& newick ){
  return NULL;  
}
