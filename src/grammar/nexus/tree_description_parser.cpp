/*
 * Brandon Chisham
 * Created: 3-26-08
 * Implementation of a tree description parser for Nexus/Newick style wstrings.
 */
#include "tree_description_parser.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <util.hpp>
#include <node.hpp>
/* Enforce assertions here */
#ifdef NDEBUG
#define NDEBUG_WAS_SET NDEBUG
#undef NDEBUG
#endif
/*
 * Print the tree topology.
 */
using namespace std;

static void indent( wostream& out, unsigned level );

namespace CDAO {  
  /*
   * Start point parse the tree from the beginning.
   */
  void TreeDescriptionParser::parse(){
    parsetree_ = new Node( labelMaker( L"tree" ) );
    //cerr << L"parse()\n";
    consume_tree( parsetree_ );
    //cerr << L"exiting: parse()\n";
    
  }
  /*
   * Print an error message then exit.
   */
  void print_parse_error_message( wstring error_token  );
  
  /*
   * Parse a subtree.
   * Cases:
   * TREE -> ( TREE_LIST ) LABEL
   * TREE -> ( TREE_LIST ) 
   */
  void  TreeDescriptionParser::consume_tree(  Node* current, unsigned level ){
    
    assert( current );
    //indent( wcerr, level );
    //wcerr << L"consume_tree( current: " << current << L", level" << level << L")\n";
    /* Expect ( */
    TokenPackage start_tree = scanner.lookAhead();

   
    if (start_tree.getType() == START_TREE){
      consume_start_tree( current );
      /*Found ( */
      /* Expect TREE_LIST */
      consume_tree_list( current, level+1 ) ;
      /* Found TREE_LIST */
      /* Expect ) */
      consume_end_tree( current );
      /* Found ) */
      if ( scanner.lookAhead().getType() == LABEL || scanner.lookAhead().getType() == QUOTED_STRING_MARKER ){
	/* Expect LABEL */
	consume_label( current, level+1 );
      }
      else if (scanner.lookAhead().getType() == DELIMITER){
         //skip and pass up to caller
      }
      else if (scanner.lookAhead().getType() == END_TREE){
	//make up a name for the tree if it's not supplied
	current->setLabel( labelMaker( L"tree" ) );
      }
      else if (scanner.lookAhead().getType() == END){
        //we're done parsing the tree YAY!
      }
      else {
        //indent( wcerr, level );
        print_parse_error_message( L"consume_tree: " + scanner.lookAhead().getContents() );
      }
    }
    //indent( wcerr, level );
    //wcerr << L"exiting consume_tree( " << current << L", level: " << level << L")\n";
    return;
  }
  
  
  /*
   * Consume a TREE_LIST
   * TREE_LIST -> TREE
   *           -> LABEL
   *           -> TREE , TREE_LIST
   *           -> LABEL, TREE_LIST  
   */
  void TreeDescriptionParser::consume_tree_list( Node* current, unsigned level ){
    assert( current );
    //indent( wcerr, level );
    //wcerr << L"consume_tree_list( current: " << current << L" level:" << level <<  L")\n";
    
    /* Expect TREE or LABEL */
    TokenPackage next_token = scanner.lookAhead();
    //indent(wcerr, level );
    //wcerr << L"Next Token is type: " << next_token.getType() << L" has contents: \"" << next_token.getContents() << L"\"\n";
    //indent( wcerr, level );
    //wcerr << L"Expecting type: START_TREE or LABEL" << L"\n"; 
    
    if (next_token.getType() == START_TREE){
      Node* subtree = new Node( labelMaker( L"node" ) );
      consume_tree( subtree, level +1 );
      subtree->setAncestor( current );
      current->addDescendant( subtree );
      /* Found TREE  */
      /* Expect TREE_LIST or END_TREE */
      TokenPackage continue_list_or_end_tree = scanner.lookAhead();
      if (continue_list_or_end_tree.getType() == DELIMITER){
	consume_delimiter( current );
        consume_tree_list( current, level+1 );
      }
      else if (continue_list_or_end_tree.getType() == END_TREE){
     	/* Found TREE_LIST */
      }
      else {
        //indent( wcerr, level );
	print_parse_error_message( L"consume_tree_list:" + continue_list_or_end_tree.getContents()  );
      }
    }
    else if ( next_token.getType() == LABEL || next_token.getType() == QUOTED_STRING_MARKER){
      Node* subtree = new Node();
      consume_label( subtree, level );
      current->addDescendant( subtree );
      subtree->setAncestor( current );
      /* Found LABEL */
      /* Expect END_TREE or TREE_LIST */
      TokenPackage continue_list_or_end_tree = scanner.lookAhead();
      if(continue_list_or_end_tree.getType() == DELIMITER){
	consume_delimiter( current );
	/* Found ....LABEL, TREE_LIST */
	consume_tree_list( current, level+1 );
      }
      else if (continue_list_or_end_tree.getType() == END_TREE){
          //indent(wcerr,level);
          //wcerr << "End of sub-tree level: " << level << "\n";
      }
      else {
        print_parse_error_message( L"expected LABEL or \'LABEL\': " + continue_list_or_end_tree.getContents() );
       }
    }
    else {
      print_parse_error_message( next_token.getContents() );
    }
    //indent( wcerr, level );
    //wcerr << L"exiting consume_tree_list( " << current << L", level: " << level << L" )\n";
    
  }
  
  
  
