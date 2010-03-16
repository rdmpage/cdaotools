%{

#include <node.hpp>
#include <iostream>
#include <cstdio>
#include <string>
#include <util.hpp>
extern "C" {
 int yylex();
 int yywrap();
}
void yyerror( const char* );
extern CDAO::Node* pTree;
%}
%error-verbose
%glr-parser
%union {
    char tree_marker;
    char delimiter;
    char weight_marker;
    char space;
    char comment_marker;
    double weight;
    char quote;
    std::wstring* label;
    std::wstring* extra_meta;
    CDAO::Node* node;
}
%token <tree_marker> '(' ')';
%token <delimiter> ',';
%token <space> ' ';
%token <quote> '\'' '\"';
%token <weight_marker> ':';
%token <comment_marker> '[' ']';
%token <weight> CONST;
%token <label> IDENTIFIER QUOTED_STRING

%type <node> parse_tree tree tree_list label 


%%

parse_tree: tree { pTree = $1; }
    ; 
tree: '(' tree_list ')' label { $$ = $2; $$->setLabel( $4->getLabel() ); delete $4; }
    | '(' tree_list ')' { $$ = $2; }
    | '(' ',' ')' { $$ = new CDAO::Node(CDAO::labelMaker(L"node")); }
    ;

tree_list: tree { fprintf(stderr, "tree\n"); $$ = $1; }
         | label { fprintf(stderr, "label\n"); $$ = $1; }
         | tree ',' tree_list { /*fprintf(stderr, "tree, tree_list\n");*/ $$ = new CDAO::Node(CDAO::labelMaker(L"node")); $$->addDescendant($1); $$->addDescendant($3);}
         | label ',' tree_list { fprintf(stderr, "label, treelist"); $$ = new CDAO::Node(CDAO::labelMaker(L"node")); $$->addDescendant($1); $$->addDescendant($3);}
         ;

label: IDENTIFIER { fprintf(stderr, "Identifer\n"); $$ = new CDAO::Node( *($1) ); /*fprintf(stderr, "allocated node\n");*/ }
     | QUOTED_STRING { fprintf(stderr, "QuotedString\n"); $$ = new CDAO::Node( *($1) ); }
     ;

%%

void yyerror( const char* msg){
    std::cerr << "Parse error: " << msg << std::endl; 
}

