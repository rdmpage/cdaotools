#include "parser.hpp"
#include "codegen.hpp"
#include <iostream>
using namespace std;
using namespace Treebase;

extern int yyparse();
extern ListNode<StudyNode>* treebase_study_list;

using namespace std;

int main( int argc, char** argv ){

  if (0 == yyparse()){
      generate( cout, treebase_study_list );
  }

  return 0;
}
