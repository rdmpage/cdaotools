#include "codegen.hpp"
#include "constants.hpp"
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <node.hpp>
#include <tree_description_parser.hpp>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <set>

/*
 * Contains implementation of the code generation library.
 */

using namespace std;
using namespace CDAO::Codegen;
namespace CDAO {
  
  //static const wstring CSDM_ANNOTATION_ID = L"csdm_annotation_id";
  //static const wstring CSDM_ID            = L"csdm_id";
  //converts the data type code into unknown, nucleotide, or amino acid
  static int dataTypeTranslationPosition[] = { 0,0,1,1,1,2 };
  /*
   * Defines an easier to use short-hand for tu operations.
   */
  typedef void (*tu_op_t)(wostream&, const DataRepresentation*, const Node*);
  typedef void (*matrix_op_t)(wostream&, const DataRepresentation*);
  
  typedef void (CodeGenerator::*mem_matrix_op_t)( wostream& out );
  typedef void (CodeGenerator::*mem_tu_op_t)( wostream& out, const Node* );
  
  
  /*
   * Provides  a closure like construct for processing TU's
   */
  class ProcessTUDelegate : public Node::Delegate {
  public:
    /*
     * Initialized the saved part of the state.
     */
    ProcessTUDelegate(wostream& out, const DataRepresentation* model, tu_op_t operation):out_(out),
										   model_(model),
										   operation_(operation){}
    
    virtual ~ProcessTUDelegate(){}
    /*
     * Do the specified operation.
     */
    virtual void operator()(const Node* in){ (*operation_)( out_, model_, in); }
  private:
    wostream& out_;
    const DataRepresentation* model_;
    tu_op_t operation_;
  };
  
  
  wstring XMLizeName( wstring in);
 
  
  /*
   * Write basic document header info.
   */
  void writeHeaderBoilerPlate( wostream& out, const DataRepresentation* model );
  /*
   * Write document closing info.  
   */
  void writeClosing( wostream& out, const DataRepresentation* model );
  /*
   * Write wchar_tacters to the specifed stream.
   */
  void writeCharacters( wostream& out, const DataRepresentation* model);
  /*
   * Create wchar_tacter state matrix annotation.
   */
  void writeCharacterStateMatrixAnnotation(wostream& out, const DataRepresentation* model);
  
  /*
   * Write the wchar_tacter state matrix definition.
   */
  void writeCharacterStateMatrix(wostream& out, const  DataRepresentation* model);
  /*
   * Writes the states of each wchar_tacter for each taxon.
   */
  void writeCharacterStates( wostream& out, const  DataRepresentation* model);
  /*
   */
  void writeTree( wostream& out, const DataRepresentation* model);
  void writeLineages( wostream& out, const DataRepresentation* model);
  /*
   * Write a single state instance
   */
  void writeState( wostream& out, const  DataRepresentation* model , const unsigned int data_type, const int state_code);
  /*
   * Generate TU instances
   */
  void writeTUS( wostream& out, const DataRepresentation* model);
  /**
   */
  void writeSubtrees( wostream& out, const DataRepresentation* model, const Node* current);
  /*
   * Generate TU instance.
   */
  void writeTU( wostream& out, const DataRepresentation* model, const Node* current);
  /*
   * Write an Edge instance
   */
  void writeEdge( wostream& out, const  Node* parent, const  Node* child);
  /*
   * Write each wchar_tacter state modification.
   */
  void writeCharacterStateModifications( wostream& out, const DataRepresentation* model );
  
  void writeChildConnections( wostream& out, const Node* current,  const vector< const Node* >& children);
  
  void writeLineageConnections( wostream& out, const Node* current,  const vector< const Node* >& leaves );
  
  void writeNcaInfo( wostream& out,const Node* current,  const vector< const Node* >& allDescendants );
  
