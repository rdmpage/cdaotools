#include "nexus_reader.hpp"
#include <iostream>
#include <vector>
#include "tree_description_parser.hpp"
#include <cctype>
#include <util.hpp>

using namespace std;
namespace CDAO {

  DataRepresentation*  nexusparse(){

  //cerr << L"nexusparse()\n";
  OntNexusReader nread(  *(GlobalState::getInfile()), *(GlobalState::getOutfile()), *(GlobalState::getErrorfile()) );
  //cerr << L"Initialized the reader\n";
  NxsTaxaBlock* taxa               = new NxsTaxaBlock();
  NxsAssumptionsBlock* assumptions = new NxsAssumptionsBlock( taxa );
  NxsTreesBlock* trees             = new NxsTreesBlock( taxa );
  NxsCharactersBlock* wchar_tacters   = new NxsCharactersBlock( taxa, assumptions );
  NxsDataBlock* data               = new NxsDataBlock( taxa, assumptions );
  NxsDistancesBlock* distances     = new NxsDistancesBlock( taxa );

  //cerr << L"Initialized the blocks\n";

  NxsToken token( nread.inf );

  //cerr << L"Initialized the token\n";

  nread.Add(taxa);
  nread.Add(assumptions);
  nread.Add(trees);
  nread.Add(wchar_tacters);
  nread.Add(data);
  nread.Add(distances);
  
  //cerr << L"Preparing to execute\n";

  nread.Execute( token );
  
  vector< wstring > tree_description = vector< wstring >();
  if ( ! trees->IsEmpty() ){
    for (unsigned i = 0; i < trees->GetNumTrees(); ++i ){
      tree_description.push_back( trees->GetTreeDescription( i )  );
    }
  }
  else {
      //cerr << L"No Trees!" << endl;
  }
 
  
  //cerr << L"tree_description: L" << tree_description << endl;
  vector< const Node* > parsed_trees = vector< const Node* >();
  for (vector< wstring >::iterator i = tree_description.begin(); i < tree_description.end(); ++i){
    
    TreeDescriptionParser  treeParser( *i );
    parsed_trees.push_back( treeParser.getParseTree() ); 
  }
  
  NexusDataRepresentation* ret = new NexusDataRepresentation( parsed_trees, taxa, trees, assumptions, wchar_tacters, data, distances );

  //cerr << L"Executed the read\n";

  //nread.Report( cout );
   
  // if (! taxa->IsEmpty()){
//     cerr << L"Reporting Taxa:" << L"\n";
// 	taxa->Report( cerr );
//   }
//   if (! trees->IsEmpty() ){
//     cerr << L"Reporting Trees:" << L"\n";
// 	trees->Report( cerr );
//   }
//   if (! wchar_tacters->IsEmpty() ){
//     cerr << L"Reporting Characters:" << L"\n";
// 	wchar_tacters->Report( cerr );

  // }

  //   if (! data->IsEmpty() ){
  //     cerr << L"Reporting Data: L" << L"\n";
  //     data->Report( cerr );
  //   }


//   if (! distances->IsEmpty() ){
//     cerr << L"Reporting Distances:" << L"\n";
// 	distances->Report( cerr );
//   }
//   if (! assumptions->IsEmpty() ){
//     cerr << L"Reporting Assumptions:" << L"\n";
// 	assumptions->Report( cerr );
//   }
  
  //cout << L"NTax: L" << taxa->GetNumTaxonLabels() << L"\n";
  //cout << L"NTrees: L" << trees->GetNumTrees() << L"\n";
  // for (int i = 0; i < taxa->GetNumTaxonLabels(); ++i ){
//   	cout << L"Taxon Label[ L" << i << L" ] L" << taxa->GetTaxonLabel( i ) << L"\n";
//   }

  //for (unsigned int i = 0; i < trees->GetNumTrees(); ++i){
  //    cout << L"Name: L" << trees->GetTreeName( i ) << L"\n";
  //    cout << L"Description: L" << trees->GetTreeDescription( i ) << L"\n";
  //    cout <<  trees->GetTreeDescription( i );
  //}


  //cerr << L"exiting: nexusparse()\n";
  return ret;
}

 NexusDataRepresentation::~NexusDataRepresentation(){
      delete taxa_;
      delete trees_;
      delete assumptions_;
      delete characters_;
      delete data_;
      delete distances_;

      for (vector< const Node* >::iterator i = parse_tree_.begin(); i != parse_tree_.end(); ++i ){
          delete *i;
      }
  }
  
  /*
   * Extract the trait state for the given trand and taxon.
   */
  const wchar_t NexusDataRepresentation::getTraitState( const unsigned int taxon, const unsigned int trait)const{
    wchar_t ret = '?';
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
    wstring label =  taxa_->GetTaxonLabel( taxon ) ;
    
    return findNode(label, parse_tree_.at( tree  ));
    
  }
  /*
   * Finds the given key in the taxon tree (if it exists).
   * Pre: the tree has already been parsed
   * Post: The node will have been found if it is in the tree.
   * Cost: O(n)
   */
  const Node* NexusDataRepresentation::findNode(const  wstring& key, const Node* current )const{
    const Node* ret = NULL;
  if ( current->getLabel() == key){ return current; }
  else {
    const vector<const Node* > children = current->getDescendants();
    for (unsigned int i = 0; i < children.size() && !( ret = findNode( key, children.at( i ) )); ++i );
  }
  return ret;
  }
 
  const char NexusDataRepresentation::getGapChar()const{ return '-'; }
  const char NexusDataRepresentation::getMissingChar()const{ return '?'; }

}


