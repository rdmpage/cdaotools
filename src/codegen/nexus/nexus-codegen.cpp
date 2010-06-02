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
    //const unsigned PHY_TAX_WIDTH = 10;
    if ( data_model_ ){
       //out << data_model_->getNTax() << " " << data_model_->getNTraits() << endl;
    } 
}

void writeHeader( wostream& out, const DataRepresentation* model ){
    out << L"#NEXUS" << endl;
}
void writeTaxa( wostream& out, const DataRepresentation* model ){
    if ( model ){
      out << L"BEGIN TAXA;" << endl;
      out << L"DIMENSIONS ntax=" << model->getNTax() << L";" << endl;
      out << L"TAXLABELS" << endl;
      for ( unsigned taxon = 0; taxon < model->getNTax(); ++taxon ){
          out << "\'" << model->getTaxonLabel( taxon ) << "\'" << endl;
      }
      out << L";" << endl;
      out << "END;" << endl;
    }
}

void writeMatrix( wostream& out, const DataRepresentation* model ){
   if ( model ){ 
     out << L"BEGIN CHARACTERS;" << endl;
     out << L" dimensions nchar=" << model->getNTraits() << ";" << endl;
     out << L" format datatype=" << model->getDataType() << L" missing=" << model->getMissingChar() << L" gap=" << model->getGapChar() << ";" << endl;
     out << L"MATRIX" << endl;
     for (unsigned taxon = 0; taxon < model->getNTax(); ++taxon){
	 out << model->getTaxonLabel( taxon ) << L" ";
	 for (unsigned trait = 0; trait < model->getNTraits(); ++trait){
              out << model->getTraitState( taxon, trait );
	 }
	 out << endl;
       }
    }
    out << L";" << endl; 
    out << L"END;" << endl;
}

void writeTree( wostream& out, const DataRepresentation* model ){
   if ( model &&  model->getNumTrees() ){
	out << "BEGIN TREES;" << endl;
	for (unsigned tree = 0; tree < model->getNumTrees(); ++tree){
 	   out << L"TREE " << model->getTreeLabel( tree ) << L" ";
	   //if ( model->isRooted( tree ) ){
	   //  out << L"[&R] ";
	   //}
	   ///else { out << L"[&U] "; }
	   out << model->getParseTree( tree );
	}
	out << "END;" << endl;
   }
}

