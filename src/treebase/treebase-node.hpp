#ifndef TREEBASE_NODE_HPP_
#define TREEBASE_NODE_HPP_
#include <string>
#include <fstream>

namespace Treebase {
  /** All node types. */
  typedef enum {
      STUDY,
      HISTORY,
      MATRIX,
      ANALYSIS,
      AUTHOR,
      INPUT_MATRIX,
      OUTPUT_TREE,
      QUOTED_STRING,
      CONST,
      LIST
  }NodeType_t;
  /**
   * Base for all node types.
   */
  class Node {
     public:
       Node( NodeType_t t );
       virtual ~Node();
       //virtual Node* copy()const=0;
       /** Get the node's type */
       NodeType_t getnodetype()const;
     private:
       NodeType_t node_type_;
  };
  /**
   * List type for a specific kind of node.
   */ 
   template <typename T>  
   class ListNode : public Node {
    public:
     /** Create a new list element */
     ListNode(T* current=NULL, ListNode<T>* next=NULL):Node(LIST),current_(current), next_(next){}
     /** Copy an existing list */
     ListNode( const ListNode<T>& rhs ):Node(LIST){  
       current_ = rhs.current_? rhs.current_->copy(): NULL;
       next_ = rhs.next_ ? rhs.next_->copy() : NULL;
     }
     /** Free a list */
     virtual ~ListNode(){ delete current_; delete next_; }
     /** Copy a list */
     ListNode<T>* copy()const{
        ListNode<T>* ret = NULL; 
        if (current_ && next_) ret = new ListNode<T>( current_->copy(), next_->copy() );
        else if (current_) ret = new ListNode<T>( current_->copy() );
        return ret;
     }
     /** Get the contents of the current node */
     const T* getcurrent()const{ return current_; }
     /** Get the rest of the list */
     const ListNode<T>* getnext()const{ return next_; }
     /** see if there is a next node in the list */
     const bool hasnext()const{ return next_; }
     /** see if the list is empty */
     bool isempty()const{ return NULL == current_; }
    private:
      T* current_;
      ListNode<T>* next_;

  };
  /**
   * Node for string constants.
   */
  class QuotedStringNode : public Node {
     public:
       /**
        * Specifies the contents of the node.
        */
       QuotedStringNode( std::wstring contents);
       /**
        * Copies the specified node.
        */
       QuotedStringNode( const QuotedStringNode& );
       /**
        * Does nothing
        */
       virtual ~QuotedStringNode();
       /**
        * Create a copy of this node.
        */
       QuotedStringNode* copy()const;
       /**
        * Get the contents of the node.
        */
       std::wstring getcontents()const;
       /**
        * Output the specified node to the specified stream.
        */
       friend std::wostream& operator<<(std::wostream& out, const QuotedStringNode rhs){ 
         out << rhs.getcontents();
         return out;
       }
      // friend std::ostream& operator<<(std::ostream& out, const QuotedStringNode* rhs){
       //   if (NULL != rhs){ out << rhs->getcontents(); }
      //    return out;
       //}
     private:
        std::wstring contents_;
  };
  /**
   * Type for integer constants
   */
  class ConstNode : public Node {
      public:
        /**
         * Create a new integer constant node with the specified value.
         */
        ConstNode( unsigned long int value);
        /**
         * Copy 
         */
        ConstNode( const ConstNode& rhs );
        /**
         * Does nothing
         */
        virtual ~ConstNode();
        /**
         * Copy
         */
        ConstNode* copy()const;
        /**
         * Get the value of the node
         */
        unsigned long int getvalue()const;
        /**
         * Output the value of the specified node to the specified stream
         */
        friend std::wostream& operator<<(std::wostream& out, const ConstNode* rhs ){ if (rhs){ out << rhs->getvalue(); } return out; }
      private:
        unsigned long int value_;
  };
  /**
   * Represents the output tree section in the dump file
   */
  class OutputTreeNode : public Node {
    public:
      /**
       *  Create the specified output tree
       */
      OutputTreeNode( QuotedStringNode* id, QuotedStringNode* label, QuotedStringNode* title, QuotedStringNode* type);
      /**
       * Copy an output tree
       */
      OutputTreeNode( const OutputTreeNode& );
      /**
       * Clean-up
       */
      virtual ~OutputTreeNode();
      /**
       * Copy an output tree.
       */
      OutputTreeNode* copy()const;
      /**
       * Get the tree id
       */
      const QuotedStringNode* gettreeid()const;
      /**
       * Get the tree label
       */
      const QuotedStringNode* gettreelabel()const;
      //const QuotedStringNode* gettreename()const;
      const QuotedStringNode* gettreetitle()const;
      /**
       * Get the tree type.
       */
      const QuotedStringNode* gettreetype()const;
    private:
      QuotedStringNode* id_;
      QuotedStringNode* label_;
     // QuotedStringNode* name_;
      QuotedStringNode* title_;
      QuotedStringNode* type_;
  };
  /**
   * Represents an input matrix.
   */
  class InputMatrixNode : public Node {
    public:
      /** Creates a matrix with the specified id.  */
      InputMatrixNode(QuotedStringNode* matrix_id);
      InputMatrixNode( const InputMatrixNode& );
      virtual ~InputMatrixNode();
      InputMatrixNode* copy()const;
      /** Get the matrix id. */
      const QuotedStringNode* getmatrixid()const;
    private:
      QuotedStringNode* id_;
  };
  /**
   * Represents and analysis block
   */
  class AnalysisNode : public Node {
    public:
      /**
       * Create a new analysis node. Arguments NOT NULL.
       */
      AnalysisNode(QuotedStringNode* id, QuotedStringNode* name,
          QuotedStringNode* software, QuotedStringNode* algo, 
          ListNode<InputMatrixNode>* it, ListNode<OutputTreeNode>* ot_);
      /**
       * Copy an analysis node
       */
      AnalysisNode( const AnalysisNode& );
      virtual ~AnalysisNode();
      AnalysisNode* copy()const;
      /**
       * Get the analysis id.
       */
      const QuotedStringNode* getanalysisid()const;
      /**
       * Get the analysis name
       */
      const QuotedStringNode* getanalysisname()const;
      /**
       * Get the analysis software.
       */
      const QuotedStringNode* getsoftware()const;
      /**
       * Get the analysis algorithm
       */
      const QuotedStringNode* getalgorithm()const;
      /**
       * Get the input matrices
       */
      const ListNode<InputMatrixNode>* getinputmatrix()const;
      /**
       * Get the output node.
       */
      const ListNode<OutputTreeNode>* getoutputtree()const;
    private:
      QuotedStringNode* id_;
      QuotedStringNode* name_;
      QuotedStringNode* software_;
      QuotedStringNode* algorithm_;
      ListNode<InputMatrixNode>* it_;
      ListNode<OutputTreeNode>* ot_;
  };
  /**
   * Represents a matrix block
   */
  class MatrixNode : public Node {
    public:
      /**
       * Create a matrix node all arguments not NULL
       */
      MatrixNode( QuotedStringNode* id, QuotedStringNode* name, QuotedStringNode* data_type, ConstNode* nchar );
      MatrixNode( const MatrixNode& );
      virtual ~MatrixNode();
      MatrixNode* copy()const;
      /**
       * Get the matrix id.
       */
      const QuotedStringNode* getmatrixid()const;
      /**
       * Get the matrix name.
       */
      const QuotedStringNode* getmatrixname()const;
      /**
       * Get the matrix datatype.
       */
      const QuotedStringNode* getdatatype()const;
      /**
       * Get the number of characters in the matrix.
       */
      const ConstNode* getnchar()const;
    private:
        QuotedStringNode* id_;
        QuotedStringNode* name_;
        QuotedStringNode* data_type_;
        ConstNode* nchar_;
  };
  /**
   * Represents a histroy block
   */
  class HistoryNode : public Node {
    public:
      /**
       * Create a histrory node. All arguments not NULL.
       */
      HistoryNode(QuotedStringNode* date, QuotedStringNode* time, QuotedStringNode* person, QuotedStringNode* event);
      /**
       * Copy a histroy node
       */
      HistoryNode( const HistoryNode& );
      virtual ~HistoryNode();
      HistoryNode* copy()const;
      /**
       * Get the event date
       */
      const QuotedStringNode* getdate()const;
      /**
       * Get the event time
       */
      const QuotedStringNode* gettime()const;
      /**
       * Get the event person
       */
      const QuotedStringNode* getperson()const;
      /**
       * Get the event description.
       */
      const QuotedStringNode* getevent()const;
    private:
      QuotedStringNode* date_;
      QuotedStringNode* time_;
      QuotedStringNode* person_;
      QuotedStringNode* event_;
  };
  /**
   * Represents an author block
   */
  class AuthorNode : public Node {
    public:
      /**
       * Create an author node all arguments not NULL
       */
      AuthorNode(QuotedStringNode* id, QuotedStringNode* fn, QuotedStringNode* ln, QuotedStringNode* e);
      AuthorNode( const AuthorNode& );
      virtual ~AuthorNode();
      AuthorNode* copy()const;
      /**
       * Get the author id
       */
      const QuotedStringNode* getauthorid()const;
      /**
       * Get the author first name
       */
      const QuotedStringNode* getfirstname()const;
      /**
       * Get the author last name
       */
      const QuotedStringNode* getlastname()const;
      /**
       * Get the author email
       */
      const QuotedStringNode* getemail()const;
    private:
      QuotedStringNode* id_;
      QuotedStringNode* first_name_;
      QuotedStringNode* last_name_;
      QuotedStringNode* email_;
  };
  /**
   * Represents a study
   */
  class StudyNode : public Node {
    public:
      /**
       * Create a new study node all arguments not NULL
       */
      StudyNode(QuotedStringNode* id, 
          QuotedStringNode* citation,
          QuotedStringNode* abstract,
          ListNode<AuthorNode>* author,
          ListNode<HistoryNode>* history,
          ListNode<MatrixNode>* matrix,
          ListNode<AnalysisNode>* analysis);
      /**
       * Copy a study node
       */
      StudyNode( const StudyNode& );
      virtual ~StudyNode();
      StudyNode* copy()const;
      /**
       * Get the study id
       */
      const QuotedStringNode* getstudyid()const;
      /**
       * Get the study citation
       */
      const QuotedStringNode* getcitation()const;
      /**
       * Get the abstract
       */
      const QuotedStringNode* getabstract()const;
      /**
       * Get the study author
       */
      const ListNode<AuthorNode>* getauthor()const;
      /**
       * Get the study's history list
       */
      const ListNode<HistoryNode>* gethistory()const;
      /**
       * Get the matrices associated with the study.
       */
      const ListNode<MatrixNode>* getmatrix()const;
      /**
       * Get the analysis associated with the study.
       */
      const ListNode<AnalysisNode>* getanalysis()const;
    private:
      QuotedStringNode* id_;
      QuotedStringNode* citation_;
      QuotedStringNode* abstract_;
      ListNode<AuthorNode>* author_;
      ListNode<HistoryNode>* history_;
      ListNode<MatrixNode>* matrix_;
      ListNode<AnalysisNode>* analysis_;
  };

 
}

#endif
