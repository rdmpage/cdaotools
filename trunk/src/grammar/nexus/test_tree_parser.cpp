/*
 * parse and print the parse tree for a tree given as an argument
 */
#include "tree_description_parser.hpp"
#include <iostream>

using namespace std;
using namespace CDAO;

int main(int argc, wchar_t** argv){
    cerr << L"Called with argc: L" << argc << L" arguments\n";
    if ( argc < 2){
       cout << L"usage: ./test_tree_parser TREE\n";
       exit( 1 );
    }
    cerr << L"Parsing tree: L" << argv[ 1 ] << L"\n";
    TreeDescriptionParser parser( argv[ 1 ] );
    cerr << L"Parsed the tree printing the results\n";
    parser.getParseTree()->printTree();
    cerr << L"Printed the parse tree \n";
    return 0;

}
