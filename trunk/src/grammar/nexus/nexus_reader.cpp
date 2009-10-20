#include "nexus_reader.hpp"
#include <iostream>
#include "tree_description_parser.hpp"

using namespace std;
namespace CDAO {
  istream* NexusState::in_ = NULL;
  ostream* NexusState::out_ = NULL;
  ostream* NexusState::err_ = &std::cerr;

  DataRepresentation*  nexusparse(){

  //cerr << "nexusparse()\n";
    OntNexusReader nread(  *(NexusState::getInfile()), *(NexusState::getOutfile()), *(NexusState::getErrorfile()) );
  //cerr << "Initialized the reader\n";
  NxsTaxaBlock* taxa               = new NxsTaxaBlock();
  NxsAssumptionsBlock* assumptions = new NxsAssumptionsBlock( taxa );
  NxsTreesBlock* trees             = new NxsTreesBlock( taxa );
  NxsCharactersBlock* characters   = new NxsCharactersBlock( taxa, assumptions );
  NxsDataBlock* data               = new NxsDataBlock( taxa, assumptions );
  NxsDistancesBlock* distances     = new NxsDistancesBlock( taxa );

  //cerr << "Initialized the blocks\n";

  NxsToken token( nread.inf );

  //cerr << "Initialized the token\n";

  nread.Add(taxa);
  nread.Add(assumptions);
  nread.Add(trees);
  nread.Add(characters);
  nread.Add(data);
  nread.Add(distances);
  
  //cerr << "Preparing to execute\n";

  nread.Execute( token );
  
  string tree_description = "";
  if ( ! trees->IsEmpty() ){
    if ( trees->GetNumTrees() > 0 ){
      tree_description  = trees->GetTreeDescription( 0 );
    }
    else {
      //cerr << "No Trees!" << endl;
    }
  }
  
  //cerr << "tree_description: " << tree_description << endl;
  
  TreeDescriptionParser treeParser( tree_description ); 
  
  NexusDataRepresentation* ret = new NexusDataRepresentation( treeParser.getParseTree(), taxa, trees, assumptions, characters, data, distances );

  //cerr << "Executed the read\n";

  //nread.Report( cout );
   
  // if (! taxa->IsEmpty()){
//     cerr << "Reporting Taxa:" << "\n";
// 	taxa->Report( cerr );
//   }
//   if (! trees->IsEmpty() ){
//     cerr << "Reporting Trees:" << "\n";
// 	trees->Report( cerr );
//   }
//   if (! characters->IsEmpty() ){
//     cerr << "Reporting Characters:" << "\n";
// 	characters->Report( cerr );

//   }
//   if (! distances->IsEmpty() ){
//     cerr << "Reporting Distances:" << "\n";
// 	distances->Report( cerr );
//   }
//   if (! assumptions->IsEmpty() ){
//     cerr << "Reporting Assumptions:" << "\n";
// 	assumptions->Report( cerr );
//   }
  
  //cout << "NTax: " << taxa->GetNumTaxonLabels() << "\n";
  //cout << "NTrees: " << trees->GetNumTrees() << "\n";
  // for (int i = 0; i < taxa->GetNumTaxonLabels(); ++i ){
//   	cout << "Taxon Label[ " << i << " ] " << taxa->GetTaxonLabel( i ) << "\n";
//   }

  //for (unsigned int i = 0; i < trees->GetNumTrees(); ++i){
  //    cout << "Name: " << trees->GetTreeName( i ) << "\n";
  //    cout << "Description: " << trees->GetTreeDescription( i ) << "\n";
  //    cout <<  trees->GetTreeDescription( i );
  //}


  //cerr << "exiting: nexusparse()\n";
  return ret;
}
}
