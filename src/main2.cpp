
/*
 * Brandon Chisham
 * Prototype to read nexus files and translate into the new ontology.
 */
#include <unistd.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

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
//static const string INFILE_ARG  = "-i";
//static const string OUTFILE_ARG = "-o";
static const string COMPRESS_OUTPUT_ARG = "-z";
//static const string VERBOSE_1   = "-v";
//static const string VERBOSE_2   = "-vv";
//static const string VERBOSE_3   = "-vvv";
//static const string VERBOSE_4   = "-vvvv";
//static const string VERBOSE_5   = "-vvvvv";

//TypedOutputStream tocerr = *StreamFactory::getOutputInstance( cerr );

/*
 * Process the argument list and setup the environment.
 */
void myprocessArgs(int argc, char** argv, char** env);

void doCompressedOutput( const string& inname, const string& outname );
void doNormalOutput( const string& inname, const string& outname );

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

bool compress_output = false;

int main(int argc, char** argv, char** env){
  //setup environment
  myprocessArgs( argc, argv, env );
  //Logger has been configured.
//  LogManager lmgr = LogManager::getInstance();
  unsigned long int file_no = 1;     
  for (vector< string >::iterator i = input_files.begin(); i != input_files.end(); ++i, ++file_no ){
      in = new ifstream( i->c_str() );
      
      cerr << "Processing (" << file_no << "/" << input_files.size() << " : " << 100*(double)file_no/input_files.size() << "%):" << *i << endl;

      if (compress_output){ doCompressedOutput(*i, *i + ".cdao"); }
      else { doNormalOutput( *i, *i + ".cdao" ); }
                         
  }
  return 0;

}

void myprocessArgs(int argc, char** argv, char** env){
 // cerr << "processArgs( argc:"  << argc << ", argv:" << argv << ", env: " << env << ")\n";
  //int default_log_level = NO_MESSAGES_LR;
  //logger = new CppLogger( cerr, default_log_level );
  in = &cin;
  out = &cout;
  GlobalState::setInfile( &cin );
  GlobalState::setOutfile( &cout );
  input_files = vector< string >();
  output_files = vector< string >();
  //LogManager lmgr = LogManager::getInstance();
  //GlobalState::setErrorfile( &cerr );
  for (int i = 1; i < argc  - 1; ++i){
    if ( argv[ i ] == INFILE_ARG){
	input_files.push_back( argv[ i + 1 ] );
        //ifstream* inf = new ifstream( input_file.c_str() );
        
	//GlobalState::setInfile( in =  dynamic_cast<istream*>(inf) );
    }
    else if ( argv[ i ] == OUTFILE_ARG ){
	//ofstream* outf = new ofstream( argv[ i + 1 ]  );
      	//GlobalState::setOutfile( out =  dynamic_cast<ostream*>(outf) );
	//Codegen::Imports::setBaseURI( argv[ i + 1 ] );
        output_files.push_back( argv[ i + 1 ] );
    }
    else if ( argv[ i ] == COMPRESS_OUTPUT_ARG){
        compress_output = true;
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

void doCompressedOutput( const string& inname, const string& outname ){
  ifstream* in;
  //ofstream* out;
  DataRepresentation* data;
  const unsigned int READ_END = 0;
  const unsigned int WRITE_END = 1;
  int pipe_descriptors[2];
  if (pipe(pipe_descriptors) > -1){
      pid_t child_pid = fork();
      if (0 == child_pid){
        /* child-code */
        if ( close(pipe_descriptors[ WRITE_END ]) > -1 ){
          if (dup2( pipe_descriptors[ READ_END ], STDIN_FILENO ) > -1){
            int outfiledes = open( (outname + ".gz").c_str(), O_CREAT|O_WRONLY|O_TRUNC,0644);
            if (outfiledes > -1){
              if (dup2( outfiledes, STDOUT_FILENO ) > -1){
                  close( outfiledes );
                  execlp("gzip","gzip", (char*)NULL);
              }
              else {
                perror("Unable to dup STDOUT_FILENO ");
                exit( 1 );
              }
            }
            else {
              perror(("unable to open " + outname + ".gz ").c_str());
              exit( 1 );
            }
          }
          else {
             perror("Unable to dup STDIN_FILENO ");
             exit( 1 );
          }

        }
        else {
          perror("Unable to close write end of pipe");
          exit( 1 );
        }
      }
      else {
        if (close( pipe_descriptors[ READ_END ]) > -1 ){
          if (dup2( pipe_descriptors[ WRITE_END ], STDOUT_FILENO) > -1){
            in = new ifstream( inname.c_str() );
            GlobalState::setInfile( in );
            GlobalState::setOutfile( &cout );

            data = nexusparse();

            cerr << "Parsed: " << inname << endl;

            data->setMatrixLabel( inname );
            CodeGenerator outputFormatter( data );

            outputFormatter.generate( cout );
        
            cerr << "Wrote output" << endl;

            in->close();
            close( pipe_descriptors[ WRITE_END ] );
            close( STDOUT_FILENO );
            delete in;
            delete data;
            int status; 
            cerr << "Waiting for child to exit" << endl;
            waitpid(-1, &status, 0);
            cerr << "Child Exited" << endl;
          }
          else { perror( "Unable to dup STDOUT_FILENO" ); }
        }
        else { perror( "Unable to close read end of pipe" ); }
      }
  }
  else {
      perror("Unable to open pipe: ");
      exit( 1 );

  }
 // delete data;

}

void doNormalOutput( const string& inname, const string& outname ){

  ifstream* in;
  ofstream* out;
  DataRepresentation* data;

  in = new ifstream( inname.c_str() );
  out = new ofstream( outname.c_str() );

  GlobalState::setInfile( in );
  GlobalState::setOutfile( out );
  
  data = nexusparse();

  data->setMatrixLabel( inname );

  CodeGenerator outputFormatter( data );

  outputFormatter.generate( *out );

  in->close();
  out->close();
  delete in;
  delete out;
  delete data;


}

