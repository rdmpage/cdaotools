#include "grammar/nexus/tree_description_parser.hpp"
#include "data_representation.hpp"

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
     do_extraction( argv[1] );       
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
       if ( dup2( STDOUT_FILENO, pipefd[ WRITE_END ] ) > -1 ){
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
   if ( dup2( STDIN_FILENO, pipefd[READ_END] ) > -1 ){
      string command;
      string taxon_name;
      string tree_name;
      string rooted;
      string newick_data;
      vector< string > taxa(0);
      map< string, Node* > trees = map< string, Node* >();
      while (!cin.eof()){
        cin >> command;
        if (command == "TU"){
           cin >> taxon_name;
           taxa.push_back( taxon_name );
        }
        else if ( command == "TREE"){
           cin >> tree_name;
           cin >> rooted;
           cin >> newick_data;
           trees[ tree_name ] = TreeDescriptionParser( newick_data ).getParseTree();
        }
        else {
            cerr << "Unrecognized command: " << command << endl;
            exit( 1 );
        }
      }
   }
   else { perror( "dup failed" ); exit( 1 ); }
  }
  else { perror( "close failed" ); exit( 1 ); }
  return NULL;
}

Node* parse_description( const string& newick ){
    
}
