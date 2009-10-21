#include "codegen.hpp"
#include <set>
using namespace std;
using namespace Treebase;
/*
 * Write header boiler-plate to the specified stream.
 */
static void header( ostream& out );
/*
 * Write a list of studies to the specified stream
 */
static void study_list( ostream& out, const ListNode<StudyNode>* study_list );
/*
 * Write a study to the specified stream
 */
static void study( ostream& out, const StudyNode* study );
/*
 * Write a list of authors to the specified stream
 */
static void author_list( ostream& out, const ListNode<AuthorNode>* authors );
/*
 * Write an author to the specified stream
 */
static void author( ostream& out, const AuthorNode* author );
/*
 * Write the history list to the specified stream.
 */
static void history_list( ostream& out, const ListNode<HistoryNode>* history );
/*
 * Write the history to the specified stream.
 */
static void history( ostream& out, const HistoryNode* history);
/*
 * Write the matrix list to the specified stream.
 */
static void matrix_list( ostream& out, const ListNode<MatrixNode>* matrix_list);
/*
 * Write the matrix to the specified stream.
 */
static void matrix( ostream& out, const MatrixNode* matrix );
/*
 * Write the analysis list to the specified stream.
 */
static void analysis_list( ostream& out, const ListNode<AnalysisNode>* list );
/*
 * Write the analysis to the specified stream.
 */
static void analysis( ostream& out, const AnalysisNode* analysis );
/*
 * Write the input matrix list to the specified stream.
 */
static void inputmatrix_list( ostream& out, const ListNode<InputMatrixNode>* list );
/*
 * Write the input matrix to the specified stream.
 */
static void inputmatrix( ostream& out, const InputMatrixNode* matrix );
/*
 * Write the output tree list to the specified stream.
 */
static void outputtree_list( ostream& out, const ListNode<OutputTreeNode>* tree_list);
/*
 * Write the output tree to the specified stream.
 */
static void outputtree( ostream& out, const OutputTreeNode* tree );
/*
 * Write the footer boiler-plate to the specified stream.
 */
static void footer( ostream& out );

static string& xmlescape( string& input );


/*
 * Public interface for code generation.
 * Delegate to the study_list for actual implementation.
 */
void Treebase::generate( ostream& out, ListNode<StudyNode>* treebase ){
     header( out );
     //cerr << "starting study list\n";
     study_list( out, treebase );
     //cerr << "finished study list\n";
     footer( out );
}


void header( ostream& out ){
  out << "<?xml version=\"1.0\"?>\n";
  out << "<rdf:RDF\n"
      << "\txmlns=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << "\txml:base=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << "\txmlns:owl2xml=\"http://www.w3.org/2006/12/owl2-xml#\"\n"
      << "\txmlns:study=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << "\txmlns:contact=\"http://www.w3.org/2000/10/swap/pim/contact#\"\n"
      << "\txmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"\n"
      << "\txmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"\n"
      << "\txmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
      << "\txmlns:owl=\"http://www.w3.org/2002/07/owl#\"\n"
      //<< "\txmlns:study=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << "\txmlns:foaf=\"http://www.mindswap.org/2003/owl/foaf#\""
      << ">\n";
  out << "<owl:Ontology rdf:about=\"\">\n"
      << "\t<owl:imports rdf:resource=\"http://www.cs.nmsu.edu/~bchisham/study.owl\"/>\n"
      << "</owl:Ontology>\n";

}

void study_list( ostream& out, const ListNode<StudyNode>* study_list ){
     while (study_list){
         study(out, study_list->getcurrent() );
         study_list = study_list->getnext();
     }
     return;                                                                                                                                                                                    
}

void study( ostream& out, const StudyNode* study ){
  if (study){
   out << "<study:Study rdf:ID=\"" << study->getstudyid()->getcontents() << "\">\n";
   out << "\t<study:has_citation>"<< study->getcitation()->getcontents() <<"</study:has_citation>\n";
   out << "\t<study:has_abstract>" << study->getabstract()->getcontents() <<"</study:has_abstract>\n";
   author_list( out, study->getauthor() );
   history_list(out, study->gethistory());
   matrix_list( out, study->getmatrix() );
   analysis_list( out, study->getanalysis());
   out << "</study:Study>\n";
  }

}

void author_list( ostream& out, const ListNode<AuthorNode>* authors ){
   while (authors){
      out << "<study:has_author>\n";
      author( out,authors->getcurrent());
      out << "</study:has_author>\n";
      authors = authors->getnext();
   }
}

void author( ostream& out, const AuthorNode* author ){
   static set< string > authors_used = set< string >();
   if (author){
       if ( authors_used.find( author->getauthorid()->getcontents() ) == authors_used.end() ){
         authors_used.insert( author->getauthorid()->getcontents() );
         out << "\t<study:Author rdf:ID=\"" << author->getauthorid()->getcontents() << "\">\n";
         out << "\t\t<foaf:first_name>" << author->getfirstname()->getcontents() << "</foaf:first_name>\n";
         out << "\t\t<foaf:last_name>" << author->getlastname()->getcontents() << "</foaf:last_name>\n";
         out << "\t\t<foaf:email>" << author->getemail()->getcontents() << "</foaf:email>\n";
         out << "\t</study:Author>\n";
       }
       else {
         out << "\t<study:Author rdf:about=\"" << author->getauthorid()->getcontents() << "\"/>\n";
       }
   }
   return;
}