  /*
   * Write ontology to the specified stream.
   */
  void CodeGenerator::generate( wostream& out ){
    
    if ( data_model_ ){
      /* Setup order writing will happen and which functions will do it. */
      matrix_op_t writers[] = {
                              writeHeaderBoilerPlate, 
                              writeTree,
  			      //writeLineages,
			      writeCharacterStateMatrixAnnotation, 
			      writeCharacterStateMatrix, 
			      writeCharacters, 
			      writeCharacterStates, 
			      writeTUS,
                              writeClosing };
      Imports::setBaseURI( Imports::BASE_URI_PREFIX + data_model_->getMatrixLabel() );
      //writeHeaderBoilerPlate( out );
      /* Invoke the writer delegates */
      for (unsigned int i = 0; i < sizeof(writers)/sizeof(writers[0]); ++i ){
        (*writers[i])( out, data_model_ );
      }
      //writeTUS( out, data_model_ );
      //writeClosing( out );
    }
    return;
  }
  
  wstring XMLizeName(wstring in){
    wstring ret = L"";
    if ( isdigit(in.at(0)) ){ in = L"n" + in; }
    for (unsigned int i = 0; i < in.size(); ++i){
      if (isalnum(in.at(i))){ ret+= in.at(i);  }
      else { ret += L"_"; }
    }
    return ret;
  }

