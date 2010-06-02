#ifndef PHYLIP_CODEGEN_HPP_
#define PHYLIP_CODEGEN_HPP_

#include <fstream>
#include <cassert>
#include <data_representation.hpp>
#include <codegen/codegen.hpp>

namespace CDAO {

  class PhylipCodeGenerator : public CodeGenerator {
	public:
	   /**
	    * Create a generator for the specified model.
	    */
	   PhylipCodeGenerator( const DataRepresentation* data_model );
	   /**
	    * Cleanup
	    */
	   virtual ~PhylipCodeGenerator();
	   /**
	    * Write a phylip format version of the data-model to the specified stream.
	    */
	   virtual void generate( std::wostream& out );

  };

}

#endif
