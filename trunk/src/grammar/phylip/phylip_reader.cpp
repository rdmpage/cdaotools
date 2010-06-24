#include "phylip_reader.hpp"
#include <cassert>
#include <fstream>
#include <Logger.hpp>
#include <LogManager.hpp>
#include <util.hpp>
#include <ncl/ncl.h>
#include <ncl/nxsmultiformat.h>


using namespace std;
using namespace CDAO;

PhylipDataRepresentation::PhylipDataRepresentation( vector< wstring > matrix_data, vector< wstring > labels ):DataRepresentation(), matrix_data_( matrix_data ), taxon_labels_( labels ){}

PhylipDataRepresentation::~PhylipDataRepresentation(){
  //ntax_ = 0;
  //ntraits_ = 0;
  matrix_data_.empty();
}

static unsigned  getntaxtrait( wistream& in );
static wstring gettaxon( wistream& in );
static wstring getdataline( wistream& in, unsigned ntraits );

static void readinterleaved( wistream& in, vector< wstring >& taxons, vector< wstring >& data, unsigned ntax, unsigned ntraits  );
static void readstandard( wistream& in, vector< wstring >& taxons, vector< wstring >& data, unsigned ntax, unsigned ntraits  );

static const int TAXON_LABEL_SIZE = 10;

namespace CDAO {
  DataRepresentation* phylipparse(){
   DataRepresentation* model=NULL;
   
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
  
    model = new NexusDataRepresentation( parsed_trees, taxa, trees, assumptions, wchar_tacters, data, distances );
    return model;
  }
}

void readinterleaved( wistream& in, vector< wstring >& taxons, vector< wstring >& data, unsigned ntax, unsigned ntraits  ){
    unsigned current_taxon = 0;
    while (!in.eof()){
         ++current_taxon;
         if (taxons.size() < ntax){
               taxons.push_back( gettaxon( in ) );
               data.push_back( getdataline( in, ntraits ) );
         }
         else {
               data[ current_taxon % ntax ] += getdataline( in, ntraits );
         }


    }
}

void readstandard( wistream& in, vector< wstring >& taxons, vector< wstring >& data, unsigned ntax, unsigned ntraits ){

    wstring taxon;
    wstring data_line;
    wchar_t new_line_wchar_t;
    for (unsigned tax =0; tax < ntax; ++tax){
        taxon = gettaxon( in );
        data_line = getdataline( in, ntraits );
        taxons.push_back( taxon );
        data.push_back( data_line );
        in.get( new_line_wchar_t );
    }
    return;
}



unsigned getntaxtrait( wistream& in ){
   unsigned ret = 0;
   in >> ret;
   return ret;
}

wstring gettaxon( wistream& in ){
  wstring ret;
  wchar_t buff[ TAXON_LABEL_SIZE ];

  in.get( buff, TAXON_LABEL_SIZE );
  ret = buff ;

  return ret;
}

wstring getdataline( wistream& in, unsigned ntraits ){
   wstring ret;
   wchar_t buff[ ntraits ];

   in.getline( buff, ntraits );
   ret = buff ;

   return ret;
}

const unsigned int PhylipDataRepresentation::getTaxonNumber( const wstring& label )const{
    unsigned int ret = 0;
    for (; ret < taxon_labels_.size() && taxon_labels_.at( ret ) != label; ++ret);
    return ret;
}
