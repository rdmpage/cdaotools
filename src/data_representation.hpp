#ifndef DATA_REPRESENTATION_HPP_
#define DATA_REPRESENTATION_HPP_
#include <vector>
#include <string>
#include <cassert>
#include <ncl/ncl.h>
#include <climits>
#include "grammar/nexus/tree_description_parser.hpp"
//#include "grammar/nexus/nexus_reader.hpp"

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
    virtual const Node* getParseTree(unsigned int i)const=0;
    /*
     * Extract a particular taxon label.
     */
    virtual const std::wstring  getTaxonLabel( const unsigned int i )const=0;
    virtual const std::wstring  getTreeLabel( const unsigned int i)const=0;
    virtual const std::wstring  getMatrixLabel()const=0;
    virtual void setMatrixLabel(const std::wstring& )=0;
    /*
     * Get the state of a wchar_tacter for the specified taxon.
     */
    virtual const wchar_t getTraitState( const unsigned int taxon, const unsigned int trait)const=0;
    /*
     * True if all the taxa agree on the specified wchar_tacteristic, otherwise false.
     */
    virtual const bool isTrait( const unsigned int wchar_tacter )const=0;
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
    /**
     * Get number of trees
     */
    virtual const unsigned int getNumTrees()const=0;
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
    virtual  const Node* findNode( const unsigned int taxon, const unsigned int tree  )const=0;
    /*
     * Find the taxon number associated with a name.
     */
    virtual const unsigned int getTaxonNumber( const wstring& label)const=0;

    virtual const bool isGap( const wchar_t ch )const=0;
    
    virtual const bool isMissing( const wchar_t ch )const=0;
  private:
  };
}
#endif
