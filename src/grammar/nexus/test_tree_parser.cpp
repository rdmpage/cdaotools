/*
 * parse and print the parse tree for a tree given as an argument
 */
#include "tree_description_parser.hpp"
#include <iostream>

using namespace std;
using namespace CDAO;

int main(int argc, char** argv){
    cerr << "Called with argc: " << argc << " arguments\n";
    if ( argc < 2){
       cout << "usage: ./test_tree_parser TREE\n";
       exit( 1 );
    }
    cerr << "Parsing tree: " << argv[ 1 ] << "\n";
    TreeDescriptionParser parser( argv[ 1 ] );
    cerr << "Parsed the tree printing the results\n";
    parser.getParseTree()->printTree();
    cerr << "Printed the parse tree \n";
    return 0;

}
