#ifndef TREE_DESCRIPTION_SCANNER_HPP_
#define TREE_DESCRIPTION_SCANNER_HPP_

#include <string>
/*
 * Brandon Chisham
 * Created: 3-26-08
 * Description: Tree description lexical analyzer.
 * Revised: 3-27-08: added white space token types.
 *                   added public look ahead capability.
 */
namespace CDAO {
  typedef enum {
    UNDEFINED  = 0,
    START_TREE = '(',
    END_TREE   = ')',
    DELIMITER = ',',
    SPACE           = ' ',
    TAB             = '\t',
    START_WEIGHT    = ':',
    START_COMMENT   = '[',
    END_COMMENT     = ']',
    QUOTED_STRING_MARKER   = '\'',
    LABEL = 510,
    END   = 511,
    TOKENS_SIZE = 512
  } Token_t;
  /*
   * A token package combines a token's type and value into a single object. 
   * Like YYVAL  and YYSTYPE in lex/bison.
   */
  class TokenPackage {
  public: 
    TokenPackage(Token_t type, std::wstring contents):type_(type), contents_(contents){}
    Token_t getType(){return type_; }
    std::wstring getContents()const{ return contents_;}
  private:
    Token_t type_;
    std::wstring contents_;
    
  };
  /*
   * Tree description scanner provides an interface to lexically analyze a given Newick format wstring.
   */
  class TreeDescriptionScanner {
  public:
    TreeDescriptionScanner(std::wstring target):current_position_(0),target_(target){}
    bool isDone()const{ return current_position_ >= target_.size();}
    TokenPackage lex();
    TokenPackage lookAhead();
  private:
    unsigned int current_position_;
    std::wstring target_;
  }; 
}

#endif
