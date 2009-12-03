#include "phylip_reader.hpp"
#include <cassert>
#include <fstream>

using namespace std;
using namespace CDAO;

PhylipDataRepresentation::PhylipDataRepresentation( vector< string > matrix_data, vector< string > labels ):DataRepresentation(), matrix_data_( matrix_data ), taxon_labels_( labels ){}

PhylipDataRepresentation::~PhylipDataRepresentation(){
  //ntax_ = 0;
  //ntraits_ = 0;
  matrix_data_.empty();
}

static unsigned  getntaxtrait( istream& in );
static string gettaxon( istream& in );
static string getdataline( istream& in, unsigned ntraits );

static void readinterleaved( istream& in, vector< string >& taxons, vector< string >& data, unsigned ntax, unsigned ntraits  );
static void readstandard( istream& in, vector< string >& taxons, vector< string >& data, unsigned ntax, unsigned ntraits  );

static const int TAXON_LABEL_SIZE = 10;


DataRepresentation* phylipparse(PhylipEnvironment* env){
   DataRepresentation* model=NULL;
   if ( env ){
     vector< string > data = vector< string >();
     vector< string > taxons = vector< string >();
     unsigned ntax = getntaxtrait( env->getinput() );
     unsigned ntraits = getntaxtrait( env->getinput() );
     
     if ( env->isInterleaved() ){
         readinterleaved( in, taxons, data, ntax, ntraits );
     }
     else {
         readstandard(in, taxons, data, ntax, ntraits );
     }

   }
   return model;
}

void readinterleaved( istream& in, vector< string >& taxons, vector< string >& data, unsigned ntax, unsigned ntraits  ){
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

void readstandard( istream& in, vector< string >& taxons, vector< string >& data, unsigned ntax, unsigned ntraits ){

    string taxon;
    string data_line;
    char new_line_char;
    for (unsigned tax =0; tax < ntax; ++tax){
        taxon = gettaxon( in );
        data_line = getdataline( in, ntraits );
        taxons.push_back( taxon );
        data.push_back( data_line );
        in.get( new_line_char );
    }
    return;
}



unsigned getntaxtrait( istream& in ){
   unsigned ret = 0;
   in >> ret;
   return ret;
}

string gettaxon( istream& in ){
  string ret;
  char buff[ TAXON_LABEL_SIZE ];

  in.get( buff, TAXON_LABEL_SIZE );
  ret = buff;

  return ret;
}

string getdataline( istream& in, unsigned ntraits ){
   string ret;
   char buff[ ntraits ];

   in.getline( buff, ntraits );
   ret = buff;

   return ret;
}

