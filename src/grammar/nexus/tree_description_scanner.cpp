/*
 * Scan tree description strings produced by NCL and extract tokens
 * needed to build the heirarchy.
 */

#include "tree_description_scanner.hpp"
#include <limits.h>
#include <cstdlib>
#include <cctype>
#include <iostream>
using namespace std;
namespace CDAO {
const int TABLE_SIZE = ( TOKENS_SIZE  );
/*
 * Looks the the next character for token boundries.
 */
static Token_t lookAheadString( int pos, const string& target);
/*
 * Extracts a token from the target string.
 */
TokenPackage TreeDescriptionScanner::lex(){
  
  //cerr << "lex(): current_position_: " << current_position_ << " target_.size(): " << target_.size() << "\n";

  if (isDone()){ return TokenPackage(END, ""); }
  string token_in_progress;
   Token_t next_tok_type;
   Token_t prev_tok_type;
   /* Consume leading white-space */
   while (!isDone() && (next_tok_type = lookAheadString(current_position_, target_)) == SPACE){ current_position_++; }
   if (next_tok_type != LABEL ){  return TokenPackage(next_tok_type, string("") + target_.at(current_position_++)); }

   /* Extract token contents */
   while (!isDone() && (next_tok_type = lookAheadString(current_position_, target_)) == LABEL){
   	prev_tok_type = next_tok_type;
        token_in_progress += target_.at(current_position_);
	current_position_++;
   }
//   cerr << "Extracted token type: " << prev_tok_type << " as character: \'" << 
  //        (isprint( prev_tok_type) ? (char)prev_tok_type : '?') << 
//	  "\' contents: \""  << token_in_progress << "\"\n";
   
   /* Contruct and hand off the token. */
   return   TokenPackage(prev_tok_type, token_in_progress);
}
/*
 * Build the next token, but do not move the input pointer.
 */
TokenPackage TreeDescriptionScanner::lookAhead(){
	int start_position = current_position_;
	TokenPackage ret = this->lex();
	current_position_ = start_position;
	return ret;
}

Token_t lookAheadString( int pos, const string& target ){
      static Token_t* lookupTable = NULL;
      /* Only first invocation, build a lookup table. */
      if (NULL == lookupTable){ 
	/*
	 * Allocate table. (Entries innitialized in UNDEFINED which is conveniently 0.
	 */
	lookupTable = (Token_t*)calloc(TABLE_SIZE, sizeof(Token_t));
	/*
	 * Define entries of interest.
	 */
	lookupTable[ '(' ] = START_TREE;
	lookupTable[ ')']  = END_TREE;
	lookupTable[ ',' ] = DELIMITER;
	lookupTable[ ' ' ] = SPACE;
	lookupTable[ '\t'] = SPACE;
	lookupTable[ ':' ] = START_WEIGHT;
	lookupTable[ '[' ] = START_COMMENT;
	lookupTable[ ']' ] = END_COMMENT;
	for (char letter = 'a'; letter <= 'z'; ++letter){
	   lookupTable[ letter ] = LABEL;
	   lookupTable[ toupper(letter) ] = LABEL;
	}
	for (char number = '0'; number <= '9'; ++number){
	   lookupTable[ number ] = LABEL;
	}
        lookupTable[ '.' ] = LABEL;
        lookupTable[ '_' ] = LABEL;
        lookupTable[ '-' ] = LABEL;
        //lookupTable[ ':' ] = LABEL;
        //lookupTable[ '[' ] = LABEL;
        //lookupTable[ ']' ] = LABEL;
      }
      /*
       * Return token type to the caller.
       */
      return lookupTable[ target.at( pos ) ];
      
}
}
