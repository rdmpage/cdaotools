%{
#include "treebase-node.hpp"
#include <iostream>
#include <wchar.h>
#include <string>
extern int yylex();
void yyerror( const wchar_t* );
void yyerror( const char* );

using namespace std;
using namespace Treebase;

%}
%error-verbose
%glr-parser
%union {
    char chtok;
    unsigned long int reference;
    std::wstring* string_literal;
    std::wstring* keyword;
    Treebase::Node* node;
    Treebase::QuotedStringNode* quoted_string;
    Treebase::ConstNode* constant;
    Treebase::StudyNode* study;
    Treebase::AuthorNode* author;
    Treebase::HistoryNode* history;
    Treebase::MatrixNode* matrix;
    Treebase::AnalysisNode* analysis;
    Treebase::InputMatrixNode* input_matrix;
    Treebase::OutputTreeNode* output_tree;
    Treebase::ListNode<Treebase::StudyNode>* study_list;
    Treebase::ListNode<Treebase::AuthorNode>* author_list;
    Treebase::ListNode<Treebase::HistoryNode>* history_list;
    Treebase::ListNode<Treebase::MatrixNode>* matrix_list;
    Treebase::ListNode<Treebase::InputMatrixNode>* input_matrix_list;
    Treebase::ListNode<Treebase::OutputTreeNode>* output_tree_list;
    Treebase::ListNode<Treebase::AnalysisNode>* analysis_list;
};
//Define tokens and types
%token <chtok> '=' '>' '[' ']';
%token <reference> CONSTANT;
%token <string_literal> QUOTED_STRING_TOK;
%token <keyword> STUDY_TOK STUDY_ID_TOK AUTHOR_TOK HISTORY_TOK MATRIX_TOK ANALYSIS_TOK INPUT_MATRIX_TOK OUTPUT_TREE_TOK AUTHOR_ID FIRST_NAME LAST_NAME EMAIL DATE TIME PERSON EVENT MATRIX_ID MATRIX_NAME DATA_TYPE NCHAR ANALYSIS_ID ANALYSIS_NAME SOFTWARE ALGORITHM TREE_ID TREE_LABEL TREE_TITLE TREE_TYPE CITATION_TOK ABSTRACT_TOK;
%type <study_list> treebase study_list;
%type <quoted_string> analysis_name author_id first_name last_name email date time person event matrix_id matrix_name data_type analysis_id algorithm software tree_id tree_title tree_label tree_type study_id abstract citation;
%type <constant> nchar;
%type <author> author;
%type <author_list> author_list;
%type <matrix> matrix;
%type <study> study;
//%type <study_list> study_list;
%type <history> history;
%type <history_list> history_list;
%type <matrix_list> matrix_list;
%type <input_matrix> input_matrix;
%type <output_tree> output_tree;
%type <input_matrix_list> input_matrix_list;
%type <output_tree_list> output_tree_list;
%type <analysis> analysis;
%type <analysis_list> analysis_list;
//%type <node> treebase study study_list study_id citation abstract author_list author history history_list matrix_list matrix tree_id tree_title tree_label tree_type output_tree output_tree_list analysis_list analysis_name analysis input_matrix author_id first_name last_name email date time person event matrix_id matrix_name data_type analysis_id algorithm nchar software input_matrix_list;

%{
using namespace std;
using namespace Treebase;
ListNode<StudyNode>* treebase_study_list = NULL;
%}

%%
//the tree base file is represented as a list of studies.
treebase: study_list { wcerr << "parsed file\n"; treebase_study_list = $1;}
        ;
//rule for creating a list of studies.
study_list: study { $$ = new ListNode<StudyNode>( $1 );  }
          | study study_list { $$ = new ListNode<StudyNode>($1, $2); }
          ;
