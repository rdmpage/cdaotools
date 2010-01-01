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

void do_extraction( const string& nexus_tree_file_path );

//Node* parse_description( const string& newick );

//DataRepresentation* populate_model( vector< string > TUs, map< string, Node* > trees );

int main( int argc, char** argv ){
  if (argc > 1 ){
     processArgs( argc, argv, NULL );
     static_cast< ifstream* >( getInputStream() )->close();
     static_cast< ofstream* >( getOutputStream() )->close();
     //cerr << "Extracting tress from: " << getInputFile() << endl;
     do_extraction( getInputFile() );       
  }

  return 0;
}


void do_extraction( const string& nfp ){
  const string extractor_script = "extract_trees.pl";
  const int READ_END = 0;
  const int WRITE_END = 1;
  int pipefd[2];
  int pipe_err = pipe( pipefd );
  if (pipe_err > -1 ){ 
    pid_t extractor = fork();
    if ( 0 == extractor ){
     /* child code */
     if (close( pipefd[ READ_END ] ) > -1 ){
       if ( dup2( pipefd[ WRITE_END ], STDOUT_FILENO ) > -1 ){
         if(  execlp( extractor_script.c_str(), extractor_script.c_str(), nfp.c_str(), (char*)NULL ) < 0 ){
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
               if ( execlp("cdao-read-tree", "cdao-read-tree", "-i", nfp.c_str(), "-o", getOutputFile().c_str(), (char*)NULL) < 0 ){
                perror("Exec Failed");
              }
          } else { perror( "dup for read-tree process failed" ); }
       }
      close(pipefd[READ_END]);
      waitpid( extractor, NULL, 0 );
      waitpid( writer, NULL, 0 );
    }
    else { perror( "close failed" ); exit( 1 ); }
  }
  return;
}

//Node* parse_description( const string& newick ){
//  return NULL;  
//}
