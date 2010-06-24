#include <fasta_reader.hpp>
#include <ncl/ncl.h>
#include <ncl/multiformat.h>
#include <vector>

using CDAO;
using std;

DataRepresentation* fastaparse(){
   DataRepresentation* ret = NULL;


    MultiFormatReader mfr = MultiFormatReader( );

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

   mfr.ReadFilePath( GlobalState::getFileName(), GlobalState::getDataFormatType() );
   vector< const Node* > parsed_trees = vector< const Node* >();
   for (vector< wstring >::iterator i = tree_description.begin(); i < tree_description.end(); ++i){
    
     TreeDescriptionParser  treeParser( *i );
     parsed_trees.push_back( treeParser.getParseTree() ); 
   }
  
   ret = new NexusDataRepresentation( parsed_trees, taxa, trees, assumptions, wchar_tacters, data, distances );
   return ret;
}
