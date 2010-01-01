#include "util.hpp"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <data_representation.hpp>
#include <grammar/nexus/nexus_reader.hpp>
#include <codegen/constants.hpp>
#include <codegen/codegen.hpp>

#include <fstream>
#include <iostream>

#include <Logger.hpp>
#include <AbstractStream.hpp>
#include <LogManager.hpp>

using namespace std;
namespace CDAO {

  istream* GlobalState::in_ = NULL;
  ostream* GlobalState::out_ = NULL;
  ostream* GlobalState::err_ = &std::cerr;
  bool GlobalState::interleaved_ = false;

static std::string input_file = "";
static std::string output_file = "";

   /*
    * Input and output streams.
    */
//static istream* in;
//static ostream* out;
TypedOutputStream tocerr = *StreamFactory::getOutputInstance( cerr );


string getInputFile(){ return input_file; }
string getOutputFile(){ return output_file;}

istream* getInputStream(){ return GlobalState::getInfile(); }
ostream* getOutputStream(){ return GlobalState::getOutfile(); }


string labelMaker(  string tag_type ){
	static int serial_number = 0;

	return tag_type + number_to_string( ++serial_number );
}

string number_to_string( int number ){
   
	const int NUM_SIZE = 13;
	const int NUM_ASIZE = NUM_SIZE -1;
 	char str_number[NUM_SIZE];
	snprintf( str_number, NUM_ASIZE, "%d", number );

	return string(str_number);
}

void processArgs(int argc, char** argv, char** env){
 // cerr << "processArgs( argc:"  << argc << ", argv:" << argv << ", env: " << env << ")\n";
  //int default_log_level = NO_MESSAGES_LR;
  //logger = new CppLogger( cerr, default_log_level );
  //in = &cin;
  //out = &cout;
  GlobalState::setInfile( &cin );
  GlobalState::setOutfile( &cout );
  LogManager lmgr = LogManager::getInstance();
  //GlobalState::setErrorfile( &cerr );
  for (int i = 1; i < argc  - 1; ++i){
    if ( argv[ i ] == INFILE_ARG){
	input_file = argv[ i + 1 ];
        ifstream* inf = new ifstream( input_file.c_str() );
        
	GlobalState::setInfile( dynamic_cast<istream*>(inf) );
    }
    else if ( argv[ i ] == OUTFILE_ARG ){
	ofstream* outf = new ofstream( argv[ i + 1 ]  );
        output_file = argv[ i + 1 ];
      	GlobalState::setOutfile( dynamic_cast<ostream*>(outf) );
	Codegen::Imports::setBaseURI( argv[ i + 1 ] );
    }
    else if (argv[ i ] == INTERLEAVED_ARG){
        GlobalState::setInterleaved( true );
    }
    else if ( argv[ i ] == VERBOSE_1){
      initLog( lmgr, argv[ i + 1], ALERT_MESSAGES_LR ); 
    }
    else if ( argv[ i ] == VERBOSE_2){  
      initLog( lmgr, argv[ i + 1], CRITICAL_MESSAGES_LR ); 
    }
    else if ( argv[ i ] == VERBOSE_3){ 
      initLog( lmgr, argv[ i + 1], ERROR_MESSAGES_LR ); 
    }
    else if ( argv[ i ] == VERBOSE_4){
      initLog( lmgr, argv[ i + 1], INFO_MESSAGES_LR );
    }
    else if ( argv[ i ] == VERBOSE_5){ 
      initLog( lmgr, argv[ i + 1], ALL_MESSAGES_LR );
    }
  }
 // cerr << "exiting processArgs()\n";
  return;
}

void initLog(LogManager& lmgr, const char* out, const int level){

  if (isOption( out )){
    
    lmgr.registerListener( LoggerFactory::getLogger( tocerr, level  ) );
  }
  else {
    ofstream logStream( out );
    lmgr.registerListener( LoggerFactory::getLogger( logStream, level  ) );
  } 
}

}

