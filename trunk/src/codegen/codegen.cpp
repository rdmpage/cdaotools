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
  
  //static const string CSDM_ANNOTATION_ID = "csdm_annotation_id";
  //static const string CSDM_ID            = "csdm_id";
  //converts the data type code into unknown, nucleotide, or amino acid
  static int dataTypeTranslationPosition[] = { 0,0,1,1,1,2 };
  /*
   * Defines an easier to use short-hand for tu operations.
   */
  typedef void (*tu_op_t)(ostream&, const DataRepresentation*, const Node*);
  typedef void (*matrix_op_t)(ostream&, const DataRepresentation*);
  
  typedef void (CodeGenerator::*mem_matrix_op_t)( ostream& out );
  typedef void (CodeGenerator::*mem_tu_op_t)( ostream& out, const Node* );
  
  
  /*
   * Provides  a closure like construct for processing TU's
   */
  class ProcessTUDelegate : public Node::Delegate {
  public:
    /*
     * Initialized the saved part of the state.
     */
    ProcessTUDelegate(ostream& out, const DataRepresentation* model, tu_op_t operation):out_(out),
										   model_(model),
										   operation_(operation){}
    
    virtual ~ProcessTUDelegate(){}
    /*
     * Do the specified operation.
     */
    virtual void operator()(const Node* in){ (*operation_)( out_, model_, in); }
  private:
    ostream& out_;
    const DataRepresentation* model_;
    tu_op_t operation_;
  };
  
  
  string XMLizeName(const string& in);
 
  
  /*
   * Write basic document header info.
   */
  void writeHeaderBoilerPlate( ostream& out, const DataRepresentation* model );
  /*
   * Write document closing info.  
   */
  void writeClosing( ostream& out, const DataRepresentation* model );
  /*
   * Write characters to the specifed stream.
   */
  void writeCharacters( ostream& out, const DataRepresentation* model);
  /*
   * Create character state matrix annotation.
   */
  void writeCharacterStateMatrixAnnotation(ostream& out, const DataRepresentation* model);
  
  /*
   * Write the character state matrix definition.
   */
  void writeCharacterStateMatrix(ostream& out, const  DataRepresentation* model);
  /*
   * Writes the states of each character for each taxon.
   */
  void writeCharacterStates( ostream& out, const  DataRepresentation* model);
  /*
   */
  void writeTree( ostream& out, const DataRepresentation* model);
  void writeLineages( ostream& out, const DataRepresentation* model);
  /*
   * Write a single state instance
   */
  void writeState( ostream& out, const  DataRepresentation* model , const unsigned int data_type, const int state_code);
  /*
   * Generate TU instances
   */
  void writeTUS( ostream& out, const DataRepresentation* model);
  /**
   */
  void writeSubtrees( ostream& out, const DataRepresentation* model, const Node* current);
  /*
   * Generate TU instance.
   */
  void writeTU( ostream& out, const DataRepresentation* model, const Node* current);
  /*
   * Write an Edge instance
   */
  void writeEdge( ostream& out, const  Node* parent, const  Node* child);
  /*
   * Write each character state modification.
   */
  void writeCharacterStateModifications( ostream& out, const DataRepresentation* model );
  
  void writeChildConnections( ostream& out, const Node* current,  const vector< const Node* >& children);
  
  void writeLineageConnections( ostream& out, const Node* current,  const vector< const Node* >& leaves );
  
  void writeNcaInfo( ostream& out,const Node* current,  const vector< const Node* >& allDescendants );
  
  /*
   * Write ontology to the specified stream.
   */
  void CodeGenerator::generate( ostream& out ){
    
    if ( data_model_ ){
      /* Setup order writing will happen and which functions will do it. */
      matrix_op_t writers[] = {
                              writeHeaderBoilerPlate, 
                              writeTree,
  			      writeLineages,
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
  
  string XMLizeName(const string& in){
    string ret = "";
    for (unsigned int i = 0; i < in.size(); ++i){
      if (isalnum(in.at(i))){ ret+= in.at(i);  }
      else { ret += "_"; }
    }
    return ret;
  }

  /*
   * Return a standardized reference to a character. 
   */
  string character_reference_string(  unsigned int trait );
  /*
   * Write basic header information.
   */
  void writeHeaderBoilerPlate( ostream& out, const DataRepresentation* model ){
    out << "<?xml version=\"1.0\"?>" << endl << endl << endl
	<< "<!DOCTYPE "  << NSDefs::RDF      << ":RDF [" << endl
	<< "\t<!ENTITY " << NSDefs::OWL      << " \""    << Imports::OWL_URI << "#\">" << endl
        << "\t<!ENTITY " << NSDefs::OWL11    << " \""    << Imports::OWL11_URI  << "#\">" <<endl
        << "\t<!ENTITY " << NSDefs::DC       << " \""    << Imports::DC         << "#\">" << endl
      	<< "\t<!ENTITY " << NSDefs::XSD      << " \""    << Imports::XSD_URI  <<"#\">" << endl
      	<< "\t<!ENTITY " << NSDefs::OWL11XML << " \""    << Imports::OWL11XML_URI  <<"#\">" <<endl
	<< "\t<!ENTITY " << NSDefs::RDFS     << " \""    << Imports::RDFS_URI << "#\">" << endl
	<< "\t<!ENTITY " << NSDefs::RDF      << " \""    << Imports::RDF_URI  <<"#\">"  <<endl
	<< "\t<!ENTITY " << NSDefs::CDAO     << " \""    << Imports::CDAO_DEF_URI << "#\">" << endl
	<< "\t<!ENTITY " << NSDefs::AMINO_ACID << " \""  << Imports::AMINO_ACID << "#\">" << endl
	<< "]>" << endl << endl << endl;
    
    out << "<"        << Builtins::RDF    <<" xmlns=\"" << Imports::BASE_URI_PREFIX << model->getMatrixLabel()   << "#\"" << endl
	<< "\txml:"   <<  NSDefs::BASE     <<"=\""      << Imports::BASE_URI_PREFIX << model->getMatrixLabel()        << "#\"" << endl
	<< "\txmlns:" << NSDefs::OWL11    <<"=\""      << Imports::OWL11_URI       << "#\"" << endl
	<< "\txmlns:" << NSDefs::OWL11XML <<"=\""      << Imports::OWL11XML_URI    << "#\"" << endl
	<< "\txmlns:" << NSDefs::XSD      <<"=\""      << Imports::XSD_URI         << "#\""  << endl
	<< "\txmlns:" << NSDefs::RDFS     <<"=\""      << Imports::RDFS_URI        << "#\""  << endl
	<< "\txmlns:" << NSDefs::RDF      <<"=\""      << Imports::RDF_URI         << "#\""  << endl
	<< "\txmlns:" << NSDefs::OWL      <<"=\""      << Imports::OWL_URI         << "#\""  << endl
	<< "\txmlns:" << NSDefs::CDAO     <<"=\""      << Imports::CDAO_DEF_URI    << "#\""  << endl
      
	<< "\txmlns:" << NSDefs::AMINO_ACID << "=\""   << Imports::AMINO_ACID      <<"#\">" << endl;
    out << "\t<"   << Builtins::ONT     <<  " " << Builtins::ABOUT  <<"=\"" << Imports::CDAO_DEF_URI  <<"\">" << endl
	<< "\t\t<" << Builtins::IMPORTS <<  " " << Builtins::RESOURCE <<"=\"" << Imports::CDAO_DEF_URI  <<  "\"/>" << endl
	<< "\t</"  << Builtins::ONT     <<">"   << endl;
  }
  /*
   * Write basic closing information.
   */
  void writeClosing( ostream& out, const DataRepresentation* model ){ 
    out << "</" << Builtins::RDF  <<  ">" << endl;
    return;
  }
  /*
   * Write characters representation.
   */
  void writeCharacters( ostream& out, const DataRepresentation* model){
    if ( model && model->getNTraits() ){
      //character tags.
      static const string dataTypeTranslation[] = {NSDefs::CDAO + ":" + Classes::UKN,
  					   NSDefs::CDAO + ":" + Classes::NUC_CHARACTER , 
  					   NSDefs::CDAO + ":" + Classes::AA_CHARACTER};
      //the particular tag corresponding to the set's datatype.
      const string dtype_tag = dataTypeTranslation[ dataTypeTranslationPosition[ model->getDataType() ] ];
      //write each character for each taxon.
      for (unsigned int trait = 0; trait < model->getNTraits(); ++trait){
        out << "\t<" << dtype_tag << " " << Builtins::ID << "=\"" <<  "trait_" << trait << "\">" << endl;
        for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
	  out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_DATUM << " " << Builtins::RESOURCE << "=\"#trait_" << trait << "_taxon_" << taxon << "\" />" << endl;  
        }
        out << "\t\t<" << NSDefs::CDAO << ":" << Properties::BELONGS_TO_CSDM << " " << Builtins::RESOURCE << "=\"#" << XMLizeName(model->getMatrixLabel()) << "\" />" << endl;
        out << "\t</" << dtype_tag << ">" << endl;
      }
    }
    return;
  }
  //write character state matrix annotation definion.
  void writeCharacterStateMatrixAnnotation(ostream& out, const DataRepresentation* model){
    if ( model && model->getNTraits() ){
       out << "\t<" << NSDefs::CDAO <<":" << Classes::CSDM_ANNOTATION << " " << Builtins::ID << "=\"" << XMLizeName( model->getMatrixLabel() ) + "_annotation_id" << "\" />" << endl; 
    }
  }
  //write character state matrix definition.
  void writeCharacterStateMatrix(ostream& out, const DataRepresentation* model){
    if ( model && model->getNTraits() ){
       out << "\t<" << NSDefs::CDAO << ":" << Classes::CSDM << " " << Builtins::ID << "=\"" << XMLizeName( model->getMatrixLabel() ) << "\">" << endl;
       out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS << " " << Builtins::RESOURCE << "=\"#" <<XMLizeName( model->getMatrixLabel()) + "_annotation_id" << "\" />" << endl;
      //include references for each taxon in the dataset.
      for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
        out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_TU << " " << Builtins::RESOURCE << "=\"#" << "taxon_" << taxon << "\" />" << endl;
      }
      for (unsigned int trait = 0; trait < model->getNTraits(); ++trait){
        //  for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
        out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_CHARACTER << " " << Builtins::RESOURCE << "=\"#" << "trait_" << trait 
	  /*<< "_taxon_" << taxon */ << "\" />" << endl; 
	   //  }
      }
      out << "\t</" << NSDefs::CDAO << ":" << Classes::CSDM << ">" << endl;
    }
    return;
  }
  /*
   * Create a state instance for each taxon and trait.
   */
  void writeCharacterStates( ostream& out, const DataRepresentation* model){
    //tags used for character states.
    static string dataTypeTranslation[] = {NSDefs::CDAO + ":" + Classes::UKN,
					   NSDefs::CDAO + ":" + Classes::NUC_DATUM , 
					   NSDefs::CDAO + ":" + Classes::AA_DATUM };
    if ( model ){
      //selects the particular state tag needed for this set.
      const string dtype_tag = dataTypeTranslation[ dataTypeTranslationPosition[ model->getDataType() ] ];
      //write state for each trait and taxon.
      for (unsigned int trait = 0; trait < model->getNTraits(); ++trait){
        for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
	  out << "\t<" <<dtype_tag  << " " << Builtins::ID << "=\"" << "trait_" << trait << "_taxon_" << taxon << "\">" << endl;
	  //identify which TU this trait state belongs to.
	  out << "\t\t<" << NSDefs::CDAO << ":" << Properties::BELONGS_TO_TU << " " << Builtins::RESOURCE << "=\"#" << /*XMLizeName( model->getTaxonLabel( taxon ))*/ "taxon_" << taxon << "\" />" << endl;
	  out << "\t\t<" << NSDefs::CDAO << ":" << Properties::PART_OF << " " << Builtins::RESOURCE << "=\"#" << /*XMLizeName( model->getTaxonLabel( taxon ))*/ "trait_" << trait << "\" />" << endl;
	  //write the state value.
	  writeState( out, model,  model->getDataType(),  model->getTraitState( taxon, trait ) );
	  out << "\t</" << dtype_tag << ">" << endl;
        }
      }
    }
  }
  /*
   */
  void writeTree( ostream& out, const DataRepresentation* model){
    if ( model ){
      for (unsigned i = 0; i < model->getNumTrees(); ++i ){
          out << "<" << NSDefs::CDAO << ":" << Classes::TREE << " " << Builtins::ID << "=\"" << XMLizeName( model->getTreeLabel( i ) ) << "\" />" << endl;
      }
    }
    return;
  }
  
  void writeLineages( ostream& out, const DataRepresentation* model){
    if( model ){
      for (unsigned int tree = 0; tree < model->getNumTrees(); ++tree){
        vector< const Node* > leaves = model->getParseTree( tree )->getLeaves( model->getParseTree( tree ) );
        for (vector< const Node* >::const_iterator i = leaves.begin(); i != leaves.end(); ++i){
          out << "\t<" << NSDefs::CDAO << ":" << Classes::LINEAGE << " " << Builtins::ID << "=\"Lineage_" << XMLizeName( (*i)->getLabel() ) << "\">" << endl;
          out << "\t\t<" << NSDefs::CDAO << ":" << Properties::SUBTREE_OF << " " << Builtins::RESOURCE << "=\"#" << XMLizeName( model->getTreeLabel( 0 ) ) << "\" />" << endl;
          vector< const Node* > ancestors = (*i)->getAncestors();
          //add the current node to it's own lineage
          out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_LINEAGE_NODE << " " << Builtins::RESOURCE << "=\"#" << XMLizeName( (*i)->getLabel() ) << "\" />" << endl;
          for ( vector< const Node* >::const_iterator j = ancestors.begin(); j != ancestors.end(); ++j){
	    out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_LINEAGE_NODE << " " << Builtins::RESOURCE << "=\"#" << XMLizeName( (*j)->getLabel() ) << "\" />" << endl;
          }
          out << "\t</" << NSDefs::CDAO << ":" << Classes::LINEAGE << ">" << endl;
        }
      }
    }
    return;
  }
  
  void writeState( ostream& out, const DataRepresentation* model, const unsigned int data_type, const int state_code){
    //tags used for state types.
       const unsigned int UKN_TYPE = 0;
    const unsigned int NUC_TYPE = 1;
     const unsigned int AA_TYPE  = 2;
    static string dataTypeTranslation[] = {Properties::HAS_STATE,
    					    Properties::HAS_NUC_STATE , 
    				            Properties::HAS_AA_STATE };
      if (model && model->getNTraits() && model->getNTax()){
      //select the appropriate tag.
      const string dtype_tag = dataTypeTranslation[ dataTypeTranslationPosition[ data_type ] ];
      //the character is a gap for this taxon.
      if ( model->isGap( state_code ) ){
        out << "\t\t<"   << NSDefs::CDAO  << ":" << dtype_tag << " " << Builtins::RESOURCE << "=\"" << Imports::CDAO_DEF_URI << "#"   << Classes::GAP << "\"" <<  " />" << endl;
      }
      //the character is missing for this taxon.
      else if( model->isMissing( state_code ) ){
        out << "\t\t<"   << NSDefs::CDAO << ":" << dtype_tag << " " << Builtins::RESOURCE << "=\"" << Imports::CDAO_DEF_URI << "#"   << Classes::ABSENT << "\"" << " />" << endl;
      }
      //reference the amino-acid or nucleotide instance.
      else {
        //cerr << "Data type: " << data_type << " translation: " << dataTypeTranslationPosition[ data_type ] << endl;
        if ( NUC_TYPE == dataTypeTranslationPosition[ data_type ] ){
	  out << "\t\t<" << NSDefs::CDAO << ":" << dtype_tag << " " << Builtins::RESOURCE << "=\"" << Imports::CDAO_DEF_URI << "#"  << "d" << (char)toupper(state_code) << "\"" << " />" << endl;
        } 
        else if ( AA_TYPE == dataTypeTranslationPosition[ data_type ] ){
	  out << "\t\t<" << NSDefs::CDAO << ":" << dtype_tag << " " << Builtins::RESOURCE << "=\"" << Imports::AMINO_ACID << "#" << (char)toupper(state_code) << "\"" << " />" << endl;
        }
        else { 
          
          out << "\t\t<" << NSDefs::CDAO << ":" << dtype_tag << ">" << state_code << "</" << NSDefs::CDAO << ":" << dtype_tag << ">" << endl; 
        }
      }
    }
    return;
  }

  string character_reference_string( const unsigned int trait){
    return  number_to_string( trait );
  }
  
  /*
   * Write the character state modifications for each taxon, character.
   */
  void writeCharacterStateModifications( ostream& out, const DataRepresentation* model ){}

  /*
   * 
   */
  void writeTUS( ostream& out, const DataRepresentation* model){
    if ( model ){
      ProcessTUDelegate* del = new ProcessTUDelegate( out, model, writeTU);
      ProcessTUDelegate* subTrees = new ProcessTUDelegate( out, model, writeSubtrees);

      if ( del && subTrees ){

        //write TU definitions
        for (unsigned int taxon = 0; taxon < model->getNTax(); ++taxon){
          out << "\t<" << NSDefs::CDAO << ":" << Classes::TU << " " << Builtins::ID << "=\"" << "taxon_" << taxon << "\">" << endl;
          //reference the corresponding node.
          out << "\t\t<" << NSDefs::CDAO << ":" << Properties::REPRESENTED_BY_NODE << " " << Builtins::RESOURCE << "=\"#" << XMLizeName( model->getTaxonLabel( taxon )) << "\" />" << endl;
          out << "\t\t<" << NSDefs::CDAO << ":" << Properties::BELONGS_TO_CSDM << " " << Builtins::RESOURCE << "=\"#" << XMLizeName( model->getMatrixLabel() ) << "\" />" << endl;
          out << "\t</" << NSDefs::CDAO << ":" << Classes::TU << ">" << endl;
        }
        //write the tree structure using the ProcessTUDelegate.
        for ( unsigned i = 0; i < model->getNumTrees(); ++i){
          const Node* tree = model->getParseTree( i );
          if ( tree ){
            tree->postOrderTraversal( del );
            tree->postOrderTraversal( subTrees );
          }
        }
        delete del;
        delete subTrees;
      }
    }
    return;
  }
  
  void writeSubtrees( ostream& out, const DataRepresentation* model, const Node* current){
    if ( model ){
      if ( current ){
    
        out << "\t<" << NSDefs::CDAO << ":" << Classes::NODE_SET << " " << Builtins::ID << "=\"Subtree_" << XMLizeName( current->getLabel() ) << "\">" << endl;
       vector< const Node* > elements = current->getAllDescendants();
       //elements.push_back( current );
       for (vector< const Node* >::const_iterator i = elements.begin(); i != elements.end(); ++i){
         out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_ELEMENT << " " << Builtins::RESOURCE << "=\"#" << XMLizeName( (*i)->getLabel() ) << "\" />" << endl;
      }
       out << "\t</" << NSDefs::CDAO << ":" << Classes::NODE_SET  << ">" << endl;
      }
    }
    return;
  }
  
  /*
   * Print a TU and it's edges
   */
  void writeTU( ostream& out, const DataRepresentation* model, const Node* current){
    if ( model && current ){
      //if the node has children it must be the MRCA of those children.
      static const string NODE_TYPE[] = {Classes::NODE, Classes::MRCA_NODE}; 
      if (current->getLabel() != ""){
        //write the edge for this TU

        writeEdge( out,  current->getAncestor(), current );

        //write the node definition.
        out << "\t<" << NSDefs::CDAO << ":" <<  NODE_TYPE[ current->hasChildren() ]  << " " 
	    << Builtins::ID << "=\"" << XMLizeName( current->getLabel() )<< "\">" << endl;
        if (model->getTaxonNumber( current->getLabel()) != NO_TAXON){
  	//connect the node to this corresponding TU.
  	  out << "\t\t<" << NSDefs::CDAO << ":" << Properties::REPRESENTS_TU << " " 
	      << Builtins::RESOURCE << "=\"#" << "taxon_" << model->getTaxonNumber( current->getLabel() ) << "\" />" << endl;
        } 
        //if the node has an ancestor connect the node to it.
        if (current->getAncestor()){
	  out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_PARENT << " " 
	      << Builtins::RESOURCE << "=\"#" << XMLizeName( current->getAncestor()->getLabel() )  << "\" />" << endl;
	  out << "\t\t<" << NSDefs::CDAO << ":" << Properties::BELONGS_TO_EDGE_CH << " "
	      << Builtins::RESOURCE << "=\"#" << XMLizeName( current->getAncestor()->getLabel()  ) << "_" << XMLizeName( current->getLabel() ) << "\" />" << endl;
        } 
        //if the node has children connect it to them.
        if (current->hasChildren()){
	  writeChildConnections( out, current, current->getDescendants() );	
	  //write the nca info
	  writeNcaInfo( out, current, current->getDescendants() );
	  //vector < const Node* > nca_of = current->getAllDescendants();	
        }
        else {
	  out << "\t\t<" << NSDefs::CDAO << ":" << Properties::PART_OF << " " 
	      << Builtins::RESOURCE << "=\"#Lineage_" << XMLizeName( current->getLabel() ) << "\" />" << endl;
        }
        out << "\t\t<" << NSDefs::CDAO << ":" << Properties::PART_OF << " " << Builtins::RESOURCE << "=\"#" << XMLizeName( model->getTreeLabel( 0 ) ) << "\" />" << endl;
        //end node definition.
        out << "\t</" << NSDefs::CDAO << ":" << NODE_TYPE[ current->hasChildren() ] << ">" << endl;
      }
    }
    return;
  }
  //write edge definition.
  void writeEdge( ostream& out, const Node* parent, const Node* child){
    
    static vector< string > edgesWritten = vector< string >();
    
    if (parent && child ){
      string edge_name = XMLizeName( parent->getLabel() ) + "_" + XMLizeName( child->getLabel() );
      

      if (find( edgesWritten.begin(), edgesWritten.end(), edge_name  ) == edgesWritten.end()){
      
	//open definition.
	out << "\t<" << NSDefs::CDAO << ":" << Classes::DIRECTED_EDGE << " " 
	    << Builtins::ID << "=\"" << edge_name  << "\">" << endl;
	//connect the parent to the child
	out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_CHILD_NODE <<" " << Builtins::RESOURCE << "=\"#" << XMLizeName( child->getLabel()) << "\" />" << endl;
	//connect the child to the parent.
	out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_PAR_NODE << " " << Builtins::RESOURCE << "=\"#" << XMLizeName( parent->getLabel() ) << "\" />" << endl;
	//close definition
	out << "\t</" << NSDefs::CDAO << ":" << Classes::DIRECTED_EDGE << ">" << endl;
      }
    }
    return;
  }

  void writeChildConnections( ostream& out, const Node* current,  const vector< const Node* >& children){
    
   if ( current ){

      //vector< const Node* > children = current->getDescendants();
      for (vector< const Node* >::const_iterator it = children.begin(); it != children.end(); ++it){
        out << "\t\t<" << NSDefs::CDAO << ":" << Properties::HAS_CHILD << " " 
	    << Builtins::RESOURCE << "=\"#" << XMLizeName( (*it)->getLabel() ) << "\" />" << endl;
        out << "\t\t<" << NSDefs::CDAO << ":" << Properties::BELONGS_TO_EDGE_PR << " "
	    << Builtins::RESOURCE << "=\"#" << XMLizeName( current->getLabel() ) << "_" <<  XMLizeName( (*it)->getLabel() ) << "\" />" << endl; 
      
        //writeLineageConnections( out, current, (*it)->getLeaves( *it ) );
        //vector< const Node*> leaves = (*it)->getLeaves( *it );
      }
   }
    return;
  }

  void writeLineageConnections( ostream& out, const Node* current,  const vector< const Node* >& leaves ){
    if ( current ){
      for (vector< const Node* >::const_iterator i = leaves.begin(); i != leaves.end(); ++i){
        out << "\t\t<" << NSDefs::CDAO << ":" << Properties::PART_OF << " " << Builtins::RESOURCE << "=\"#Lineage_" << XMLizeName( (*i)->getLabel(  ) ) << "\" />" << endl;    
      }
    }
    return;
  }
  
  void writeNcaInfo( ostream& out, const Node* current, const vector< const Node* >& nca_of ){
    if ( current ){
      set < const Node* > ncas_written = set< const Node* >();
      ncas_written.insert( current );
      out << "\t\t<" << NSDefs::CDAO << ":" << Properties::NCA_OF << " " << Builtins::RESOURCE << "=\"#Subtree_" << XMLizeName( current->getLabel() ) << "\" />" << endl;
      for ( vector< const Node* >::const_iterator i = nca_of.begin(); i != nca_of.end(); ++i){
        for ( vector< const Node* >::const_iterator j = nca_of.begin(); j != nca_of.end(); ++j){
	  if ( *i != *j){
	    const Node* mrca_node = (*i)->getMrca( *j );
	    //cerr << "Comparing: *i:" << (*i)->getLabel() << " to:" << (*j)->getLabel() << endl;
	    if (  mrca_node && mrca_node == current && ncas_written.find( *i  ) == ncas_written.end() && ncas_written.find( *j ) == ncas_written.end() ){
	      //cerr << "\tMrca is: " << mrca_node->getLabel() << endl; 
	      out << "\t\t<" << NSDefs::CDAO << ":" << Properties::NCA_OF << " " << Builtins::RESOURCE << "=\"#Subtree_" << XMLizeName( (*i)->getLabel() ) << "\" />" << endl;
	      out << "\t\t<" << NSDefs::CDAO << ":" << Properties::NCA_OF << " " << Builtins::RESOURCE << "=\"#Subtree_" << XMLizeName( (*j)->getLabel() ) << "\" />" << endl;
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
