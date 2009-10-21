#include "treebase-node.hpp"
#include <cassert>
#include <iostream>

using namespace Treebase;
using namespace std;
//initialize the node type.
Node::Node( NodeType_t t):node_type_(t){}
//Nothing to do here
Node::~Node(){}
//Return the node's type
NodeType_t Node::getnodetype()const{ return node_type_; }
//Initialize the string node
QuotedStringNode::QuotedStringNode( string c ):Node(QUOTED_STRING),contents_(c){  }
//Make a deep copy of the node
QuotedStringNode::QuotedStringNode( const QuotedStringNode& rhs ):Node(QUOTED_STRING), contents_(rhs.contents_){ 
   // cerr << "copy constructor\n";
}
//Nothing to do here
QuotedStringNode::~QuotedStringNode(){ /*cerr << "~QuotedStringNode() contents_:" << contents_ << "\n";*/}
//Make a deep copy of the node
QuotedStringNode* QuotedStringNode::copy()const{ /*cerr << "QuotedStringNode::copy()\n"; return new QuotedStringNode(this->getcontents());*/ }
//Return the contents of the string
string QuotedStringNode::getcontents()const{ return contents_; }
//initialize the constant with the specified value
ConstNode::ConstNode( unsigned long int v ):Node(CONST),value_(v){ /*cerr << "ConstNode(v:"<< value_ <<")\n";*/}
//Makde a deep copy of the node
ConstNode::ConstNode( const ConstNode& rhs ):Node(CONST), value_(rhs.value_){}
//Nothing to do here
ConstNode::~ConstNode(){ /*cerr << "~ConstNode()\n";*/}
//Make a deep copy of the node
ConstNode* ConstNode::copy()const{ /*cerr << "ConstNode::copy()\n"; return new ConstNode( this->getvalue() );*/ }
//Extract the node's value.
unsigned long int ConstNode::getvalue()const{ return value_; }
//Initialize the output tree.
OutputTreeNode::OutputTreeNode( QuotedStringNode* id, QuotedStringNode* l, QuotedStringNode* ti,QuotedStringNode* ty  ):Node(OUTPUT_TREE),id_(id), label_(l), title_(ti), type_(ty){}
//Initialize a deep copy of the output tree
OutputTreeNode::OutputTreeNode(const OutputTreeNode& rhs):Node(OUTPUT_TREE),
                                                          id_(rhs.id_->copy()),
                                                          label_(rhs.label_->copy()),
                                                          title_(rhs.title_->copy()),
                                                          type_(rhs.type_->copy()){}
