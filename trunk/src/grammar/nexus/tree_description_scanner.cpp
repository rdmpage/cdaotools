/*
 * Scan tree description wstrings produced by NCL and extract tokens
 * needed to build the heirarchy.
 */

#include "tree_description_scanner.hpp"
#include <limits.h>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <cassert>
using namespace std;
namespace CDAO {
const int TABLE_SIZE = ( TOKENS_SIZE  );
/*
 * Looks the the next wchar_tacter for token boundries.
 */
static Token_t lookAheadString( int pos, const wstring& target);
/*
 * Extracts a token from the target wstring.
 */
TokenPackage TreeDescriptionScanner::lex(){
  
  //cerr << L"lex(): current_position_: L" << current_position_ << L" target_.size(): L" << target_.size() << L"\n";

  if (isDone()){ return TokenPackage(END, L""); }
  wstring token_in_progress;
   Token_t next_tok_type;
   Token_t prev_tok_type;
   /* Consume leading white-space */
   while (!isDone() && (next_tok_type = lookAheadString(current_position_, target_)) == SPACE){ current_position_++; }
   if (next_tok_type != LABEL ){  return TokenPackage(next_tok_type, L"" + target_.at(current_position_++)); }

   if (!isDone() && lookAheadString( current_position_, target_ ) == QUOTED_STRING_MARKER){
     /* Extract token contents */
     while ((!isDone() && (next_tok_type = lookAheadString(current_position_, target_)) == LABEL) || next_tok_type == SPACE){
     	  prev_tok_type = next_tok_type;
          token_in_progress += target_.at(current_position_);
	  current_position_++;
     }
     assert( QUOTED_STRING_MARKER == next_tok_type );
     current_position_++;//skip over the end mark

   }
   else {

     /* Extract token contents */
     while (!isDone() && (next_tok_type = lookAheadString(current_position_, target_)) == LABEL){
     	  prev_tok_type = next_tok_type;
          token_in_progress += target_.at(current_position_);
	  current_position_++;
     }
   }
//   cerr << L"Extracted token type: L" << prev_tok_type << L" as wchar_tacter: \'" << 
  //        (isprint( prev_tok_type) ? (wchar_t)prev_tok_type : '?') << 
//	  L"\' contents: \""  << token_in_progress << L"\"\n";
   
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

Token_t lookAheadString( int pos, const wstring& target ){
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
	for (wchar_t letter = 'a'; letter <= 'z'; ++letter){
	   lookupTable[ letter ] = LABEL;
	   lookupTable[ toupper(letter) ] = LABEL;
	}
	for (wchar_t number = '0'; number <= '9'; ++number){
	   lookupTable[ number ] = LABEL;
	}
        lookupTable[ '.' ] = LABEL;
        lookupTable[ '_' ] = LABEL;
        lookupTable[ '-' ] = LABEL;
        lookupTable[ '\'' ] = QUOTED_STRING_MARKER;
        lookupTable[ '\"' ] = QUOTED_STRING_MARKER;
        lookupTable[ '{' ]  = LABEL;
        lookupTable[ '}' ]  = LABEL;
        lookupTable[ 0x9a ] = LABEL;
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
