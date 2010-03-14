#include "codegen.hpp"
#include <set>
using namespace std;
using namespace Treebase;
/*
 * Write header boiler-plate to the specified stream.
 */
static void header( wostream& out );
/*
 * Write a list of studies to the specified stream
 */
static void study_list( wostream& out, const ListNode<StudyNode>* study_list );
/*
 * Write a study to the specified stream
 */
static void study( wostream& out, const StudyNode* study );
/*
 * Write a list of authors to the specified stream
 */
static void author_list( wostream& out, const ListNode<AuthorNode>* authors );
/*
 * Write an author to the specified stream
 */
static void author( wostream& out, const AuthorNode* author );
/*
 * Write the history list to the specified stream.
 */
static void history_list( wostream& out, const ListNode<HistoryNode>* history );
/*
 * Write the history to the specified stream.
 */
static void history( wostream& out, const HistoryNode* history);
/*
 * Write the matrix list to the specified stream.
 */
static void matrix_list( wostream& out, const ListNode<MatrixNode>* matrix_list);
/*
 * Write the matrix to the specified stream.
 */
static void matrix( wostream& out, const MatrixNode* matrix );
/*
 * Write the analysis list to the specified stream.
 */
static void analysis_list( wostream& out, const ListNode<AnalysisNode>* list );
/*
 * Write the analysis to the specified stream.
 */
static void analysis( wostream& out, const AnalysisNode* analysis );
/*
 * Write the input matrix list to the specified stream.
 */
static void inputmatrix_list( wostream& out, const ListNode<InputMatrixNode>* list );
/*
 * Write the input matrix to the specified stream.
 */
static void inputmatrix( wostream& out, const InputMatrixNode* matrix );
/*
 * Write the output tree list to the specified stream.
 */
static void outputtree_list( wostream& out, const ListNode<OutputTreeNode>* tree_list);
/*
 * Write the output tree to the specified stream.
 */
static void outputtree( wostream& out, const OutputTreeNode* tree );
/*
 * Write the footer boiler-plate to the specified stream.
 */
static void footer( wostream& out );

static wstring& xmlescape( wstring& input );


/*
 * Public interface for code generation.
 * Delegate to the study_list for actual implementation.
 */
void Treebase::generate( wostream& out, ListNode<StudyNode>* treebase ){
     header( out );
     //cerr << L"starting study list\n";
     study_list( out, treebase );
     //cerr << L"finished study list\n";
     footer( out );
}


void header( wostream& out ){
  out << L"<?xml version=\"1.0\"?>\n";
  out << L"<rdf:RDF\n"
      << L"\txmlns=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << L"\txml:base=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << L"\txmlns:owl2xml=\"http://www.w3.org/2006/12/owl2-xml#\"\n"
      << L"\txmlns:study=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << L"\txmlns:contact=\"http://www.w3.org/2000/10/swap/pim/contact#\"\n"
      << L"\txmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"\n"
      << L"\txmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"\n"
      << L"\txmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
      << L"\txmlns:owl=\"http://www.w3.org/2002/07/owl#\"\n"
      //<< L"\txmlns:study=\"http://www.cs.nmsu.edu/~bchisham/study.owl#\"\n"
      << L"\txmlns:foaf=\"http://www.mindswap.org/2003/owl/foaf#\""
      << L">\n";
  out << L"<owl:Ontology rdf:about=\"\">\n"
      << L"\t<owl:imports rdf:resource=\"http://www.cs.nmsu.edu/~bchisham/study.owl\"/>\n"
      << L"</owl:Ontology>\n";

}

void study_list( wostream& out, const ListNode<StudyNode>* study_list ){
     while (study_list){
         study(out, study_list->getcurrent() );
         study_list = study_list->getnext();
     }
     return;                                                                                                                                                                                    
}

void study( wostream& out, const StudyNode* study ){
  if (study){
   out << L"<study:Study rdf:ID=\"" << L"study_" << study->getstudyid()->getcontents() << L"\">\n";
   out << L"\t<study:has_citation>"<< study->getcitation()->getcontents() << L"</study:has_citation>\n";
   out << L"\t<study:has_abstract>" << study->getabstract()->getcontents() << L"</study:has_abstract>\n";
   author_list( out, study->getauthor() );
   history_list(out, study->gethistory());
   matrix_list( out, study->getmatrix() );
   analysis_list( out, study->getanalysis());
   out << L"</study:Study>\n";
  }

}

void author_list( wostream& out, const ListNode<AuthorNode>* authors ){
   while (authors){
      out << L"<study:has_author>\n";
      author( out,authors->getcurrent());
      out << L"</study:has_author>\n";
      authors = authors->getnext();
   }
}

void author( wostream& out, const AuthorNode* author ){
   static set< wstring > authors_used = set< wstring >();
   if (author){
       if ( authors_used.find( author->getauthorid()->getcontents() ) == authors_used.end() ){
         authors_used.insert( author->getauthorid()->getcontents() );
         out << L"\t<study:Author rdf:ID=\"" << L"author_" <<author->getauthorid()->getcontents() << L"\">\n";
         out << L"\t\t<foaf:first_name>" << author->getfirstname()->getcontents() << L"</foaf:first_name>\n";
         out << L"\t\t<foaf:last_name>" << author->getlastname()->getcontents() << L"</foaf:last_name>\n";
         out << L"\t\t<foaf:email>" << author->getemail()->getcontents() << L"</foaf:email>\n";
         out << L"\t</study:Author>\n";
       }
       else {
         out << L"\t<study:Author rdf:about=\"" << author->getauthorid()->getcontents() << L"\"/>\n";
       }
   }
   return;
}

