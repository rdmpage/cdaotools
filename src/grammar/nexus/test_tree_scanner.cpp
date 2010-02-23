#include "tree_description_scanner.hpp"
#include <iostream>

using namespace std;
using namespace CDAO;

int main( int argc, wchar_t** argv){
  
  if (argc < 2){
  	cerr << L"Usage: ./test_tree_scanner TREE\n";
  }
  
  TreeDescriptionScanner scanner( argv[ 1 ] );

  while ( !scanner.isDone()){
	TokenPackage current = scanner.lex();
	cout << L"Token Type: L" << current.getType() << L" Contents: \"" << current.getContents() << L"\"\n";
  }

  return 0;
}
