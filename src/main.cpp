
/*
 * Brandon Chisham
 * Prototype to read nexus files and translate into the new ontology.
 */

#include <string>
#include <data_representation.hpp>
#include <grammar/nexus/nexus_reader.hpp>
#include <codegen/constants.hpp>
#include <codegen/codegen.hpp>

#include <fstream>
#include <iostream>

#include <Logger.hpp>
#include <AbstractStream.hpp>
#include <LogManager.hpp>


//using namespace std;
using namespace CDAO;


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
  DataRepresentation* data = nexusparse();
  data->setMatrixLabel( CDAO::str_to_wstr( getInputFile() ) );
  lmgr.log(INFO_MESSAGES_LR, "Parsed the input file\n");
  lmgr.log(INFO_MESSAGES_LR, "Preparing to generate output\n");
  //configure the output generator
  CodeGenerator outputFormatter( data );
  //write the output
  outputFormatter.generate( *(GlobalState::getOutfile()) );
  lmgr.log(INFO_MESSAGES_LR, "Generated output\n");
  return 0;

}