void history_list( wostream& out, const ListNode<HistoryNode>* history_list ){
  while (history_list){
      out << L"\t<study:has_history>\n";
      history( out, history_list->getcurrent() );
      out << L"\t</study:has_history>\n";
      history_list = history_list->getnext();
  }
  return;
}
void history( wostream& out, const HistoryNode* history_node){
  static unsigned history_counter = 0;
  if (history_node){
    out << L"\t\t<study:History rdf:ID=\"history_"<< history_counter++   << L"\">\n";
    out << L"\t\t\t<study:has_date>"<< history_node->getdate()->getcontents() << L"</study:has_date>\n";
    out << L"\t\t\t<study:has_time>" << history_node->gettime()->getcontents() << L"</study:has_time>\n";
    out << L"\t\t\t<study:has_person>" << history_node->getperson()->getcontents() << L"</study:has_person>\n";
    out << L"\t\t\t<study:has_event>" << history_node->getevent()->getcontents() << L"</study:has_event>\n";
    out << L"\t\t</study:History>\n";
  }
  return;
}

void matrix_list( wostream& out, const ListNode<MatrixNode>* matrix_list){
   while ( matrix_list ){
       out << L"<study:has_matrix>\n";
       matrix( out, matrix_list->getcurrent());
       out << L"</study:has_matrix>\n";
       matrix_list = matrix_list->getnext() ;
   }
   return;
}


void matrix( wostream& out, const MatrixNode* matrix ){
  if (matrix){
    out << L"\t\t<study:Matrix rdf:ID=\"matrix_" << matrix->getmatrixid()->getcontents() << L"\">\n";
    out << L"\t\t\t<study:has_matrixname>" <<  matrix->getmatrixname()->getcontents() << L"</study:has_matrixname>\n";
    out << L"\t\t\t<study:has_datatype>" <<  matrix->getdatatype()->getcontents() << L"</study:has_datatype>\n";
    out << L"\t\t\t<study:has_nchar>" << matrix->getnchar()->getvalue() << L"</study:has_nchar>\n";
    out << L"\t\t</study:Matrix>\n";
  }

}

void analysis_list( wostream& out, const ListNode<AnalysisNode>* list ){

  while (list){
    out << L"<study:has_analysis>\n";
    analysis(out, list->getcurrent());
    out << L"</study:has_analysis>\n";
    list = list->getnext();
  }
  return;
}

void analysis( wostream& out, const AnalysisNode* analysis ){
  if (analysis){
    out << L"\t\t<study:Analysis rdf:ID=\"analysis_" << analysis->getanalysisid()->getcontents() << L"\">\n";
    out << L"\t\t\t<study:has_analysisname>" << analysis->getanalysisname()->getcontents() << L"</study:has_analysisname>\n";
    out << L"\t\t\t<study:has_software>" << analysis->getsoftware()->getcontents() << L"</study:has_software>\n";
    out << L"\t\t\t<study:has_algorithm>" << analysis->getalgorithm()->getcontents() << L"</study:has_algorithm>\n";
    out << L"\t\t\t<study:has_input_matrix>\n";
    inputmatrix_list(out, analysis->getinputmatrix());
    out << L"\t\t\t</study:has_input_matrix>\n";
    outputtree_list(out, analysis->getoutputtree());
    out << L"\t\t</study:Analysis>\n";
  }
  return; 
}

void inputmatrix_list( wostream& out, const ListNode<InputMatrixNode>* imatrix_list ){
    while ( imatrix_list ){
        out << L"\t\t\t<study:has_input_matrix>\n";
        inputmatrix( out, imatrix_list->getcurrent());
        out << L"\t\t\t</study:has_input_matrix>\n";
        imatrix_list = imatrix_list->getnext();
    }
    return;
}


void inputmatrix( wostream& out, const InputMatrixNode* matrix ){
    if (matrix){
        out << L"<study:InputMatrix rdf:ID=\"input_matrix_"<< matrix->getmatrixid() << L"\"/>\n";
    }
    return;
}


void outputtree_list( wostream& out, const ListNode<OutputTreeNode>* tree_list){
   while ( tree_list ){
       out << L"\t\t\t<study:has_output_tree>\n";
       outputtree( out, tree_list->getcurrent() );
       out << L"\t\t\t</study:has_output_tree>\n";
        tree_list = tree_list->getnext();
   }
   return;
}
void outputtree( wostream& out, const OutputTreeNode* tree ){
  if (tree){
     out << L"<study:OutputTree rdf:ID=\"tree_"<<  tree->gettreeid()->getcontents() << L"\">\n";
     //out << L"<study:has_tree_name>" << tree->gettreename() << L"</study:has_tree_name>\n";
     out << L"<study:has_label>" << tree->gettreelabel()->getcontents() << L"</study:has_label>\n";
     out << L"<study:has_title>" << tree->gettreetitle()->getcontents() << L"</study:has_title>\n";
     out << L"<study:has_type>" << tree->gettreetype()->getcontents() << L"</study:has_type>\n";
     out << L"</study:OutputTree>\n";
  }
}


void footer( wostream& out){
    out << L"</rdf:RDF>";
}

wstring& xmlescape( wstring& input ){
    size_t pos;
    while ( (pos = input.find('<') ) != wstring::npos ){ input = input.substr(0, pos-1) + L"&#60;" + input.substr(pos+1);  }
    while ( (pos = input.find('>') ) != wstring::npos ){ input = input.substr(0, pos-1) + L"&#62;" + input.substr(pos+1);  }
    while ( (pos = input.find(' ') ) != wstring::npos ){ input = input.substr(0, pos-1) + L"-" + input.substr(pos+1); }
    return input;
}
