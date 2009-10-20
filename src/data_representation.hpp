#ifndef DATA_REPRESENTATION_HPP_
#define DATA_REPRESENTATION_HPP_
#include <vector>
#include <string>
#include <cassert>
#include <ncl.h>
#include <climits>
#include "grammar/nexus/tree_description_parser.hpp"
#include "grammar/nexus/nexus_reader.hpp"

namespace CDAO {
  /*
   * NCL constant for the gap state.
   */
  const int NXS_GAP = -3;
  /*
   * NCL constant for the missing state.
   */
  const int NXS_MISSING = -2;

  const unsigned int NO_TAXON   = INT_MAX;
  /*
   * Data Representation provides the interface for obtaining
   * state information needed to built an Ontology representation 
   * of a data set.
   */
  class DataRepresentation {
  public:
    virtual ~DataRepresentation(){}
    /*
     * Get the tree topology.
     */
    virtual const Node* getParseTree()const=0;
    /*
     * Extract a particular taxon label.
     */
    virtual const std::string  getTaxonLabel( const unsigned int i )const=0;
    virtual const std::string  getTreeLabel( const unsigned int i)const=0;
    /*
     * Get the state of a character for the specified taxon.
     */
    virtual const char getTraitState( const unsigned int taxon, const unsigned int trait)const=0;
    /*
     * True if all the taxa agree on the specified characteristic, otherwise false.
     */
    virtual const bool isTrait( const unsigned int character )const=0;
    /*
     * Get the number of taxa in the dataset
     */
    virtual const unsigned int getNTax()const=0;
    /*
     * Get the number of traits in the dataset
     */
    virtual const unsigned int getNTraits()const=0;
    /*
     * Get the dataset's type.
     */
    virtual const unsigned int getDataType()const=0;
    /*
     * There is a gap for the given trait for some taxon.
     */
    virtual const bool hasGap( const unsigned int trait)const=0;
    /*
     * True if a gap was observed for a trait and taxon.
     */
    virtual const bool isGap(const unsigned int taxon, const unsigned int trait)const=0;
    /*
     * Find the node associated with the taxon number.
     */
    virtual  const Node* findNode( const unsigned int taxon  )const=0;
    /*
     * Find the taxon number associated with a name.
     */
    virtual const unsigned int getTaxonNumber( const string& label)const=0;

    virtual const bool isGap( const char ch )const=0;
    
    virtual const bool isMissing( const char ch )const=0;
  private:
  };
  /*
   * Concrete representation for the nexus data format.
   */
  class NexusDataRepresentation : public DataRepresentation {
  public:
    /*
     * Builds a nexus data representation.
     */
    NexusDataRepresentation(Node* parse_tree, 
			    NxsTaxaBlock* taxa, 
			    NxsTreesBlock* trees, 
			    NxsAssumptionsBlock* assumptions,
			    NxsCharactersBlock* characters, 
			    NxsDataBlock* data,
			    NxsDistancesBlock* distances):parse_tree_( parse_tree ),
							  taxa_( taxa ),
							  trees_( trees ),
							  assumptions_( assumptions ),
							  characters_( characters ),
							  data_( data ),
							  distances_( distances ){}
    /*
     * Tear down the nexus data representation.
     */
    virtual ~NexusDataRepresentation(){}
    /*
     * Retrieve the parse tree. 
     */
    virtual const Node* getParseTree()const{ return parse_tree_; }
    /*
     * Retrieve the taxon label
     */
    virtual const std::string getTaxonLabel( const unsigned int i)const{ 
      assert( taxa_ ); 
      //string label;
      //try { label =  return label; }
      //catch (NxsX_NoSuchTaxon e){
      //	cerr << "No Taxon: " << e.getContents() << "\n";
      //}
      return taxa_->GetTaxonLabel( i );
    }
    virtual const std::string  getTreeLabel( const unsigned int i)const{
      assert(  trees_  );
      return trees_->GetTreeName( i );
    }
    /*
     * Get the state of a particular trait for a particular taxon.
     */
    virtual const char getTraitState( const unsigned int taxon, const unsigned int trait)const;
    /*
     * True if all taxa have the same state for the given trait.
     */
    virtual const bool  isTrait( const unsigned int character)const{ assert(characters_ && !characters_->IsEmpty() ); return characters_->GetObsNumStates( character ) == 1;  }
    /*
     * Returns the number of taxa in the set.
     */
    virtual const unsigned int getNTax()const{ assert( taxa_ && !taxa_->IsEmpty() ); return taxa_->GetNumTaxonLabels(); }
    /*
     * Returns the number of characters/traits in the set.
     */
    virtual const unsigned int getNTraits()const{ assert( characters_ ); return  characters_->IsEmpty()? 0 : characters_->GetNumMatrixCols(); }
    /*
     * Returns the datatype of the set.
     */
    virtual const unsigned int getDataType()const{ assert( characters_ ); return characters_->IsEmpty()? 0: characters_->GetDataType();  }
    /*
     * True if some taxon has a gap for the specified trait.
     */
    virtual const bool hasGap( const unsigned int trait)const;
    /*
     * True of the specified taxon and traic is in a gap state.
     */
    virtual const bool isGap( const unsigned int taxon, const unsigned int trait)const{ 
      assert( characters_ && !characters_->IsEmpty() ); 
      return NXS_GAP == characters_->GetInternalRepresentation( taxon, trait, 0);  
    }
    /*
     * Finds the node associated with the specifed taxon number. 
     */
    virtual const Node* findNode( const unsigned int taxon  )const;
    /*
     * Finds the taxon number associated with the specified label.
     */
    virtual const unsigned int getTaxonNumber( const string& label)const{ 
      assert( taxa_ && !taxa_->IsEmpty() ); 
      int ret;
      try {
	ret = taxa_->FindTaxon( NxsString(label.c_str()) ); 
	return ret;
      } catch (NxsTaxaBlock::NxsX_NoSuchTaxon e){
	//cerr << "No taxon: " << label << "\n";
	return NO_TAXON;
	//exit ( 1 );
      }
    }
    virtual const bool isGap( const char ch )const{ return characters_->IsEmpty() ? false : characters_->GetGapSymbol() == ch; }
    
    virtual const bool isMissing( const char ch )const{ return characters_->IsEmpty()? false : characters_->GetMissingSymbol() == ch; }
  private:
    const Node* findNode(const string& key, const Node* current )const;
    Node* parse_tree_;
    NxsTaxaBlock* taxa_;
    NxsTreesBlock* trees_;
    NxsAssumptionsBlock* assumptions_;
    NxsCharactersBlock* characters_;
    NxsDataBlock* data_;
    NxsDistancesBlock* distances_;
    
  };
}
#endif
