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
  
 // wcerr << L"lex(): current_position_: " << current_position_ << L" target_.size(): " << target_.size() << L"\n";


  if (isDone()){ return TokenPackage(END, L""); }
  wstring token_in_progress;
   Token_t next_tok_type;
   Token_t prev_tok_type;
   /* Consume leading white-space */
   while (!isDone() && (next_tok_type = lookAheadString(current_position_, target_)) == SPACE){ current_position_++;  }
   //wcerr << L"extracted leading whitespace\n";

   if ( next_tok_type == START_TREE ){ 
    // wcerr << "start subtree token\n";
     current_position_++; return TokenPackage( next_tok_type, wstring( token_in_progress + (wchar_t)START_TREE ) ); 
   }
   if ( next_tok_type == END_TREE){ 
     //wcerr << "end subtree token\n";
     current_position_++; return TokenPackage( next_tok_type, wstring( token_in_progress + (wchar_t)END_TREE) ); 
   }
   if ( next_tok_type == DELIMITER){
     //wcerr << "delimiter\n";
     current_position_++; return TokenPackage( next_tok_type, wstring( token_in_progress + (wchar_t)DELIMITER ) );
   }  
   
   /* Extract token contents */

   
   if (!isDone() && (next_tok_type = lookAheadString( current_position_, target_)) == QUOTED_STRING_MARKER){
      //wcerr << "quoted_string_mark\n";
      current_position_++;
      return TokenPackage( next_tok_type, token_in_progress + (wchar_t)QUOTED_STRING_MARKER );   
   }

   //wcerr << L"processing a label\n";
   //wcerr << "current character is: " << target_.at( current_position_ ) << "current postion: " << current_position_ << endl;
   while (!isDone() && (next_tok_type = lookAheadString(current_position_, target_)) == LABEL){
       	prev_tok_type = next_tok_type;
        token_in_progress += target_.at(current_position_);
	current_position_++;

        //wcerr << L"token in progress: \"" << token_in_progress << L"\" current_position: " << current_position_ << L"\n";
   }
   //   cerr << L"Extracted token type: L" << prev_tok_type << L" as wchar_tacter: \'" << 
  //        (isprint( prev_tok_type) ? (wchar_t)prev_tok_type : '?') << 
//	  L"\' contents: \""  << token_in_progress << L"\"\n";
   
   /* Contruct and hand off the token. */
   //wcerr << L"scanner::lex() returing token: \"" << token_in_progress << L"\"\n";
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
        for (wchar_t number =0xc380; number <= 0xc3bf; ++number){
           lookupTable[ number ]  = LABEL;
        }
        lookupTable[ '.' ] = LABEL;
        lookupTable[ '_' ] = LABEL;
        lookupTable[ '-' ] = LABEL;
        lookupTable[ '\'' ] = QUOTED_STRING_MARKER;
        lookupTable[ '\"' ] = QUOTED_STRING_MARKER;
        lookupTable[ '{' ]  = LABEL;
        lookupTable[ '}' ]  = LABEL;
       // lookupTable[ 0x9a ] = LABEL;
        //lookupTable[ ':' ] = LABEL;
        //lookupTable[ '[' ] = LABEL;
        //lookupTable[ ']' ] = LABEL;
      }
      /*
       * Return token type to the caller.
       */

      //wcerr << "lookAhead returing: \'" << lookupTable[ target.at( pos ) ] << "\'" << endl;

      return lookupTable[ target.at( pos ) ];
      
}
}