//Clean-up this node's attributes.
OutputTreeNode::~OutputTreeNode(){ delete id_; delete label_; delete title_; delete type_; }
//Make a deep copy of the node
OutputTreeNode* OutputTreeNode::copy()const{ assert(id_ && label_ && title_ && type_); return new OutputTreeNode(id_->copy(), label_->copy(), title_->copy(), type_->copy()); }
//Return the tree id
const QuotedStringNode* OutputTreeNode::gettreeid()const{ return id_; }
//Return the tree label
const QuotedStringNode* OutputTreeNode::gettreelabel()const{ return label_; }
//Return the tree title
const QuotedStringNode* OutputTreeNode::gettreetitle()const{ return title_; }
//Return the tree type
const QuotedStringNode* OutputTreeNode::gettreetype()const{ return type_;  }
//Initialize the input matrix node
InputMatrixNode::InputMatrixNode( QuotedStringNode* mid ):Node(INPUT_MATRIX),id_(mid){}
//Make a deep copy of the input matrix node
InputMatrixNode::InputMatrixNode( const InputMatrixNode& rhs ):Node(INPUT_MATRIX), id_( rhs.id_->copy() ){}
//Clean-up the node's attributes.
InputMatrixNode::~InputMatrixNode(){ delete id_; }
//Make a deep copy of the input matrix node
InputMatrixNode* InputMatrixNode::copy()const{ assert( id_ ); return new InputMatrixNode(id_->copy()); }
//Get the input matrix id.
const QuotedStringNode* InputMatrixNode::getmatrixid()const{ return id_; }
//Make an analysis node
AnalysisNode::AnalysisNode( QuotedStringNode* id, QuotedStringNode* name, QuotedStringNode* soft, QuotedStringNode* algo, ListNode<InputMatrixNode>* it, ListNode<OutputTreeNode>* ot ):Node(ANALYSIS),id_(id), name_(name), software_(soft), algorithm_(algo), it_(it), ot_(ot){}
//Make a deep copy of the analysis node
AnalysisNode::AnalysisNode(const AnalysisNode& rhs):Node(ANALYSIS), id_(rhs.id_->copy()), name_(rhs.name_->copy()), software_(rhs.software_), algorithm_(rhs.algorithm_->copy()), it_(rhs.it_->copy()), ot_(rhs.ot_->copy()){ }
//Clean-up analysis node resources.
AnalysisNode::~AnalysisNode(){ delete id_; delete name_; delete software_; delete algorithm_; delete it_; delete ot_; }
//Make a deep copy of the analysis node
AnalysisNode* AnalysisNode::copy()const{
      //Node* ret = NULL;
       return new AnalysisNode( id_->copy(), name_->copy(), software_->copy(), algorithm_->copy(), it_->copy(), ot_->copy() );
      //if ( it_ ) ret = new AnalysisNode( id_->copy(), name_->copy(), software_->copy(), algorithm_->copy(), it_->copy(), NULL );
      //if ( ot_ ) ret = new AnalysisNode( id_->copy(), name_->copy(), software_->copy(), algorithm_->copy(), NULL, ot_->copy() );
     // else ret = new AnalysisNode( id_->copy(), name_->copy(), software_->copy(), algorithm_->copy(), NULL, NULL);
}
//Get the analysis id
const QuotedStringNode* AnalysisNode::getanalysisid()const{ return id_; }
//Get the analysis name
const QuotedStringNode* AnalysisNode::getanalysisname()const{ return name_; }
//Get the analysis software
const QuotedStringNode* AnalysisNode::getsoftware()const{ return software_; }
//Get the analysis algorithm
const QuotedStringNode* AnalysisNode::getalgorithm()const{ return algorithm_; }
//Get the input matrix list
const ListNode<InputMatrixNode>* AnalysisNode::getinputmatrix()const{ return it_; }
//Get the output tree list
const ListNode<OutputTreeNode>* AnalysisNode::getoutputtree()const{ return ot_; }

//Initialize the matrix node
MatrixNode::MatrixNode(QuotedStringNode* id, QuotedStringNode* n, QuotedStringNode* dt, ConstNode* nchar):Node(MATRIX),id_(id), name_(n), data_type_(dt), nchar_(nchar){}
//Make a deep copy of a matrix node.
MatrixNode::MatrixNode( const MatrixNode& rhs ):Node(MATRIX),id_(rhs.id_->copy()), name_(rhs.name_->copy()), data_type_(rhs.data_type_->copy()), nchar_(rhs.nchar_->copy()){}
//Clean-up the matrix node's attributes
MatrixNode::~MatrixNode(){ delete id_; delete name_; delete data_type_; delete nchar_; }
//Make a deep copy of the matrix node
MatrixNode* MatrixNode::copy()const{ return new MatrixNode(id_->copy(), name_->copy(), data_type_->copy(), nchar_->copy()); }
//Get the matrix id.
const QuotedStringNode* MatrixNode::getmatrixid()const{ return id_;  }
//Get the matrix name.
const QuotedStringNode* MatrixNode::getmatrixname()const{ return name_; }
//Get the matrix data type
const QuotedStringNode* MatrixNode::getdatatype()const{ return data_type_; }
//Get the number of characters in the matrix.
const ConstNode* MatrixNode::getnchar()const{ return nchar_; }