  /*
   * Return a standardized reference to a wchar_tacter. 
   */
  wstring chartacter_reference_string(  unsigned int trait );
  /*
   * Write basic header information.
   */
  void writeHeaderBoilerPlate( wostream& out, const DataRepresentation* model ){
    out << L"<?xml version=\"1.0\"?>" << endl << endl << endl
	<< L"<!DOCTYPE "  << NSDefs::RDF      << L":RDF [" << endl
	<< L"\t<!ENTITY " << NSDefs::OWL      << L" \""    << Imports::OWL_URI << L"#\">" << endl
        << L"\t<!ENTITY " << NSDefs::OWL11    << L" \""    << Imports::OWL11_URI  << L"#\">" <<endl
        << L"\t<!ENTITY " << NSDefs::DC       << L" \""    << Imports::DC         << L"#\">" << endl
      	<< L"\t<!ENTITY " << NSDefs::XSD      << L" \""    << Imports::XSD_URI  << L"#\">" << endl
      	<< L"\t<!ENTITY " << NSDefs::OWL11XML << L" \""    << Imports::OWL11XML_URI  <<L"#\">" <<endl
	<< L"\t<!ENTITY " << NSDefs::RDFS     << L" \""    << Imports::RDFS_URI << L"#\">" << endl
	<< L"\t<!ENTITY " << NSDefs::RDF      << L" \""    << Imports::RDF_URI  <<"#\">"  <<endl
	<< L"\t<!ENTITY " << NSDefs::CDAO     << L" \""    << Imports::CDAO_DEF_URI << L"#\">" << endl
	<< L"\t<!ENTITY " << NSDefs::AMINO_ACID << L" \""  << Imports::AMINO_ACID << L"#\">" << endl
	<< L"]>" << endl << endl << endl;
    
    out << L"<"        << Builtins::RDF    <<L" xmlns=\"" << Imports::BASE_URI_PREFIX << model->getMatrixLabel()   << L"#\"" << endl
	<< L"\txml:"   <<  NSDefs::BASE     <<L"=\""      << Imports::BASE_URI_PREFIX << model->getMatrixLabel()        << L"#\"" << endl
	<< L"\txmlns:" << NSDefs::OWL11    <<L"=\""      << Imports::OWL11_URI       << L"#\"" << endl
	<< L"\txmlns:" << NSDefs::OWL11XML <<L"=\""      << Imports::OWL11XML_URI    << L"#\"" << endl
	<< L"\txmlns:" << NSDefs::XSD      <<L"=\""      << Imports::XSD_URI         << L"#\""  << endl
	<< L"\txmlns:" << NSDefs::RDFS     <<L"=\""      << Imports::RDFS_URI        << L"#\""  << endl
	<< L"\txmlns:" << NSDefs::RDF      <<L"=\""      << Imports::RDF_URI         << L"#\""  << endl
	<< L"\txmlns:" << NSDefs::OWL      <<L"=\""      << Imports::OWL_URI         << L"#\""  << endl
	<< L"\txmlns:" << NSDefs::CDAO     <<L"=\""      << Imports::CDAO_DEF_URI    << L"#\""  << endl
      
	<< L"\txmlns:" << NSDefs::AMINO_ACID << L"=\""   << Imports::AMINO_ACID      <<L"#\">" << endl;
    out << L"\t<"   << Builtins::ONT     <<  L" " << Builtins::ABOUT  <<L"=\"" << Imports::CDAO_DEF_URI  <<L"\">" << endl
	<< L"\t\t<" << Builtins::IMPORTS <<  L" " << Builtins::RESOURCE <<L"=\"" << Imports::CDAO_DEF_URI  <<  L"\"/>" << endl
	<< L"\t</"  << Builtins::ONT     <<L">"   << endl;
  }
  /*
   * Write basic closing information.
   */
  void writeClosing( wostream& out, const DataRepresentation* model ){ 
    out << L"</" << Builtins::RDF  <<  L">" << endl;
    return;
  }
  /*
   * Write wchar_tacters representation.
   */
  void writeCharacters( wostream& out, const DataRepresentation* model){
    if ( model && model->getNTraits() ){
      //wchar_tacter tags.
      static const wstring dataTypeTranslation[] = {NSDefs::CDAO + L":" + Classes::UKN,
  					   NSDefs::CDAO + L":" + Classes::NUC_CHARACTER , 
  					   NSDefs::CDAO + L":" + Classes::AA_CHARACTER};
      //the particular tag corresponding to the set's datatype.
      const wstring dtype_tag = dataTypeTranslation[ dataTypeTranslationPosition[ model->getDataType() ] ];
      //write each wchar_tacter for each taxon.
      for (unsigned int trait = 0; trait < model->getNTraits(); ++trait){
        out << L"\t<" << dtype_tag << L" " << Builtins::ID << L"=\"" <<  L"trait_" << trait << L"\">" << endl;
        for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
	  out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_DATUM << L" " << Builtins::RESOURCE << L"=\"#trait_" << trait << L"_taxon_" << taxon << L"\" />" << endl;  
        }
        out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::BELONGS_TO_CSDM << L" " << Builtins::RESOURCE << L"=\"#" << XMLizeName(model->getMatrixLabel()) << L"\" />" << endl;
        out << L"\t</" << dtype_tag << L">" << endl;
      }
    }
    return;
  }
  //write wchar_tacter state matrix annotation definion.
  void writeCharacterStateMatrixAnnotation(wostream& out, const DataRepresentation* model){
    if ( model && model->getNTraits() ){
       out << L"\t<" << NSDefs::CDAO <<":" << Classes::CSDM_ANNOTATION << L" " << Builtins::ID << L"=\"" << XMLizeName( model->getMatrixLabel() ) + L"_annotation_id" << L"\" />" << endl; 
    }
  }
  //write wchar_tacter state matrix definition.
  void writeCharacterStateMatrix(wostream& out, const DataRepresentation* model){
    if ( model && model->getNTraits() ){
       out << L"\t<" << NSDefs::CDAO << L":" << Classes::CSDM << L" " << Builtins::ID << L"=\"" << XMLizeName( model->getMatrixLabel() ) << L"\">" << endl;
       out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS << L" " << Builtins::RESOURCE << L"=\"#" <<XMLizeName( model->getMatrixLabel()) + L"_annotation_id" << L"\" />" << endl;
      //include references for each taxon in the dataset.
      for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
        out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_TU << L" " << Builtins::RESOURCE << L"=\"#" << L"taxon_" << taxon << L"\" />" << endl;
      }
      for (unsigned int trait = 0; trait < model->getNTraits(); ++trait){
        //  for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
        out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_CHARACTER << L" " << Builtins::RESOURCE << L"=\"#" << L"trait_" << trait 
	  /*<< L"_taxon_" << taxon */ << L"\" />" << endl; 
	   //  }
      }
      out << L"\t</" << NSDefs::CDAO << L":" << Classes::CSDM << L">" << endl;
    }
    return;
  }
  /*
   * Create a state instance for each taxon and trait.
   */
  void writeCharacterStates( wostream& out, const DataRepresentation* model){
    //tags used for chartacter states.
    static wstring dataTypeTranslation[] = {NSDefs::CDAO + L":" + Classes::UKN,
					   NSDefs::CDAO + L":" + Classes::NUC_DATUM , 
					   NSDefs::CDAO + L":" + Classes::AA_DATUM };
    if ( model ){
      //selects the particular state tag needed for this set.
      const wstring dtype_tag = dataTypeTranslation[ dataTypeTranslationPosition[ model->getDataType() ] ];
      //write state for each trait and taxon.
      for (unsigned int trait = 0; trait < model->getNTraits(); ++trait){
        for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
	  out << L"\t<" <<dtype_tag  << L" L" << Builtins::ID << L"=\"" << L"trait_" << trait << L"_taxon_" << taxon << L"\">" << endl;
	  //identify which TU this trait state belongs to.
	  out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::BELONGS_TO_TU << L" " << Builtins::RESOURCE << L"=\"#" << /*XMLizeName( model->getTaxonLabel( taxon ))*/ L"taxon_" << taxon << L"\" />" << endl;
	  out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::PART_OF << L" " << Builtins::RESOURCE << L"=\"#" << /*XMLizeName( model->getTaxonLabel( taxon ))*/ L"trait_" << trait << L"\" />" << endl;
	  //write the state value.
	  writeState( out, model,  model->getDataType(),  model->getTraitState( taxon, trait ) );
	  out << L"\t</" << dtype_tag << L">" << endl;
        }
      }
    }
  }
  /*
   */
  void writeTree( wostream& out, const DataRepresentation* model){
    if ( model ){
      for (unsigned i = 0; i < model->getNumTrees(); ++i ){
          out << L"<" << NSDefs::CDAO << L":" << Classes::TREE << L" " << Builtins::ID << L"=\"node_" << XMLizeName( model->getTreeLabel( i ) ) << L"\" />" << endl;
      }
    }
    return;
  }
  
  void writeLineages( wostream& out, const DataRepresentation* model){
    if( model ){
      for (unsigned int tree = 0; tree < model->getNumTrees(); ++tree){
        vector< const Node* > leaves = model->getParseTree( tree )->getLeaves( model->getParseTree( tree ) );
        for (vector< const Node* >::const_iterator i = leaves.begin(); i != leaves.end(); ++i){
          out << L"\t<" << NSDefs::CDAO << L":" << Classes::LINEAGE << L" " << Builtins::ID << L"=\"Lineage_" << XMLizeName( model->getTreeLabel( tree ) ) << L"_" << XMLizeName( (*i)->getLabel() ) << L"\">" << endl;
          out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::SUBTREE_OF << L" " << Builtins::RESOURCE << L"=\"#node_" << XMLizeName( model->getTreeLabel( 0 ) ) << L"\" />" << endl;
          vector< const Node* > ancestors = (*i)->getAncestors();
          //add the current node to it's own lineage
          out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_LINEAGE_NODE << L" " << Builtins::RESOURCE << L"=\"#node_" << XMLizeName( (*i)->getLabel() ) << L"\" />" << endl;
          for ( vector< const Node* >::const_iterator j = ancestors.begin(); j != ancestors.end(); ++j){
	    out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_LINEAGE_NODE << L" " << Builtins::RESOURCE << L"=\"#node_" << XMLizeName( (*j)->getLabel() ) << L"\" />" << endl;
          }
          out << L"\t</" << NSDefs::CDAO << L":" << Classes::LINEAGE << L">" << endl;
        }
      }
    }
    return;
  }
  
  void writeState( wostream& out, const DataRepresentation* model, const unsigned int data_type, const int state_code){
    //tags used for state types.
       const unsigned int UKN_TYPE = 0;
    const unsigned int NUC_TYPE = 1;
     const unsigned int AA_TYPE  = 2;
    static wstring dataTypeTranslation[] = {Properties::HAS_STATE,
    					    Properties::HAS_NUC_STATE , 
    				            Properties::HAS_AA_STATE };
      if (model && model->getNTraits() && model->getNTax()){
      //select the appropriate tag.
      const wstring dtype_tag = dataTypeTranslation[ dataTypeTranslationPosition[ data_type ] ];
      //the wchar_tacter is a gap for this taxon.
      if ( model->isGap( state_code ) ){
        out << L"\t\t<"   << NSDefs::CDAO  << L":" << dtype_tag << L" " << Builtins::RESOURCE << L"=\"" << Imports::CDAO_DEF_URI << L"#"   << Classes::GAP << L"\"" <<  L" />" << endl;
      }
      //the wchar_tacter is missing for this taxon.
      else if( model->isMissing( state_code ) ){
        out << L"\t\t<"   << NSDefs::CDAO << L":" << dtype_tag << L" " << Builtins::RESOURCE << L"=\"" << Imports::CDAO_DEF_URI << L"#"   << Classes::ABSENT << L"\"" << L" />" << endl;
      }
      //reference the amino-acid or nucleotide instance.
      else {
        //cerr << L"Data type: L" << data_type << L" translation: L" << dataTypeTranslationPosition[ data_type ] << endl;
        if ( NUC_TYPE == dataTypeTranslationPosition[ data_type ] ){
	  out << L"\t\t<" << NSDefs::CDAO << L":" << dtype_tag << L" " << Builtins::RESOURCE << L"=\"" << Imports::CDAO_DEF_URI << L"#"  << L"d" << (wchar_t)towupper(state_code) << L"\"" << L" />" << endl;
        } 
        else if ( AA_TYPE == dataTypeTranslationPosition[ data_type ] ){
	  out << L"\t\t<" << NSDefs::CDAO << L":" << dtype_tag << L" " << Builtins::RESOURCE << L"=\"" << Imports::AMINO_ACID << L"#" << (wchar_t)towupper(state_code) << L"\"" << L" />" << endl;
        }
        else { 
          
          out << L"\t\t<" << NSDefs::CDAO << L":" << dtype_tag << L">" << state_code << L"</" << NSDefs::CDAO << L":" << dtype_tag << L">" << endl; 
        }
      }
    }
    return;
  }

  wstring chartacter_reference_string( const unsigned int trait){
    return  number_to_wstring( trait );
  }
  
  /*
   * Write the chartacter state modifications for each taxon, wchar_tacter.
   */
  void writeCharacterStateModifications( wostream& out, const DataRepresentation* model ){}

  /*
   * 
   */
  void writeTUS( wostream& out, const DataRepresentation* model){
    if ( model ){
      ProcessTUDelegate* del = new ProcessTUDelegate( out, model, writeTU);
     // ProcessTUDelegate* subTrees = new ProcessTUDelegate( out, model, writeSubtrees);

      if ( del /*&& subTrees*/ ){

        //write TU definitions
        for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
          out << L"\t<" << NSDefs::CDAO << L":" << Classes::TU << L" " << Builtins::ID << L"=\"" << L"taxon_" << taxon << L"\">" << endl;
          //reference the corresponding node.
          out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::REPRESENTED_BY_NODE << L" " << Builtins::RESOURCE << L"=\"#" << XMLizeName( model->getTaxonLabel( taxon )) << L"\" />" << endl;
          out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::BELONGS_TO_CSDM << L" " << Builtins::RESOURCE << L"=\"#" << XMLizeName( model->getMatrixLabel() ) << L"\" />" << endl;
          out << L"\t</" << NSDefs::CDAO << L":" << Classes::TU << L">" << endl;
        }
        //write the tree structure using the ProcessTUDelegate.
        for ( unsigned i = 0; i < model->getNumTrees(); ++i){
          const Node* tree = model->getParseTree( i );
          if ( tree ){
            tree->postOrderTraversal( del );
            //tree->postOrderTraversal( subTrees );
          }
        }
        delete del;
        //delete subTrees;
      }
    }
    return;
  }
  
  void writeSubtrees( wostream& out, const DataRepresentation* model, const Node* current){
    if ( model ){
      if ( current ){
    
        out << L"\t<" << NSDefs::CDAO << L":" << Classes::NODE_SET << L" " << Builtins::ID << L"=\"Subtree_" << XMLizeName( current->getLabel() ) << L"\">" << endl;
       vector< const Node* > elements = current->getAllDescendants();
       //elements.push_back( current );
       for (vector< const Node* >::const_iterator i = elements.begin(); i != elements.end(); ++i){
         out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_ELEMENT << L" " << Builtins::RESOURCE << L"=\"#" << XMLizeName( (*i)->getLabel() ) << L"\" />" << endl;
      }
       out << L"\t</" << NSDefs::CDAO << L":" << Classes::NODE_SET  << L">" << endl;
      }
    }
    return;
  }
  
  /*
   * Print a TU and it's edges
   */
  void writeTU( wostream& out, const DataRepresentation* model, const Node* current){
    if ( model && current ){
      //if the node has children it must be the MRCA of those children.
      static const wstring NODE_TYPE[] = {Classes::NODE, /*Classes::MRCA_NODE*/ Classes::NODE}; 
      if (current->getLabel() != L""){
        //write the edge for this TU

        writeEdge( out,  current->getAncestor(), current );

        //write the node definition.
        out << L"\t<" << NSDefs::CDAO << L":" <<  NODE_TYPE[ current->hasChildren() ]  << L" " 
	    << Builtins::ID << L"=\"node_" << XMLizeName( current->getLabel() )<< L"\">" << endl;
        if (model->getTaxonNumber( current->getLabel()) != NO_TAXON){
  	//connect the node to this corresponding TU.
  	  out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::REPRESENTS_TU << L" " 
	      << Builtins::RESOURCE << L"=\"#" << L"taxon_" << model->getTaxonNumber( current->getLabel() ) << L"\" />" << endl;
        } 
        //if the node has an ancestor connect the node to it.
        if (current->getAncestor()){
	  out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_PARENT << L" " 
	      << Builtins::RESOURCE << L"=\"#node_" << XMLizeName( current->getAncestor()->getLabel() )  << L"\" />" << endl;
	  out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::BELONGS_TO_EDGE_CH << L" "
	      << Builtins::RESOURCE << L"=\"#node_" << XMLizeName( current->getAncestor()->getLabel()  ) << L"_" << XMLizeName( current->getLabel() ) << L"\" />" << endl;
        } 
        //if the node has children connect it to them.
        if (current->hasChildren()){
	  writeChildConnections( out, current, current->getDescendants() );	
	  //write the nca info
	  //writeNcaInfo( out, current, current->getDescendants() );
	  //vector < const Node* > nca_of = current->getAllDescendants();	
        }
        else {
	//  out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::PART_OF << L" " 
	//      << Builtins::RESOURCE << L"=\"#Lineage_" << XMLizeName( current->getLabel() ) << L"\" />" << endl;
        }
        out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::PART_OF << L" " << Builtins::RESOURCE << L"=\"#" << XMLizeName( model->getTreeLabel( 0 ) ) << L"\" />" << endl;
        //end node definition.
        out << L"\t</" << NSDefs::CDAO << L":" << NODE_TYPE[ current->hasChildren() ] << L">" << endl;
      }
    }
    return;
  }
  //write edge definition.
  void writeEdge( wostream& out, const Node* parent, const Node* child){
    
    static set< wstring > edgesWritten = set< wstring >();
    
    if (parent && child ){
      wstring edge_name = XMLizeName( parent->getLabel() ) + L"_" + XMLizeName( child->getLabel() );
      

      if (find( edgesWritten.begin(), edgesWritten.end(), edge_name  ) == edgesWritten.end()){
        edgesWritten.insert( edge_name );
	//open definition.
	out << L"\t<" << NSDefs::CDAO << L":" << Classes::DIRECTED_EDGE << L" " 
	    << Builtins::ID << L"=\"" << edge_name  << L"\">" << endl;
	//connect the parent to the child
	out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_CHILD_NODE <<L" " << Builtins::RESOURCE << L"=\"#" << XMLizeName( child->getLabel()) << L"\" />" << endl;
	//connect the child to the parent.
	out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_PAR_NODE << L" " << Builtins::RESOURCE << L"=\"#" << XMLizeName( parent->getLabel() ) << L"\" />" << endl;
	//close definition
	out << L"\t</" << NSDefs::CDAO << L":" << Classes::DIRECTED_EDGE << L">" << endl;
      }
    }
    return;
  }

  void writeChildConnections( wostream& out, const Node* current,  const vector< const Node* >& children){
    
   if ( current ){

      //vector< const Node* > children = current->getDescendants();
      for (vector< const Node* >::const_iterator it = children.begin(); it != children.end(); ++it){
        out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::HAS_CHILD << L" " 
	    << Builtins::RESOURCE << L"=\"#" << XMLizeName( (*it)->getLabel() ) << L"\" />" << endl;
        out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::BELONGS_TO_EDGE_PR << L" "
	    << Builtins::RESOURCE << L"=\"#" << XMLizeName( current->getLabel() ) << L"_" <<  XMLizeName( (*it)->getLabel() ) << L"\" />" << endl; 
      
        //writeLineageConnections( out, current, (*it)->getLeaves( *it ) );
        //vector< const Node*> leaves = (*it)->getLeaves( *it );
      }
   }
    return;
  }

  void writeLineageConnections( wostream& out, const Node* current,  const vector< const Node* >& leaves ){
    if ( current ){
      for (vector< const Node* >::const_iterator i = leaves.begin(); i != leaves.end(); ++i){
        out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::PART_OF << L" " << Builtins::RESOURCE << L"=\"#Lineage_" << XMLizeName( (*i)->getLabel(  ) ) << L"\" />" << endl;    
      }
    }
    return;
  }
  
  void writeNcaInfo( wostream& out, const Node* current, const vector< const Node* >& nca_of ){
    if ( current ){
      
      set < const Node* > ncas_written = set< const Node* >();
      ncas_written.insert( current );
      out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::NCA_OF << L" " << Builtins::RESOURCE << L"=\"#Subtree_" << XMLizeName( current->getLabel() ) << L"\" />" << endl;
      for ( vector< const Node* >::const_iterator i = nca_of.begin(); i != nca_of.end(); ++i){
        for ( vector< const Node* >::const_iterator j = nca_of.begin(); j != nca_of.end(); ++j){
	  if ( *i != *j){
	    const Node* mrca_node = (*i)->getMrca( *j );
	    //cerr << L"Comparing: *i:" << (*i)->getLabel() << L" to:" << (*j)->getLabel() << endl;
	    if (  mrca_node && mrca_node == current && ncas_written.find( *i  ) == ncas_written.end() && ncas_written.find( *j ) == ncas_written.end() ){
	      //cerr << L"\tMrca is: L" << mrca_node->getLabel() << endl; 
	      out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::NCA_OF << L" " << Builtins::RESOURCE << L"=\"#Subtree_" << XMLizeName( (*i)->getLabel() ) << L"\" />" << endl;
	      out << L"\t\t<" << NSDefs::CDAO << L":" << Properties::NCA_OF << L" " << Builtins::RESOURCE << L"=\"#Subtree_" << XMLizeName( (*j)->getLabel() ) << L"\" />" << endl;
	      ncas_written.insert( *i );
	      ncas_written.insert( *j ); 
	    }
	  }
        }
      }
      
    }
    return;
  }
  
}
