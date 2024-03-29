#ifndef CODEGEN_HPP_
#define CODEGEN_HPP_

#include <data_representation.hpp>
#include <util.hpp>
#include <cassert>
#include <fstream>


namespace CDAO {
/*
 * Brandon Chisham
 * Created: 3-27-2008
 * Updated: 4/18/08 - 4/21/08: Changed format to CDAO 12
                               Updated some style issues.
			       Made more generalized/flexible
 * Owl format generator for CDAO_12
 * Updated: 5/3/08: Compute MRCA node info.
 */

class CodeGenerator {
   public:
     /*
      * Initialize the code generator to use the specified data model.
      */
     CodeGenerator( const DataRepresentation* data_model)/*:data_model_( data_model ){ assert( data_model_); }*/;
     virtual ~CodeGenerator();
     /*
      * Generate the output in ontology format.
      */
     virtual void generate( std::wostream& out );
   protected:
     const DataRepresentation* data_model_;
};

}
#endif
