
/*
 * Brandon Chisham
 * Prototype to read nexus files and translate into the new ontology.
 */

#include <string>
#include <data_representation.hpp>
#include <grammar/nexus/nexus_reader.hpp>
#include <grammar/phylip/phylip_reader.hpp>
#include <codegen/constants.hpp>
#include <codegen/codegen.hpp>
#include <codegen/nexus/nexus-codegen.hpp>
#include <codegen/phylip/phylip-codegen.hpp>
#include <codegen/mega/mega-codegen.hpp>
#include <fstream>
#include <iostream>

#include <Logger.hpp>
#include <AbstractStream.hpp>
#include <LogManager.hpp>

#include <shm_heap.hpp>

//using namespace std;
using namespace CDAO;

#define SHARED_HEAP_SIZE ( 1024 * 1024 * 64 )

//Logger* logger;

//wstring input_file = L"";
//wstring output_file = L"";
int main(int argc, char** argv, char** env){

  //setup environment
  processArgs( argc, argv, env );
  //Logger has been configured.
  LogManager lmgr = LogManager::getInstance();
  lmgr.log(INFO_MESSAGES_LR, "Preparing to parse the input file\n");
  //parse the input stream

  map_segment();  
  DataRepresentation* data = NULL;
  switch ( GlobalState::getInFormat() ){
      case PHYLIP_FORMAT:
	data = phylipparse();
        break;
      default:
	data = nexusparse();
	break;
  }

  //DataRepresentation* data = nexusparse();
  data->setMatrixLabel( CDAO::str_to_wstr( getInputFile() ) );
  lmgr.log(INFO_MESSAGES_LR, "Parsed the input file\n");
  lmgr.log(INFO_MESSAGES_LR, "Preparing to generate output\n");
  //configure the output generator
  CodeGenerator* outputFormatter = NULL;
  switch ( GlobalState::getOutFormat() ){
    case NEXUS_FORMAT:
      outputFormatter = new NexusCodeGenerator( data );
      break;
    case PHYLIP_FORMAT:
      outputFormatter = new PhylipCodeGenerator( data );
      break;
    case MEGA_FORMAT:
      //TODO write mega format output generator and initialize it here.
      break;
    default:
      outputFormatter = new CodeGenerator( data );
      break;
  }
  //write the output
  outputFormatter->generate( *(GlobalState::getOutfile()) );

  delete data;
  unmap_segment();

  lmgr.log(INFO_MESSAGES_LR, "Generated output\n");
  return 0;

}


