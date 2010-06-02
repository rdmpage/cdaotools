#ifndef NEXUS_CODEGEN_HPP_
#define NEXUS_CODEGEN_HPP_

#include <fstream>
#include <cassert>
#include <data_representation.hpp>
#include <codegen/codegen.hpp>

namespace CDAO {

  class NexusCodeGenerator : public CodeGenerator {
	public:
	   /**
	    * Create a generator for the specified model.
	    */
	   NexusCodeGenerator( const DataRepresentation* data_model );
	   /**
	    * Cleanup
	    */
	   virtual ~NexusCodeGenerator();
	   /**
	    * Write a nexus format version of the data-model to the specified stream.
	    */
	   virtual void generate( std::wostream& out );

  };

}

#endif
