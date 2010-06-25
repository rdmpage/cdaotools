#include "fasta_reader.hpp"
#include <ncl/ncl.h>
#include <ncl/nxsmultiformat.h>
#include <grammar/nexus/nexus_reader.hpp>
#include <vector>

using namespace CDAO;
using namespace std;

DataRepresentation* fastaparse(){
   DataRepresentation* ret = NULL;


    MultiFormatReader nread = MultiFormatReader( );

   NxsTaxaBlock* taxa               = new NxsTaxaBlock();
   NxsAssumptionsBlock* assumptions = new NxsAssumptionsBlock( taxa );
   NxsTreesBlock* trees             = new NxsTreesBlock( taxa );
   NxsCharactersBlock* wchar_tacters   = new NxsCharactersBlock( taxa, assumptions );
   NxsDataBlock* data               = new NxsDataBlock( taxa, assumptions );
   NxsDistancesBlock* distances     = new NxsDistancesBlock( taxa );

     //cerr << L"Initialized the blocks\n";

   NxsToken token( *(GlobalState::getInfile()) );

  //cerr << L"Initialized the token\n";

   nread.Add(taxa);
   nread.Add(assumptions);
   nread.Add(trees);
   nread.Add(wchar_tacters);
   nread.Add(data);
   nread.Add(distances);

   nread.ReadFilepath( GlobalState::getFileName(), GlobalState::getFormatDataType() );
   vector< const Node* > parsed_trees = vector< const Node* >();
   ret = new NexusDataRepresentation( parsed_trees, taxa, trees, assumptions, wchar_tacters, data, distances );
   return ret;
}