//Rules for individual studies.
study: '>' STUDY_TOK '[' CONSTANT ']' study_id citation abstract author_list history_list matrix_list analysis_list 
         {  //wcerr << "Parsed study: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
            $$ = new StudyNode($6,$7,$8,$9,$10,$11,$12);
          }
      | '>' STUDY_TOK '[' CONSTANT ']' study_id citation abstract history_list matrix_list analysis_list 
         {  //wcerr << "parsed study: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
            $$ = new StudyNode($6,$7,$8,new ListNode<AuthorNode>(),$9,$10,$11); 
         }
      | '>' STUDY_TOK '[' CONSTANT ']' study_id citation abstract author_list matrix_list analysis_list
         { //wcerr << "parsed study: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
           $$ = new StudyNode($6,$7,$8,$9,new ListNode<HistoryNode>(),$10,$11);
         }
       | '>' STUDY_TOK '[' CONSTANT ']' study_id citation abstract author_list history_list analysis_list
          { //wcerr << "parsed study: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
            $$ = new StudyNode($6,$7,$8,$9,$10,new ListNode<MatrixNode>(),$11);
          }
       | '>' STUDY_TOK '[' CONSTANT ']' study_id citation abstract author_list analysis_list 
           {   //wcerr << "parsed study: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
               $$ = new StudyNode($6,$7,$8,$9,new ListNode<HistoryNode>(),new ListNode<MatrixNode>(),$10);
           }
        | '>' STUDY_TOK '[' CONSTANT ']' study_id citation abstract history_list analysis_list
           {  //wcerr << "Parsed Study: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
              $$ = new StudyNode($6,$7,$8,new ListNode<AuthorNode>(),$9,new ListNode<MatrixNode>(),$10);

           }
        | '>' STUDY_TOK '[' CONSTANT ']' study_id citation abstract  analysis_list
          {  //wcerr << "parsed study: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
             $$ = new StudyNode($6,$7,$8,new ListNode<AuthorNode>(),new ListNode<HistoryNode>(),new ListNode<MatrixNode>(),$9);
          }
       ;

study_id: STUDY_ID_TOK '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( wstring($3->c_str()) ); delete $3; }
       ;
citation: CITATION_TOK '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( wstring($3->c_str()) ); delete $3; }
       ;
abstract: ABSTRACT_TOK '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( wstring($3->c_str()) ); delete $3; }
       ;

author_list: author { $$ = new ListNode<AuthorNode>( $1 );}
           | author author_list { $$ = new ListNode<AuthorNode>($1, $2);}
           ;

author: '>' AUTHOR_TOK '[' CONSTANT ']' author_id first_name last_name email 
                     {  //wcerr << "parsed author: " << *$6 <<"\n";
                        $$ = new AuthorNode($6,$7,$8,$9);
                     }
      ;
author_id: AUTHOR_ID '=' QUOTED_STRING_TOK { QuotedStringNode* a = new QuotedStringNode( *$3 ); /*cerr << "parsed author id: " << a->getcontents() << "\n"; */
                                             $$ = a; delete $3;  }
      ;