//Initialize the history node
HistoryNode::HistoryNode( QuotedStringNode* date, QuotedStringNode* time, QuotedStringNode* person, QuotedStringNode* event):Node(HISTORY),date_(date), time_(time), person_(person), event_(event){
  // cerr << "HistoryNode( date:" <<(void*)date << ",time: " <<(void*) time << ",person: " <<(void*) person << ",event:"<<(void*)event << ")\n";
}
//Make a deep copy of the history node
HistoryNode::HistoryNode( const HistoryNode& rhs ):Node(HISTORY),date_(rhs.date_->copy()), time_(rhs.time_->copy()), person_(rhs.person_->copy()), event_(rhs.event_->copy()){ }
//Clean-up history node resources
HistoryNode::~HistoryNode(){ delete date_; delete time_; delete person_; delete event_; }
//Make a deep copy of the history node
HistoryNode* HistoryNode::copy()const{ return new HistoryNode( date_->copy(), time_->copy(), person_->copy(), event_->copy() ); }
//Get the event date
const QuotedStringNode* HistoryNode::getdate()const{ return date_; }
//Get the event time
const QuotedStringNode* HistoryNode::gettime()const{ return time_; }
//Get the event source
const QuotedStringNode* HistoryNode::getperson()const{ return person_; }
//Get the event description
const QuotedStringNode* HistoryNode::getevent()const{ return event_; }

//Initialize an author node
AuthorNode::AuthorNode( QuotedStringNode* i, QuotedStringNode* f, QuotedStringNode* l, QuotedStringNode* e):Node(AUTHOR),id_(i), first_name_(f), last_name_(l), email_(e){
  //cerr << "AuthorNode(id:" <<(void*)i<<",first:"<<(void*)f<<",last:"<<(void*)l<<",email:"<<(void*)e<<")\n";
}
//Make a deep copy of an author node
AuthorNode::AuthorNode(const AuthorNode& rhs):Node(AUTHOR),id_(rhs.id_->copy()), first_name_(rhs.first_name_->copy()), last_name_(rhs.last_name_->copy()), email_(rhs.email_->copy()){}
//Cleanup author node resources
AuthorNode::~AuthorNode(){ delete id_; delete first_name_; delete last_name_; delete email_; }
//Make a deep copy of an author node
AuthorNode* AuthorNode::copy()const{ return new AuthorNode( id_->copy(), first_name_->copy(), last_name_->copy(), email_->copy() ); }
//Get author id
const QuotedStringNode* AuthorNode::getauthorid()const{ return id_; }
//Get author first name
const QuotedStringNode* AuthorNode::getfirstname()const{ return first_name_; }
//Get author last name
const QuotedStringNode* AuthorNode::getlastname()const{ return last_name_; }
//Get author email
const QuotedStringNode* AuthorNode::getemail()const{ return email_; }

//Initialize a study
StudyNode::StudyNode( QuotedStringNode* id, QuotedStringNode* cit, QuotedStringNode* abs, ListNode<AuthorNode>* a, ListNode<HistoryNode>* history, ListNode<MatrixNode>* m, ListNode<AnalysisNode>* anal):Node(STUDY),id_(id), citation_(cit), abstract_(abs), author_(a), history_(history), matrix_(m), analysis_(anal){}
//Create a deep copy of a study
StudyNode::StudyNode(const StudyNode& rhs):Node(STUDY), id_(rhs.id_->copy()), citation_(rhs.citation_->copy()), abstract_(rhs.abstract_->copy()), author_(rhs.author_->copy()), history_(rhs.history_->copy()), analysis_(rhs.analysis_->copy())
{}
//Clean-up resources used by the study
StudyNode::~StudyNode(){ delete id_; delete citation_; delete abstract_; delete author_; delete history_; delete matrix_; delete analysis_; }
//Make a deep copy of the study
StudyNode* StudyNode::copy()const{
            return new StudyNode(id_->copy(),
                                 citation_->copy(),
                                 abstract_->copy(),
                                 author_->copy(),
                                 history_->copy(),
                                 matrix_->copy(),
                                 analysis_->copy());
   }
//Get the study id
const QuotedStringNode* StudyNode::getstudyid()const{ return id_; }
//Get the study citation.
const QuotedStringNode* StudyNode::getcitation()const{ return citation_; }
//Get the study abstract
const QuotedStringNode* StudyNode::getabstract()const{ return abstract_; }
//Get the study's list of authors
const ListNode<AuthorNode>* StudyNode::getauthor()const{ return author_; }
//Get the study's history list
const ListNode<HistoryNode>* StudyNode::gethistory()const{ return history_; }
//Get the matrices associated with the study
const ListNode<MatrixNode>* StudyNode::getmatrix()const{ return matrix_; }
//Get the list of analyses associated with the study.
const ListNode<AnalysisNode>* StudyNode::getanalysis()const{ return analysis_; }
