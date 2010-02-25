#include <iwostream>
#include "treebase-node.hpp"

using namespace std;
using namespace Treebase;

int main(int argc, char** argv){
  QuotedStringNode* foo = new QuotedStringNode( "bar" );
  
  cerr << "Foo: \"" << foo << "\""  << endl;

  return 0;
}