first_name: FIRST_NAME '=' QUOTED_STRING_TOK { //wcerr << "first_name: " << $3->c_str() << "\n";
                                               $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
last_name: LAST_NAME '=' QUOTED_STRING_TOK { //wcerr << "last_name: " << $3->c_str() << "\n";
                                              $$ = new QuotedStringNode( $3->c_str() ); delete $3;  }
      ;
email: EMAIL '=' QUOTED_STRING_TOK { //wcerr << "email: " << $3->c_str() << "\n";
                                     QuotedStringNode* em = new QuotedStringNode( $3->c_str() );
                                     //wcerr << "email: " << em << "\n";
                                     $$ = em; delete $3; /*cerr << em;*/ }
      ;
history_list: history { $$ = new ListNode<HistoryNode>( dynamic_cast<HistoryNode*>( $1 ) ); }
      | history history_list { $$ = new ListNode<HistoryNode>( dynamic_cast<HistoryNode*>($1), dynamic_cast<ListNode<HistoryNode>*>($2) ); }
      ;
history: '>' HISTORY_TOK '[' CONSTANT ']' date time person event 
                                   { //cerr << "parsed history: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
                                     $$ = new HistoryNode($6,$7,$8, $9);
                                   }
      ;
date: DATE '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
time: TIME '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
person: PERSON '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
event: EVENT '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;

matrix_list: matrix { $$ = new ListNode<MatrixNode>( $1 ); }
           | matrix matrix_list { $$ = new ListNode<MatrixNode>( $1, $2 ); }
           ;

matrix: '>' MATRIX_TOK '[' CONSTANT ']' matrix_id matrix_name data_type nchar 
                                      { $$ = new MatrixNode( $6, $7, $8, $9 ); 
                                      }
      ;
matrix_id: MATRIX_ID '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
matrix_name: MATRIX_NAME '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
data_type: DATA_TYPE '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
nchar: NCHAR '=' QUOTED_STRING_TOK { $$ = new ConstNode( wcstol($3->c_str(), NULL, 10) ); delete $3; }
      ;

analysis_list: analysis { $$ = new ListNode<AnalysisNode>( dynamic_cast<AnalysisNode*>($1) );}
             | analysis analysis_list { $$ = new ListNode<AnalysisNode>(dynamic_cast<AnalysisNode*>($1), dynamic_cast<ListNode<AnalysisNode>*>($2));}
             ;
analysis: '>' ANALYSIS_TOK '[' CONSTANT ']' analysis_id analysis_name software algorithm input_matrix_list output_tree_list 
          { //cerr << "Parsed analysis: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
            $$ = new  AnalysisNode( $6, $7, $8, $9, $10, $11 );
         }
      | '>' ANALYSIS_TOK '[' CONSTANT ']' analysis_id analysis_name software algorithm output_tree_list
       {   //cerr << "parsed analysis: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
           $$ = new  AnalysisNode(  $6, $7, $8, $9, new ListNode<InputMatrixNode>(), $10);
       }
      | '>' ANALYSIS_TOK '[' CONSTANT ']' analysis_id analysis_name software algorithm input_matrix_list
       {  //cerr << "parsed analysis: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
          $$ = new  AnalysisNode( $6, $7, $8, $9, $10, new ListNode<OutputTreeNode>() );
       }
      | '>' ANALYSIS_TOK '[' CONSTANT ']' analysis_id analysis_name software algorithm 
       {   //cerr << "parsed analysis: " << dynamic_cast<QuotedStringNode*>($6) << "\n";
           $$ = new  AnalysisNode( $6, $7, $8, $9, new ListNode<InputMatrixNode>(), new ListNode<OutputTreeNode>() );
       }
      ;
analysis_id: ANALYSIS_ID '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3;  }
      ;
analysis_name: ANALYSIS_NAME '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
software: SOFTWARE '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
algorithm: ALGORITHM '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;

input_matrix_list: input_matrix { $$ = new ListNode<InputMatrixNode>( $1 );  }
      | input_matrix input_matrix_list { $$ = new ListNode<InputMatrixNode>($1, $2);}
      ;
input_matrix: '>' INPUT_MATRIX_TOK '[' CONSTANT ']' matrix_id { $$ = new InputMatrixNode( $6 ); }
      ;
output_tree_list: output_tree { $$ = new ListNode<OutputTreeNode>( $1 ); }
      | output_tree output_tree_list { $$ = new ListNode<OutputTreeNode>( $1, $2); }
      ; 
output_tree: '>' OUTPUT_TREE_TOK '[' CONSTANT ']' tree_id tree_label tree_title tree_type 
                   { $$ = new OutputTreeNode( $6, $7, $8, $9 );
                   }
      ;
tree_id: TREE_ID '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3; }
      ;
tree_type: TREE_TYPE '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3;}
      ;
tree_label: TREE_LABEL '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3;}
      ;
tree_title: TREE_TITLE '=' QUOTED_STRING_TOK { $$ = new QuotedStringNode( $3->c_str() ); delete $3;}
      ;
%%
void yyerror(const wchar_t* err){ std::wcerr << err; }
void yyerror( const char* err ){ std::cerr << err;  }
