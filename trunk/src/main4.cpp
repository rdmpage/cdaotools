
/*
 * Program to translate phylip format files into CDAO
 */

//#include <unistd.h>
//#include <cstdio>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/wait.h>
//#include <fcntl.h>

//#include <string>
//#include <vector>
#include <data_representation.hpp>
#include <grammar/phylip/phylip_reader.hpp>
//#include <codegen/constants.hpp>
#include <codegen/codegen.hpp>
#include <util.hpp>
//#include <fstream>
//#include <iostream>

//#include <Logger.hpp>
//#include <AbstractStream.hpp>
//#include <LogManager.hpp>

using namespace std;
using namespace CDAO;

int main( int argc, char** argv, char** env){

  processArgs( argc, argv, env );
  map_segment();
  DataRepresentation* model = phylipparse();
  model->setMatrixLabel( CDAO::str_to_wstr( getInputFile()) );
  CodeGenerator formatter( model );
  formatter.generate( *(GlobalState::getOutfile()) );
  delete model;
  unmap_segment();
  return 0;
}
