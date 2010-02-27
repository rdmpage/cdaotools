#include "codegen.hpp"
#include <set>
#include <iostream>
using namespace std;
using namespace Treebase;

std::wstring DATA_URI_PREFIX= L"http://www.cs.nmsu.edu/~epontell/";
std::wstring TREE_FILE_SUFFIX = L".tre";
std::wstring MATRIX_FILE_SUFFIX = L".nex";

/*
 * Write header boiler-plate to the specified stream.
 */
static void header( wostream& out );
/*
 * Write a list of studies to the specified stream
 */
static void study_list( wostream& out, const ListNode<StudyNode>* study_list );
static void define_studies_dependencies( wostream& out, const ListNode<StudyNode>* study_list );
/*
 * Write a study to the specified stream
 */
static void study( wostream& out, const StudyNode* study );
static void define_study_dependencies( wostream& out, const StudyNode* study );
/*
 * Write a list of authors to the specified stream
 */
static void author_list( wostream& out, const ListNode<AuthorNode>* authors );

static void define_authors( wostream& out, const ListNode<AuthorNode>* authors );

/*
 * Write an author to the specified stream
 */
static void author( wostream& out, const AuthorNode* author );
static void define_author( wostream& out, const AuthorNode* author);
/*
 * Write the history list to the specified stream.
 */
static void history_list( wostream& out, const ListNode<HistoryNode>* history );
static void define_histories( wostream& out, const ListNode<HistoryNode>* history);
/*
 * Write the history to the specified stream.
 */
static void history( wostream& out, const HistoryNode* history);
static void define_history( wostream& out, const HistoryNode* history);


/*
 * Write the matrix list to the specified stream.
 */
static void matrix_list( wostream& out, const ListNode<MatrixNode>* matrix_list);
static void define_matrices( wostream& out, const ListNode<MatrixNode>* matrix_list);
/*
 * Write the matrix to the specified stream.
 */
static void matrix( wostream& out, const MatrixNode* matrix );
static void define_matrix( wostream& out, const MatrixNode* matrix);
/*
 * Write the analysis list to the specified stream.
 */
static void analysis_list( wostream& out, const ListNode<AnalysisNode>* list );
static void define_analyses( wostream& out, const ListNode<AnalysisNode>* list);
/*
 * Write the analysis to the specified stream.
 */
static void analysis( wostream& out, const AnalysisNode* analysis );
static void define_analysis( wostream& out, const AnalysisNode* analysis );
/*
 * Write the input matrix list to the specified stream.
 */
static void inputmatrix_list( wostream& out, const ListNode<InputMatrixNode>* list );
static void define_imatrices( wostream& out, const ListNode<InputMatrixNode>* list );
/*
 * Write the input matrix to the specified stream.
 */
static void inputmatrix( wostream& out, const InputMatrixNode* matrix );
static void define_inputmatrix( wostream& out, const InputMatrixNode* matrix );
/*
 * Write the output tree list to the specified stream.
 */
static void outputtree_list( wostream& out, const ListNode<OutputTreeNode>* tree_list);
static void define_outputtrees( wostream& out, const ListNode<OutputTreeNode>* tree_list );
/*
 * Write the output tree to the specified stream.
 */
static void outputtree( wostream& out, const OutputTreeNode* tree );
static void define_outputtree( wostream& out, const OutputTreeNode* tree );
/*
 * Write the footer boiler-plate to the specified stream.
 */
static void footer( wostream& out );

//static wstring& xmlescape( wstring& input );


/*
 * Public interface for code generation.
 * Delegate to the study_list for actual implementation.
 */
void Treebase::generate( wostream& out, ListNode<StudyNode>* treebase ){
     header( out );
     define_studies_dependencies( out, treebase );
     //cerr << L"starting study list\n";
     study_list( out, treebase );
     //cerr << L"finished study list\n";
     footer( out );
}