  /*
   * Consume a label token.
   * LABEL -> [a-zA-Z0-9]+ EXTENDED_META
   *       | ' LABEL '
 */
  void  TreeDescriptionParser::consume_label(  Node* current, unsigned level ){
    assert( current );
    //indent( wcerr, level );
    //wcerr << L"consume_label( " << current << L" )\n";
    TokenPackage label = scanner.lex();
    if ( label.getType() == LABEL ){
     // indent( wcerr, level );
     // wcerr << "consumed a label: \"" <<  label.getContents()<<  "\n";
        current->setLabel( label.getContents()  );
    }
    else if (label.getType() == QUOTED_STRING_MARKER){
        label = scanner.lex();
        wstring current_label = L"";
        while ( label.getType() == LABEL ){
          //indent( wcerr, level );
         // wcerr << "consumed a quoted label\n";
          current_label += label.getContents() + L"_";
          //indent( wcerr, level );
          //wcerr << "contents: \"" << label.getContents()  << "\" current_label \"" << current_label << "\n";
           //current->setLabel( label.getContents() );
           label = scanner.lex();
        }
        if (current_label.size()){current_label = current_label.substr(0, current_label.size() -1 );}
        current->setLabel( current_label );
        //indent( wcerr, level );
        //wcerr << "consumed a label: \"" <<  label.getContents()<<  "\"\n";

        assert( label.getType() == QUOTED_STRING_MARKER );
    }
    consume_extended_meta( current );
    //cerr << L"exiting: consume_label( L" << current << L" )\n";
  return ;
  
  }
  /*
   * Consume the start tree token.
   * START_TREE -> '('
   */
  void  TreeDescriptionParser::consume_start_tree(   Node* current ){
    assert( current );
    assert( scanner.lex().getType() == START_TREE );
    
    //cerr << L"enter/exit consume_start_tree()\n";
    
    return;

  }
  /*
   * Consume the end tree token.
   * END_TREE -> ')'
   */
  void  TreeDescriptionParser::consume_end_tree(   Node* current ){
    assert( current );
    assert( scanner.lex().getType() == END_TREE );
    
    //cerr << L"enter/exit consume_end_tree()\n";
    return;
  }
  /*
   * Consume a delimiter.
   * DELIMITER -> ','
   */
  void  TreeDescriptionParser::consume_delimiter(   Node* current ){
    assert( current );
    assert( scanner.lex().getType() == DELIMITER );
    return;
  }
   /*
    * Consume weight/comment after tree labels.
    * EXTENDED_META -> WEIGHT | COMMENT | WEIGHT COMMENT | Epsilon
    */
  void TreeDescriptionParser::consume_extended_meta( Node* current ){
    //Epsilon
    if ( scanner.lookAhead().getType() == SPACE){ return; }
    //WEIGHT | WEIGHT COMMENT
    else if ( scanner.lookAhead().getType() == START_WEIGHT ){ 
      consume_weight( current ); 
      if (scanner.lookAhead().getType() == START_COMMENT){ consume_comment( current );  }
    }
    //COMMENT
    if (scanner.lookAhead().getType() == START_COMMENT){ consume_comment( current );  }
    return;
  }
  
  /*
   * WEIGHT -> ':' WEIGHT
   */
  void TreeDescriptionParser::consume_weight( Node* current ){
    assert( scanner.lex().getType() == START_WEIGHT );
    TokenPackage weight = scanner.lex();
    current->setWeight( strtod( wstr_to_str( weight.getContents()).c_str(), NULL )  );
    return;
  }
  /*
   * Consume a comment.
   * COMMENT -> '[' (LABEL | SPACE)* ']'
   */
  void TreeDescriptionParser::consume_comment( Node* current ){
    assert( scanner.lex().getType() == START_COMMENT );
    while ( scanner.lookAhead().getType() != END_COMMENT ){ scanner.lex(); }
    scanner.lex();//consume the end comment mark
    return;
    
  }

void print_parse_error_message( wstring error_token  ){
	wcerr << L"Unexpected Token \"" << error_token << L"\" in the input.\n";  
	exit( 1 );

}
}

void indent( wostream& out, unsigned level ){
    //for (unsigned i = 0; i < level; ++i){ out << "  "; }
    return;
}

#ifdef NDEBUG_WAS_SET
#define NDEBUG NDEBUG_WAS_SET
#undef NDEBUG_WAS_SET
#endif