void history_list( ostream& out, const ListNode<HistoryNode>* history_list ){
  while (history_list){
      out << "\t<study:has_history>\n";
      history( out, history_list->getcurrent() );
      out << "\t</study:has_history>\n";
      history_list = history_list->getnext();
  }
  return;
}
void history( ostream& out, const HistoryNode* history_node){
  if (history_node){
    out << "\t\t<study:History>\n";
    out << "\t\t\t<study:has_date>"<< history_node->getdate()->getcontents() <<"</study:has_date>\n";
    out << "\t\t\t<study:has_time>" << history_node->gettime()->getcontents() << "</study:has_time>\n";
    out << "\t\t\t<study:has_person>" << history_node->getperson()->getcontents() << "</study:has_person>\n";
    out << "\t\t\t<study:has_event>" << history_node->getevent()->getcontents() << "</study:has_event>\n";
    out << "\t\t</study:History>";
  }
  return;
}

void matrix_list( ostream& out, const ListNode<MatrixNode>* matrix_list){
   while ( matrix_list ){
       out << "<study:has_matrix>\n";
       matrix( out, matrix_list->getcurrent());
       out << "</study:has_matrix>\n";
       matrix_list = matrix_list->getnext() ;
   }
   return;
}


void matrix( ostream& out, const MatrixNode* matrix ){
  if (matrix){
    out << "\t\t<study:Matrix rdf:ID=\"" << matrix->getmatrixid()->getcontents() << "\">\n";
    out << "\t\t\t<study:has_matrixname>" <<  matrix->getmatrixname()->getcontents() << "</study:has_matrixname>\n";
    out << "\t\t\t<study:has_datatype>" <<  matrix->getdatatype()->getcontents() << "</study:has_datatype>\n";
    out << "\t\t\t<study:has_nchar>" << matrix->getnchar()->getvalue() << "</study:has_nchar>\n";
    out << "\t\t</study:Matrix>\n";
  }

}

void analysis_list( ostream& out, const ListNode<AnalysisNode>* list ){

  while (list){
    out << "<study:has_analysis>\n";
    analysis(out, list->getcurrent());
    out << "</study:has_analysis>\n";
    list = list->getnext();
  }
  return;
}

void analysis( ostream& out, const AnalysisNode* analysis ){
  if (analysis){
    out << "\t\t<study:Analysis rdf:ID=\"" << analysis->getanalysisid()->getcontents() << "\">\n";
    out << "\t\t\t<study:has_analysisname>" << analysis->getanalysisname()->getcontents() << "</study:has_analysisname>\n";
    out << "\t\t\t<study:has_software>" << analysis->getsoftware()->getcontents() << "</study:has_software>\n";
    out << "\t\t\t<study:has_algorithm>" << analysis->getalgorithm()->getcontents() << "</study:has_algorithm>\n";
    out << "\t\t\t<study:has_input_matrix>\n";
    inputmatrix_list(out, analysis->getinputmatrix());
    out << "\t\t\t</study:has_input_matrix>\n";
    outputtree_list(out, analysis->getoutputtree());
    out << "\t\t</study:Analysis>\n";
  }
  return; 
}

void inputmatrix_list( ostream& out, const ListNode<InputMatrixNode>* imatrix_list ){
    while ( imatrix_list ){
        out << "\t\t\t<study:has_input_matrix>\n";
        inputmatrix( out, imatrix_list->getcurrent());
        out << "\t\t\t</study:has_input_matrix>\n";
        imatrix_list = imatrix_list->getnext();
    }
    return;
}


void inputmatrix( ostream& out, const InputMatrixNode* matrix ){
    if (matrix){
        out << "<study:InputMatrix rdf:ID=\"input_matrix_"<< matrix->getmatrixid() <<"\"/>\n";
    }
    return;
}


void outputtree_list( ostream& out, const ListNode<OutputTreeNode>* tree_list){
   while ( tree_list ){
       out << "\t\t\t<study:has_output_tree>\n";
       outputtree( out, tree_list->getcurrent() );
       out << "\t\t\t</study:has_output_tree>\n";
        tree_list = tree_list->getnext();
   }
   return;
}
void outputtree( ostream& out, const OutputTreeNode* tree ){
  if (tree){
     out << "<study:OutputTree rdf:ID=\""<<  tree->gettreeid()->getcontents() <<"\">\n";
     //out << "<study:has_tree_name>" << tree->gettreename() << "</study:has_tree_name>\n";
     out << "<study:has_label>" << tree->gettreelabel()->getcontents() << "</study:has_label>\n";
     out << "<study:has_title>" << tree->gettreetitle()->getcontents() << "</study:has_title>\n";
     out << "<study:has_type>" << tree->gettreetype()->getcontents() << "</study:has_type>\n";
     out << "</study:OutputTree>\n";
  }
}


void footer( ostream& out){
    out << "</rdf:RDF>";
}

string& xmlescape( string& input ){
    size_t pos;
    while ( (pos = input.find('<') ) != string::npos ){ input = input.substr(0, pos-1) + "&#60;" + input.substr(pos+1);  }
    while ( (pos = input.find('>') ) != string::npos ){ input = input.substr(0, pos-1) + "&#62;" + input.substr(pos+1);  }
    while ( (pos = input.find(' ') ) != string::npos ){ input = input.substr(0, pos-1) + "-" + input.substr(pos+1); }
    return input;
}
