
/*
 * Brandon Chisham
 * Prototype to read nexus files and translate into the new ontology.
 */

#include <string>
#include <vector>
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
using namespace CDAO;
static const string INFILE_ARG  = "-i";
static const string OUTFILE_ARG = "-o";
static const string VERBOSE_1   = "-v";
static const string VERBOSE_2   = "-vv";
static const string VERBOSE_3   = "-vvv";
static const string VERBOSE_4   = "-vvvv";
static const string VERBOSE_5   = "-vvvvv";

//TypedOutputStream tocerr = *StreamFactory::getOutputInstance( cerr );

/*
 * Process the argument list and setup the environment.
 */
void processArgs(int argc, char** argv, char** env);

inline bool isOption( const char* str){ return str ? *str == '-' : false; }
/*
 * Initializes the logging system. 
 */
//void initLog(LogManager& lmgr, const char* out, const int level);
/*
 * Input and output streams.
 */
istream* in;
ostream* out;

//Logger* logger;

vector< string > input_files;
vector< string > output_files;

int main(int argc, char** argv, char** env){
  //setup environment
  processArgs( argc, argv, env );
  //Logger has been configured.
//  LogManager lmgr = LogManager::getInstance();
  unsigned long int file_no = 0;
  ifstream* in;
  ofstream* out;
  DataRepresentation* data;
  for (vector< string >::iterator i = input_files.begin(); i != input_files.end(); ++i, ++file_no ){
      in = new ifstream( i->c_str() );
      out = new ofstream( (*i + ".cdao").c_str() );

      NexusState::setInfile( in );
      NexusState::setOutfile( out );
      
      cerr << "Finished initialization\n";
      //lmgr.log(INFO_MESSAGES_LR, "Preparing to parse the input file\n");
 
      cerr << "Processing (" << file_no << "/" << input_files.size() << " : " << 100*(double)file_no/input_files.size() << "%):" << *i << endl;

      //parse the input stream
      data = nexusparse();
      cerr << "Parsed: " << *i << endl;
      data->setMatrixLabel( *i  );
      //lmgr.log(INFO_MESSAGES_LR, "Parsed the input file\n");
      //lmgr.log(INFO_MESSAGES_LR, "Preparing to generate output\n");
      //configure the output generator
      CodeGenerator outputFormatter( data );
      
      cerr << "Initialized output formatter\n";

      //write the output
      outputFormatter.generate( *out );

      cerr << "Generated output\n";
      //lmgr.log(INFO_MESSAGES_LR, "Generated output\n");
  
      in->close();
      out->close();
      delete in;
      delete out;
      delete data;
      
      cerr << "Finished Cleanip\n";
  }

   
  return 0;

}

void processArgs(int argc, char** argv, char** env){
 // cerr << "processArgs( argc:"  << argc << ", argv:" << argv << ", env: " << env << ")\n";
  //int default_log_level = NO_MESSAGES_LR;
  //logger = new CppLogger( cerr, default_log_level );
  in = &cin;
  out = &cout;
  NexusState::setInfile( &cin );
  NexusState::setOutfile( &cout );
  input_files = vector< string >();
  output_files = vector< string >();
  //LogManager lmgr = LogManager::getInstance();
  //NexusState::setErrorfile( &cerr );
  for (int i = 1; i < argc  - 1; ++i){
    if ( argv[ i ] == INFILE_ARG){
	input_files.push_back( argv[ i + 1 ] );
        //ifstream* inf = new ifstream( input_file.c_str() );
        
	//NexusState::setInfile( in =  dynamic_cast<istream*>(inf) );
    }
    else if ( argv[ i ] == OUTFILE_ARG ){
	//ofstream* outf = new ofstream( argv[ i + 1 ]  );
      	//NexusState::setOutfile( out =  dynamic_cast<ostream*>(outf) );
	//Codegen::Imports::setBaseURI( argv[ i + 1 ] );
        output_files.push_back( argv[ i + 1 ] );
    }
    //else if ( argv[ i ] == VERBOSE_1){
    //  initLog( lmgr, argv[ i + 1], ALERT_MESSAGES_LR ); 
    //}
    //else if ( argv[ i ] == VERBOSE_2){  
    //  initLog( lmgr, argv[ i + 1], CRITICAL_MESSAGES_LR ); 
    //}
    //else if ( argv[ i ] == VERBOSE_3){ 
    //  initLog( lmgr, argv[ i + 1], ERROR_MESSAGES_LR ); 
    //}
    //else if ( argv[ i ] == VERBOSE_4){
    //  initLog( lmgr, argv[ i + 1], INFO_MESSAGES_LR );
    //}
    //else if ( argv[ i ] == VERBOSE_5){ 
    //  initLog( lmgr, argv[ i + 1], ALL_MESSAGES_LR );
    //}
  }
 // cerr << "exiting processArgs()\n";
  return;
}

//void initLog(LogManager& lmgr, const char* out, const int level){

//  if (isOption( out )){
    
//    lmgr.registerListener( LoggerFactory::getLogger( tocerr, level  ) );
 // }
 // else {
 //   ofstream logStream( out );
 //   lmgr.registerListener( LoggerFactory::getLogger( logStream, level  ) );
 // } 
//}
