#include "data_representation.hpp"

namespace CDAO {
  /*
   * Extract the trait state for the given trand and taxon.
   */
  const char NexusDataRepresentation::getTraitState( const unsigned int taxon, const unsigned int trait)const{
    char ret = '?';
    if ( characters_ && !characters_->IsEmpty() ){ 
       int state = characters_->GetInternalRepresentation( taxon, trait, 0  );
       if (NXS_GAP == state){
         ret = characters_->GetGapSymbol();
       }
       else if (NXS_MISSING == state){
         ret = characters_->GetMissingSymbol();
       }
       else {
         ret = characters_->GetState( taxon, trait, 0);
       }
    }
    return ret;
    
  }
  /*
   * True if a gap has been observed for some taxon for the given trait.
   */
  const bool NexusDataRepresentation::hasGap( const unsigned int trait)const{
    if ( characters_ && !characters_->IsEmpty() && trait < this->getNTraits() ){
      for ( unsigned int taxon = 0; taxon < this->getNTax(); ++taxon ){
        if ( characters_->GetInternalRepresentation(taxon, trait, 0) == NXS_GAP  ){
	  return true;
        }
      }
    }
    return false;
  }
  /*
   * Find the node corresponding to the given taxon number.
   */
  const Node* NexusDataRepresentation::findNode( const unsigned int taxon, const unsigned int tree  )const{
    assert ( taxa_ );
    string label = taxa_->GetTaxonLabel( taxon );
    
    return findNode(label, parse_tree_.at( tree  ));
    
  }
  /*
   * Finds the given key in the taxon tree (if it exists).
   * Pre: the tree has already been parsed
   * Post: The node will have been found if it is in the tree.
   * Cost: O(n)
   */
  const Node* NexusDataRepresentation::findNode(const  string& key, const Node* current )const{
    const Node* ret = NULL;
  if ( current->getLabel() == key){ return current; }
  else {
    const vector<const Node* > children = current->getDescendants();
    for (unsigned int i = 0; i < children.size() && !( ret = findNode( key, children.at( i ) )); ++i );
  }
  return ret;
  }
  
}
