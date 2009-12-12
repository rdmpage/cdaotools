
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
  DataRepresentation* model = phylipparse();
  model->setMatrixLabel( getInputFile() );
  CodeGenerator formatter( model );
  formatter.generate( *(GlobalState::getOutfile()) );
  return 0;
}
