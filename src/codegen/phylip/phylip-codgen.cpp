#include "phylip-codegen.hpp"

using namespace std;
using namespace CDAO;

PhylipCodeGenerator::PhylipCodeGenerator(const DataRepresentation* dm):CodeGenerator( dm ){}
PhylipCodeGenerator::~PhylipCodeGenerator(){}



void PhylipCodeGenerator::generate( std::wostream& out){
    const unsigned PHY_TAX_WIDTH = 10;
    if ( data_model_ ){
       out << data_model_->getNTax() << " " << data_model_->getNTraits() << endl;
       for (unsigned taxon = 0; taxon < data_model_->getNTax(); ++taxon){
	 out << setw( PHY_TAX_WIDTH ) << data_model_->getTaxonLabel( taxon );
	 for (unsigned trait = 0; trait < data_model_->getNTraits(); ++trait){
              out << data_model_->getTraitState( taxon, trait );
	 }
	 out << endl;
       }
    }

}
