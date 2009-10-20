#include "tree_description_scanner.hpp"
#include <iostream>

using namespace std;
using namespace CDAO;

int main( int argc, char** argv){
  
  if (argc < 2){
  	cerr << "Usage: ./test_tree_scanner TREE\n";
  }
  
  TreeDescriptionScanner scanner( argv[ 1 ] );

  while ( !scanner.isDone()){
	TokenPackage current = scanner.lex();
	cout << "Token Type: " << current.getType() << " Contents: \"" << current.getContents() << "\"\n";
  }

  return 0;
}
