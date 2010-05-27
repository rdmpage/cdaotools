#include "nexus-codegen.hpp"

using namespace std;
using namespace CDAO;

NexusCodeGenerator::NexusCodeGenerator(const DataRepresentation* dm):CodeGenerator( dm ){}
NexusCodeGenerator::~NexusCodeGenerator(){}

static void writeHeader( wostream& out, const DataRepresentation* model );
static void writeTaxa( wostream& out, const DataRepresentation* model );
static void writeMatrix( wostream& out, const DataRepresentation* model );
static void writeTree( wostream& out, const DataRepresentation* model );


void NexusCodeGenerator::generate( std::wostream& out){
    const unsigned PHY_TAX_WIDTH = 10;
    if ( data_model_ ){
       //out << data_model_->getNTax() << " " << data_model_->getNTraits() << endl;
      
}

void writeHeader( wostream& out, const DataRepresentation* model ){
    out << L"#NEXUS" << endl;
}
void writeTaxa( wostream& out, const DataRepresentation* model ){
    if ( model ){
      out << L"BEGIN TAXA " << endl;
      for ( unsigned taxon = 0; taxon < model->getNTax(); ++taxon ){
          out << model->getTaxonLabel( taxon );
	  out << " ";
      }
      out << L"END;" << endl;
    }
}

void writeMatrix( wostream& out, const DataRepresentation* model ){
   if ( model ){ 
     out << L"BEGIN MATRIX" << endl;
     for (unsigned taxon = 0; taxon < model->getNTax(); ++taxon){
	 out << model->getTaxonLabel( taxon ) << L" ";
	 for (unsigned trait = 0; trait < model->getNTraits(); ++trait){
              out << model->getTraitState( taxon, trait );
	 }
	 out << endl;
       }
    }

     out << L"END;" << endl;
   }
}
void writeTree( wostream& out, const DataRepresentation* model ){
   if ( model ){

   }
}

