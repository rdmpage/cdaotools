#include "codegen.hpp"

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
      << "\txmlns:study_ontology=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << "\txmlns:contact=\"http://www.w3.org/2000/10/swap/pim/contact#\"\n"
      << "\txmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"\n"
      << "\txmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"\n"
      << "\txmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
      << "\txmlns:owl=\"http://www.w3.org/2002/07/owl#\"\n"
      //<< "\txmlns:study=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << "\txmlns:foaf=\"http://www.mindswap.org/2003/owl/foaf\""
      << ">\n";
  out << "<owl:Ontology rdf:about=\"\">\n"
      << "\t<owl:imports rdf:about=\"http://www.cs.nmsu.edu/~bchisham/study.owl\"/>\n"
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
   out << "<study_ontology:Study rdf:ID=\"" << study->getstudyid()->getcontents() << "\">\n";
   out << "\t<study_ontology:has_citation>"<< study->getcitation()->getcontents() <<"</study_ontology:has_citation>\n";
   out << "\t<study_ontology:has_abstract>" << study->getabstract()->getcontents() <<"</study_ontology:has_abstract>\n";
   author_list( out, study->getauthor() );
   history_list(out, study->gethistory());
   matrix_list( out, study->getmatrix() );
   analysis_list( out, study->getanalysis());
   out << "</study_ontology:Study>\n";
  }

}

void author_list( ostream& out, const ListNode<AuthorNode>* authors ){
   while (authors){
      out << "<study_ontology:has_author>\n";
      author( out,authors->getcurrent());
      out << "</study_ontology:has_author>\n";
      authors = authors->getnext();
   }
}

void author( ostream& out, const AuthorNode* author ){
   if (author){
       out << "\t<study_ontology:Author rdf:ID=\"" << author->getauthorid()->getcontents() << "\">\n";
       out << "\t\t<foaf:first_name>" << author->getfirstname()->getcontents() << "</foaf:first_name>\n";
       out << "\t\t<foaf:last_name>" << author->getlastname()->getcontents() << "</foaf:last_name>\n";
       out << "\t\t<foaf:email>" << author->getemail()->getcontents() << "</foaf:email>\n";
       out << "\t</study_ontology:Author>\n";
   }
   return;
}

void history_list( ostream& out, const ListNode<HistoryNode>* history_list ){
  while (history_list){
      out << "\t<study_ontology:has_history>\n";
      history( out, history_list->getcurrent() );
      out << "\t</study_ontology:has_history>\n";
      history_list = history_list->getnext();
  }
  return;
}
void history( ostream& out, const HistoryNode* history_node){
  if (history_node){
    out << "\t\t<study_ontology:History>\n";
    out << "\t\t\t<study_ontology:has_date>"<< history_node->getdate()->getcontents() <<"</study_ontology:has_date>\n";
    out << "\t\t\t<study_ontology:has_time>" << history_node->gettime()->getcontents() << "</study_ontology:has_time>\n";
    out << "\t\t\t<study_ontology:has_person>" << history_node->getperson()->getcontents() << "</study_ontology:has_person>\n";
    out << "\t\t\t<study_ontology:has_event>" << history_node->getevent()->getcontents() << "</study_ontology:has_event>\n";
    out << "\t\t</study_ontology:History>";
  }
  return;
}

void matrix_list( ostream& out, const ListNode<MatrixNode>* matrix_list){
   while ( matrix_list ){
       out << "<study_ontology:has_matrix>\n";
       matrix( out, matrix_list->getcurrent());
       out << "</study_ontology:has_matrix>\n";
       matrix_list = matrix_list->getnext() ;
   }
   return;
}


void matrix( ostream& out, const MatrixNode* matrix ){
  if (matrix){
    out << "\t\t<study_ontology:Matrix rdf:ID=\"" << matrix->getmatrixid()->getcontents() << "<\">\n";
    out << "\t\t\t<study_ontology:has_matrixname>" <<  matrix->getmatrixname()->getcontents() << "</study_ontology:has_matrixname>\n";
    out << "\t\t\t<study_ontology:has_datatype>" <<  matrix->getdatatype()->getcontents() << "<study_ontology:has_datatype>\n";
    out << "\t\t\t<study_ontology:has_nchar>" << matrix->getnchar()->getvalue() << "</study_ontology:nchar>\n";
    out << "\t\t</study_ontology:Matrix>\n";
  }

}

void analysis_list( ostream& out, const ListNode<AnalysisNode>* list ){

  while (list){
    out << "<study_ontology:has_analysis>\n";
    analysis(out, list->getcurrent());
    out << "</study_ontology:has_analysis>\n";
    list = list->getnext();
  }
  return;
}

void analysis( ostream& out, const AnalysisNode* analysis ){
  if (analysis){
    out << "\t\t<study_ontology:Analysis rdf:ID=\"" << analysis->getanalysisid()->getcontents() << "\">\n";
    out << "\t\t\t<study_ontology:has_analysisname>" << analysis->getanalysisname()->getcontents() << "</study_ontology:has_analysisname>\n";
    out << "\t\t\t<study_ontology:has_software>" << analysis->getsoftware()->getcontents() << "</study_ontology:has_software>\n";
    out << "\t\t\t<study_ontology:has_algorithm>" << analysis->getalgorithm()->getcontents() << "</study_ontology:has_algorithm>\n";
    out << "\t\t\t<study_ontology:has_input_matrix>\n";
    inputmatrix_list(out, analysis->getinputmatrix());
    out << "\t\t\t</study_ontology:has_input_matrix>\n";
    outputtree_list(out, analysis->getoutputtree());
    out << "\t\t</study_ontology:Analysis>\n";
  }
  return; 
}

void inputmatrix_list( ostream& out, const ListNode<InputMatrixNode>* imatrix_list ){
    while ( imatrix_list ){
        out << "\t\t\t<study_ontology:has_input_matrix>\n";
        inputmatrix( out, imatrix_list->getcurrent());
        out << "\t\t\t</study_ontology:has_input_matrix>\n";
        imatrix_list = imatrix_list->getnext();
    }
    return;
}


void inputmatrix( ostream& out, const InputMatrixNode* matrix ){
    if (matrix){
        out << "<study_ontology:InputMatrix rdf:about=\""<< matrix->getmatrixid() <<"\"/>\n";
    }
    return;
}


void outputtree_list( ostream& out, const ListNode<OutputTreeNode>* tree_list){
   while ( tree_list ){
       out << "\t\t\t<study_ontology:has_output_tree>\n";
       outputtree( out, tree_list->getcurrent() );
       out << "\t\t\t</study_ontology:has_output_tree>\n";
        tree_list = tree_list->getnext();
   }
   return;
}
void outputtree( ostream& out, const OutputTreeNode* tree ){
  if (tree){
     out << "<study_ontology:OutputTree rdf:ID=\""<<  tree->gettreeid()->getcontents() <<"\">\n";
     //out << "<study_ontology:has_tree_name>" << tree->gettreename() << "</study_ontology:has_tree_name>\n";
     out << "<study_ontology:has_label>" << tree->gettreelabel()->getcontents() << "</study_ontology:has_label>\n";
     out << "<study_ontology:has_title>" << tree->gettreetitle()->getcontents() << "</study_ontology:has_title>\n";
     out << "<study_ontology:has_type>" << tree->gettreetype()->getcontents() << "</study_ontology:has_type>\n";
     out << "</study_ontology:OutputTree>\n";
  }
}


void footer( ostream& out){
    out << "</rdf:RDF>";
}
