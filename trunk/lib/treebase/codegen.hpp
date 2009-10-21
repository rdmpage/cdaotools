#ifndef TREEBASE_CODEGEN_HPP_
#define TREEBASE_CODEGEN_HPP_
#include <string>
#include <fstream>
#include <iostream>
#include "treebase-node.hpp"

namespace Treebase {
    void generate( std::ostream& out, ListNode<StudyNode>* tb);
}

#endif