void header( wostream& out ){
  out << L"<?xml version=\"1.0\"?>\n";

  out << "<!DOCTYPE rdf:RDF [ \n"
      << "<!ENTITY owl \"http://www.w3.org/2002/07/owl#\" >\n"
      << "<!ENTITY xsd \"http://www.w3.org/2001/XMLSchema#\" >\n"
      << "<!ENTITY owl2xml \"http://www.w3.org/2006/12/owl2-xml#\" >\n"
      << "<!ENTITY rdfs \"http://www.w3.org/2000/01/rdf-schema#\" >\n"
      << "<!ENTITY study \"http://www.cs.nmsu.edu/~bchisham/study.owl#\" >\n"
      << "<!ENTITY rdf \"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n"
      << "]>\n";


  out << L"<rdf:RDF\n"
      << L"\txmlns=\"http://www.cs.nmsu.edu/~bchisham/dump_utf8.txt.owl#\"\n"
      << L"\txml:base=\"http://www.cs.nmsu.edu/~bchisham/dump_utf8.txt.owl#\"\n"
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

void define_studies_dependencies( wostream& out, const ListNode<StudyNode>* study_list ){
    while (study_list){
        define_study_dependencies( out, study_list->getcurrent());
        study_list = study_list->getnext();
    }
}

void study( wostream& out, const StudyNode* study ){
  if (study){

      out << L"\t<rdf:Description rdf:ID=\"" << study->getstudyid()->getcontents() << L"\">\n";
      out << L"\t\t<rdf:type rdf:resource=\"&study;Study\"/>\n";
      out << L"\t\t<study:has_citation>" << study->getcitation()->getcontents() << "</study:has_citation>\n";
      out << L"\t\t<study:has_abstract>" << study->getabstract()->getcontents() << "</study:has_abstract>\n";
      author_list( out, study->getauthor());
      history_list(out, study->gethistory());
      matrix_list( out, study->getmatrix() );
      analysis_list( out, study->getanalysis());
      out << L"\t</rdf:Description>\n";
  }
}

void define_study_dependencies( wostream& out, const StudyNode* study ){
    if ( study ){
        define_authors( out, study->getauthor() );
        define_histories( out, study->gethistory());
        define_matrices( out, study->getmatrix());
        define_analyses( out, study->getanalysis());
    }
}

void author_list( wostream& out, const ListNode<AuthorNode>* authors ){
    while (authors){
        author( out,authors->getcurrent());
        authors = authors->getnext();
   }
   //wcerr << "<!-- processed author list -->\n";
}

void define_authors( wostream& out, const ListNode<AuthorNode>* list ){
    while( list ){
        define_author( out, list->getcurrent());
        list = list->getnext();
    }
}

void author( wostream& out, const AuthorNode* author ){
    if (author){
        out << "\t\t<study:has_author rdf:resource=\"#" << author->getauthorid()->getcontents() <<"\"/>\n";
    }
   return;
}

void define_author(wostream& out, const AuthorNode* author){
    static set< wstring > authors = set< wstring >();
    if (author){
        if( authors.find(author->getauthorid()->getcontents()) == authors.end() ){
            authors.insert(author->getauthorid()->getcontents());
            out << L"\t<rdf:Description rdf:ID=\"" << L"" <<author->getauthorid()->getcontents() << L"\">\n";
            out << L"\t\t<rdf:type rdf:resource=\"&study;Author\"/>\n";
            out << L"\t\t<foaf:first_name>" << author->getfirstname()->getcontents() << L"</foaf:first_name>\n";
            out << L"\t\t\t<foaf:last_name>" << author->getlastname()->getcontents() << L"</foaf:last_name>\n";
            out << L"\t\t<foaf:email>" << author->getemail()->getcontents() << L"</foaf:email>\n";
            out << L"\t</rdf:Description>\n";
        }   
   }
}

void history_list( wostream& out, const ListNode<HistoryNode>* history_list ){
  while (history_list){
      history( out, history_list->getcurrent() );
      history_list = history_list->getnext();
  }
  return;
}

void define_histories( wostream& out, const ListNode<HistoryNode>* list ){
    while (list){
        define_history( out, list->getcurrent() );
        list = list->getnext();
    }
}

void history( wostream& out, const HistoryNode* history_node){
    static unsigned history_counter = 0;
    out << "\t\t\t<study:has_history rdf:resource=\"#H" << history_counter++ << "\"/>\n";
    return;
}

void define_history( wostream& out, const HistoryNode* history_node){
    static unsigned history_counter = 0;
  if (history_node){
    out << L"\t<rdf:Description rdf:ID=\"H"<< history_counter++   << L"\">\n";
    out << L"\t\t<rdf:type rdf:resource=\"&study;History\"/>\n";
    out << L"\t\t<study:has_date>"<< history_node->getdate()->getcontents() << L"</study:has_date>\n";
    out << L"\t\t<study:has_time>" << history_node->gettime()->getcontents() << L"</study:has_time>\n";
    out << L"\t\t<study:has_person>" << history_node->getperson()->getcontents() << L"</study:has_person>\n";
    out << L"\t\t<study:has_event>" << history_node->getevent()->getcontents() << L"</study:has_event>\n";
    out << L"\t</rdf:Description>\n";
    //out << L"\t\t\t<study:has_history rdf:resource=\"#H" << history_counter << "\"/>\n";
  }
}

void matrix_list( wostream& out, const ListNode<MatrixNode>* matrix_list){
   while ( matrix_list ){   
       matrix( out, matrix_list->getcurrent());
       matrix_list = matrix_list->getnext() ;
   }
   return;
}

void define_matrices( wostream& out, const ListNode<MatrixNode>* list){
    while( list ){
        define_matrix(out, list->getcurrent());
        list = list->getnext();
    }
}

void matrix( wostream& out, const MatrixNode* matrix ){
    if (matrix){
        out << "\t\t<study:has_matrix rdf:resource=\"#" << matrix->getmatrixid()->getcontents() << "\"/>\n";
    }
}

void define_matrix( wostream& out, const MatrixNode* matrix){
    static set< wstring > matrices_used = set< wstring >();
    wstring mat;
    if (matrix){
        mat = matrix->getmatrixid()->getcontents();
        if ( matrices_used.find(mat) == matrices_used.end() ){
            matrices_used.insert(mat);
            out << L"\t<rdf:Description rdf:ID=\"" << mat  << L"\">\n";
            out << L"\t\t<rdf:type rdf:resource=\"&study;Matrix\"/>\n";
            out << L"\t\t<study:has_matrixname>" <<  matrix->getmatrixname()->getcontents() << L"</study:has_matrixname>\n";
            //out << L"\t<study:has_datatype rdf:datatype=\"&xsd;string\">" << matrix->getdatatype() << L"</study:has_datatype>\n";
            out << L"\t\t<study:has_datatype>" <<  matrix->getdatatype()->getcontents() << L"</study:has_datatype>\n";
            const ConstNode* nchar = matrix->getnchar();
            if (nchar){
                out << L"\t\t<study:has_nchar>" << nchar->getvalue() << L"</study:has_nchar>\n";
            }
            out << L"\t</rdf:Description>\n";
        }
    }
}

void analysis_list( wostream& out, const ListNode<AnalysisNode>* list ){
  while (list){
      analysis( out, list->getcurrent());
      list = list->getnext();
  }
  return;
}

void define_analyses(wostream& out, const ListNode<AnalysisNode>* list){
    while (list){
        define_analysis( out, list->getcurrent());
        list = list->getnext();
    }
}

void analysis( wostream& out, const AnalysisNode* analysis ){
  if (analysis){
          out << L"\t\t<study:has_analysis rdf:resource=\"#" << analysis->getanalysisid()->getcontents() <<  "\"/>\n";
          //out << "\t\t<rdf:Description rdf:about=\"#" << analysis->getanalysisid()->getcontents() << "\">\n";
          //inputmatrix_list(out, analysis->getinputmatrix());
          //outputtree_list(out, analysis->getoutputtree());
         // out << "\t\t</rdf:Description>\n";
          //out << L"\t\t</study:has_analysis>\n";
      }   
  return; 
}


void define_analysis( wostream& out, const AnalysisNode* analysis ){
    static set< wstring > anal_done = set< wstring >();
    wstring anal = analysis->getanalysisid()->getcontents();
     if( anal_done.find( anal ) == anal_done.end() ){
          anal_done.insert( anal );
          define_imatrices(out, analysis->getinputmatrix());
          define_outputtrees(out, analysis->getoutputtree());
          out << L"\t<rdf:Description rdf:ID=\""  << analysis->getanalysisid()->getcontents() << "\">\n";
          out << L"\t\t<rdf:type rdf:resource=\"&study;Analysis\"/>\n";
          out << L"\t\t<study:has_analysisname>" << analysis->getanalysisname()->getcontents() << L"</study:has_analysisname>\n";
          out << L"\t\t<study:has_software>" << analysis->getsoftware()->getcontents() << L"</study:has_software>\n";
          out << L"\t\t<study:has_algorithm>" << analysis->getalgorithm()->getcontents() << L"</study:has_algorithm>\n";

          inputmatrix_list( out, analysis->getinputmatrix());
          outputtree_list( out, analysis->getoutputtree());


          out << L"\t</rdf:Description>\n";
          
     }
}

void inputmatrix_list( wostream& out, const ListNode<InputMatrixNode>* imatrix_list ){
    //static std::set< std::wstring > imatrix_used

    while ( imatrix_list ){
        inputmatrix( out, imatrix_list->getcurrent());
        imatrix_list = imatrix_list->getnext();
    }
    return;
}

void define_imatrices( wostream& out, const ListNode<InputMatrixNode>* list){
    while (list){
        define_inputmatrix( out, list->getcurrent());
        list = list->getnext();
    }
}

void inputmatrix( wostream& out, const InputMatrixNode* matrix ){
    if (matrix){
        out << L"\t\t\t<study:has_input_matrix rdf:resource=\"#" << matrix->getmatrixid()->getcontents() << L"\"/>\n";
    }
    return;
}

void define_inputmatrix( wostream& out, const InputMatrixNode* matrix ){
    return;
}

void outputtree_list( wostream& out, const ListNode<OutputTreeNode>* tree_list){
    
    while ( tree_list ){
        outputtree( out, tree_list->getcurrent());
        tree_list = tree_list->getnext();
   }
   return;
}

void define_outputtrees( wostream& out, const ListNode<OutputTreeNode>* trees ){
    while ( trees ){
        define_outputtree(out, trees->getcurrent());
        trees = trees->getnext();
    }
}

void outputtree( wostream& out, const OutputTreeNode* tree ){
   // static unsigned serio = 0;
    static std::set< std::wstring > trees_used = std::set< std::wstring >();
  if (tree){
     out << L"\t\t\t<study:has_output_tree rdf:resource=\"#" << tree->gettreeid()->getcontents() << "\"/>\n";
  }
}

void define_outputtree( wostream& out, const OutputTreeNode* tree){

    if (tree){
     out << L"\t<rdf:Description rdf:ID=\"" << tree->gettreeid()->getcontents() << L"\">\n";
     out << L"\t\t<rdf:type rdf:resource=\"&study;OutputTree\"/>\n";
     //out << L"<study:has_tree_name>" << tree->gettreename() << L"</study:has_tree_name>\n";
     out << L"\t\t<study:has_label>" << tree->gettreelabel()->getcontents() << L"</study:has_label>\n";
     out << L"\t\t<study:has_title>" << tree->gettreetitle()->getcontents() << L"</study:has_title>\n";
     out << L"\t\t<study:has_type>" << tree->gettreetype()->getcontents() << L"</study:has_type>\n";
     out << L"\t</rdf:Description>\n";
    }

}

void footer( wostream& out){
    out << L"</rdf:RDF>";
}
