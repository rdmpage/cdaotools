#include "phylip_reader.hpp"
#include <cassert>
#include <fstream>
#include <Logger.hpp>
#include <LogManager.hpp>
#include <util.hpp>

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
   //if ( env ){
     vector< wstring > data = vector< wstring >();
     vector< wstring > taxons = vector< wstring >();
     unsigned ntax = getntaxtrait( *(GlobalState::getInfile()) );
     unsigned ntraits = getntaxtrait( *(GlobalState::getInfile()) );
     
     if ( GlobalState::isInterleaved() ){
         readinterleaved( *(GlobalState::getInfile()), taxons, data, ntax, ntraits );
     }
     else {
         readstandard( *(GlobalState::getInfile()), taxons, data, ntax, ntraits );
     }
     model = new PhylipDataRepresentation( data, taxons );
   //}
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
