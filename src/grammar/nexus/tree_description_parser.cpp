/*
 * Brandon Chisham
 * Created: 3-26-08
 * Implementation of a tree description parser for Nexus/Newick style strings.
 */
#include "tree_description_parser.hpp"
#include <cassert>
#include <iostream>
#include <string>
/* Enforce assertions here */
#ifdef NDEGUG
#define NDBUG_WAS_SET NDEBUG
#undef NDEGUG
#endif
/*
 * Print the tree topology.
 */
using namespace std;
namespace CDAO {  
  /*
   * Start point parse the tree from the beginning.
   */
  void TreeDescriptionParser::parse(){
    parsetree_ = new Node();
    //cerr << "parse()\n";
    consume_tree( parsetree_ );
    //cerr << "exiting: parse()\n";
    
  }
  /*
   * Print an error message then exit.
   */
  void print_parse_error_message( string error_token  );
  
  /*
   * Parse a substee.
   * Cases:
   * TREE -> ( TREE_LIST ) LABEL
   * TREE -> ( TREE_LIST ) 
   */
  void  TreeDescriptionParser::consume_tree(  Node* current ){
    
    assert( current );
    //cerr << "consume_tree( current: " << current << ")\n";
    /* Expect ( */
    TokenPackage start_tree = scanner.lookAhead();
    if (start_tree.getType() == START_TREE){
      consume_start_tree( current );
      /*Found ( */
      /* Expect TREE_LIST */
      consume_tree_list( current ) ;
      /* Found TREE_LIST */
      /* Expect ) */
      consume_end_tree( current );
      /* Found ) */
      if ( scanner.lookAhead().getType() == LABEL ){
	/* Expect LABEL */
	consume_label( current );
      }
      else if (scanner.lookAhead().getType() == END){
	//make up a name for the tree if it's not supplied
	current->setLabel( labelMaker( "tree" ) );
      }
      //else {
      //  print_parse_error_message( scanner.lookAhead().getContents() );
      //}
    }
    //cerr << "exiting consume_tree( " << current << ")\n";
    return;
  }
  
  
  /*
   * Consume a TREE_LIST
   * TREE_LIST -> TREE
   *           -> LABEL
   *           -> TREE , TREE_LIST
   *           -> LABEL, TREE_LIST  
   */
  void TreeDescriptionParser::consume_tree_list( Node* current ){
    assert( current );
    
    //cerr << "consume_tree_list( current: " << current << ")\n";
    
    /* Expect TREE or LABEL */
    TokenPackage next_token = scanner.lookAhead();
    
    //cerr << "Next Token is type: " << next_token.getType() << " has contents: \"" << next_token.getContents() << "\"\n";
    //cerr << "Expecting type: START_TREE: " << START_TREE << " or LABEL: " << LABEL << "\n"; 
    
    if (next_token.getType() == START_TREE){
      Node* subtree = new Node();
      consume_tree( subtree );
      subtree->setAncestor( current );
      current->addDescendant( subtree );
      /* Found TREE  */
      /* Expect TREE_LIST or END_TREE */
      TokenPackage continue_list_or_end_tree = scanner.lookAhead();
      if (continue_list_or_end_tree.getType() == DELIMITER){
	consume_delimiter( current );
        consume_tree_list( current );
      }
      else if (continue_list_or_end_tree.getType() == END_TREE){
     	/* Found TREE_LIST */
      }
      else {
	print_parse_error_message( continue_list_or_end_tree.getContents()  );
      }
    }
    else if ( next_token.getType() == LABEL){
      Node* subtree = new Node();
      consume_label( subtree );
      current->addDescendant( subtree );
      subtree->setAncestor( current );
      /* Found LABEL */
      /* Expect END_TREE or TREE_LIST */
      TokenPackage continue_list_or_end_tree = scanner.lookAhead();
      if(continue_list_or_end_tree.getType() == DELIMITER){
	consume_delimiter( current );
	/* Found ....LABEL, TREE_LIST */
	consume_tree_list( current );
      }
      else if (continue_list_or_end_tree.getType() == END_TREE){
	/* FOUND ...LABEL */
      }
      else {
        print_parse_error_message( continue_list_or_end_tree.getContents() );
      }
  }
    else {
      print_parse_error_message( next_token.getContents() );
    }
    
    //cerr << "exiting consume_tree_list( " << current << " )\n";
    
  }
  
  
  
  /*
   * Consume a label token.
   * LABEL -> [a-zA-Z0-9]+ EXTENDED_META
 */
  void  TreeDescriptionParser::consume_label(  Node* current){
    assert( current );
    //cerr << "consume_label( " << current << " )\n";
    TokenPackage label = scanner.lex();
    assert( label.getType() == LABEL );
    current->setLabel( label.getContents()  );
    consume_extended_meta( current );
    //cerr << "exiting: consume_label( " << current << " )\n";
  return ;
  
  }
  /*
   * Consume the start tree token.
   * START_TREE -> '('
   */
  void  TreeDescriptionParser::consume_start_tree(   Node* current ){
    assert( current );
    assert( scanner.lex().getType() == START_TREE );
    
    //cerr << "enter/exit consume_start_tree()\n";
    
    return;

  }
  /*
   * Consume the end tree token.
   * END_TREE -> ')'
   */
  void  TreeDescriptionParser::consume_end_tree(   Node* current ){
    assert( current );
    assert( scanner.lex().getType() == END_TREE );
    
    //cerr << "enter/exit consume_end_tree()\n";
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
    current->setWeight( strtod( weight.getContents().c_str(), NULL )  );
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

void print_parse_error_message( string error_token  ){
	cerr << "Unexpected Token \"" << error_token << "\" in the input.\n";  
	exit( 1 );

}
}
#ifdef NDEBUG_WAS_SET
#define NDEBUG NDEBUG_WAS_SET
#undef NDEBUG_WAS_SET
#endif
