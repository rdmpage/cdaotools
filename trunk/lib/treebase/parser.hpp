/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CONSTANT = 258,
     QUOTED_STRING_TOK = 259,
     STUDY_TOK = 260,
     STUDY_ID_TOK = 261,
     AUTHOR_TOK = 262,
     HISTORY_TOK = 263,
     MATRIX_TOK = 264,
     ANALYSIS_TOK = 265,
     INPUT_MATRIX_TOK = 266,
     OUTPUT_TREE_TOK = 267,
     AUTHOR_ID = 268,
     FIRST_NAME = 269,
     LAST_NAME = 270,
     EMAIL = 271,
     DATE = 272,
     TIME = 273,
     PERSON = 274,
     EVENT = 275,
     MATRIX_ID = 276,
     MATRIX_NAME = 277,
     DATA_TYPE = 278,
     NCHAR = 279,
     ANALYSIS_ID = 280,
     ANALYSIS_NAME = 281,
     SOFTWARE = 282,
     ALGORITHM = 283,
     TREE_ID = 284,
     TREE_LABEL = 285,
     TREE_TITLE = 286,
     TREE_TYPE = 287,
     CITATION_TOK = 288,
     ABSTRACT_TOK = 289
   };
#endif


/* Copy the first part of user declarations.  */
#line 1 "parser.yy"

#include "treebase-node.hpp"
#include <iostream>
#include <string>
extern int yylex();
void yyerror( const char* );



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE 
#line 11 "parser.yy"
{
    char chtok;
    unsigned long int reference;
    std::string* string_literal;
    std::string* keyword;
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
}
/* Line 2616 of glr.c.  */
#line 116 "parser.hpp"
	YYSTYPE;
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{

  char yydummy;

} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;



